#ifndef _RENDERER_
#define _RENDERER_
#include <core/artemis/device/device.h>
#include <core/artemis/renderCommandBuffer.h>
#include <core/artemis/rendering/frameBuffer.h>
#include <core/assets/publicTypes/image.h>
#include <core/assets/publicTypes/font.h>
#include <core/debug/debugMacros.h>
//TODO placing the implemntation of the draw functions is a hack don't know why it does not detect the implementation in the cpp file?
//TODO scene viewport is wrong
namespace luna 
{
	namespace artemis 
	{
		class renderer
		{
		public:
			 renderer(const ref<vulkan::window>& window);
#ifdef IMGUI_API
			 void beginImGuiScene();
			 void endImGuiScene();
#endif // IMGUI_API

			 void beginScene();
			 void endScene();
			 void update();
			 glm::vec4 normalizeColor(const glm::vec4& color);
			 LN_API void drawLabel(const glm::vec3& position, const glm::vec2& size, const ref<assets::font> font, const std::string labelText, const glm::vec4& color)
			 {
				 //TODO FONT BINDING
				 bindImage(std::dynamic_pointer_cast<assets::image>(font));
				 float xAdvance = 0.0f;
				 const ref<assets::image> spaceGlyph = font->getGlyph('_');
				 const glm::vec2 normalizedDimensions = glm::vec2(1.0f) / getSceneDimensions();
				 for (size_t i = 0; i < labelText.size(); i++)
				 {
					 xAdvance += font->getAdvance(labelText[i]).x * normalizedDimensions.x;
					 drawCharQuadBound({ xAdvance + position.x, position.y + font->getAdvance(labelText[i]).y * normalizedDimensions.y, position.z }, size, font->getGlyph(labelText[i]));
					 if (labelText[i] == ' ')
					 {
						 xAdvance += spaceGlyph->getExtent().x * normalizedDimensions.x;
					 }
					 else
					 {
						 xAdvance += font->getGlyph(labelText[i])->getExtent().x * normalizedDimensions.x;
					 }
				 }

			 }

			 void bindImage(const ref<assets::image> p_image);
			 LN_API void drawCharQuadBound(const glm::vec3 position, const glm::vec2& size, const ref<assets::image> image,const glm::vec4& color = {1.0f,1.0f,1.0f,1.0f})
			 {
				 glm::mat4 transform = glm::mat4(1.0f); // Identity matrix

				 // Set the translation
				 transform[3] = glm::vec4(position, 1.0f);

				 // Set the scale
				 transform[0][0] = size.x;
				 transform[1][1] = size.y;

				 drawQuad({ transform,color,image->getUvCoords(),{*image,true} });
			 }
			 LN_API void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::array<glm::vec2, 4>& textureCoords)
			 {
				 //TODO implement 
			 }
			 LN_API void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
			 {
				 const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
					 * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
				 drawQuad(transform, color);
			 }
			 LN_API void drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<assets::image> image, const std::array<glm::vec2, 4>& textureCoords)
			 {
				 //TODO implement
			 }
			 
