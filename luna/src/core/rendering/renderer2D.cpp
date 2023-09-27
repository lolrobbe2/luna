#include "renderer2D.h"
#include <core/assets/assetManager.h>
#include <core/debug/debugMacros.h>
#include <core/debug/typedefs.h>
#define RGB255_TO_RGB1(color) { color.x * 0.00392156862f,color.y * 0.00392156862f,color.z * 0.00392156862f,1.0f };
namespace luna
{
	namespace renderer 
	{
		struct quadVertex
		{
			glm::vec4 vert;
			glm::vec4 color;
			glm::vec2 textureCoords;
			float textureIndex;
			float text;
		};
		struct renderer2DData
		{
			const uint32_t maxQuads = 2000;
			const uint32_t maxVertices = maxQuads * 4;
			const uint32_t maxIndices = maxQuads * 6;
			ref<vertexArray> vertexArray  = vertexArray::create();
			ref<vertexBuffer> quadVertexBuffer;
			ref<indexBuffer> quadIndexBuffer;
			uint32_t quadVertexCount = 0;
			uint32_t quadIndexCount = 0;
			quadVertex* quadVertexBufferBase = nullptr;
			quadVertex* quadVertexBufferPtr = nullptr;
			quadVertex* quadVertexBufferPreviousPtr = nullptr;
			uint32_t* quadIndices = nullptr;
			std::vector<uint64_t> textures;
			glm::vec4 quadVertexPositions[4];
			glm::vec4 quadCharVertexPositions[4];
			renderer2D::statistics stats;
			float isText = false;
		};

		static renderer2DData rendererData;

		_ALWAYS_INLINE_ static bool outOfView(const glm::vec4& vert,const glm::vec4& bounds = {-1.0f,1.0f,-1.0f,1.0f})
		{
			return (vert.x < bounds.x || vert.x > bounds.y) || (vert.y < bounds.z || vert.y > bounds.w);
		}

		void renderer2D::init()
		{
			LN_PROFILE_SCOPE("renderer2D init");
			ref<assets::asset> blankImageAsset = assets::assetManager::getAsset(assets::assetManager::importAsset("src/assets/media/blank.png", assets::texture));
			blankImage = std::dynamic_pointer_cast<texture>(blankImageAsset);
			rendererData.quadVertexBuffer = vertexBuffer::create(rendererData.maxVertices * sizeof(quadVertex));
			rendererData.quadVertexBufferBase = (quadVertex*)rendererData.quadVertexBuffer->data;
			rendererData.quadVertexBufferPtr = rendererData.quadVertexBufferBase;
			rendererData.quadVertexBufferPreviousPtr = rendererData.quadVertexBufferBase;
			rendererData.quadIndexBuffer = indexBuffer::create(rendererData.maxIndices);
			rendererData.quadIndices = rendererData.quadIndexBuffer->data;

			rendererData.vertexArray->addVertexBuffer(rendererData.quadVertexBuffer);
			rendererData.vertexArray->setIndexBuffer(rendererData.quadIndexBuffer);

			rendererData.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			rendererData.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
			rendererData.quadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
			rendererData.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

			rendererData.quadCharVertexPositions[0] = { 0.0f, 0.0f, 0.0f, 1.0f };
			rendererData.quadCharVertexPositions[1] = { 1.0f, 0.0f, 0.0f, 1.0f };
			rendererData.quadCharVertexPositions[2] = { 1.0f,  1.0f, 0.0f, 1.0f };
			rendererData.quadCharVertexPositions[3] = { 0.0f,  1.0f, 0.0f, 1.0f };

			uint32_t offset = 0;
			for (uint32_t i = 0; i < rendererData.maxIndices; i += 6)
			{
				rendererData.quadIndices[i + 0] = offset + 0;
				rendererData.quadIndices[i + 1] = offset + 1;
				rendererData.quadIndices[i + 2] = offset + 2;

				rendererData.quadIndices[i + 3] = offset + 2;
				rendererData.quadIndices[i + 4] = offset + 3;
				rendererData.quadIndices[i + 5] = offset + 0;

				offset += 4;
			}
		}

		void renderer2D::shutdown()
		{
			LN_PROFILE_SCOPE("shutdown renderer2D");
			renderer::beginScene(); //to clear draw commands
			renderer::endScene();
			rendererData.vertexArray.~shared_ptr();
			rendererData.quadIndexBuffer.~shared_ptr();

			rendererData.quadVertexBuffer.~shared_ptr();
			blankImage.~shared_ptr();
			renderer::shutdown();
		}

