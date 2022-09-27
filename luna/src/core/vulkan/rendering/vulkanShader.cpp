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
			shaderName = std::filesystem::path{ filepath }.filename().string();
			file.open(filepath, std::ios::ate | std::ios::binary);
			std::vector<char> buffer;
			if (file.is_open())
			{
				size_t fileSize = (size_t)file.tellg();
				buffer.resize(fileSize);
				file.seekg(0);
				file.read(buffer.data(), fileSize);
				file.close();
				//compile stage.
				utils::shaderCompiler compiler;
				utils::compileSpec compileSpec;
				compileSpec.fileName = shaderName;
				compileSpec.compileOptions = shaderc_compile_options_t();
				compileSpec.shaderKind = shaderc_shader_kind::shaderc_glsl_anyhit_shader;
				compileSpec.source = buffer;
				compiler.compile(compileSpec);
			}
			else LN_CORE_ERROR("file could not be loaded: {0}", filepath);
			
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