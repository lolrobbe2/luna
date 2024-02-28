#include "pipelineBuilder.h"
#include <core/artemis/rendering/renderPass.h>
#include <core/debug/debugMacros.h>

namespace luna 
{
	namespace artemis 
	{
		pipelineBuilder::pipelineBuilder(const VkDevice* p_device)
		{
			this->p_device = p_device;
			colorBlendAttachementState.blendEnable = VK_FALSE;
		}
		pipelineBuilder& pipelineBuilder::addShaderStage(const ref<shader> shader, const VkPipelineShaderStageCreateFlags flags)
		{
			VkPipelineShaderStageCreateInfo info = *shader;
			info.flags = flags;
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
			vertexInputCreateInfo[shader->name()].sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
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

		pipelineBuilder& pipelineBuilder::setColorBlendingParams(const VkBlendFactor srcColor, const VkBlendFactor dstColor, const VkBlendOp blendOp)
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

		pipelineBuilder& pipelineBuilder::setColorMask(VkColorComponentFlags colorWriteMask)
		{
			
			colorBlendAttachementState.colorWriteMask = colorWriteMask;
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

		pipelineBuilder& pipelineBuilder::setTopology(const VkPrimitiveTopology topology)
		{
			this->topology = topology;
			return *this;
		}

		pipelineBuilder& pipelineBuilder::setPolygonMode(const VkPolygonMode polygonMode)
		{
			this->polygonMode = polygonMode;
			return *this;
		}

		pipelineBuilder& pipelineBuilder::addDescriptorSetLayout(const VkDescriptorSetLayout layout)
		{
			setLayouts.insert(layout);
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


				LN_ERR_FAIL_COND_V_MSG(dynamicStates.size() == 0, ref<pipeline>(), "[Artemis] a graphics pipeline must have atleast 1 dynamic state!");
				VkPipelineDynamicStateCreateInfo* pDynamicStateCreateInfo = new VkPipelineDynamicStateCreateInfo();//FREED when the pipeline is destroyed?
				pDynamicStateCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
				pDynamicStateCreateInfo->dynamicStateCount = dynamicStates.size();
				pDynamicStateCreateInfo->flags = 0; //NOT SUPPORTED!
				pDynamicStateCreateInfo->pNext = nullptr;
				pDynamicStateCreateInfo->pDynamicStates = dynamicStates.data();

				LN_ERR_FAIL_COND_V_MSG(viewports.size() == 0, ref<pipeline>(), "[Artemis] a graphics pipeline must have atleast 1 viewport!");
				LN_ERR_FAIL_COND_V_MSG(scissors.size() == 0, ref<pipeline>(), "[Artemis] a graphics pipeline must have atleast 1 viewport scissor!");

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

				VkPipelineMultisampleStateCreateInfo* pMultisampleState = new VkPipelineMultisampleStateCreateInfo();
				pMultisampleState->sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
				pMultisampleState->pNext = nullptr;

				pMultisampleState->sampleShadingEnable = VK_FALSE;
				//multisampling defaulted to no multisampling (1 sample per pixel)
				pMultisampleState->rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
				pMultisampleState->minSampleShading = 1.0f;
				pMultisampleState->pSampleMask = nullptr;
				pMultisampleState->alphaToCoverageEnable = VK_FALSE;
				pMultisampleState->alphaToOneEnable = VK_FALSE;

				VkPipelineRasterizationStateCreateInfo* pRasterizationState = new VkPipelineRasterizationStateCreateInfo();
				pRasterizationState->sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
				pRasterizationState->pNext = nullptr;

				pRasterizationState->depthClampEnable = VK_FALSE;
				//discards all primitives before the rasterization stage if enabled which we don't want
				pRasterizationState->rasterizerDiscardEnable = VK_FALSE;

				pRasterizationState->polygonMode = polygonMode;
				pRasterizationState->lineWidth = 1.0f;
				//no backface cull
				pRasterizationState->cullMode = VK_CULL_MODE_BACK_BIT;
				pRasterizationState->frontFace = VK_FRONT_FACE_CLOCKWISE;
				//no depth bias
				pRasterizationState->depthBiasEnable = VK_FALSE;


				LN_ERR_FAIL_COND_V_MSG(setLayouts.size() == 0, ref<pipeline>(), "[Artemis] a graphics pipeline must have atleast 1 descriptorSetLayout!");
				VkPipelineLayout pipelineLayout;
				VkPipelineLayoutCreateInfo layoutCreateInfo{};
				layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				layoutCreateInfo.pNext = nullptr;

				//empty defaults
				layoutCreateInfo.flags = 0;
				layoutCreateInfo.setLayoutCount = setLayouts.size();
				layoutCreateInfo.pSetLayouts = std::vector<VkDescriptorSetLayout>(setLayouts.begin(), setLayouts.end()).data();
				layoutCreateInfo.pushConstantRangeCount = 0;
				layoutCreateInfo.pPushConstantRanges = nullptr; //no pushConstants
				VkResult layoutCreateRes = vkCreatePipelineLayout(*p_device, &layoutCreateInfo, nullptr, &pipelineLayout);
				LN_ERR_FAIL_COND_V_MSG(layoutCreateRes != VK_SUCCESS, ref<pipeline>(), "[Artemis] and error occured when creating the graphics pipeline layout, VkResult: " + VK_RESULT(layoutCreateRes));

				VkGraphicsPipelineCreateInfo createInfo{ VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
				createInfo.basePipelineHandle = VK_NULL_HANDLE; //NOT SUPPORTED!
				createInfo.basePipelineIndex = 0; //NOT_SUPPORTED!
				createInfo.flags = createFlags;
				std::vector<VkPipelineVertexInputStateCreateInfo>  vertexInputState;
				vertexInputState.reserve(vertexInputCreateInfo.size());

				std::transform(vertexInputCreateInfo.begin(), vertexInputCreateInfo.end(), std::back_inserter(vertexInputState),
					[](const auto& pair) { return pair.second; });
				createInfo.pVertexInputState = vertexInputState.data();
				createInfo.pColorBlendState = pColorBlendState;
				createInfo.pDepthStencilState = nullptr; //not needed since 3D is not supported!
				createInfo.pDynamicState = pDynamicStateCreateInfo;
				createInfo.pInputAssemblyState = pInputAssemblyState;
				createInfo.pMultisampleState = pMultisampleState;
				createInfo.pRasterizationState = pRasterizationState;
				createInfo.pViewportState = pViewportState;
				createInfo.layout = pipelineLayout;
				createInfo.stageCount = shaderStages.size();
				createInfo.pStages = shaderStages.data();
				LN_ERR_FAIL_NULL_V_MSG(p_renderPass, ref<pipeline>(), "[Artemis] a graphics pipeline requires a renderPass!");
				createInfo.renderPass = *p_renderPass;
				return ref<pipeline>(new pipeline(p_device, createInfo));
			}
			else
			{
				LN_ERR_FAIL_COND_V_MSG(setLayouts.size() == 0, ref<pipeline>(), "[Artemis] a compute pipeline must have atleast 1 descriptorSetLayout!");
				VkPipelineLayout pipelineLayout;
				VkPipelineLayoutCreateInfo layoutCreateInfo{};
				layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				layoutCreateInfo.pNext = nullptr;

				//empty defaults
				layoutCreateInfo.flags = 0;
				layoutCreateInfo.setLayoutCount = setLayouts.size();
				layoutCreateInfo.pSetLayouts = std::vector<VkDescriptorSetLayout>(setLayouts.begin(), setLayouts.end()).data();
				layoutCreateInfo.pushConstantRangeCount = 0;
				layoutCreateInfo.pPushConstantRanges = nullptr; //no pushConstants
				VkResult layoutCreateRes = vkCreatePipelineLayout(*p_device, &layoutCreateInfo, nullptr, &pipelineLayout);
				LN_ERR_FAIL_COND_V_MSG(layoutCreateRes != VK_SUCCESS, ref<pipeline>(), "[Artemis] an error occured when creating the compute pipeline layout, VkResult: " + VK_RESULT(layoutCreateRes));

				LN_ERR_FAIL_COND_V_MSG(shaderStages.size() != 1, ref<pipeline>(), "[Artemis] a compute pipeline can only have one shaderStage!");
				VkComputePipelineCreateInfo createInfo = { VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO };
				createInfo.basePipelineHandle = VK_NULL_HANDLE; //not supported!
				createInfo.basePipelineIndex = 0; //not supported!
				createInfo.flags = createFlags;
				createInfo.layout = pipelineLayout;
				createInfo.stage = shaderStages[0]; //no other posibility so hardcode it!
				createInfo.pNext = nullptr;
				return ref<pipeline>(new pipeline(p_device, createInfo));
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
	}
}