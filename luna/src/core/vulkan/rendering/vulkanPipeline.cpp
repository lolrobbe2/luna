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
		void vulkanPipeline::createPipeLineLayout()
		{
			createShaderStages();
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

	}
}
