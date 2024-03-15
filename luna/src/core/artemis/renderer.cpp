#include "renderer.h"
#include "rendering/builders/attachementBuilder.h"
#include <core/utils/shaderLibrary.h>
#include <core/assets/assetImporter.h>
#include <core/assets/assetManager.h>
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
		
			assets::assetImporter::setAllocator(p_allocator);
			maxFramesInFlight = p_swapChain->size() - 1;

			setUpComputePipeline();
			
			setUpGraphicsPipeline();
			ref<assets::image> blankImageAsset = assets::assetManager::getAsset<assets::image>(assets::assetManager::importAsset("src/assets/media/blank.png", assets::texture));
			p_allocator->flush();
			renderCmdBuffers[0].bind(blankImageAsset, 0);
		}
		void renderer::beginScene()
		{
			currentBuffer = &renderCmdBuffers[0];
		}
		void renderer::endScene()
		{
			//for (renderCommandBuffer& renderCommandBuffer : renderCmdBuffers)
				//renderCommandBuffer.update();
		}
		void renderer::update()
		{
			//c_device.waitIdle();
			computeInflightFences[currentFrame]->wait();
			inFlightFences[currentFrame]->wait();
			VkResult result = p_swapChain->acquireNextImage(UINT64_MAX, *imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &swapchainImageIndex);
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				if (p_swapChain->invalid()) return;
				LN_CORE_ERROR("out of date!");
			}
			computeInflightFences[currentFrame]->reset();
			inFlightFences[currentFrame]->reset();
			//graphicsFences[swapchainImageIndex] = inFlightFences[currentFrame];

			//inFlightFences[currentFrame]->reset();
			for (renderCommandBuffer& commandBuffer : renderCmdBuffers) {
				commandBuffer.update(currentFrame);
				commandBuffer.reset();
			}
			recordCommands();
			for (renderCommandBuffer& commandBuffer : renderCmdBuffers) {
				commandBuffer.reset();
			}
			
			VkResult presentResult = p_graphicsCommandPool->present({ p_swapChain }, { renderFinishedSemaphores[currentFrame] },&swapchainImageIndex);

			if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR)
			{
				LN_CORE_ERROR("out of date!");
			}
			currentFrame = (currentFrame + 1) % maxFramesInFlight;
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

			p_computeCommandBuffer.resize(p_swapChain->size() - 1);
			for (auto& commandBuffer : p_computeCommandBuffer) commandBuffer = p_computeCommandPool->getCommandBuffer();
			
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

			computeFinishedSemaphores.resize(maxFramesInFlight);
			for (auto& semaphore : computeFinishedSemaphores) semaphore = c_device.getSemaphore(0);

			computeInflightFences.resize(maxFramesInFlight);
			for (auto& fence : computeInflightFences) fence = c_device.getFence(VK_FENCE_CREATE_SIGNALED_BIT);
		}

		void renderer::setUpGraphicsPipeline()
		{

			p_graphicsCommandPool = c_device.getCommandPool(vkb::QueueType::graphics);

			p_graphicsCommandBuffer.resize(maxFramesInFlight);
			for (auto& commandBuffer : p_graphicsCommandBuffer) commandBuffer = p_graphicsCommandPool->getCommandBuffer();
			
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
				.setClearColorValue(1.0f, 0.0f, 1.0f, 1.0f)
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
				
			frameBuffers.resize(p_swapChain->size());
			for (size_t i = 0; i < frameBuffers.size(); ++i) frameBuffers[i] = p_swapChain->getFrameBuffer(p_renderPass, i, 0, 1);
			
			pipelineBuilder graphicsPipelineBuilder = c_device.getPipelineBuilder();
			graphicsPipeline = graphicsPipelineBuilder
				.setColorBlendingParams()
				.setAlphaBlendingParams(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD)
				.setColorMask()
				.setPipelineType(GRAPHICS)
				.addShaderStage(vertexShader)
				.addShaderStage(fragmentShader)
				.addDescriptorSetLayout(grapchicsDescriptorPool)
				//.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT)
				//.addDynamicState(VK_DYNAMIC_STATE_SCISSOR)
				.addViewport(p_swapChain->getViewport())
				.addScissor(*p_swapChain)
				.setRenderPass(p_renderPass)
				.build();

			sampler = c_device.getSampler(VK_FILTER_NEAREST);
			renderCmdBuffers.reserve(10);
			for (size_t i = 0; i < 1; ++i) {
				renderCmdBuffers.push_back(renderCommandBuffer(p_allocator, computeDescriptorPool, grapchicsDescriptorPool, sampler,maxFramesInFlight));
			}
			currentBuffer = &renderCmdBuffers[0];

			inFlightFences.resize(maxFramesInFlight);
			for (auto& fence : inFlightFences) fence = c_device.getFence(VK_FENCE_CREATE_SIGNALED_BIT);
			

			imageAvailableSemaphores.resize(p_swapChain->size());
			for (auto& semaphore : imageAvailableSemaphores) semaphore = c_device.getSemaphore(0);
			

			// Resize and initialize renderFinishedSemaphores
			renderFinishedSemaphores.resize(maxFramesInFlight);
			for (auto& semaphore : renderFinishedSemaphores) semaphore = c_device.getSemaphore(0);
		}
		void renderer::recordCommands()
		{
			p_computeCommandBuffer[currentFrame]->begin(0);
			p_computeCommandBuffer[currentFrame]->bindPipeline(computePipeline);
			for (renderCommandBuffer& renderCmdBuffer : renderCmdBuffers)
			{
				if (renderCmdBuffer.commandsAmount) {
					renderCmdBuffer.generateIndices();
					p_computeCommandBuffer[currentFrame]->bindDescriptorSet(computePipeline, renderCmdBuffer.computeDescriptorSet);
					p_computeCommandBuffer[currentFrame]->dispatch(renderCmdBuffer.commandsAmount, 1, 1); //dispatch per batch
				}
			}
			p_computeCommandBuffer[currentFrame]->end();

			p_graphicsCommandBuffer[currentFrame]->begin(0);
			p_graphicsCommandBuffer[currentFrame]->beginRenderPass(p_renderPass, frameBuffers[swapchainImageIndex]);
			p_graphicsCommandBuffer[currentFrame]->bindPipeline(graphicsPipeline);
			for (renderCommandBuffer& renderCmdBuffer : renderCmdBuffers)
			{
				if (renderCmdBuffer.commandsAmount)
				{
					static VkDeviceSize offsets = 0;
					p_graphicsCommandBuffer[currentFrame]->bindDescriptorSet(graphicsPipeline, renderCmdBuffer.graphicsDescriptorSets[currentFrame]);
					p_graphicsCommandBuffer[currentFrame]->bindIndexBuffer(renderCmdBuffer.cpuIndicesBuffer,0, VK_INDEX_TYPE_UINT32);
					p_graphicsCommandBuffer[currentFrame]->bindVertexBuffers(0, { renderCmdBuffer.gpuBuffer }, &offsets);
					p_graphicsCommandBuffer[currentFrame]->drawIndexed(renderCmdBuffer.commandsAmount * 6, 1, 0, 0, 0);
				}
			}
			p_graphicsCommandBuffer[currentFrame]->endCurrentRenderPass();
			p_graphicsCommandBuffer[currentFrame]->end();

			p_computeCommandPool->flush({ p_computeCommandBuffer[currentFrame].get() }, { computeFinishedSemaphores[currentFrame] }, {  }, computeInflightFences[currentFrame], nullptr, false);

			
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			p_graphicsCommandPool->flush({ p_graphicsCommandBuffer[currentFrame].get() }, {renderFinishedSemaphores[currentFrame]}, {computeFinishedSemaphores[currentFrame],imageAvailableSemaphores[currentFrame] }, inFlightFences[currentFrame],waitStages, false);
		}
		void renderer::flush() 
		{

		}
	}
}