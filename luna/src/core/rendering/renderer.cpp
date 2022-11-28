
#include <core/rendering/renderer.h>
//vulkan
#include <core/vulkan/device/vulkanDevice.h>
#include <core/vulkan/rendering/vulkanPipeline.h>
#include <core/vulkan/rendering/vulkanVertexArray.h>
#include <core/vulkan/rendering/vulkanVertexBuffer.h>
#include <core/vulkan/rendering/vulkanIndexBuffer.h>
#include <core/utils/shaderLibrary.h>
#include <imgui_demo.cpp>
namespace luna
{
	namespace renderer
	{
		void renderer::init(const ref<vulkan::window>& window)
		{
			vertex vert1;
			vert1.vert = { 0.0f, -0.5f , 0.0f};
			vertex vert2;
			vert2.vert = { 0.5f, 0.5f , 0.0f };
			vertex vert3;
			vert3.vert = { -0.5f, 0.5f , 0.0f };
			testMesh.vertices = { vert1,vert2,vert3 };
			testMesh.indices = { 0, 1, 2, 2, 3, 0 };
			switch (window->graphicsApi)
			{
			case vulkan::NONE:
				LN_CORE_TRACE("NONE is not supported");
				break;
			case vulkan::OPENGL:
				LN_CORE_TRACE("OPENGL is not supported");
				__debugbreak();
				break;
			case vulkan::VULKAN:
			{
				renderer::rendererDevice = ref<device>(new vulkan::vulkanDevice(window));
				layout.device = rendererDevice;
				layout.pipelineShaders.push_back(utils::shaderLibrary::get("fragment.glsl"));
				layout.pipelineShaders.push_back(utils::shaderLibrary::get("vertex.glsl"));
				renderer::rendererPipeline = ref<pipeline>(new vulkan::vulkanPipeline(layout));
				renderer::gui = ref<gui::vulkanImgui>(new gui::vulkanImgui(renderer::rendererPipeline));
				renderer::vertArray = ref <vertexArray>(new vulkan::vulkanVertexArray());
				ref<vulkan::vulkanVertexBuffer> vertexBuffer(new vulkan::vulkanVertexBuffer(testMesh.vertices.data(), sizeof(vertex) * testMesh.vertices.size()));
				ref<vulkan::vulkanIndexBuffer> indexBuffer(new vulkan::vulkanIndexBuffer(testMesh.indices.data(), sizeof(uint32_t) * testMesh.indices.size()));
				vertArray->addVertexBuffer(vertexBuffer);
				vertArray->setIndexBuffer(indexBuffer);
			}
				break;
			default:
				break;		
			}
		}
		void renderer::createFrame()
		{

		}
		void renderer::newFrame()
		{
			rendererPipeline->begin();

			Submit(vertArray, 6);

			rendererPipeline->end();
			//rendererPipeline->createCommands();
			rendererPipeline->flush();
		}

		void renderer::beginScene()
		{
		}

		void renderer::Submit(const ref<vertexArray>& vertexArray,const uint64_t& indexCount)
		{
			rendererPipeline->drawIndexed(vertexArray,indexCount);
		}

		void renderer::endScene()
		{
		}

	}
}
