#include "renderer2D.h"
namespace luna
{
	namespace renderer 
	{
		struct renderer2DData
		{
			const uint32_t maxQuads = 2000;
			const uint32_t maxVertices = maxQuads * 4;
			const uint32_t maxIndices = maxQuads * 6;
			ref<vertexArray> vertexArray;

		};

		static renderer2DData rendererData;

	}
}
