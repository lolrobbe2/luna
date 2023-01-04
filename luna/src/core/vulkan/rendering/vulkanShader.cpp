#include "vulkanShader.h"
#include <core/utils/shaderCompiler.h>
#include "lnpch.h"

namespace luna
{
	namespace vulkan
	{
		vulkanShader::vulkanShader(const std::string& filepath,const renderer::shaderStage& stage)
		{
			//loading shader stage.
			LN_PROFILE_FUNCTION();
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
				compileSpec.source = buffer;
				this->stage = stage;
				switch (stage)
				{
				case luna::renderer::shaderStageVertex:
					compileSpec.shaderKind = shaderc_shader_kind::shaderc_vertex_shader;
					break;
				case luna::renderer::shaderStageTessellationControl:
					compileSpec.shaderKind = shaderc_shader_kind::shaderc_tess_control_shader;
					break;
				case luna::renderer::shaderStageTessellationEvaluation:
					compileSpec.shaderKind = shaderc_shader_kind::shaderc_tess_evaluation_shader;
					break;
				case luna::renderer::shaderStageGeometry:
					compileSpec.shaderKind = shaderc_shader_kind::shaderc_geometry_shader;
					break;
				case luna::renderer::shaderStageFragment:
					compileSpec.shaderKind = shaderc_shader_kind::shaderc_fragment_shader;
					break;
				case luna::renderer::shaderStageCompute:
					compileSpec.shaderKind = shaderc_shader_kind::shaderc_compute_shader;
					break;
				default:
					compileSpec.shaderKind = shaderc_shader_kind::shaderc_anyhit_shader;
					break;
				}
				shaderSrc = compiler.compile(compileSpec);
				spirv_cross::Compiler Compiler(shaderSrc);
				spirv_cross::ShaderResources resources = Compiler.get_shader_resources();
				createLayout();
			}
			else LN_CORE_ERROR("file could not be loaded: {0}", filepath);
		}

		void vulkanShader::bind() const
		{
		}

		void vulkanShader::unbind() const
		{
		}
		void vulkanShader::createLayout()
		{
			LN_PROFILE_FUNCTION();
			LN_CORE_TRACE("creating shader Layout");
			spirv_cross::Compiler compiler(shaderSrc);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();
			for (const auto& resource : resources.uniform_buffers) shaderLayout.push_back(getShaderResource(resource, shaderSrc,renderer::uniformBuffers));
			for (const auto& resource : resources.push_constant_buffers) shaderLayout.push_back(getShaderResource(resource, shaderSrc,renderer::pushConstantBuffers));
			for (const auto& resource : resources.storage_buffers) shaderLayout.push_back(getShaderResource(resource, shaderSrc, renderer::storageBuffers));
			for (const auto& resource : resources.stage_inputs) shaderLayout.push_back(getShaderResource(resource, shaderSrc,renderer::stageInputs));
			for (const auto& resource : resources.sampled_images) shaderLayout.push_back(getShaderResource(resource, shaderSrc, renderer::sampledImages));
			for (const auto& resource : resources.separate_images) shaderLayout.push_back(getShaderResource(resource, shaderSrc, renderer::storageImages));
			for (const auto& resource : resources.separate_samplers) shaderLayout.push_back(getShaderResource(resource, shaderSrc, renderer::separateSamplers));

			//for (const auto& resource : resources.stage_outputs) shaderLayout.push_back(getShaderResource(resource, shaderSrc,renderer::stageOutputs));
			createOffsets(&shaderLayout);
			LN_CORE_TRACE("created shaderLayout");
		}

