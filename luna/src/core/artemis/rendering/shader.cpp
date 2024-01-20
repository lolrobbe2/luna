#include "shader.h"
#include <core/utils/shaderCompiler.h>
#include <core/debug/debugMacros.h>

namespace luna 
{
	namespace artemis
	{
		shader::shader(const VkDevice* device,const std::string& filepath, const shaderStage stage,const std::string& entrypoint) : _stage(stage)
		{
			std::ifstream file;
			shaderName = std::filesystem::path{ filepath }.filename().string();
			file.open(filepath, std::ios::ate | std::ios::binary);
			std::vector<char> buffer;
			LN_ERR_FAIL_COND_MSG(!file.is_open(), "[Artemis] unable to open shader file");

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
			compileSpec.reflect = true;
			switch (stage)
			{
			case shaderStageVertex:
				compileSpec.shaderKind = shaderc_shader_kind::shaderc_vertex_shader;
				shaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
				break;
			case shaderStageTessellationControl:
				compileSpec.shaderKind = shaderc_shader_kind::shaderc_tess_control_shader;
				shaderStageCreateInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				break;
			case shaderStageTessellationEvaluation:
				compileSpec.shaderKind = shaderc_shader_kind::shaderc_tess_evaluation_shader;
				shaderStageCreateInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
				break;
			case shaderStageGeometry:
				compileSpec.shaderKind = shaderc_shader_kind::shaderc_geometry_shader;
				shaderStageCreateInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
				break;
			case shaderStageFragment:
				compileSpec.shaderKind = shaderc_shader_kind::shaderc_fragment_shader;
				shaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			case shaderStageCompute:
				compileSpec.shaderKind = shaderc_shader_kind::shaderc_compute_shader;
				shaderStageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
				break;
			default:
				compileSpec.shaderKind = shaderc_shader_kind::shaderc_anyhit_shader;
				shaderStageCreateInfo.stage = VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
				break;
			}
			shaderSrc = compiler.compile(compileSpec);
		
			createLayout();
			
			_stage = stage;
			this->device = device;
			this->entrypoint = entrypoint;

			createShaderModule();

			shaderStageCreateInfo.pName = entrypoint.c_str();
			shaderStageCreateInfo.pSpecializationInfo = nullptr; //no specilization!
			shaderStageCreateInfo.module = _module;
			shaderStageCreateInfo.flags = 0;
		}
		void shader::createLayout()
		{
			spirv_cross::Compiler compiler(shaderSrc);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();
			for (const auto& resource : resources.uniform_buffers) shaderLayout.push_back(getShaderResource(resource, shaderSrc, uniformBuffers));
			for (const auto& resource : resources.push_constant_buffers) shaderLayout.push_back(getShaderResource(resource, shaderSrc, pushConstantBuffers));
			for (const auto& resource : resources.storage_buffers) shaderLayout.push_back(getShaderResource(resource, shaderSrc, storageBuffers));
			for (const auto& resource : resources.stage_inputs) shaderLayout.push_back(getShaderResource(resource, shaderSrc, stageInputs));
			for (const auto& resource : resources.sampled_images) shaderLayout.push_back(getShaderResource(resource, shaderSrc, sampledImages));
			for (const auto& resource : resources.separate_images) shaderLayout.push_back(getShaderResource(resource, shaderSrc, storageImages));
			for (const auto& resource : resources.separate_samplers) shaderLayout.push_back(getShaderResource(resource, shaderSrc, separateSamplers));

			//for (const auto& resource : resources.stage_outputs) shaderLayout.push_back(getShaderResource(resource, shaderSrc,stageOutputs));
			if (_stage == shaderStageVertex)
			{
				std::sort(std::begin(shaderLayout), std::end(shaderLayout), [](const shaderResource& a,
					const shaderResource& b) {return a.location < b.location; });
			}
			createOffsets(&shaderLayout);
		}
		shaderResource shader::getShaderResource(const spirv_cross::Resource& _shaderResource, const std::vector<uint32_t>& shaderSource, typeClass typeClass)
		{
			LN_PROFILE_FUNCTION();
			spirv_cross::Compiler compiler(shaderSource);
			spirv_cross::SPIRType::BaseType baseType = compiler.get_type(_shaderResource.base_type_id).basetype;
			const spirv_cross::SPIRType& type = compiler.get_type(_shaderResource.type_id);
			shaderResource resource;
			resource.name = _shaderResource.name;
			resource.binding = compiler.get_decoration(_shaderResource.id, spv::DecorationBinding);
			resource.location = compiler.get_decoration(_shaderResource.id, spv::DecorationLocation);
			if (compiler.get_type(_shaderResource.type_id).array.size()) resource.amount = compiler.get_type(_shaderResource.type_id).array[0];
			resource.resourceClass = typeClass;
			//resource.offset = compiler.get_decoration(_shaderResource.id, spv::DecorationXfbStride);
			switch (baseType)
			{
			case spirv_cross::SPIRType::Unknown:
				break;
			case spirv_cross::SPIRType::Void:
				resource.type = Void;
				break;
			case spirv_cross::SPIRType::Boolean:
				resource.stride = sizeof(uint8_t);
				resource.type = Boolean;
				break;
			case spirv_cross::SPIRType::SByte:
				resource.stride = sizeof(int8_t);
				resource.type = SByte;
				break;
			case spirv_cross::SPIRType::UByte:
				resource.stride = sizeof(uint8_t);
				resource.type = UByte;
				break;
			case spirv_cross::SPIRType::Short:
				resource.stride = sizeof(int16_t);
				resource.type = Short;
				break;
			case spirv_cross::SPIRType::UShort:
				resource.stride = sizeof(uint16_t);
				resource.type = UShort;
				break;
			case spirv_cross::SPIRType::Int:
				resource.stride = 4;
				resource.type = Int;
				break;
			case spirv_cross::SPIRType::UInt:
				resource.stride = sizeof(uint32_t);
				resource.type = UInt;
				break;
			case spirv_cross::SPIRType::Int64:
				resource.stride = sizeof(int64_t);
				resource.type = Int64;
				break;
			case spirv_cross::SPIRType::UInt64:
				resource.stride = sizeof(uint64_t);
				resource.type = UInt64;
				break;
			case spirv_cross::SPIRType::AtomicCounter:
				resource.type = AtomicCounter;
				break;
			case spirv_cross::SPIRType::Half:
				resource.type = Half;
				break;
			case spirv_cross::SPIRType::Float:

				switch (compiler.get_type(_shaderResource.base_type_id).vecsize)
				{

				case 2:
					resource.stride = 2 * 4;
					resource.type = Vec2;
					break;
				case 3:
					resource.stride = 3 * 4;
					resource.type = Vec3;
					break;
				case 4:
					resource.stride = 4 * 4;
					resource.type = Vec4;
					break;

				default:
					resource.stride = 4;
					resource.type = Float;
					break;
				}
				switch (compiler.get_type(_shaderResource.base_type_id).columns)
				{
					resource.stride = compiler.get_decoration(_shaderResource.id, spv::DecorationMatrixStride);
				case 2:
					resource.type = mat2;
					break;
				case 3:
					resource.type = mat3;
					break;
				case 4:
					resource.type = mat4;
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
				if (typeClass == uniformBuffers)
				{
					resource.type = Uniform;

				}
				else if (typeClass == pushConstantBuffers)
				{
					resource.type = PushConstant;
				}
				else if (typeClass == storageBuffers)
				{
					resource.type = StorageBuffer;
				}
			}
			break;
			case spirv_cross::SPIRType::Image:
				resource.type = Image;
				break;
			case spirv_cross::SPIRType::SampledImage:
				resource.type = SampledImage;
				break;
			case spirv_cross::SPIRType::Sampler:
				resource.type = Sampler;
				break;
			case spirv_cross::SPIRType::AccelerationStructure:
				resource.type = AccelerationStructure;
				break;
			case spirv_cross::SPIRType::RayQuery:
				resource.type = RayQuery;
				break;
			case spirv_cross::SPIRType::ControlPointArray:
				resource.type = ControlPointArray;
				break;
			case spirv_cross::SPIRType::Interpolant:
				resource.type = Interpolant;
				break;
			case spirv_cross::SPIRType::Char:
				resource.stride = sizeof(int8_t);
				resource.type = Char;
				break;
			default:
				break;
			}
			return resource;
		}
		void shader::createOffsets(std::vector<shaderResource>* layout)
		{
			if (layout->size() < 2) return;
			uint64_t prevStride = 0;
			shaderResource* _layout = layout->data();
			for (size_t i = 0; i < layout->size(); i++)
			{
				_layout[i].offset = prevStride;
				_layout[i].stride += _layout[i].offset;
				prevStride = _layout[i].stride;
			}
		}
		void shader::createShaderModule()
		{
			VkShaderModuleCreateInfo shaderModuleCreateInfo{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
			
			shaderModuleCreateInfo.codeSize = shaderSrc.size() * sizeof(uint32_t);
			shaderModuleCreateInfo.pCode = shaderSrc.data();

			VkResult res = vkCreateShaderModule(*device, &shaderModuleCreateInfo, nullptr,&_module);
			LN_ERR_FAIL_COND_MSG(res != VK_SUCCESS, "[Artemis] an error occured during shader module creation, VkResult: " + VK_RESULT(res));
		}	
	}
}