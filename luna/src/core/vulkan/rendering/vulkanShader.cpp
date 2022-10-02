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
				shaderSource = compiler.compile(compileSpec);
				spirv_cross::Compiler Compiler(shaderSource);
				spirv_cross::ShaderResources resources = Compiler.get_shader_resources();
				createLayout();
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
		void vulkanShader::createLayout()
		{
			LN_CORE_TRACE("creating shader Layout");
			spirv_cross::Compiler compiler(shaderSource);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();
			for(const auto& resource : resources.uniform_buffers)
			{
				shaderLayout.push_back(getShaderResource(resource, shaderSource,renderer::uniformBuffers));
			}
			for (const auto& resource : resources.push_constant_buffers)
			{
				shaderLayout.push_back(getShaderResource(resource, shaderSource,renderer::pushConstantBuffers));
			}
			for (const auto& resource : resources.stage_inputs)
			{
				shaderLayout.push_back(getShaderResource(resource, shaderSource,renderer::stageInputs));
			}
			for (const auto& resource : resources.stage_outputs)
			{
				shaderLayout.push_back(getShaderResource(resource, shaderSource,renderer::stageOutputs));
			}
			LN_CORE_TRACE("created shaderLayout");
		}

		renderer::shaderResource vulkanShader::getShaderResource(const spirv_cross::Resource& _shaderResource,const std::vector<uint32_t>& shaderSource, renderer::typeClass typeClass)
		{
			spirv_cross::Compiler compiler(shaderSource);
			spirv_cross::SPIRType::BaseType baseType = compiler.get_type(_shaderResource.base_type_id).basetype;
			const spirv_cross::SPIRType& type = compiler.get_type(_shaderResource.type_id);
			renderer::shaderResource resource;
			resource.name = _shaderResource.name;
			LN_CORE_INFO("resource base type = {0}", baseType);
			LN_CORE_TRACE("resource resource type = {0}", type.self);
			switch (baseType)
			{
			case spirv_cross::SPIRType::Unknown:
				break;
			case spirv_cross::SPIRType::Void:
				break;
			case spirv_cross::SPIRType::Boolean:
				break;
			case spirv_cross::SPIRType::SByte:
				break;
			case spirv_cross::SPIRType::UByte:
				break;
			case spirv_cross::SPIRType::Short:
				break;
			case spirv_cross::SPIRType::UShort:
				break;
			case spirv_cross::SPIRType::Int:
				break;
			case spirv_cross::SPIRType::UInt:
				break;
			case spirv_cross::SPIRType::Int64:
				break;
			case spirv_cross::SPIRType::UInt64:
				break;
			case spirv_cross::SPIRType::AtomicCounter:
				break;
			case spirv_cross::SPIRType::Half:
				break;
			case spirv_cross::SPIRType::Float:
				
				switch (compiler.get_type(_shaderResource.base_type_id).vecsize)
				{
				case 2:
					resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
					resource.type = renderer::Vec2;
					break;
				case 3:
					resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
					resource.type = renderer::Vec3;
					break;
				case 4:
					resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
					resource.type = renderer::Vec4;
					break;

				default:
					resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
					resource.type = renderer::Float;
					break;
					
				}
				switch (compiler.get_type(_shaderResource.base_type_id).columns)
				{
				case 2:
					resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
					resource.type = renderer::mat2;
					break;
				case 3:
					resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
					resource.type = renderer::mat3;
					break;
				case 4:
					resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
					resource.type = renderer::mat4;
					break;
				}
				break;
			case spirv_cross::SPIRType::Double:
				break;
			case spirv_cross::SPIRType::Struct:
				if (typeClass == renderer::uniformBuffers)
				{
					resource.binding = compiler.get_decoration(_shaderResource.id, spv::DecorationBinding);
					resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
					resource.type = renderer::Uniform;
					uint8_t counter = 0;
					auto& type = compiler.get_type(_shaderResource.base_type_id);
					for (const auto& member : compiler.get_type(_shaderResource.base_type_id).member_types)
					{
						
						spirv_cross::Resource memberResource;
						memberResource.base_type_id = member;
						memberResource.type_id = member;
						memberResource.id = compiler.get_type(member).self;
						
						memberResource.name = compiler.get_member_name(type.self, counter);
						counter++;
						LN_CORE_TRACE("member name = {0}", memberResource.name);
						resource.members.push_back(getShaderResource(memberResource, shaderSource, typeClass));
					}
				}
				else if(typeClass == renderer::pushConstantBuffers)
				{
					resource.binding = compiler.get_decoration(_shaderResource.id, spv::DecorationBinding);
					resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
					resource.type = renderer::PushConstant;
				}
				break;
			case spirv_cross::SPIRType::Image:
				resource.binding = compiler.get_decoration(_shaderResource.id, spv::DecorationBinding);
				resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
				resource.type = renderer::Image;
				break;
			case spirv_cross::SPIRType::SampledImage:
				resource.binding = compiler.get_decoration(_shaderResource.id, spv::DecorationBinding);
				resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
				resource.type = renderer::SampledImage;
				break;
			case spirv_cross::SPIRType::Sampler:
				resource.binding = compiler.get_decoration(_shaderResource.id, spv::DecorationBinding);
				resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
				resource.type = renderer::Sampler;
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
				break;
			}
			return resource;
		}
	}
}