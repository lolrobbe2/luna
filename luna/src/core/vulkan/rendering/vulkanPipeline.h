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
			void createPipeLineLayout();

			void createShaderStages();
			void createInputStates();
			VkPipelineVertexInputStateCreateInfo createVertexInputState(const ref<renderer::shader> shader);
			void createBindingDescription(const ref<renderer::shader> shader);
			void createAttributeDescription(const ref<renderer::shader> shader);
			VkResult createShaderModule(ref<renderer::shader> shader,VkShaderModule* shaderModule);
			VkFormat getResourceFormat(renderer::typeId resourceType);
		private:
			struct shaderStage
			{
				VkPipelineShaderStageCreateInfo stageInfo;
				VkShaderModule shaderModule;
			};
			struct vertexInputDescription {

				std::vector<VkVertexInputBindingDescription> bindings;
				std::vector<VkVertexInputAttributeDescription> attributes;
				VkPipelineVertexInputStateCreateFlags flags = 0;
			};

			struct Vertex {

				glm::vec2 position;
				glm::vec3 normal;
				glm::vec2 color;
			};
			renderer::pipelineLayout layout;
			std::vector<shaderStage> shaderStages;
			std::unordered_map<std::string,vertexInputDescription> inputDescriptions;
			
		};
		
	}
}

/* notes 
* possible optimization by reusing shaderstages and createInfos;
*/
