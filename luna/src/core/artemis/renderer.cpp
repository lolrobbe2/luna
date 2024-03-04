#include "renderer.h"
#include "rendering/builders/attachementBuilder.h"
#include <core/utils/shaderLibrary.h>
namespace luna 
{
	namespace artemis 
	{
		renderer::renderer(const ref<vulkan::window>& window)
		{
			LN_PROFILE_FUNCTION();
			c_device = *new device(window);
			p_swapChain = c_device.getSwapchain();

			p_allocator = c_device.getAllocator();
		
			setUpComputePipeline();
			
			setUpGraphicsPipeline();
		}
		void renderer::beginScene()
		{
			currentBuffer = &renderCmdBuffers[0];
		}
		void renderer::endScene()
		{
		}
		void renderer::update()
		{
			inFlightFences[currentFrame].wait();
			VkResult result = p_swapChain->acquireNextImage(UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &swapchainImageIndex);
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				if (p_swapChain->invalid()) return;
				//todo recreate pipeline;
			}
			inFlightFences[currentFrame].reset();
			graphicsFences[swapchainImageIndex] = inFlightFences[currentFrame];

			inFlightFences[currentFrame].reset();
			recordCommands();
			flush();

		}

		glm::vec4 renderer::normalizeColor(const glm::vec4& color) const
		{
			return glm::normalize(color / 255.0f);
		}

		void renderer::drawQuad(const glm::mat4 transform,const glm::vec4 color1) const
		{
			drawQuad({ transform,color1 });
		}

		void renderer::drawQuad(const drawCommand& command) const
		{
			currentBuffer->addCommand(command);
		}

		void renderer::setUpComputePipeline()
		{
			p_computeCommandPool = c_device.getCommandPool(vkb::QueueType::compute);

			p_computeCommandBuffer.resize(p_swapChain->size(),p_computeCommandPool->getCommandBuffer());
			
			ref<shader> quadVertexGenerator = shaderLibrary::get("quadVertexGenerator.glsl"); //get compute shader

			descriptorPoolBuilder poolBuilder = c_device.getDescriptorPoolBuilder(quadVertexGenerator);

			//storageBuffers with fixed size used for vertex generation.
			computeDescriptorPool = poolBuilder
				.setStorageBufferAmount(1000)
				.build();
			
			pipelineBuilder computePipelineBuidler = c_device.getPipelineBuilder();
			computePipeline = computePipelineBuidler
				.setPipelineType(COMPUTE)
				.addShaderStage(quadVertexGenerator)
				.addDescriptorSetLayout(computeDescriptorPool)
				.build();

		}

		void renderer::setUpGraphicsPipeline()
		{
			p_graphicsCommandPool = c_device.getCommandPool(vkb::QueueType::graphics);

			p_graphicsCommandBuffer.resize(p_swapChain->size(),p_graphicsCommandPool->getCommandBuffer());
			currentBuffer = &renderCmdBuffers[0];

			ref<shader> vertexShader = shaderLibrary::get("vertex.glsl"); //get vertex shader
			ref<shader> fragmentShader = shaderLibrary::get("fragment.glsl"); //get vertex shader

			descriptorPoolBuilder poolBuilder = c_device.getDescriptorPoolBuilder(fragmentShader);

			//storageBuffers with fixed size used for vertex generation.
			grapchicsDescriptorPool = poolBuilder
				.setSamplerAmount(10)
				.setStorageImageAmount(32*10)
				.build();

			attachementBuilder attachementBuilder{ p_swapChain };
			attachement att = attachementBuilder
				.setClearColorValue(0.0f, 0.0f, 0.0f, 0.0f)
				.setSamples().setOp(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE)
				.setLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
				.setStencilOp(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE)
				.build();

			subPassBuilder subPassBuilder;
			subpassDescription subpass = subPassBuilder
				.addColorAttachement(att)
				.setBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS)
				.build();

			subpassDescription description = subPassBuilder.addColorAttachement(att).setBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS).build();
			subpassDependency dependency { 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,0};
			renderPassBuilder renderPassBuilder = c_device.getRenderPassBuilder();
			
			p_renderPass = renderPassBuilder
				.addSubPassDependency(dependency)
				.addSubPass(subpass)
				.build();
				
			frameBuffers.resize(p_swapChain->size(), p_swapChain->getFrameBuffer(p_renderPass, frameBuffers.size(), 0, 1));

			pipelineBuilder graphicsPipelineBuilder = c_device.getPipelineBuilder();
			graphicsPipeline = graphicsPipelineBuilder
				.setColorBlendingParams()
				.setAlphaBlendingParams(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD)
				.setColorMask()
				.setPipelineType(GRAPHICS)
				.addShaderStage(vertexShader)
				.addShaderStage(fragmentShader)
				.addDescriptorSetLayout(grapchicsDescriptorPool)
				.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT)
				.addDynamicState(VK_DYNAMIC_STATE_SCISSOR)
				.addViewport(p_swapChain->getViewport())
				.addScissor(*p_swapChain)
				.setRenderPass(p_renderPass)
				.build();
			renderCmdBuffers.resize(10, { p_allocator,computeDescriptorPool,grapchicsDescriptorPool });

		}
		void renderer::recordCommands()
		{
			p_computeCommandBuffer[currentFrame]->begin(0);
			p_computeCommandBuffer[currentFrame]->bindPipeline(computePipeline);
			for (renderCommandBuffer renderCmdBuffer : renderCmdBuffers)
			{
				if (renderCmdBuffer.commandsAmount) {
					renderCmdBuffer.generateIndices();
					p_computeCommandBuffer[currentFrame]->bindDescriptorSet(computePipeline, renderCmdBuffer.computeDescriptorSet);
					p_computeCommandBuffer[currentFrame]->dispatch(renderCmdBuffer.commandsAmount, 1, 1); //dispatch per batch
				}
			}
			p_computeCommandBuffer[currentFrame]->end();

			p_computeCommandPool->flush({ p_computeCommandBuffer[currentFrame].get() }, {}, {}, fence(), nullptr, true);

			p_graphicsCommandBuffer[currentFrame]->begin(0);
			p_graphicsCommandBuffer[currentFrame]->bindPipeline(graphicsPipeline);
			p_graphicsCommandBuffer[currentFrame]->beginRenderPass(p_renderPass, frameBuffers[currentFrame]);
			p_graphicsCommandBuffer[currentFrame]->endCurrentRenderPass();
			p_graphicsCommandBuffer[currentFrame]->end();
			p_graphicsCommandPool->flush({ p_graphicsCommandBuffer[currentFrame].get() }, {renderFinishedSemaphores[currentFrame]}, {imageAvailableSemaphores[currentFrame]}, inFlightFences[currentFrame], nullptr, true);
		}
		void renderer::flush()
		{

		}
	}
}