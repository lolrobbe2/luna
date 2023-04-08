#pragma once


#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <core/rendering/pipeline.h>
#include <core/vulkan/device/vulkanCmdPool.h>
#include <core/vulkan/utils/vulkanDescriptorPool.h>
#include <core/vulkan/rendering/vulkanSampler.h>
namespace luna
{
	namespace vulkan
	{
		/**
		 * @brief vulkan pipline implementation class.
		 * @see renderer::pipeline.
		 */
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
			 * returns current frameBuffer descriptor handle.
			 *
			 * \return
			 */
			virtual ImTextureID getWindowImage() override;
			/**
			 * @brief creates the renderer commands for the pipeline.
			 * 
			 */
			virtual void createCommands() override;
			/**
			 * @brief starts recording the pipline input.
			 * 
			 */
			virtual void begin() override;
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
			/**
			 * @brief binds vulkan textures in to there respective textureArrays.
			 *
			 * \param textureHandles
			 * \param indexSet
			 */
			virtual void bindTextures(const std::vector<uint64_t> textureHandles, const uint64_t indexSet) override;

			/**
			 * @brief clears all the vulkan pipeline draw commands.
			 * 
			 */
			virtual void clear() override;
			/**
			 * @brief return the pipeline renderpass.
			 */
			inline VkRenderPass getRenderPass() { return renderPass; };
			/**
			 * @brief draw a vertexArray indexed.
			 * 
			 * \param const ref<renderer::vertexArray>& vertexArray vertexArray to render
			 * \param int indexCount the indices count to render; 
			 * \note check if the indexCount variable is not zero when the rendered quad does not appear;
			 */
			virtual void drawIndexed(const ref<renderer::vertexArray>& vertexArray, std::vector<uint64_t> textures, int indexCount = 0) override;
			renderer::pipelineLayout layout;
			uint32_t maxFramesInFlight = 0;
		private:
			/**
			 * @brief creates the pipeline layout.
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
			/**
			 * @brief creates the rasterizer state create info.
			 * 
			 * \param polygonMode
			 * \return VkPipelineRasterizationStateCreateInfo struct
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPipelineRasterizationStateCreateInfo.html
			 */
			VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo(VkPolygonMode polygonMode);
			/**
			 * @brief creates the multisampling info.
			 * 
			 * \return VkPipelineMultisampleStateCreateInfo struct
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPipelineMultisampleStateCreateInfo.html
			 */
			VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo();
			/**
			 * @brief the color blend attachment state(color writemask).
			 * 
			 * \return VkPipelineColorBlendAttachmentState struct
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPipelineColorBlendAttachmentState.html
			 */
			VkPipelineColorBlendAttachmentState colorBlendAttachmentState();
			/**
			 * @brief creates the pipeline layout info.
			 * 
			 * \return VkPipelineLayoutCreateInfo struct
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkCreatePipelineLayout.html
			 */
			VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo();
			/**
			 * @brief initialzes the dafault pipeline renderPass.
			 * 
			 */
			void initDefaultRenderpass();
			/**
			 * @brief initializes the the pipeline syncronization structures.
			 * (VkFence and VkSemaphore)
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkFence.html
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkSemaphore.html
			 */
			void initSyncStructures();
			/**
			 * @brief destroys all pipeline sync structures.
			 * (VkFence and VkSemaphore)
			 * 
			 */
			void destroySyncStructures();
			/**
			 * @brief transitions the image layout.
			 * 
			 * \param VkImage image imageHandle
			 * \param VkFormat format
			 * \param VkImageLayout oldLayout
			 * \param VkImageLayout newLayout
			 * \param VkCommandBuffer commandBufffer
			 */
			void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandBuffer commandBufffer);
			/**
			 * @brief executes draw command.
			 */
			void fnDrawIndexed(const ref<renderer::vertexArray>& vertexArray, const uint64_t& descriptorIndex,const std::vector<uint64_t>& textures, const int& indexCount);

			VkResult createDescriptorSetLayout();

			VkAccessFlags getAccessFlag(VkImageLayout layout);
			VkPipelineStageFlags getStageFlag(VkImageLayout layout);
		private:
					//TODO improve variables usage.

			ref<renderer::vulkanSampler> sampler;

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
			uint32_t prevFrame = 0;
			VkQueue presentQueue;
			uint64_t _frameNumber = 0; //temporary
			bool justResized = false;
			/* draw */
			struct drawCommand
			{
				ref<renderer::vertexArray> vertexArray;
				uint64_t descriptorIndex;
				std::vector<uint64_t> textures;
				int indexCount = 0;
			};
			std::vector<drawCommand> drawCommands;
			ref<utils::vulkanDescriptorPool> descriptorPool;
			std::vector<ref<utils::vulkanDescriptorSet>> descriptorSets;
			//bool justResized;
			uint64_t descriptorsetIndex = 1;
		};
		
	}
}

/* notes 
* possible optimization by reusing shaderstages and createInfos;
* 
* 
* use per vertexbuffer descriptor for texture.
*/
