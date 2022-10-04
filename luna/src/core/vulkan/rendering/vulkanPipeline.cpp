#include "vulkanPipeline.h"
namespace luna
{
	namespace vulkan 
	{
		vulkanPipeline::vulkanPipeline(const pipelineLayout& layout)
		{
		
			createPipeline(layout);
		}
		void vulkanPipeline::createPipeline(const pipelineLayout& layout)
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
		void vulkanPipeline::createPipeLineLayout()
		{
			
		}

		VkResult vulkanPipeline::createShaderModule(ref<renderer::shader> shader,VkShaderModule* shaderModule)
		{
			VkShaderModuleCreateInfo shaderModuleCreateInfo{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
			std::vector<uint32_t> shadersrc(shader->self().begin(), shader->self().end());
			shaderModuleCreateInfo.codeSize = shadersrc.size();
			shaderModuleCreateInfo.pCode = shadersrc.data();
			//return vkCreateShaderModule()
		}

	}
}
