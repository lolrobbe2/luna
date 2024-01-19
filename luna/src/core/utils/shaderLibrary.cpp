#include "shaderLibrary.h"
#include <core/debug/debugMacros.h>
namespace luna
{
	namespace artemis
	{
		void shaderLibrary::init(const VkDevice* device)
		{
			
			LN_ERR_FAIL_COND_MSG(_device != nullptr && *_device != VK_NULL_HANDLE, "[Artemis] shaderLibrary has already been initialized");
			_device = (VkDevice*)device;
			load("src/assets/fragment.glsl",shaderStageFragment);
			load("src/assets/vertex.glsl",shaderStageVertex);
			load("src/assets/quadVertexGenerator.glsl", shaderStageCompute);
		}
		void shaderLibrary::load(const std::string& filePath,const shaderStage& stage)
		{
			ref<shader> shader = createRef<artemis::shader>(_device, filePath, stage);
			LN_ERR_FAIL_NULL_MSG(shader, "[Artemis] unable to create shader");
			shaders.insert({ *shader,shader });
		}
		ref<shader> shaderLibrary::get(const std::string& shaderName)
		{
			return shaders[shaderName];
		}
		void shaderLibrary::shutdown()
		{
			for(auto& [key,shader] : shaders)
			{
				shader.reset();
			}
		}
	}
}
