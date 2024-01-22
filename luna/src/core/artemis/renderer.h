#pragma once
#include <core/artemis/device/device.h>
namespace luna 
{
	namespace artemis 
	{
		class LN_API renderer
		{
		public:
			 renderer(const ref<vulkan::window>& window);
		private:
			void setUpComputePipeline();
		private:
			device& c_device = device();
			ref<swapchain> p_swapChain;
			ref<commandPool> p_graphicsCommandPool;
			ref<commandPool> p_computeCommandPool;
			ref<renderPass> p_renderPass;
			ref<pipeline> graphicsPipeline;
			ref<pipeline> computePipeline;

			descriptorPool& computeDescriptorPool = descriptorPool();
			descriptorSet& computeDescriptorSet = descriptorSet();

			ref<allocator> p_allocator;
		};
	}
}


