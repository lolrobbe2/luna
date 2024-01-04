#include "shaderLibrary.h"
#include <core/debug/debugMacros.h>
namespace luna
{
	namespace utils
	{
		void shaderLibrary::init(const VkDevice* device)
		{
			
			LN_ERR_FAIL_COND_MSG(_device != nullptr && *_device != VK_NULL_HANDLE, "[Artemis] shaderLibrary has already been initialized");
			_device = (VkDevice*)device;
			load("src/assets/fragment.glsl",artemis::shaderStageFragment);
			load("src/assets/vertex.glsl",artemis::shaderStageVertex);
		}
		void shaderLibrary::load(const std::string& filePath,const artemis::shaderStage& stage)
		{
			ref<artemis::shader> shader{ new artemis::shader(_device, filePath, stage) };
			LN_ERR_FAIL_NULL_MSG(shader, "[Artemis] unable to create shader");
			shaders.insert({ *shader,shader });
		}
		ref<artemis::shader> shaderLibrary::get(const std::string& shaderName)
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
