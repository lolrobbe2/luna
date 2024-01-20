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
			c_device = device(window);
			p_swapChain = c_device.getSwapchain();
			p_graphicsCommandPool = c_device.getCommandPool(vkb::QueueType::graphics);
			p_transferCommandPool = c_device.getCommandPool(vkb::QueueType::transfer);
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