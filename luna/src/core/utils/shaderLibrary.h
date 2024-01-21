#pragma once
#include <core/artemis/rendering/shader.h>
namespace luna
{
	namespace artemis
	{
		class LN_API shaderLibrary
		{
		public:
			static void init(const VkDevice* device);
			static void load(const std::string& filePath, const shaderStage& stage);
			static ref<shader> get(const std::string& shaderName);
			static void shutdown();
		private:
			inline static VkDevice* _device;
			inline static std::unordered_map<std::string, ref<shader>> shaders;
			friend class shader;
		};
	}
}


