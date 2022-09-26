#include "shaderCompiler.h"
#include "lnpch.h"
namespace luna
{
	namespace utils
	{
		shaderCompiler::shaderCompiler()
		{
			compiler = shaderc_compiler_initialize();
		}
		std::vector<char> shaderCompiler::compile(compileSpec compileSpec)
		{
			shaderc_compilation_result_t result;
			switch (compileSpec.language)
			{
			case luna::utils::SPIR:
				LN_CORE_ERROR("SPIR is not supported");
				break;
			case luna::utils::SPIR_V:
				result = shaderc_compile_into_spv(compiler, compileSpec.source.data(), compileSpec.source.size(), compileSpec.shaderKind, compileSpec.fileName.c_str(), "main", compileSpec.compileOptions);
				break;
			case luna::utils::GLSL:
				LN_CORE_ERROR("GLSL is not supported");
				break;
			default:
				break;
			}
			
			shaderc_compilation_status compileStatus = shaderc_result_get_compilation_status(result);
			if (compileStatus != shaderc_compilation_status::shaderc_compilation_status_success) 
			{
				const char* errorMessage = shaderc_result_get_error_message(result);
				LN_CORE_TRACE("compilation error trying to compile {0} into {1} \n errormesage = {2}", compileSpec.fileName, compileSpec.language,errorMessage);
				return std::vector<char>();
			}
			else
			{
				const char* errorMessage = shaderc_result_get_error_message(result);
				LN_CORE_INFO("dissasambly: {0}", errorMessage);
				const char* compiledSourcePtr = shaderc_result_get_bytes(result);
				return std::vector<char>(compiledSourcePtr, compiledSourcePtr + shaderc_result_get_length(result));
			}
				
		}
	}
}
