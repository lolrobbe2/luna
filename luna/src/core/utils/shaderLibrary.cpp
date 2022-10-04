#include "shaderLibrary.h"
namespace luna
{
	namespace utils
	{
		void shaderLibrary::init()
		{
			load("src/assets/fragment.glsl",renderer::shaderStageFragment);
			load("src/assets/vertex.glsl",renderer::shaderStageVertex);
		}
		void shaderLibrary::load(const std::string& filePath,const renderer::shaderStage& stage)
		{
			ref<renderer::shader> shader = renderer::shader::create(filePath,stage);
			shaders.insert({ shader->shaderName,shader });
		}
		ref<renderer::shader> shaderLibrary::get(const std::string& shaderName)
		{
			return shaders[shaderName];
		}
	}
}
