#pragma once
#include <core/artemis/rendering/pipeline.h>
namespace luna 
{
	namespace artemis 
	{
		class renderPass;
		class pipelineBuilder
		{
		public:
			/**
			* @brief adds a shaderStage from a given shader
			* @brief creates binding & attribute descriptions automaticcaly when the shaders shaderStage is vertexStage
			*/
			pipelineBuilder& addShaderStage(const ref<shader> shader, const VkPipelineShaderStageCreateFlags flags = 0);
			/**
			* @brief gets called automatticaly when a shader stage is added.
			*/
			pipelineBuilder& createBindingDescription(const ref<shader> shader);
			/**
			* @brief gets called automatticaly when a shader stage is added.
			* @brief creates the vertexAttributeDiscription.
			*/
			pipelineBuilder& createAttributeDescription(const ref<shader> shader);
			pipelineBuilder& createVertexInputState(const ref<shader> shader, VkPipelineVertexInputStateCreateFlags flags = 0);
			pipelineBuilder& addViewport(const VkViewport viewport);
			pipelineBuilder& addScissor(const VkRect2D scissor);
			pipelineBuilder& addDynamicState(const VkDynamicState dynamicState);
			/**
			* @brief seperate function to easely disable blending
			*/
			pipelineBuilder& enableBlending(bool enable = true);
			pipelineBuilder& setColorBlendingParams(const VkBlendFactor srcColor = VK_BLEND_FACTOR_SRC_ALPHA, const VkBlendFactor dstColor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, const VkBlendOp blendOp = VK_BLEND_OP_ADD);
			pipelineBuilder& setAlphaBlendingParams(const VkBlendFactor srcColor, const VkBlendFactor dstColor, const VkBlendOp blendOp);
			pipelineBuilder& setColorMask(VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_A_BIT);
			pipelineBuilder& setPipelineType(const pipelineType type);
			pipelineBuilder& setCreateFlags(const VkPipelineCreateFlags createFlags = 0);
			pipelineBuilder& setTopology(const VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
			pipelineBuilder& setPolygonMode(const VkPolygonMode polygonMode);
			pipelineBuilder& setRenderPass(const ref<renderPass> p_renderPass) { this->p_renderPass = p_renderPass; return *this; }
			pipelineBuilder& addDescriptorSetLayout(const VkDescriptorSetLayout layout);
			ref<pipeline> build();
		protected:
			friend class device;
			pipelineBuilder(const VkDevice* p_device);
		private:
			VkFormat getResourceFormat(const typeId resourceType) const;
			std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
			std::unordered_set<VkDescriptorSetLayout> setLayouts; //use set so you can't accidentaly add a layout twice!
			struct vertexInputDescription {

				std::vector<VkVertexInputBindingDescription> bindings;
				std::vector<VkVertexInputAttributeDescription> attributes;
				VkPipelineVertexInputStateCreateFlags flags = 0;
			};
			std::unordered_map <std::string, VkPipelineVertexInputStateCreateInfo> vertexInputCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
			std::unordered_map<std::string, vertexInputDescription> inputDescriptions;
			VkPipelineShaderStageCreateInfo computeShaderStage = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
			std::vector<VkViewport> viewports;
			std::vector<VkRect2D> scissors;
			std::vector<VkDynamicState> dynamicStates;

			VkPipelineColorBlendAttachmentState colorBlendAttachementState;
			ref<renderPass> p_renderPass;
			pipelineType type;
			VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
			VkPipelineCreateFlags createFlags = 0;
			const VkDevice* p_device;
		};
	}
}

