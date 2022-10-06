#include "vulkanPipeline.h"
#include <core/vulkan/device/vulkanDevice.h>
namespace luna
{
	namespace vulkan 
	{
		vulkanPipeline::vulkanPipeline(const renderer::pipelineLayout& layout)
		{
		
			createPipeline(layout);
		}
		void vulkanPipeline::createPipeline(const renderer::pipelineLayout& layout)
		{
			this->layout = layout;
			createPipeLineLayout();
		}
		void vulkanPipeline::begin() const
		{
		}
		void vulkanPipeline::end() const
		{
		}
		void vulkanPipeline::createShaderStages()
		{
			for (const auto& shader : layout.pipelineShaders) 
			{
				VkPipelineShaderStageCreateInfo stageInfo;
				stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				stageInfo.pNext = nullptr;
				VkShaderModule shaderModule;
				LN_CORE_INFO("creatings vulkan shadermodule, result = {0}", createShaderModule(shader, &shaderModule));
				stageInfo.module = shaderModule;
				shaderModules.push_back(shaderModule);
				//the entry point of the shader
				stageInfo.pName = "main";
				stageInfo.flags = 0;
				stageInfo.pSpecializationInfo = nullptr;
				// convert engine stage to api stage.
				switch (shader->stage)
				{
				case luna::renderer::shaderStageVertex:
					stageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
					break;
				case luna::renderer::shaderStageTessellationControl:
					stageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
					break;
				case luna::renderer::shaderStageTessellationEvaluation:
					stageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
					break;
				case luna::renderer::shaderStageGeometry:
					stageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
					break;
				case luna::renderer::shaderStageFragment:
					stageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
					break;
				case luna::renderer::shaderStageCompute:
					stageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
					break;
				default:
					break;
				}
				shaderStages.push_back(stageInfo);
			}
		}
		void vulkanPipeline::createInputStates()
		{
			for (const auto& shader : layout.pipelineShaders) createBindingDescription(shader);
			for (const auto& shader : layout.pipelineShaders) createAttributeDescription(shader);
		}
		VkPipelineVertexInputStateCreateInfo vulkanPipeline::createVertexInputState(const ref<renderer::shader> shader)
		{
			VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
			vertexInputCreateInfo.pVertexAttributeDescriptions = inputDescriptions[shader->shaderName].attributes.data();
			vertexInputCreateInfo.pVertexBindingDescriptions = inputDescriptions[shader->shaderName].bindings.data();
			vertexInputCreateInfo.vertexAttributeDescriptionCount = inputDescriptions[shader->shaderName].attributes.size();
			vertexInputCreateInfo.vertexBindingDescriptionCount = inputDescriptions[shader->shaderName].bindings.size();
			vertexInputCreateInfo.flags = 0;
			return vertexInputCreateInfo;
		}
		void vulkanPipeline::createBindingDescription(const ref<renderer::shader> shader)
		{
			if (shader->stage != renderer::shaderStageVertex) return;

			std::unordered_map<uint32_t, VkVertexInputBindingDescription> bindingDescriptions;
			for(const auto& shaderResource : shader->shaderLayout)
			{
				if ((shaderResource.type != renderer::Uniform) && (shaderResource.type != renderer::PushConstant) && (shaderResource.type != renderer::StorageBuffer) && shaderResource.resourceClass == renderer::stageInputs)
				{
					
					VkVertexInputBindingDescription bindingDescription;
					auto bindingDescriptionPtr = bindingDescriptions.find(shaderResource.binding);
					if (bindingDescriptionPtr == bindingDescriptions.end())
					{

						bindingDescription.binding = shaderResource.binding;
						bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
						bindingDescription.stride = shaderResource.stride;
						bindingDescriptions.insert({ bindingDescription.binding ,bindingDescription });
					}
					else
					{
						
						bindingDescriptionPtr->second.stride += shaderResource.stride;
					}
				}
			}
			for (auto bindingDescription = bindingDescriptions.begin(); bindingDescription != bindingDescriptions.end(); bindingDescription++)inputDescriptions[shader->shaderName].bindings.push_back(bindingDescription->second);
			
		}
		void vulkanPipeline::createAttributeDescription(const ref<renderer::shader> shader)
		{
			if (shader->stage != renderer::shaderStageVertex) return;

			
			for (const auto& shaderResource : shader->shaderLayout)
			{
				if ((shaderResource.type != renderer::Uniform) && (shaderResource.type != renderer::PushConstant) && (shaderResource.type != renderer::StorageBuffer) && shaderResource.resourceClass == renderer::stageInputs)
				{
					VkVertexInputAttributeDescription attributeDescription;
					attributeDescription.binding = shaderResource.binding;
					attributeDescription.location = shaderResource.location;
					attributeDescription.offset = shaderResource.offset;
					attributeDescription.format = getResourceFormat(shaderResource.type);
					inputDescriptions[shader->shaderName].attributes.push_back(attributeDescription);
				}
			}
		}
		void vulkanPipeline::createPipeLineLayout()
		{
			ref<vulkanDevice> vDevice = std::dynamic_pointer_cast<vulkanDevice>(layout.device);
			vkCreatePipelineLayout(vDevice->getDeviceHandles().device, &pipelineLayoutCreateInfo(), nullptr, &pipelineLayout);
			createShaderStages();
			createInputStates();
			for (const auto& shader : layout.pipelineShaders)
			{
				vertexInputStates.push_back(createVertexInputState(shader));
			}
			initDefaultRenderpass();
			LN_CORE_INFO("pipelinecreation result = {0}",buildPipeline(vDevice->getDeviceHandles().device, renderPass));
		}

