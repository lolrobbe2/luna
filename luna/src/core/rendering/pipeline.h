#pragma once
#include <core/rendering/device.h>
#include <core/rendering/shader.h>
#include <core/rendering/vertexArray.h>
namespace luna
{
	namespace renderer
	{
		struct pipelineLayout
		{
			std::vector<ref<shader>> pipelineShaders;
			ref<device> device;

		};
		class pipeline 
		{
		public:
			virtual ~pipeline() {};
			/**
			 * @brief creates graphics a pipeline.
			 * 
			 * \param layout
			 */
			virtual void createPipeline(const pipelineLayout& layout) = 0;
			/**
			 * @brief destroys the graphics pipeline.
			 * 
			 */
			virtual void destroyPipeline() = 0;
		#ifndef DISABLE_IMGUI
			/**
			 * returns current frameBuffer handle.
			 * 
			 * \return 
			 */
			virtual ImTextureID getWindowImage() = 0;
		#endif // !DISABLE_IMGUI
			/**
			 * @brief creates platform specific commands for the rendererPipeline using engine drawCommands.
			 * 
			 */
			virtual void createCommands() = 0;
			/**
			 * @brief starts recording pipeline input.
			 *
			 */
			virtual void begin() = 0;
			/**
			 * @brief ends recording end.
			 * 
			 */
			virtual void end() const = 0;
			/**
			 * @brief flush the renderer pipline and render a frame.
			 * 
			 */
			virtual void flush() = 0;
			/**
			 * @brief draw vertexBuffer with and indexBuffer.
			 * 
			 * \param ref<renderer::vertexArray> vertexArray ref to vertexarray containing vertexBuffers and inexBuffer;
			 * \param indexCount indexCount tp be rendered;
			 */
			virtual void drawIndexed(const ref<vertexArray>& vertexArray, std::vector<uint64_t> textures, int indexCount) = 0;
			/**
			 * @brief binds textures in to there respective textureArrays.
			 * 
			 * \param textureHandles
			 * \param indexSet 
			 */
			virtual void bindTextures(const std::vector<uint64_t> textureHandles,const uint64_t indexSet) = 0;
			/**
			 * @brief clears all drawCommands.
			 *
			 */
			virtual void clear() = 0;


			glm::vec2 getDimensions() { return windowDimensions; };
			glm::vec2 getWindowMousePos() { return windowMousePos; };
			void setDimensions(const glm::vec2& dimensions) { windowDimensions = dimensions; };
			void setWindowMousePos(const glm::vec2& windowMousePos) { this->windowMousePos = windowMousePos; };
			int test;
		#ifndef DISABLE_IMGUI
			ImTextureID currentFrameBuffer;
		#endif // !DISABLE_IMGUI
		protected:
			glm::vec2 windowMousePos;
			glm::vec2 windowDimensions;
		};
	}
}
