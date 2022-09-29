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
			double startTime;
			double endtime;
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
			if(compileResult.GetErrorMessage().size() > 0) LN_CORE_ERROR("compile error: {0}",compileResult.GetErrorMessage());
			else LN_CORE_TRACE("reflecting shader: {0}",reflect(std::vector<uint32_t>(compileResult.begin(), compileResult.end())));
			return std::vector<char>(compileResult.cbegin(), compileResult.cend());
		}
		bool shaderCompiler::reflect(const std::vector<uint32_t>& shaderData)
		{
			spirv_cross::Compiler compiler(shaderData);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();
			LN_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
			LN_CORE_TRACE("    {0} push constants", resources.push_constant_buffers.size());
			LN_CORE_TRACE("    {0} resources", resources.sampled_images.size());
			LN_CORE_TRACE("    {0} stage inputs",resources.stage_inputs.size());
			LN_CORE_TRACE("    {0} stage outputs", resources.stage_outputs.size());
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
			LN_CORE_TRACE("push constants:");
			for (const auto& pushConstant : resources.push_constant_buffers)
			{
				LN_CORE_TRACE("  {0}", pushConstant.name);
			}
			//compiler.get_decoration(spv::DecorationDescriptorSet) deduce discriptorset layout;
			LN_CORE_TRACE("stage inputs:");
			for (const auto& stageInput : resources.stage_inputs)
			{
				LN_CORE_TRACE("  {0}", stageInput.name);
				LN_CORE_TRACE("    id = {0}", stageInput.id);
				LN_CORE_TRACE("    name = {0}", getResourceTypeName(stageInput, compiler));
			}
			LN_CORE_TRACE("stage outputs:");
			for (const auto& stageOutput : resources.stage_outputs)
			{
				LN_CORE_TRACE("  {0}", stageOutput.name);
				LN_CORE_TRACE("    id = {0}", stageOutput.id);
				
			}
		
			return true;
		}
		std::string shaderCompiler::getResourceTypeName(const spirv_cross::Resource& resource,const spirv_cross::Compiler& compiler)
		{
			
			spirv_cross::SPIRType::BaseType baseType = compiler.get_type(resource.base_type_id).basetype;
			switch (baseType)
			{
				uint64_t type;
			case spirv_cross::SPIRType::Unknown:
				return "UnKnown";
			case spirv_cross::SPIRType::Void:
				return "Void";
			case spirv_cross::SPIRType::Boolean:
				return "Boolean";
			case spirv_cross::SPIRType::SByte:
				return "SByte_8";
			case spirv_cross::SPIRType::UByte:
				return "Ubyte_8";
			case spirv_cross::SPIRType::Short:
				return "Short";
			case spirv_cross::SPIRType::UShort:
				return "UShort_16";
			case spirv_cross::SPIRType::Int:
				return "Int_32";
			case spirv_cross::SPIRType::UInt:
				return "UIint_32";
			case spirv_cross::SPIRType::Int64:
				return "Int64";
			case spirv_cross::SPIRType::UInt64:
				return "Uint64";
				break;
			case spirv_cross::SPIRType::AtomicCounter:
				return "AtomicCounter";
			case spirv_cross::SPIRType::Half:
				return "Half";
			case spirv_cross::SPIRType::Float:
			{
				uint32_t vecSize = compiler.get_type(resource.base_type_id).vecsize;
				switch (vecSize)
				{
				case 2:
					return "Vec2";
				case 3:
					return "Vec3";
				default:
					return "Float";
				}
			}
			case spirv_cross::SPIRType::Double:
				break;
			case spirv_cross::SPIRType::Struct:
				break;
			case spirv_cross::SPIRType::Image:
				break;
			case spirv_cross::SPIRType::SampledImage:
				break;
			case spirv_cross::SPIRType::Sampler:
				break;
			case spirv_cross::SPIRType::AccelerationStructure:
				break;
			case spirv_cross::SPIRType::RayQuery:
				break;
			case spirv_cross::SPIRType::ControlPointArray:
				break;
			case spirv_cross::SPIRType::Interpolant:
				break;
			case spirv_cross::SPIRType::Char:
				break;
			default:
				return "undetermined";
			}
			
		}
	}
}
