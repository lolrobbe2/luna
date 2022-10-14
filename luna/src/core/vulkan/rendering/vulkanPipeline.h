#pragma once
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <core/rendering/pipeline.h>
#include <core/vulkan/device/vulkanCmdPool.h>
namespace luna
{
	namespace vulkan
	{
		class vulkanPipeline : public renderer::pipeline
		{
		public:
			/**
			 * @brief creates a vulkan pipeline.
			 * 
			 * \param renderer::pipelineLayout layout struct
			 */
			vulkanPipeline(const renderer::pipelineLayout& layout);
			/**
			 * @brief pipeline destructor.
			 */
			virtual ~vulkanPipeline() { destroyPipeline(); };
			/**
			 * @brief creates the vulkan pipeline.
			 * 
			 * \param renderer::pipelineLayout layout struct
			 */
			virtual void createPipeline(const renderer::pipelineLayout& layout) override;
			/**
			 * @brief destroys the vulkan pipeline.
			 * 
			 */
			virtual void destroyPipeline() override;
			/**
			 * @brief creates the renderer commands for the pipeline.
			 * 
			 */
			virtual void createCommands() override;
			/**
			 * @brief starts recording the pipline input.
			 * 
			 */
			virtual void begin() const override;
			/**
			 * @brief ends recording the pipline input.
			 * 
			 */
			virtual void end() const override;
			/**
			 * @brief executes the pipeline commands.
			 * 
			 */
			virtual void flush() override;
			inline VkRenderPass getRenderPass() { return renderPass; };
			renderer::pipelineLayout layout;
		private:
			/**
			 * @brief creates the shader layout.
			 * 
			 */
			void createPipeLineLayout();
			/**
			 * @brief build the vulkan pipline.
			 * 
			 * \param VkDevice device 
			 * \param VkRenderPass pass
			 * \return VK_SUCCES when pipline was succesfully build
			 */
			VkResult buildPipeline(VkDevice device, VkRenderPass pass);
			/**
			 * @brief creates the shader stages to be used in the pipeline.
			 * 
			 */
			void createShaderStages();
			/**
			 * @brief creates the vertex input.
			 */
			void createInputStates();
			/**
			 * @brief creates the vertex input state for a single shader.
			 * 
			 * \param ref<renderer::shader> shader reference to a valid shader
			 * \return VkPipelineVertexInputStateCreateInfo 
			 */
			VkPipelineVertexInputStateCreateInfo createVertexInputState(const ref<renderer::shader> shader);
			/**
			 * @brief creates the binding descriptio for a single shader.
			 * 
			 * \param ref<renderer::shader> shader reference to a valid shader
			 */
			void createBindingDescription(const ref<renderer::shader> shader);
			/**
			 * @brief creates the attribute description for a single shader.
			 * 
			 * \param ref<renderer::shader> shader reference to a valid shader
			 */
			void createAttributeDescription(const ref<renderer::shader> shader);
			/**
			 * @brief creates a shader module from a shader ref.
			 * 
			 * \param ref<renderer::shader> shader the source shader
			 * \param shaderModule pointer to the vulkan shaderModule
			 * \return VK_SUCCES when shaderModule was created sucessfully.
			 */
			VkResult createShaderModule(ref<renderer::shader> shader,VkShaderModule* shaderModule);
			/**
			 * @brief returns the corresponding resource type for a given .
			 * 
			 * \param renderer::typeId resourceType
			 * \return VkFormat the corresponding VkFormat for typeId
			 */
			VkFormat getResourceFormat(renderer::typeId resourceType);
			/**
			 * @brief creates the input Assembly
			 * 
			 * \param topology
			 * \return 
			 */
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
			VkRenderPass imGuiRenderPass;
			VkPipelineLayout pipelineLayout;
			VkPipeline pipeline;
			
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
