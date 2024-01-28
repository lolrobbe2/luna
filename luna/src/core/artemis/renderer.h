#pragma once
#include <core/artemis/device/device.h>
#include <core/artemis/renderCommandBuffer.h>
namespace luna 
{
	namespace artemis 
	{
		class LN_API renderer
		{
		public:
			 renderer(const ref<vulkan::window>& window);
			 void beginScene();
			 void endScene();
			 void update();

			 void drawQuad(const drawCommand& command);
		private:
			void setUpComputePipeline();
		private:
			device& c_device = device();
			ref<swapchain> p_swapChain;
			ref<commandPool> p_graphicsCommandPool;
			ref<commandPool> p_computeCommandPool;
			ref<commandBuffer> p_computeCommandBuffer;
			ref<renderPass> p_renderPass;
			ref<pipeline> graphicsPipeline;
			ref<pipeline> computePipeline;

			descriptorPool& computeDescriptorPool = descriptorPool();
			descriptorSet& computeDescriptorSet = descriptorSet();
			std::vector<renderCommandBuffer> renderCmdBuffers;
			renderCommandBuffer* currentBuffer;
			size_t batchCount;
			ref<allocator> p_allocator;
		};
	}
}


