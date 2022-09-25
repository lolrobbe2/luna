#include "vulkanShader.h"
#include "lnpch.h"

namespace luna
{
	namespace vulkan
	{
		vulkanShader::vulkanShader(const std::string& filepath)
		{
			std::ifstream file;
			file.open(filepath, std::ios::ate | std::ios::binary);
			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);
			file.seekg(0);
			file.read(buffer.data(), fileSize);
			file.close();
			shaderName = std::filesystem::path{ filepath }.filename().string();
			
			
			
		}
		void vulkanShader::bind()
		{

		}
		void vulkanShader::unbind()
		{
		}
	}
}
