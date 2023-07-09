#include "renderer2D.h"
#include <core/assets/assetManager.h>
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
			uint32_t* quadIndices = nullptr;
			std::vector<uint64_t> textures;
			glm::vec4 quadVertexPositions[4];
			glm::vec4 quadCharVertexPositions[4];
			renderer2D::statistics stats;
			float isText = false;
		};

		static renderer2DData rendererData;

		static bool outOfView(const glm::vec4& vert)
		{
			return (vert.x > 1.0 || vert.x < -1.0) || (vert.y > 1.0 || vert.y < -1.0);
		}

		void renderer2D::init()
		{
			LN_PROFILE_SCOPE("renderer2D init");
			ref<assets::asset> blankImageAsset = assets::assetManager::getAsset(assets::assetManager::importAsset("src/assets/media/blank.png", assets::texture));
			blankImage = std::dynamic_pointer_cast<texture>(blankImageAsset);
			rendererData.quadVertexBuffer = vertexBuffer::create(rendererData.maxVertices * sizeof(quadVertex));
			rendererData.quadVertexBufferBase = (quadVertex*)rendererData.quadVertexBuffer->data;
			rendererData.quadVertexBufferPtr = rendererData.quadVertexBufferBase;
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
			//memset(rendererData.quadVertexBufferBase, 0, rendererData.maxVertices * sizeof(quadVertex));
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

			renderer::endScene();
		}
		bool renderer2D::drawLabel(const glm::vec3& position, const glm::vec2& size, const ref<font>& font, const std::string labelText)
		{

			/**
			 * font rendering: quad origin is at top left.
			 * 1) xadvance += glyph advance.x 
			 * 2) draw glyph (+ y advance)
			 * 3) xadvance += relative glyph width.
			 * 
			 */
			LN_PROFILE_FUNCTION();
			float xAdvance = 0.0f;
			rendererData.isText = true;
			const ref<texture> spaceGlyph = font->getGlyph('_');
			uint64_t handleIndex = textureInBatch(font->handle());
			if (!handleIndex) {
				rendererData.textures.push_back(font->handle());
				handleIndex = rendererData.textures.size() - 1;
			}
			const float normalizedDimensionX = 1.0f / renderer::getSceneDimensions().x * size.x;
			const float normalizedDimensionY = 1.0f / renderer::getSceneDimensions().y * size.y;
			if (!font) return false;
			uint8_t outOfBounds = 0;
			for (size_t i = 0; i < labelText.size(); i++) 
			{
				const ref<texture> glyph = font->getGlyph(labelText[i]);
				const glm::vec2 dimensions = { glyph->getWidth(),glyph->getHeight() };
				xAdvance += font->getAdvance(labelText[i]).x * normalizedDimensionX;
				outOfBounds += drawCharQuad({ xAdvance + position.x,position.y + font->getAdvance(labelText[i]).y * normalizedDimensionY,position.z }, size, glyph, handleIndex);
				if (labelText[i] == ' ') xAdvance += spaceGlyph->getWidth() * normalizedDimensionX;
				else xAdvance += glyph->getWidth() * normalizedDimensionX;
			}
			rendererData.isText = false;
			return outOfBounds == labelText.size();
		} 
		bool renderer2D::drawCharQuad(const glm::vec3& position, const glm::vec2& size, const ref<texture>& texture,uint64_t handleIndex)
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
				rendererData.quadVertexBufferPtr->color = { 1.0f,1.0f,1.0f,1.0f };
				rendererData.quadVertexBufferPtr->vert = transform * rendererData.quadCharVertexPositions[i];
				outOfBounds += outOfView(rendererData.quadVertexBufferPtr->vert);
				rendererData.quadVertexBufferPtr->textureCoords = textureCoords[i];
				rendererData.quadVertexBufferPtr->textureIndex = handleIndex;
				rendererData.quadVertexBufferPtr->text = rendererData.isText;
				rendererData.quadVertexBufferPtr++;
			}
			if (outOfBounds == 4)
			{
				rendererData.quadVertexBufferPtr -= 4;
				//memset(rendererData.quadVertexBufferPtr, 0, 4 * sizeof(quadVertex));
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
			uint64_t handle = textureInBatch(texture->handle());
			if (!handle) {
				rendererData.textures.push_back(texture->handle());
				handle = rendererData.textures.size() - 1;
			}
			const std::vector<glm::vec2> textureCoords = texture->getUv();
			constexpr size_t quadVertexCount = 4;
			uint8_t outOfBounds = 0;
			for (size_t i = 0; i < quadVertexCount; i++)
			{
				rendererData.quadVertexBufferPtr->color = color;
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
				//memset(rendererData.quadVertexBufferPtr, 0, 4 * sizeof(quadVertex));
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
				rendererData.quadVertexBufferPtr->color = { color.x * 1 / 255.0f,color.y * 1 / 255.0f,color.z * 1 / 255.0f,1.0f };
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
				//memset(rendererData.quadVertexBufferPtr, 0, 4 * sizeof(quadVertex));
				return true;
			}
			rendererData.quadIndexCount += 6;
			rendererData.stats.quadCount++;
			return false;
		}
		void renderer2D::flush()
		{
			LN_PROFILE_FUNCTION();
			renderer::Submit(rendererData.vertexArray ,rendererData.textures, rendererData.quadIndexCount);
			rendererData.stats.drawCalls++;
		}

		renderer2D::statistics renderer2D::getStats()
		{
			return rendererData.stats;
		}
		uint64_t renderer2D::textureInBatch(const uint64_t& handle)
		{
			LN_PROFILE_FUNCTION();
			for (size_t i = 1; i < rendererData.textures.size(); i++)
			{
				if (rendererData.textures[i] == handle) return i;
			}
			return 0;
		}

	
	}
}
