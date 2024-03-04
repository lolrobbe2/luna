#pragma once
#include <core/artemis/device/device.h>
#include <core/artemis/renderCommandBuffer.h>
#include <core/artemis/rendering/frameBuffer.h>
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
			 _ALWAYS_INLINE_ glm::vec4 normalizeColor(const glm::vec4& color) const;
			 _ALWAYS_INLINE_ void drawQuad(const glm::mat4 transform, const glm::vec4 color1) const;
			 _ALWAYS_INLINE_ void drawQuad(const drawCommand& command) const;
		private:
			void setUpComputePipeline();
			void setUpGraphicsPipeline();
		private:
			device& c_device = device();
			ref<swapchain> p_swapChain;
			ref<commandPool> p_graphicsCommandPool;
			std::vector<ref<commandBuffer>>p_graphicsCommandBuffer;

			ref<commandPool> p_computeCommandPool;
			std::vector<ref<commandBuffer>> p_computeCommandBuffer;
			ref<renderPass> p_renderPass;
			ref<pipeline> graphicsPipeline;
			ref<pipeline> computePipeline;
			std::vector<semaphore> computeWaitSemaphores;
			std::vector<semaphore> computeSignalSemaphores;
			std::vector<frameBuffer> frameBuffers;

			descriptorPool& computeDescriptorPool = descriptorPool();
			descriptorPool& grapchicsDescriptorPool = descriptorPool();

			std::vector<renderCommandBuffer> renderCmdBuffers;
			renderCommandBuffer* currentBuffer;
			size_t batchCount;
			ref<allocator> p_allocator;

			uint8_t currentFrame = 0;
		};
	}
}


