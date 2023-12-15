#pragma once
#include <core/artemis/rendering/shader.h>

namespace luna 
{
	namespace artemis 
	{
		class pipelineBuilder 
		{
		public:
			pipelineBuilder(const VkDevice* device);
			/**
			* @brief adds a shaderStage from a given shader
			* @brief creates binding & attribute descriptions automaticcaly when the shaders shaderStage is vertexStage 
			*/
			pipelineBuilder addShaderStage(const ref<shader> shader, const VkPipelineShaderStageCreateFlags flags = 0);
			/**
			* @brief gets called automatticaly when a shader stage is added.
			*/
			pipelineBuilder createBindingDescription(const ref<shader> shader);
			/**
			* @brief gets called automatticaly when a shader stage is added.
			* @brief creates the vertexAttributeDiscription.
			*/
			pipelineBuilder createAttributeDescription(const ref<shader> shader);
			pipelineBuilder createVertexInputState(const ref<shader> shader, VkPipelineVertexInputStateCreateFlags flags = 0);
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
			const VkDevice* device;
		};
		class pipeline
		{
		};
	}
}