		VkResult vulkanPipeline::buildPipeline(VkDevice device, VkRenderPass pass) 
		{
			//make viewport state from our stored viewport and scissor.
			//at the moment we won't support multiple viewports or scissors
			ref<vulkanDevice> vDevice = std::dynamic_pointer_cast<vulkanDevice>(layout.device);
			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.pNext = nullptr;

			viewportState.viewportCount = 1;
			viewportState.pViewports = &vDevice->getViewport();
			viewportState.scissorCount = 1;
			viewportState.pScissors = &vDevice->getScissor();

			//setup dummy color blending. We aren't using transparent objects yet
			//the blending is just "no blend", but we do write to the color attachment
			VkPipelineColorBlendStateCreateInfo colorBlending = {};
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.pNext = nullptr;
			pipelineColorBlendAttachementState = colorBlendAttachmentState();
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &pipelineColorBlendAttachementState;
			//build the actual pipeline
			//we now use all of the info structs we have been writing into into this one to create the pipeline
			VkGraphicsPipelineCreateInfo pipelineInfo = {};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.pNext = nullptr;
			
			inputAssemblyStateCreateInfo =  inputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN);
			pipelineRasterizationStateCreateInfo = rasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);
			pipelineMultisampleStateCreateInfo = multisamplingStateCreateInfo();
			pipelineInfo.stageCount = shaderStages.size();
			pipelineInfo.pStages = shaderStages.data();
			pipelineInfo.pVertexInputState = vertexInputStates.data();
			pipelineInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
			pipelineInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.layout = pipelineLayout;
			pipelineInfo.renderPass = pass;
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

			//it's easy to error out on create graphics pipeline, so we handle it a bit better than the common VK_CHECK case
			
