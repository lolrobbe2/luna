#include "vulkanPipeline.h"
#include <core/vulkan/device/vulkanDevice.h>
namespace luna
{
	namespace vulkan 
	{
		vulkanPipeline::vulkanPipeline(const renderer::pipelineLayout& layout)
		{
		
			createPipeline(layout);
		}
		void vulkanPipeline::createPipeline(const renderer::pipelineLayout& layout)
		{
			this->layout = layout;
			createPipeLineLayout();
		}
		void vulkanPipeline::begin() const
		{
		}
		void vulkanPipeline::end() const
		{
		}
		void vulkanPipeline::createShaderStages()
		{
			for (const auto& shader : layout.pipelineShaders) 
			{
				shaderStage stage;
				stage.stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				stage.stageInfo.pNext = nullptr;
				LN_CORE_INFO("creatings vulkan shadermodule, result = {0}", createShaderModule(shader, &stage.shaderModule));
				stage.stageInfo.module = stage.shaderModule;
				//the entry point of the shader
				stage.stageInfo.pName = "main";

				// convert engine stage to api stage.
				switch (shader->stage)
				{
				case luna::renderer::shaderStageVertex:
					stage.stageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
					break;
				case luna::renderer::shaderStageTessellationControl:
					stage.stageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
					break;
				case luna::renderer::shaderStageTessellationEvaluation:
					stage.stageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
					break;
				case luna::renderer::shaderStageGeometry:
					stage.stageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
					break;
				case luna::renderer::shaderStageFragment:
					stage.stageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
					break;
				case luna::renderer::shaderStageCompute:
					stage.stageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
					break;
				default:
					break;
				}
				shaderStages.push_back(stage);
			}
		}
		void vulkanPipeline::createInputStates()
		{

		}
		VkPipelineVertexInputStateCreateInfo vulkanPipeline::createVertexInputState(const ref<renderer::shader> shader)
		{
			VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
			vertexInputCreateInfo.pVertexAttributeDescriptions = inputDescriptions[shader->shaderName].attributes.data();
			vertexInputCreateInfo.pVertexBindingDescriptions = inputDescriptions[shader->shaderName].bindings.data();
			vertexInputCreateInfo.vertexAttributeDescriptionCount = inputDescriptions[shader->shaderName].attributes.size();
			vertexInputCreateInfo.vertexBindingDescriptionCount = inputDescriptions[shader->shaderName].bindings.size();
			return vertexInputCreateInfo;
		}
		void vulkanPipeline::createBindingDescription(const ref<renderer::shader> shader)
		{
			if (shader->stage != renderer::shaderStageVertex) return;

			std::unordered_map<uint32_t, VkVertexInputBindingDescription> bindingDescriptions;
			for(const auto& shaderResource : shader->shaderLayout)
			{
				if ((shaderResource.type != renderer::Uniform) && (shaderResource.type != renderer::PushConstant) && (shaderResource.type != renderer::StorageBuffer) && shaderResource.resourceClass == renderer::stageInputs)
				{
					
					VkVertexInputBindingDescription bindingDescription;
					auto bindingDescriptionPtr = bindingDescriptions.find(shaderResource.binding);
					if (bindingDescriptionPtr == bindingDescriptions.end())
					{

						bindingDescription.binding = shaderResource.binding;
						bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
						bindingDescription.stride = shaderResource.stride;
						bindingDescriptions.insert({ bindingDescription.binding ,bindingDescription });
					}
					else
					{
						
						bindingDescriptionPtr->second.stride += shaderResource.stride;
					}
				}
			}
			for (auto bindingDescription = bindingDescriptions.begin(); bindingDescription != bindingDescriptions.end(); bindingDescription++)inputDescriptions[shader->shaderName].bindings.push_back(bindingDescription->second);
			
		}
		void vulkanPipeline::createAttributeDescription(const ref<renderer::shader> shader)
		{
			if (shader->stage != renderer::shaderStageVertex) return;

			
			for (const auto& shaderResource : shader->shaderLayout)
			{
				if ((shaderResource.type != renderer::Uniform) && (shaderResource.type != renderer::PushConstant) && (shaderResource.type != renderer::StorageBuffer) && shaderResource.resourceClass == renderer::stageInputs)
				{
					VkVertexInputAttributeDescription attributeDescription;
					attributeDescription.binding = shaderResource.binding;
					attributeDescription.location = shaderResource.location;
					attributeDescription.offset = shaderResource.offset;
					attributeDescription.format = getResourceFormat(shaderResource.type);
					inputDescriptions[shader->shaderName].attributes.push_back(attributeDescription);
				}
			}
		}
		void vulkanPipeline::createPipeLineLayout()
		{
			createShaderStages();
			for (const auto& shader : layout.pipelineShaders) createBindingDescription(shader);
			for (const auto& shader : layout.pipelineShaders) createAttributeDescription(shader);
		}

		VkResult vulkanPipeline::createShaderModule(ref<renderer::shader> shader,VkShaderModule* shaderModule)
		{
			VkShaderModuleCreateInfo shaderModuleCreateInfo{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
			std::vector<uint32_t> shadersrc = shader->self();
			shaderModuleCreateInfo.codeSize = shadersrc.size() * sizeof(uint32_t);
			shaderModuleCreateInfo.pCode = shadersrc.data();
			ref<vulkanDevice> device = std::dynamic_pointer_cast<vulkanDevice>(layout.device);
			vulkanDevice::deviceHandles handles = device->getDeviceHandles();
			
			return vkCreateShaderModule(handles.device, &shaderModuleCreateInfo, nullptr, shaderModule);
		}

		VkFormat vulkanPipeline::getResourceFormat(renderer::typeId resourceType)
		{
			switch (resourceType)
			{
			case luna::renderer::Boolean:
				return VK_FORMAT_R8_UINT;
			case luna::renderer::SByte:
				return VK_FORMAT_R8_UINT;
			case luna::renderer::UByte:
				return VK_FORMAT_R8_UINT;
			case luna::renderer::Short:
				return VK_FORMAT_R16_UINT;
			case luna::renderer::UShort:
				return VK_FORMAT_R16_UINT;
			case luna::renderer::Int:
				return VK_FORMAT_R32_UINT;
			case luna::renderer::UInt:
				return VK_FORMAT_R32_UINT;
			case luna::renderer::Int64:
				return VK_FORMAT_R64_UINT;
			case luna::renderer::UInt64:
				return VK_FORMAT_R64_UINT;
			case luna::renderer::Float:
				return VK_FORMAT_R32_SFLOAT;
			case luna::renderer::Char:
				return VK_FORMAT_R8_UINT;
			case luna::renderer::Vec2:
				return VK_FORMAT_R32G32_SFLOAT;
			case luna::renderer::Vec3:
				return VK_FORMAT_R32G32B32_SFLOAT;
			case luna::renderer::Vec4:
				return VK_FORMAT_R32G32B32A32_SFLOAT;
			default:
				return VK_FORMAT_MAX_ENUM;
			}
		}

	}
}
