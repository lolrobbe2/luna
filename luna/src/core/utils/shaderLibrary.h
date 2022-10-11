#pragma once
#include<core/rendering/shader.h>
namespace luna
{
	namespace utils
	{
		class shaderLibrary
		{
		public:
			static void init();
			static void load(const std::string& filePath, const renderer::shaderStage& stage);
			static ref<renderer::shader> get(const std::string& shaderName);
		private:
			inline static std::unordered_map<std::string, ref<renderer::shader>> shaders;
		};
	}
}


