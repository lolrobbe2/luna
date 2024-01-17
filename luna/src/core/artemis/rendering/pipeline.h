#pragma once
#include <core/artemis/rendering/shader.h>

namespace luna 
{
	namespace artemis 
	{
		enum pipelineType
		{
			GRAPHICS,
			COMPUTE
		};
		class pipelineBuilder 
		{
		public:
			pipelineBuilder(const VkDevice* p_device);
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
			pipelineBuilder& setColorMask(const bool red,const bool green,const bool blue,const bool alpha);
			pipelineBuilder& setPipelineType(const pipelineType type);
			pipelineBuilder& setCreateFlags(const VkPipelineCreateFlags createFlags = 0);
			pipelineBuilder& setTopology(const VkPrimitiveTopology topology);
			ref<pipeline> build();
		private:
			VkFormat getResourceFormat(const typeId resourceType) const;
			std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

			struct vertexInputDescription {

				std::vector<VkVertexInputBindingDescription> bindings;
				std::vector<VkVertexInputAttributeDescription> attributes;
				VkPipelineVertexInputStateCreateFlags flags = 0;
			};
			std::unordered_map <std::string, VkPipelineVertexInputStateCreateInfo> vertexInputCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
			std::unordered_map<std::string, vertexInputDescription> inputDescriptions;

			std::vector<VkViewport> viewports;
			std::vector<VkRect2D> scissors;
			std::vector<VkDynamicState> dynamicStates;

			VkPipelineColorBlendAttachmentState colorBlendAttachementState;
			
			pipelineType type;
			VkPrimitiveTopology topology;
			VkPipelineCreateFlags createFlags;
			const VkDevice* p_device;
		};
		class pipeline
		{
		public:
			pipeline(VkGraphicsPipelineCreateInfo createInfo);
			pipeline(VkComputePipelineCreateInfo createInfo);
		private:
			pipelineType type;
		};
	}
}


