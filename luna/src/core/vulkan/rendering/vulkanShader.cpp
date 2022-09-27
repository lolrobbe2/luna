#include "vulkanShader.h"
#include <core/utils/shaderCompiler.h>
#include "lnpch.h"

namespace luna
{
	namespace vulkan
	{
		vulkanShader::vulkanShader(const std::string& filepath)
		{
			//loading shader stage.
			std::ifstream file;
			file.open(filepath, std::ios::ate | std::ios::binary);
			std::vector<char> buffer;
			if (file.is_open())
			{
				size_t fileSize = (size_t)file.tellg();
				buffer.resize(fileSize);
				file.seekg(0);
				file.read(buffer.data(), fileSize);
				file.close();
				shaderName = std::filesystem::path{ filepath }.filename().string();
			}
			//compile stage.
			utils::shaderCompiler compiler;
			utils::compileSpec compileSpec;
			compileSpec.fileName = shaderName;
			compileSpec.compileOptions = shaderc_compile_options_t();
			compileSpec.shaderKind = shaderc_shader_kind::shaderc_glsl_anyhit_shader;
			compileSpec.source = buffer;
			compiler.compile(compileSpec);
			//auto atributes needs to be done

		}

		void vulkanShader::bind() const
		{
		}

		void vulkanShader::unbind() const
		{
		}

	}
}