			return vkCreateGraphicsPipelines(device, 0, 1, &pipelineInfo, nullptr, &pipeline);
		}

		VkResult vulkanPipeline::createShaderModule(ref<renderer::shader> shader,VkShaderModule* shaderModule)
		{
			VkShaderModuleCreateInfo shaderModuleCreateInfo{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
			std::vector<uint32_t> shadersrc = shader->self();
			shaderModuleCreateInfo.codeSize = shadersrc.size() * sizeof(uint32_t);
			shaderModuleCreateInfo.pCode = shadersrc.data();
			ref<vulkanDevice> device = std::dynamic_pointer_cast<vulkanDevice>(layout.device);
			vulkanDevice::deviceHandles handles = device->getDeviceHandles();
			
			return vkCreateShaderModule(handles.device, &shaderModuleCreateInfo, nullptr, shaderModule);
		}

		VkFormat vulkanPipeline::getResourceFormat(renderer::typeId resourceType)
		{
			switch (resourceType)
			{
			case luna::renderer::Boolean:
				return VK_FORMAT_R8_UINT;
			case luna::renderer::SByte:
				return VK_FORMAT_R8_UINT;
			case luna::renderer::UByte:
				return VK_FORMAT_R8_UINT;
			case luna::renderer::Short:
				return VK_FORMAT_R16_UINT;
			case luna::renderer::UShort:
				return VK_FORMAT_R16_UINT;
			case luna::renderer::Int:
				return VK_FORMAT_R32_UINT;
			case luna::renderer::UInt:
				return VK_FORMAT_R32_UINT;
			case luna::renderer::Int64:
				return VK_FORMAT_R64_UINT;
			case luna::renderer::UInt64:
				return VK_FORMAT_R64_UINT;
			case luna::renderer::Float:
				return VK_FORMAT_R32_SFLOAT;
			case luna::renderer::Char:
				return VK_FORMAT_R8_UINT;
			case luna::renderer::Vec2:
				return VK_FORMAT_R32G32_SFLOAT;
			case luna::renderer::Vec3:
				return VK_FORMAT_R32G32B32_SFLOAT;
			case luna::renderer::Vec4:
				return VK_FORMAT_R32G32B32A32_SFLOAT;
			case luna::renderer::Half:
				return VK_FORMAT_R16_SFLOAT;
			default:
				return VK_FORMAT_MAX_ENUM;
			}
		}
		VkPipelineInputAssemblyStateCreateInfo vulkanPipeline::inputAssemblyCreateInfo(VkPrimitiveTopology topology) 
		{
			VkPipelineInputAssemblyStateCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			info.pNext = nullptr;

			info.topology = topology;
			//we are not going to use primitive restart on the entire tutorial so leave it on false
			info.primitiveRestartEnable = VK_FALSE;
			return info;
		}

		VkPipelineRasterizationStateCreateInfo vulkanPipeline::rasterizationStateCreateInfo(VkPolygonMode polygonMode)
		{
			VkPipelineRasterizationStateCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			info.pNext = nullptr;

			info.depthClampEnable = VK_FALSE;
			//discards all primitives before the rasterization stage if enabled which we don't want
			info.rasterizerDiscardEnable = VK_FALSE;

			info.polygonMode = polygonMode;
			info.lineWidth = 1.0f;
			//no backface cull
			info.cullMode = VK_CULL_MODE_NONE;
			info.frontFace = VK_FRONT_FACE_CLOCKWISE;
			//no depth bias
			info.depthBiasEnable = VK_FALSE;
			info.depthBiasConstantFactor = 0.0f;
			info.depthBiasClamp = 0.0f;
			info.depthBiasSlopeFactor = 0.0f;

			return info;
		}

		VkPipelineMultisampleStateCreateInfo vulkanPipeline::multisamplingStateCreateInfo()
		{
			VkPipelineMultisampleStateCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			info.pNext = nullptr;

			info.sampleShadingEnable = VK_FALSE;
			//multisampling defaulted to no multisampling (1 sample per pixel)
			info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			info.minSampleShading = 1.0f;
			info.pSampleMask = nullptr;
			info.alphaToCoverageEnable = VK_FALSE;
			info.alphaToOneEnable = VK_FALSE;
			return info;
		}
		VkPipelineColorBlendAttachmentState vulkanPipeline::colorBlendAttachmentState() 
		{
			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;
			return colorBlendAttachment;
		}

		VkPipelineLayoutCreateInfo vulkanPipeline::pipelineLayoutCreateInfo() 
		{
			VkPipelineLayoutCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			info.pNext = nullptr;

			//empty defaults
			info.flags = 0;
			info.setLayoutCount = 0;
			info.pSetLayouts = nullptr;
			info.pushConstantRangeCount = 0;
			info.pPushConstantRanges = nullptr;
			return info;
		}
		void vulkanPipeline::initDefaultRenderpass()
		{
			// the renderpass will use this color attachment.
			VkAttachmentDescription color_attachment = {};
			//the attachment will have the format needed by the swapchain
			ref<vulkanDevice> vDevice = std::dynamic_pointer_cast<vulkanDevice>(layout.device);
			color_attachment.format = vDevice->getSwapFormat();
			//1 sample, we won't be doing MSAA
			color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
			// we Clear when this attachment is loaded
			color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			// we keep the attachment stored when the renderpass ends
			color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			//we don't care about stencil
			color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

			//we don't know or care about the starting layout of the attachment
			color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			//after the renderpass ends, the image has to be on a layout ready for display
			color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference color_attachment_ref = {};
			//attachment number will index into the pAttachments array in the parent renderpass itself
			color_attachment_ref.attachment = 0;
			color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			//we are going to create 1 subpass, which is the minimum you can do
			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &color_attachment_ref;

			VkRenderPassCreateInfo render_pass_info = {};
			render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

			//connect the color attachment to the info
			render_pass_info.attachmentCount = 1;
			render_pass_info.pAttachments = &color_attachment;
			//connect the subpass to the info
			render_pass_info.subpassCount = 1;
			render_pass_info.pSubpasses = &subpass;
			

			vkCreateRenderPass(vDevice->getDeviceHandles().device, &render_pass_info, nullptr, &renderPass);
		}
	}
}
