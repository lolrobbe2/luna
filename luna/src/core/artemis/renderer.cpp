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
			p_graphicsCommandPool = c_device.getCommandPool(vkb::QueueType::graphics);

			p_allocator = c_device.getAllocator();
		
			setUpComputePipeline();
			renderCmdBuffers.resize(10, p_allocator);
			currentBuffer = &renderCmdBuffers[0];
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
			VkDescriptorBufferInfo info;
			info.buffer = currentBuffer->cpuBuffer;
			info.offset = 0;
			info.range = VK_WHOLE_SIZE;
			computeDescriptorSet.write(0, &info);

			p_computeCommandBuffer->begin(0);
			p_computeCommandBuffer->bindPipeline(computePipeline);
			p_computeCommandBuffer->bindDescriptorSet(computePipeline, computeDescriptorSet);
			p_computeCommandBuffer->dispatch(256, 1, 1);
			p_computeCommandBuffer->end();
			
			p_computeCommandPool->flush({ p_computeCommandBuffer.get()}, computeSignalSemaphores, computeWaitSemaphores, fence(), nullptr, true);

			p_graphicsCommandBuffer->begin(0);
			p_graphicsCommandBuffer->bindPipeline(graphicsPipeline);
			p_graphicsCommandBuffer->beginRenderPass(p_renderPass,p_swapChain);
			p_graphicsCommandBuffer->bindDescriptorSets(graphicsPipeline, graphicsDescriptors);
			p_graphicsCommandBuffer->end(); 
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

			p_computeCommandPool->getCommandBuffer();
			
			ref<shader> quadVertexGenerator = shaderLibrary::get("quadVertexGenerator.glsl"); //get compute shader

			descriptorPoolBuilder poolBuilder = c_device.getDescriptorPoolBuilder(quadVertexGenerator);

			//storageBuffers with fixed size used for vertex generation.
			computeDescriptorPool = poolBuilder
				.setStorageBufferAmount(1000)
				.build();
			
			computeDescriptorSet = computeDescriptorPool.allocateDescriptorSet();
			pipelineBuilder computePipelineBuidler = c_device.getPipelineBuilder();
			computePipeline = computePipelineBuidler
				.addShaderStage(quadVertexGenerator)
				.addDescriptorSetLayout(computeDescriptorPool)
				.build();

		}

		void setupGraphicsPipeline()
		{

		}
	}
}