		renderer::shaderResource vulkanShader::getShaderResource(const spirv_cross::Resource& _shaderResource,const std::vector<uint32_t>& shaderSource, renderer::typeClass typeClass)
		{
			LN_PROFILE_FUNCTION();
			spirv_cross::Compiler compiler(shaderSource);
			spirv_cross::SPIRType::BaseType baseType = compiler.get_type(_shaderResource.base_type_id).basetype;
			const spirv_cross::SPIRType& type = compiler.get_type(_shaderResource.type_id);
			renderer::shaderResource resource;
			resource.name = _shaderResource.name;
			resource.binding = compiler.get_decoration(_shaderResource.id, spv::DecorationBinding);
			resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
			if (compiler.get_type(_shaderResource.type_id).array.size()) resource.amount = compiler.get_type(_shaderResource.type_id).array[0];
			LN_CORE_INFO("array size = {0}", resource.amount);
			resource.resourceClass = typeClass;
			//resource.offset = compiler.get_decoration(_shaderResource.id, spv::DecorationOffset);
			
			switch (baseType)
			{
			case spirv_cross::SPIRType::Unknown:
				break;
			case spirv_cross::SPIRType::Void:
				resource.type = renderer::Void;
				break;
			case spirv_cross::SPIRType::Boolean:
				resource.stride = sizeof(uint8_t);
				resource.type = renderer::Boolean;
				break;
			case spirv_cross::SPIRType::SByte:
				resource.stride = sizeof(int8_t);
				resource.type = renderer::SByte;
				break;
			case spirv_cross::SPIRType::UByte:
				resource.stride = sizeof(uint8_t);
				resource.type = renderer::UByte;
				break;
			case spirv_cross::SPIRType::Short:
				resource.stride = sizeof(int16_t);
				resource.type = renderer::Short;
				break;
			case spirv_cross::SPIRType::UShort:
				resource.stride = sizeof(uint16_t);
				resource.type = renderer::UShort;
				break;
			case spirv_cross::SPIRType::Int:
				resource.stride = sizeof(int32_t);
				resource.type = renderer::Int;
				break;
			case spirv_cross::SPIRType::UInt:
				resource.stride = sizeof(uint32_t);
				resource.type = renderer::UInt;
				break;
			case spirv_cross::SPIRType::Int64:
				resource.stride = sizeof(int64_t);
				resource.type = renderer::Int64;
				break;
			case spirv_cross::SPIRType::UInt64:
				resource.stride = sizeof(uint64_t);
				resource.type = renderer::UInt64;
				break;
			case spirv_cross::SPIRType::AtomicCounter:
				resource.type = renderer::AtomicCounter;
				break;
			case spirv_cross::SPIRType::Half:
				resource.type = renderer::Half;
				break;
			case spirv_cross::SPIRType::Float:
				
				switch (compiler.get_type(_shaderResource.base_type_id).vecsize)
				{
					
				case 2:
					resource.stride = sizeof(glm::vec2);
					resource.type = renderer::Vec2;
					break;
				case 3:
					resource.stride = sizeof(glm::vec3);
					resource.type = renderer::Vec3;
					break;
				case 4:
					resource.stride = sizeof(glm::vec4);
					resource.type = renderer::Vec4;
					break;

				default:
					resource.type = renderer::Float;
					break;
				}
				switch (compiler.get_type(_shaderResource.base_type_id).columns)
				{
					resource.stride = compiler.get_decoration(_shaderResource.id, spv::DecorationMatrixStride);
				case 2:
					resource.type = renderer::mat2;
					break;
				case 3:
					resource.type = renderer::mat3;
					break;
				case 4:
					resource.type = renderer::mat4;
					break;
				}
				break;
			case spirv_cross::SPIRType::Double:
				break;
			case spirv_cross::SPIRType::Struct:
				{
					uint8_t counter = 0;
					auto& type = compiler.get_type(_shaderResource.base_type_id);
					for (const auto& member : compiler.get_type(_shaderResource.base_type_id).member_types)
					{

						spirv_cross::Resource memberResource;
						memberResource.base_type_id = member;
						memberResource.type_id = member;
						memberResource.id = compiler.get_type(member).self;
						memberResource.name = compiler.get_member_name(type.self, counter);
						resource.members.push_back(getShaderResource(memberResource, shaderSource, typeClass));
						resource.members[counter].stride = compiler.get_declared_struct_member_size(type, counter);
						counter++;
					}
					if (typeClass == renderer::uniformBuffers)
					{
						resource.type = renderer::Uniform;

					}
					else if (typeClass == renderer::pushConstantBuffers)
					{
						resource.type = renderer::PushConstant;
					}
					else if (typeClass == renderer::storageBuffers)
					{
						resource.type = renderer::StorageBuffer;
					}
				}
				break;
			case spirv_cross::SPIRType::Image:
				resource.type = renderer::Image;
				break;
			case spirv_cross::SPIRType::SampledImage:
				resource.type = renderer::SampledImage;
				break;
			case spirv_cross::SPIRType::Sampler:
				resource.type = renderer::Sampler;
				break;
			case spirv_cross::SPIRType::AccelerationStructure:
				resource.type = renderer::AccelerationStructure;
				break;
			case spirv_cross::SPIRType::RayQuery:
				resource.type = renderer::RayQuery;
				break;
			case spirv_cross::SPIRType::ControlPointArray:
				resource.type = renderer::ControlPointArray;
				break;
			case spirv_cross::SPIRType::Interpolant:
				resource.type = renderer::Interpolant;
				break;
			case spirv_cross::SPIRType::Char:
				resource.stride = sizeof(int8_t);
				resource.type = renderer::Char;
				break;
			default:
				break;
			}
			return resource;
		}
		void vulkanShader::createOffsets(std::vector<renderer::shaderResource>* layout) 
		{
			if (layout->size() < 2) return;
			for (size_t i = layout->size() - 1; i >= 1; i--)
			{
				renderer::shaderResource prevResource = layout->at(i);
				renderer::shaderResource* resource = &layout->at(i - 1);
				resource->offset = prevResource.offset + prevResource.stride;
				createOffsets(&resource->members);
			}
		}
	}
}