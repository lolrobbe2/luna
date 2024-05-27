#pragma once
#include <core/artemis/device/device.h>
#include <core/artemis/renderCommandBuffer.h>
#include <core/artemis/rendering/frameBuffer.h>
#include <core/assets/publicTypes/image.h>

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
			 _ALWAYS_INLINE_ glm::vec4 normalizeColor(const glm::vec4& color);
			 _ALWAYS_INLINE_ void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

			 _ALWAYS_INLINE_ void drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<assets::image> image);

			 _ALWAYS_INLINE_ void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const ref<assets::image> image);

			 _ALWAYS_INLINE_ void drawQuad(const glm::mat4& transform, const ref<assets::image> image);

			 _ALWAYS_INLINE_ void drawQuad(const glm::mat4& transform, const glm::vec4& color, const ref<assets::image> image,const std::array<glm::vec2,4>& textureCoords);
			 _ALWAYS_INLINE_ void drawQuad(const glm::mat4& transform, const glm::vec4& color, const ref<assets::image> image);

			 _ALWAYS_INLINE_ void drawQuad(const glm::mat4& transform, const glm::vec4& color1);
			 _ALWAYS_INLINE_ void drawQuad(const drawCommand& command);
#ifdef IMGUI_API
			 _ALWAYS_INLINE_ void setSceneDimensions(ImVec2 size) { imguiSceneSize.x = size.x; imguiSceneSize.y = size.y; }
#endif // !IMGUI_API
			 _ALWAYS_INLINE_ glm::vec2 getSceneMousePos();
			 _ALWAYS_INLINE_  glm::vec2 getSceneDimensions();
		private:
			void setUpComputePipeline();
			void setUpGraphicsPipeline();
#ifdef IMGUI_API
			void setUpImguiPipeline();
#endif // IMGUI_API

			void recordCommands();
			void flush();
		private:
			device& c_device = *new device();
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
			std::vector<ref<semaphore>> imageAvailableSemaphores, renderFinishedSemaphores,computeFinishedSemaphores;
			std::vector<frameBuffer> frameBuffers;

			descriptorPool& computeDescriptorPool = *new descriptorPool();
			descriptorPool& grapchicsDescriptorPool = *new descriptorPool();

			std::vector<renderCommandBuffer> renderCmdBuffers;
			renderCommandBuffer* currentBuffer;
			size_t batchCount;
			ref<allocator> p_allocator;

			std::vector<ref<fence>> inFlightFences,computeInflightFences;
			uint8_t currentFrame = 0;
			uint8_t maxFramesInFlight = 0;
			uint32_t swapchainImageIndex = 0;
			ref<sampler> sampler;
			
			ref<vulkan::window> p_window;
#ifdef IMGUI_API
			std::vector<frameBuffer> imguiFrameBuffers;
			ref<renderPass> p_imguiRenderPass;
			glm::vec2 imguiSceneSize;
			std::vector<image> frameBufferImages;    

#endif //IMGUI_API

		};
	}
}


