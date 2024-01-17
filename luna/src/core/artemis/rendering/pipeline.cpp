#include "pipeline.h"
namespace luna 
{
	namespace artemis 
	{
		pipelineBuilder::pipelineBuilder(const VkDevice* p_device)
		{
			this->p_device = p_device;
		}
		pipelineBuilder& pipelineBuilder::addShaderStage(const ref<shader> shader, const VkPipelineShaderStageCreateFlags flags)
		{
			VkPipelineShaderStageCreateInfo info{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
			info.flags = flags;
			info.pName = shader->entryPoint().c_str();
			info.stage = *shader;
			info.module = *shader;
			shaderStages.push_back(info);
			return createBindingDescription(shader).createAttributeDescription(shader).createVertexInputState(shader);
		}

		pipelineBuilder& pipelineBuilder::createBindingDescription(const ref<shader> shader)
		{
			LN_PROFILE_FUNCTION();
			if (shader->stage() != VK_SHADER_STAGE_VERTEX_BIT) return *this;
			VkVertexInputBindingDescription bindingDescription;
			bindingDescription.stride = 0;
			inputDescriptions[shader->name()].bindings.resize(0);
			for (size_t i = shader->layout().size() - 1; i > 0; i--)
			{
				if ((shader->layout()[i].type == Uniform) || (shader->layout()[i].type == PushConstant) || (shader->layout()[i].type == StorageBuffer) && shader->layout()[i].resourceClass != stageInputs && bindingDescription.stride > shader->layout()[i].stride) break;
				bindingDescription.binding = shader->layout()[i].binding;
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				bindingDescription.stride = shader->layout()[i].stride;
				inputDescriptions[shader->name()].bindings.push_back(bindingDescription);
				return *this;
			}
		}
		pipelineBuilder& pipelineBuilder::createAttributeDescription(const ref<shader> shader)
		{
			LN_PROFILE_FUNCTION();
			if (shader->stage() != VK_SHADER_STAGE_VERTEX_BIT) return *this;
			inputDescriptions[shader->name()].attributes.resize(0);
			for (const auto& shaderResource : shader->layout())
			{
				if (!((shaderResource.type != Uniform) && (shaderResource.type != PushConstant) && (shaderResource.type != StorageBuffer) && shaderResource.resourceClass == stageInputs)) break;

				VkVertexInputAttributeDescription attributeDescription;
				attributeDescription.binding = shaderResource.binding;
				attributeDescription.location = shaderResource.location;
				attributeDescription.offset = shaderResource.offset;//TODO offset is wrong
				attributeDescription.format = getResourceFormat(shaderResource.type);
				inputDescriptions[shader->name()].attributes.push_back(attributeDescription);
			}
			return *this;
		}

		pipelineBuilder& pipelineBuilder::createVertexInputState(const ref<shader> shader, VkPipelineVertexInputStateCreateFlags flags)
		{
			if (shader->stage() != VK_SHADER_STAGE_VERTEX_BIT) return *this;

			vertexInputCreateInfo[shader->name()].pVertexAttributeDescriptions = inputDescriptions[shader->name()].attributes.data();
			vertexInputCreateInfo[shader->name()].pVertexBindingDescriptions = inputDescriptions[shader->name()].bindings.data();
			vertexInputCreateInfo[shader->name()].vertexAttributeDescriptionCount = inputDescriptions[shader->name()].attributes.size();
			vertexInputCreateInfo[shader->name()].vertexBindingDescriptionCount = inputDescriptions[shader->name()].bindings.size();
			vertexInputCreateInfo[shader->name()].flags = flags;
			return *this;
		}

		pipelineBuilder& pipelineBuilder::addViewport(const VkViewport viewport)
		{
			viewports.push_back(viewport);
			return *this;
		}

		pipelineBuilder& pipelineBuilder::addScissor(const VkRect2D scissor)
		{
			scissors.push_back(scissor);
			return *this;
		}

		pipelineBuilder& pipelineBuilder::addDynamicState(const VkDynamicState dynamicState)
		{
			dynamicStates.push_back(dynamicState);
			return *this;
		}

		pipelineBuilder& pipelineBuilder::enableBlending(bool enable)
		{
			colorBlendAttachementState.blendEnable = enable ? VK_TRUE : VK_FALSE;
			return *this;
		}

		pipelineBuilder& pipelineBuilder::setColorBlendingParams(const VkBlendFactor srcColor ,const VkBlendFactor dstColor ,const VkBlendOp blendOp)
		{
			colorBlendAttachementState.srcColorBlendFactor = srcColor;
			colorBlendAttachementState.dstColorBlendFactor = dstColor;
			colorBlendAttachementState.colorBlendOp = blendOp;
			return *this;
		}

		pipelineBuilder& pipelineBuilder::setAlphaBlendingParams(const VkBlendFactor srcColor, const VkBlendFactor dstColor, const VkBlendOp blendOp)
		{
			colorBlendAttachementState.srcAlphaBlendFactor = srcColor;
			colorBlendAttachementState.dstAlphaBlendFactor = dstColor;
			colorBlendAttachementState.alphaBlendOp = blendOp;
			return *this;
		}

		pipelineBuilder& pipelineBuilder::setColorMask(const bool red, const bool green, const bool blue, const bool alpha)
		{
			if (red) colorBlendAttachementState.colorWriteMask |= VK_COLOR_COMPONENT_R_BIT;
			if (green) colorBlendAttachementState.colorWriteMask |= VK_COLOR_COMPONENT_G_BIT;
			if (blue) colorBlendAttachementState.colorWriteMask |= VK_COLOR_COMPONENT_B_BIT;

			if (red) colorBlendAttachementState.colorWriteMask |= VK_COLOR_COMPONENT_A_BIT;

			return *this;
		}

		pipelineBuilder& pipelineBuilder::setPipelineType(const pipelineType type)
		{
			this->type = type;
			return *this;
		}

		pipelineBuilder& pipelineBuilder::setCreateFlags(const VkPipelineCreateFlags createFlags)
		{
			this->createFlags = createFlags;
			return *this;
		}

		ref<pipeline> pipelineBuilder::build()
		{
			if (type == GRAPHICS)
			{
				VkPipelineColorBlendStateCreateInfo* pColorBlendState = new VkPipelineColorBlendStateCreateInfo();//FREED when the pipeline is destroyed?
				pColorBlendState->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
				pColorBlendState->attachmentCount = 1;
				pColorBlendState->pAttachments = &colorBlendAttachementState;
				pColorBlendState->logicOpEnable = VK_FALSE;

				VkPipelineDynamicStateCreateInfo* pDynamicStateCreateInfo = new VkPipelineDynamicStateCreateInfo();//FREED when the pipeline is destroyed?
				pDynamicStateCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
				pDynamicStateCreateInfo->dynamicStateCount = dynamicStates.size();
				pDynamicStateCreateInfo->flags = 0; //NOT SUPPORTED!
				pDynamicStateCreateInfo->pNext = nullptr;
				pDynamicStateCreateInfo->pDynamicStates = dynamicStates.data();

				VkPipelineViewportStateCreateInfo* pViewportState = new VkPipelineViewportStateCreateInfo();
				pViewportState->sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
				pViewportState->pNext = nullptr;
				pViewportState->viewportCount = viewports.size();
				pViewportState->pViewports = viewports.data();
				pViewportState->scissorCount = scissors.size();
				pViewportState->pScissors = scissors.data();

				VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyState = new VkPipelineInputAssemblyStateCreateInfo();
				pInputAssemblyState->sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
				pInputAssemblyState->pNext = nullptr;
				pInputAssemblyState->topology = topology;
				pInputAssemblyState->primitiveRestartEnable = VK_FALSE;//NOT SUPPORTED!
				

				VkGraphicsPipelineCreateInfo createInfo{ VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
				createInfo.basePipelineHandle = VK_NULL_HANDLE; //NOT SUPPORTED!
				createInfo.basePipelineIndex = 0; //NOT_SUPPORTED!
				createInfo.flags = createFlags;

				
				createInfo.pColorBlendState = pColorBlendState;
				createInfo.pDepthStencilState = nullptr; //not needed since 3D is not supported!
				createInfo.pDynamicState = pDynamicStateCreateInfo;
				createInfo.pInputAssemblyState = pInputAssemblyState;
				createInfo.pMultisampleState = 
				return createRef<pipeline>(createInfo); 
			}
			return ref<pipeline>();
		}

		VkFormat pipelineBuilder::getResourceFormat(const typeId resourceType) const
		{
			LN_PROFILE_FUNCTION();
			switch (resourceType)
			{
			case Boolean:
				return VK_FORMAT_R8_UINT;
			case SByte:
				return VK_FORMAT_R8_UINT;
			case UByte:
				return VK_FORMAT_R8_UINT;
			case Short:
				return VK_FORMAT_R16_UINT;
			case UShort:
				return VK_FORMAT_R16_UINT;
			case Int:
				return VK_FORMAT_R32_UINT;
			case UInt:
				return VK_FORMAT_R32_UINT;
			case Int64:
				return VK_FORMAT_R64_UINT;
			case UInt64:
				return VK_FORMAT_R64_UINT;
			case Float:
				return VK_FORMAT_R32_SFLOAT;
			case Char:
				return VK_FORMAT_R8_UINT;
			case Vec2:
				return VK_FORMAT_R32G32_SFLOAT;
			case Vec3:
				return VK_FORMAT_R32G32B32_SFLOAT;
			case Vec4:
				return VK_FORMAT_R32G32B32A32_SFLOAT;
			case Half:
				return VK_FORMAT_R16_SFLOAT;
			default:
				LN_CORE_ERROR("[Artemis] resourceType not recognized!");
				return VK_FORMAT_MAX_ENUM;
			}
		}
		pipeline::pipeline(VkGraphicsPipelineCreateInfo createInfo)
		{
			type = GRAPHICS;
		}
		pipeline::pipeline(VkComputePipelineCreateInfo createInfo)
		{
			type = COMPUTE;
		}
}
}

