#include "renderer2D.h"
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
			renderer2D::statistics stats;
		};

		static renderer2DData rendererData;

		void renderer2D::init()
		{
			LN_PROFILE_SCOPE("renderer2D init");
			
			blankImage = texture::create("./src/assets/media/blank.png");
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
			rendererData.vertexArray = nullptr;
			rendererData.quadIndexBuffer = nullptr;

			rendererData.quadVertexBuffer = nullptr;
			renderer::shutdown();
		}

		void renderer2D::BeginScene()
		{
			LN_PROFILE_SCOPE("begin scene");
			renderer::beginScene();
			rendererData.quadVertexBufferPtr = rendererData.quadVertexBufferBase;
			rendererData.quadIndexCount = 0;
			rendererData.stats.drawCalls = 0;
			rendererData.stats.quadCount = 0;
			rendererData.textures.resize(1);
			rendererData.textures[0] = blankImage->handle();
		}

		void renderer2D::endScene()
		{
			LN_PROFILE_FUNCTION();
			flush();

			renderer::endScene();
		}
		void renderer2D::drawLabel(const glm::vec3& position, const glm::vec2& size, const ref<font>& font, const std::string labelText)
		{
			float xAdvance = 0.1;
			if (!labelText.size()) return;
			for (char character : labelText) {
				ref<texture> glyph = font->getGlyph(character);
				if(glyph) drawQuad({ xAdvance + position.x,position.y,position.z }, size * font->getScale(character), glyph);
				xAdvance += size.x * font->getScale(character).x;
			}
		}
		void renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<texture>& texture)
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
			drawQuad(transform, { 1.0f,1.0f,1.0f,1.0f }, texture);
		}
		void renderer2D::drawQuad(const glm::mat4 transform, const glm::vec4 color, const ref<texture>& texture)
		{
			
			uint64_t handle = textureInBatch(texture->handle());
			if (!handle) {
				rendererData.textures.push_back(texture->handle());
				handle = textureInBatch(texture->handle());
			}
			std::vector<glm::vec2> textureCoords = texture->getUv();
			constexpr size_t quadVertexCount = 4;
			for (size_t i = 0; i < quadVertexCount; i++)
			{
				rendererData.quadVertexBufferPtr->color = color;
				rendererData.quadVertexBufferPtr->vert = transform * rendererData.quadVertexPositions[i];
				rendererData.quadVertexBufferPtr->textureCoords = textureCoords[i];
				rendererData.quadVertexBufferPtr->textureIndex = handle + 0.1;
				rendererData.quadVertexBufferPtr++;
			}
			rendererData.quadIndexCount += 6;
			rendererData.stats.quadCount++;
		}
		void renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) 
		{
			LN_PROFILE_FUNCTION();
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
			drawQuad(transform, color);
		}

		void renderer2D::drawQuad(const glm::vec3& position,const glm::vec2& size)
		{
			LN_PROFILE_FUNCTION();
			drawQuad(position, size, { 255.0f,255.0f,255.0f ,1.0f });
		}
		void renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
		{
			LN_PROFILE_FUNCTION();
			uint64_t handle = textureInBatch(blankImage->handle());
			constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
			constexpr size_t quadVertexCount = 4;
			for (size_t i = 0; i < quadVertexCount; i++)
			{
				rendererData.quadVertexBufferPtr->color = { color.x * 1 / 255.0f,color.y * 1 / 255.0f,color.z * 1 / 255.0f,1.0f };
				rendererData.quadVertexBufferPtr->vert = transform * rendererData.quadVertexPositions[i];	
				rendererData.quadVertexBufferPtr->textureCoords = textureCoords[i];
				rendererData.quadVertexBufferPtr->textureIndex = handle + 0.1;
				rendererData.quadVertexBufferPtr++;
			}
			rendererData.quadIndexCount += 6;
			rendererData.stats.quadCount++;
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
			for (size_t i = 1; i < rendererData.textures.size(); i++)
			{
				if (rendererData.textures[i] == handle) return i;
			}
			return 0;
		}
	}
}
