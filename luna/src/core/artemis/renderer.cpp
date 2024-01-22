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
			buffer& buffer = p_allocator->allocateBuffer(100, GPU_ONLY, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			image& image = p_allocator->allocateImage({ 100,100 }, 4, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
			LN_CORE_INFO("buffer size: {0}", buffer.getSize());
			setUpComputePipeline();
		}

		void renderer::setUpComputePipeline()
		{
			p_computeCommandPool = c_device.getCommandPool(vkb::QueueType::compute);

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
	}
}