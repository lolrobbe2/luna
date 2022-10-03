#include "shaderLibrary.h"
namespace luna
{
	namespace utils
	{
		void shaderLibrary::init()
		{
			load("src/assets/fragment.glsl");
			load("src/assets/vertex.glsl");
		}
		void shaderLibrary::load(const std::string& filePath)
		{
			ref<renderer::shader> shader = renderer::shader::create(filePath);
			shaders.insert({ shader->shaderName,shader });
		}
		ref<renderer::shader> shaderLibrary::get(const std::string& shaderName)
		{
			return shaders[shaderName];
		}
	}
}