			 LN_API void drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<assets::image> image)
			 {
				 glm::mat4 transform = glm::mat4(1.0f); // Identity matrix

				 // Set the translation
				 transform[3] = glm::vec4(position, 1.0f);

				 // Set the scale
				 transform[0][0] = size.x;
				 transform[1][1] = size.y;

				 drawQuad(transform, { 1,1,1,1 }, image);

			 }
			 LN_API void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const ref<assets::image> image, const std::array<glm::vec2, 4>& textureCoords)
			 {
			 }
			 LN_API void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const ref<assets::image> image)
			 {
				 glm::mat4 transform = glm::mat4(1.0f); // Identity matrix

				 // Set the translation
				 transform[3] = glm::vec4(position, 1.0f);

				 // Set the scale
				 transform[0][0] = size.x;
				 transform[1][1] = size.y;

				 drawQuad(transform, color, image);

			 }

			 LN_API void drawQuad(const glm::mat4& transform, const ref<assets::image> image, const std::array<glm::vec2, 4>& textureCoords)
			 {
				 if (image)
				 {
					 for (size_t i = 0; i < renderCmdBuffers.size(); i++)
						 if (renderCmdBuffers[i].bind(image, i)) return drawQuad({ transform,glm::vec4(1,1,1,1),textureCoords,{image->getImageIndex(),false}}); //if an empty texture slot was found then bind it otherwise create new buffer
					 renderCmdBuffers.push_back(renderCommandBuffer(p_allocator, computeDescriptorPool, grapchicsDescriptorPool, sampler, maxFramesInFlight));
					 renderCmdBuffers.back().bind(image, renderCmdBuffers.size());
					 return drawQuad({ transform,glm::vec4(1,1,1,1),image->getUvCoords(),{image->getImageIndex(),false}});
				 }
				 return drawQuad({ transform,glm::vec4(1,1,1,1),textureCoords,{*image , false} });
			 }
			 LN_API void drawQuad(const glm::mat4& transform, const ref<assets::image> image)
			 {
				 drawQuad(transform, image, *image);
			 }

			 LN_API void drawQuad(const glm::mat4& transform, const glm::vec4& color, const ref<assets::image> image,const std::array<glm::vec2,4>& textureCoords)
			 {
				 if (image)
				 {
					 for (size_t i = 0; i < renderCmdBuffers.size(); i++)
						 if (renderCmdBuffers[i].bind(image, i)) return drawQuad({ transform,color,textureCoords,{image->getImageIndex(),false}});
					 renderCmdBuffers.push_back(renderCommandBuffer(p_allocator, computeDescriptorPool, grapchicsDescriptorPool, sampler, maxFramesInFlight));
					 renderCmdBuffers.back().bind(image, renderCmdBuffers.size());
					 return drawQuad({ transform,color,textureCoords,{image->getImageIndex(),false} });
				 }
				 return drawQuad({ transform,color,textureCoords,{image->getImageIndex(), false} });
			 }

			 LN_API void drawQuad(const glm::mat4& transform, const glm::vec4& color, const ref<assets::image> image)
			 { drawQuad(transform, color, image, *image); }

			 LN_API void drawQuad(const glm::mat4& transform, const glm::vec4& color1)
			 { drawQuad(transform,color1,blankImage);}

			 LN_API void drawQuad(const drawCommand& command)
			 {
				 if (currentBuffer->addCommand(command))
				 {
					 LN_CORE_INFO("rip currentBuffer full"); //TODO: needs fixing.
				 }
			 }
#ifdef IMGUI_API
			 void setSceneDimensions(ImVec2 size) 
			 {
				 if (size.x != imguiSceneSize.x || size.y != imguiSceneSize.y) 
				 {
					 imguiSceneSize.x = size.x; imguiSceneSize.y = size.y;
					 resized = true;
				 }
			 }

			 ImTextureID registerImGuiImage(const ref<assets::image> image);
			

			 void unregisterImGuiImage(ImTextureID imGuiImageHandle);
			 LN_API ImTextureID getWindowImage();
			 LN_API ImGuiContext* getImGuiContext();
			 LN_API void setTransition(bool transition) { this->transition = transition; }
#endif // !IMGUI_API
			 LN_API const glm::vec2 getSceneMousePos() const
			 {
				 //TODO mousepose
				 return sceneMousePos;
			 }
			 LN_API void setSceneMousePos(glm::vec2& sceneMousePos) 
			 {
				 //TODO mousepose
				 this->sceneMousePos = sceneMousePos;
			 }
			 const glm::vec2 getSceneDimensions() const
			 {
#ifndef IMGUI_API
				 return { p_window->windowSpec.width, p_window->windowSpec.height };
#else 
				 return  imguiSceneSize;
#endif // !IMGUI_API
			 }
			 LN_API void flush();

		private:
			void setUpComputePipeline();
			void setUpGraphicsPipeline();
#ifdef IMGUI_API
			void setUpImguiPipeline();
#endif // IMGUI_API

			void recordCommands();
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
			
			ref<assets::image> blankImage;

			ref<vulkan::window> p_window;
#ifdef IMGUI_API
			std::vector<frameBuffer> imguiFrameBuffers;
			ref<renderPass> p_imguiRenderPass;
			glm::vec2 imguiSceneSize = { 100.0f,100.0f }; //set default because otherwise the float max will be taken wich is to big for any gpu to render to!!!
			glm::vec2 sceneMousePos;
			bool transition = false;
			bool resized = false;
			std::vector<image> frameBufferImages;    
			std::vector<ImTextureID> frambufferGuiImages;

			std::vector<ref<assets::image>> imguiEnabledImages;
			std::vector<ref<assets::image>> imguiEnabledImagesResize;

#endif //IMGUI_API

		};
	}
}

#endif