		void renderer2D::BeginScene()
		{
			LN_PROFILE_SCOPE("begin scene");
			renderer::beginScene();
			rendererData.quadVertexBufferPtr = rendererData.quadVertexBufferBase;
			//we use memset because it gets fully transformed in to pure assembly an is thus the fastest way to reset the vertexBUffer.
			//NEEDED FOR TEXT RENDERING!
			rendererData.quadIndexCount = 0;
			rendererData.stats.drawCalls = 0;
			rendererData.stats.quadCount = 0;
			rendererData.textures.resize(0);
			rendererData.textures.push_back(blankImage->handle());
		}

		void renderer2D::endScene()
		{
			LN_PROFILE_FUNCTION();
			flush();
			rendererData.quadVertexBufferPreviousPtr =  rendererData.quadVertexBufferPtr;
			renderer::endScene();
		}
		bool renderer2D::drawLabel(const glm::vec3& position, const glm::vec2& size, const ref<font>& font, const std::string labelText, const glm::vec4& bounds, uint8_t& startOutBounds)
		{

			/**
			 * font rendering: quad origin is at top left.
			 * 1) xadvance += glyph advance.x 
			 * 2) draw glyph (+ y advance)
			 * 3) xadvance += relative glyph width.
			 * 
			 */
			LN_PROFILE_FUNCTION();
			//LN_ERR_FAIL_NULL_V_MSG(font, true, "font was invalid!");
			float xAdvance = 0.0f;
			rendererData.isText = true;
			const ref<texture> spaceGlyph = font->getGlyph('_');
			const float normalizedDimensionX = 1.0f / renderer::getSceneDimensions().x * size.x;
			const float normalizedDimensionY = 1.0f / renderer::getSceneDimensions().y * size.y;
			uint8_t outOfBounds = 0;
			for (size_t i = 0; i < labelText.size(); i++) 
			{
				const ref<texture> glyph = font->getGlyph(labelText[i]);
				const glm::vec2 dimensions = { glyph->getWidth(),glyph->getHeight() };
				xAdvance += font->getAdvance(labelText[i]).x * normalizedDimensionX;
				if (drawCharQuad({ xAdvance + position.x,position.y + font->getAdvance(labelText[i]).y * normalizedDimensionY,position.z }, size, glyph, checkHandle(font->handle()), { 1.0f,1.0f,1.0f,1.0f }, bounds)) 
				{
					outOfBounds++;
					startOutBounds = i;
				}
				if (labelText[i] == ' ') xAdvance += spaceGlyph->getWidth() * normalizedDimensionX;
				else xAdvance += glyph->getWidth() * normalizedDimensionX;
			}
			
			rendererData.isText = false;
			return outOfBounds == labelText.size();
		} 
		bool renderer2D::drawCharQuad(const glm::vec3& position, const glm::vec2& size, const ref<texture>& texture,uint64_t handleIndex,glm::vec4 color,glm::vec4 bounds)
		{
			LN_PROFILE_FUNCTION();
			constexpr size_t quadVertexCount = 4;

			const glm::vec2 dimensions = { texture->getWidth(),texture->getHeight() };
			const glm::vec2 normalizedDimensions = dimensions / renderer::getSceneDimensions();
			const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f })
				* glm::scale(glm::mat4(1.0f), { normalizedDimensions.x, normalizedDimensions.y, 1.0f });
			const std::vector<glm::vec2> textureCoords = texture->getUv();
			uint8_t outOfBounds = 0;
			for (size_t i = 0; i < quadVertexCount; i++)
			{
				rendererData.quadVertexBufferPtr->color = color;
				rendererData.quadVertexBufferPtr->vert = transform * rendererData.quadCharVertexPositions[i];
				outOfBounds += outOfView(rendererData.quadVertexBufferPtr->vert,bounds);
				rendererData.quadVertexBufferPtr->textureCoords = textureCoords[i];
				rendererData.quadVertexBufferPtr->textureIndex = handleIndex;
				rendererData.quadVertexBufferPtr->text = rendererData.isText;
				rendererData.quadVertexBufferPtr++;
			}
			if (outOfBounds)
			{
				rendererData.quadVertexBufferPtr -= outOfBounds;
				return true;
			}
			rendererData.quadIndexCount += 6;
			rendererData.stats.quadCount++;
			return false;
			
		}
		bool renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<texture>& texture)
		{
			const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
			return drawQuad(transform, { 1.0f,1.0f,1.0f,1.0f }, texture);
		}
		bool renderer2D::drawQuad(const glm::mat4 transform, const glm::vec4 color, const ref<texture>& texture)
		{
			LN_PROFILE_FUNCTION();
			const std::vector<glm::vec2> textureCoords = texture->getUv();
			constexpr size_t quadVertexCount = 4;
			uint8_t outOfBounds = 0;
			for (size_t i = 0; i < quadVertexCount; i++)
			{
				rendererData.quadVertexBufferPtr->color = color;
				rendererData.quadVertexBufferPtr->vert = transform * rendererData.quadVertexPositions[i];
				outOfBounds += outOfView(rendererData.quadVertexBufferPtr->vert);
				rendererData.quadVertexBufferPtr->textureCoords = textureCoords[i];
				rendererData.quadVertexBufferPtr->textureIndex = checkHandle(texture->handle());
				rendererData.quadVertexBufferPtr->text = rendererData.isText;
				rendererData.quadVertexBufferPtr++;
			}
			if (outOfBounds == 4)
			{
				rendererData.quadVertexBufferPtr -= 4;
				return true;
			} 
			rendererData.quadIndexCount += 6;
			rendererData.stats.quadCount++;
			return false;
		}
		bool renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) 
		{
			LN_PROFILE_FUNCTION();
			const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
			return drawQuad(transform, color);
		}

		bool renderer2D::drawQuad(const glm::vec3& position,const glm::vec2& size)
		{
			LN_PROFILE_FUNCTION();
			return drawQuad(position, size, { 255.0f,255.0f,255.0f ,1.0f });
		}
		bool renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
		{
			LN_PROFILE_FUNCTION();
			uint64_t handle = 0;
			constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
			constexpr size_t quadVertexCount = 4;
			uint8_t outOfBounds = 0;
			for (size_t i = 0; i < quadVertexCount; i++)
			{
				rendererData.quadVertexBufferPtr->color = RGB255_TO_RGB1(color);
				rendererData.quadVertexBufferPtr->vert = transform * rendererData.quadVertexPositions[i];	
				outOfBounds += outOfView(rendererData.quadVertexBufferPtr->vert);
				rendererData.quadVertexBufferPtr->textureCoords = textureCoords[i];
				rendererData.quadVertexBufferPtr->textureIndex = handle;
				rendererData.quadVertexBufferPtr->text = rendererData.isText;
				rendererData.quadVertexBufferPtr++;
			}
			if (outOfBounds == 4) 
			{
				rendererData.quadVertexBufferPtr -= 4;
				return true;
			}
			rendererData.quadIndexCount += 6;
			rendererData.stats.quadCount++;
			return false;
		}
		void renderer2D::flush()
		{
			LN_PROFILE_FUNCTION();
			std::ptrdiff_t numElementsToClear = rendererData.quadVertexBufferPreviousPtr - rendererData.quadVertexBufferPtr;
			if(numElementsToClear > 0) std::fill(rendererData.quadVertexBufferPtr, rendererData.quadVertexBufferPtr + numElementsToClear, quadVertex());
			renderer::Submit(rendererData.vertexArray ,rendererData.textures, rendererData.quadIndexCount);
			rendererData.stats.drawCalls++;
		}

		renderer2D::statistics renderer2D::getStats()
		{
			return rendererData.stats;
		}
		uint64_t renderer2D::checkHandle(const uint64_t handle)
		{
			uint64_t handleIndex = textureInBatch(handle);
			if (!handleIndex) {
				handleIndex = rendererData.textures.size();
				rendererData.textures.push_back(handle);
			}
			return handleIndex;
		}
		uint64_t renderer2D::textureInBatch(const uint64_t& handle)
		{
			LN_PROFILE_FUNCTION();
			auto it = std::find_if(rendererData.textures.begin(), rendererData.textures.end(), [&](size_t textureHandle) {return textureHandle == handle; });
			return rendererData.textures.end() - it; //zero wil be return when the texture is not in the bacth and subsequently reaches rendererData.textures.end() and returns 0.
		}
	}
}
