#pragma once

#include <core/rendering/pipeline.h>
namespace luna
{
	namespace vulkan
	{
		class vulkanPipeline : public renderer::pipeline
		{
		public:
			vulkanPipeline(const renderer::pipelineLayout& layout);
			virtual ~vulkanPipeline() {};
			virtual void createPipeline(const renderer::pipelineLayout& layout) override;
			virtual void begin() const override;
			virtual void end() const override;
		private:
			void createShaderStages();
			void createPipeLineLayout();
			VkResult createShaderModule(ref<renderer::shader> shader,VkShaderModule* shaderModule);
		private:
			struct shaderStage
			{
				VkPipelineShaderStageCreateInfo stageInfo;
				VkShaderModule shaderModule;
			};
			renderer::pipelineLayout layout;
			std::vector<shaderStage> shaderStages;

		};
		
	}
}

/* notes 
* possible optimization by reusing shaderstages and createInfos;
*/
