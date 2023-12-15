#pragma once
#include <core/artemis/rendering/shader.h>
namespace luna
{
	namespace utils
	{
		class shaderLibrary
		{
		public:
			static void init(const VkDevice* device);
			static void load(const std::string& filePath, const artemis::shaderStage& stage);
			static ref<artemis::shader> get(const std::string& shaderName);
		private:
			inline static VkDevice* _device;
			inline static std::unordered_map<std::string, ref<artemis::shader>> shaders;
		};
	}
}


