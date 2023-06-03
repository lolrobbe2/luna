#pragma once
#include <core/vulkan/window/window.h>
#include <core/rendering/device.h>
#include <core/rendering/pipeline.h>
#ifndef DISABLE_IMGUI
#include <core/vulkan/gui/vulkanImgui.h> //TODO make base gui header
#endif //!DISABLE_IMGUI
#include <core/rendering/vertexArray.h>
namespace luna
{
	namespace renderer
	{
		class renderer
		{
		public:
			/**
			 * @brief the main renderer class.
			 * 
			 * \param window a ref to the main window
			 */
			static void init(const ref<vulkan::window>& window);
			/**
			 * @biref shutsdown the base renderer.
			 * 
			 */
			static void shutdown();
			/**
			 * @brief create a frame in the render pipline based on the given input.
			 * 
			 */
			static void createFrame();
			/**
			 * @brief renders a new frame.
			 * 
			 */
			static void newFrame();
		#ifndef DISABLE_IMGUI
			LN_API _ALWAYS_INLINE_ static ImTextureID getWindowImage();
		#endif // !DISABLE_IMGUI
			/**
			 * @brief starts recording a new scene.
			 * 
			 */
			static void beginScene();
			/**
			 * @brief submits a vertexArray to be rendered containing vertexBuffers and 1 indexBuffer.
			 * @note vertexArray generation is done by the 2D/3D renderer api's respectevly.
			 * \param vertexArray
			 */
			static void Submit(const ref<vertexArray>& vertexArray,const uint64_t& indexCount = 0);
			/**
			 * @brief submits a vertexArray to be rendered containing vertexBuffers and 1 indexBuffer.
			 * @note vertexArray generation is done by the 2D/3D renderer api's respectevly.
			 * \param vertexArray
			 * \param textures (vector of textureHandles)
			 */
			static void Submit(const ref<vertexArray>& vertexArray, std::vector<uint64_t> textures, const uint64_t& indexCount = 0);
			/**
			 * brief ends the scene.
			 * 
			 */
			static void endScene();
			inline static glm::vec2 getSceneDimensions() { return rendererPipeline->getDimensions(); };
		#ifndef DISABLE_IMGUI
			LN_API inline static ImVec2 getSceneGuiDimensions() { return { rendererPipeline->getDimensions().x,rendererPipeline->getDimensions().y }; };
		#endif // !DISABLE_IMGUI
			inline static glm::vec2 getSceneMousePos() { return rendererPipeline->getWindowMousePos(); };
			LN_API inline static void setSceneDimensions(const glm::vec2& sceneDimensions) { rendererPipeline->setDimensions(sceneDimensions); };
			LN_API inline static void setSceneMouse(const glm::vec2& sceneMousePos) { rendererPipeline->setWindowMousePos(sceneMousePos); };
		private:
			inline static ref<device> rendererDevice; //the main renderDevice (GPU handle)
			inline static ref<pipeline> rendererPipeline;// the main graphics pipeline
			inline static renderer::pipelineLayout layout; //the main graphics pipline layout
		#ifndef DISABLE_IMGUI
			inline static ref<gui::vulkanImgui> gui;
		#endif //!DISABLE_IMGUI
			inline static bool Selected;
		};
	}
}
