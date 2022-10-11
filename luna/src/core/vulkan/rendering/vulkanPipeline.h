#pragma once

#include <core/rendering/pipeline.h>
#include <core/vulkan/device/vulkanCmdPool.h>
namespace luna
{
	namespace vulkan
	{
		class vulkanPipeline : public renderer::pipeline
		{
		public:
			vulkanPipeline(const renderer::pipelineLayout& layout);
			virtual ~vulkanPipeline() { destroyPipeline(); };
			virtual void createPipeline(const renderer::pipelineLayout& layout) override;
			virtual void destroyPipeline() override;
			virtual void begin() const override;
			virtual void end() const override;
			virtual void flush() override;
		private:
			void createPipeLineLayout();

			VkResult buildPipeline(VkDevice device, VkRenderPass pass);
			

			void createShaderStages();
			void createInputStates();
			VkPipelineVertexInputStateCreateInfo createVertexInputState(const ref<renderer::shader> shader);
			void createBindingDescription(const ref<renderer::shader> shader);
			void createAttributeDescription(const ref<renderer::shader> shader);
			VkResult createShaderModule(ref<renderer::shader> shader,VkShaderModule* shaderModule);
			VkFormat getResourceFormat(renderer::typeId resourceType);
			VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo(VkPrimitiveTopology topology);
			VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo(VkPolygonMode polygonMode);
			VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo();
			VkPipelineColorBlendAttachmentState colorBlendAttachmentState();
			VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo();
			void initDefaultRenderpass();
			void initSyncStructures();
		private:
			//TODO improve variables usage.
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
			VkRenderPass renderPass;
			VkPipelineLayout pipelineLayout;
			VkPipeline pipeline;
			renderer::pipelineLayout layout;
			std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
			std::vector<VkShaderModule> shaderModules;
			std::unordered_map<std::string,vertexInputDescription> inputDescriptions;
			std::vector<VkPipelineVertexInputStateCreateInfo> vertexInputStates;

			VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
			VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo;
			VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;
			VkPipelineColorBlendAttachmentState pipelineColorBlendAttachementState;


			std::vector<VkSemaphore> imageAvailableSemaphores, renderFinishedSemaphores;
			std::vector<VkFence> inFlightFences;
			std::vector<VkFence> imagesInFlight;
			
			ref<vulkanCmdPool> commandPool = nullptr;
			std::vector<virtualCmdBuffer> commandBuffers;
			uint32_t swapchainImageIndex = 0;
			uint32_t currentFrame = 0;
			VkQueue presentQueue;

			uint32_t maxFramesInFlight = 0;
		};
		
	}
}

/* notes 
* possible optimization by reusing shaderstages and createInfos;
*/
