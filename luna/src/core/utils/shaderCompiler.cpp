#include "shaderCompiler.h"
#include "lnpch.h"
namespace luna
{
	namespace utils
	{
		shaderCompiler::shaderCompiler()
		{
			
		}
		std::vector<char> shaderCompiler::compile(compileSpec compileSpec)
		{
			
			shaderc::Compiler compiler;
			shaderc::SpvCompilationResult compileResult;
			switch (compileSpec.language)
			{
			case luna::utils::SPIR:
				LN_CORE_ERROR("SPIR is not supported");
				break;
			case luna::utils::SPIR_V:
				compileResult = compiler.CompileGlslToSpv(compileSpec.source.data(),compileSpec.source.size(), shaderc_shader_kind::shaderc_glsl_anyhit_shader, compileSpec.fileName.c_str());
				break;
			case luna::utils::GLSL:
				LN_CORE_ERROR("GLSL is not supported");
				break;
			default:
				LN_CORE_ERROR("You need to select a shader language!");
				break;
			}
			
			LN_CORE_TRACE("error messages: {0}",compileResult.GetErrorMessage());
			reflect(std::vector<uint32_t>(compileResult.begin(), compileResult.end()));	
			return std::vector<char>(compileResult.cbegin(), compileResult.cend());
		}
		void shaderCompiler::reflect(const std::vector<uint32_t>& shaderData)
		{
			spirv_cross::Compiler compiler(shaderData);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();
			LN_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
			LN_CORE_TRACE("    {0} resources", resources.sampled_images.size());

			LN_CORE_TRACE("Uniform buffers:");
			for (const auto& resource : resources.uniform_buffers)
			{
				const auto& bufferType = compiler.get_type(resource.base_type_id);
				uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
				uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
				int memberCount = bufferType.member_types.size();

				LN_CORE_TRACE("  {0}", resource.name);
				LN_CORE_TRACE("    Size = {0}", bufferSize);
				LN_CORE_TRACE("    Binding = {0}", binding);
				LN_CORE_TRACE("    Members = {0}", memberCount);
			}
		}
	}
}
