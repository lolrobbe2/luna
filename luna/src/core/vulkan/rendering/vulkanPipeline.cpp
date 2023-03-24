#include "vulkanPipeline.h"
#include <core/vulkan/device/vulkanDevice.h>
#include <backends/imgui_impl_vulkan.cpp>
#include <core/vulkan/utils/vulkanAllocator.h>
#include <core/vulkan/rendering/vulkanVertexBuffer.h>
#include <core/vulkan/rendering/vulkanIndexBuffer.h>
#include <core/platform/platform.h>
#include <imgui_internal.h>
namespace luna
{
	namespace vulkan
	{
		vulkanPipeline::vulkanPipeline(const renderer::pipelineLayout& layout)
		{
		
			LN_PROFILE_FUNCTION();
			vulkanCmdPoolSpec commandPoolSpec;
			ref<vulkanDevice> device = std::dynamic_pointer_cast<vulkanDevice>(layout.device);
			this->layout = layout;
			maxFramesInFlight = device->swapchain->mSwapchain.image_count - 1;
			commandPoolSpec.device = device->getDeviceHandles().device;
			commandPoolSpec.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			commandPoolSpec.queueFamilyIndex = device->getQueueIndex(vkb::QueueType::present);
			ref<vulkanCmdPool> commandpool{ new vulkanCmdPool(commandPoolSpec) };
			commandPool = commandpool;
			commandBuffers.resize(maxFramesInFlight);
			LN_CORE_INFO("commandbuffer create info = {0}", commandPool->createNewBuffer(commandBuffers.data(), maxFramesInFlight, VK_COMMAND_BUFFER_LEVEL_PRIMARY));
			presentQueue = device->getQueue(vkb::QueueType::present);
			utils::vulkanAllocator::init(layout.device);
			sampler = ref<renderer::vulkanSampler>(new renderer::vulkanSampler(device, VK_FILTER_NEAREST));

			VkDescriptorPoolSize poolSizes[] =
			{
				{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
			};
			descriptorPool = ref<utils::vulkanDescriptorPool>(new utils::vulkanDescriptorPool(device, poolSizes, std::size(poolSizes), layout.pipelineShaders[0]->shaderLayout, 0));
			descriptorSets.resize(100);

			descriptorPool->createDescriptorSets(descriptorSets);
			createPipeline(layout);
			drawCommands.reserve(1000);

		}
		void vulkanPipeline::createPipeline(const renderer::pipelineLayout& layout)
		{
			LN_PROFILE_FUNCTION();
			this->layout = layout;
			createPipeLineLayout();
		}

		void vulkanPipeline::destroyPipeline()
		{
			LN_PROFILE_FUNCTION();
			ref<vulkanDevice> vDevice = std::dynamic_pointer_cast<vulkanDevice>(layout.device);
			VkDevice device = vDevice->getDeviceHandles().device;
			vkDeviceWaitIdle(device);
			for (const auto& shaderModule : shaderModules)
			{
				vkDestroyShaderModule(device, shaderModule, nullptr);
			}
			vkDestroyRenderPass(device, renderPass, nullptr);
			vkDestroyPipeline(device, pipeline, nullptr);
			vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
		}

		void vulkanPipeline::createCommands()
		{
			LN_PROFILE_FUNCTION();
			ref<vulkanDevice> vDevice = std::dynamic_pointer_cast<vulkanDevice>(layout.device);
			VkDevice device = vDevice->getDeviceHandles().device;
			VkClearColorValue blankValue = { 0.0f,0.0f,0.0f,0.0f };
			VkClearValue clearValue;
			clearValue.color = blankValue;

			//start the main renderpass.
			//We will use the clear color from above, and the framebuffer of the index the swapchain gave us
			VkExtent2D extent = vDevice->swapchain->mSwapchain.extent;
			VkImageSubresourceRange imageSubRange;
			imageSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageSubRange.baseMipLevel = 0;
			imageSubRange.baseArrayLayer = 0;
			imageSubRange.layerCount = 1;
			imageSubRange.levelCount = VK_REMAINING_MIP_LEVELS;


			VkRenderPassBeginInfo rpInfo = {};
			rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			rpInfo.pNext = nullptr;


			rpInfo.renderPass = renderPass;
			rpInfo.renderArea.offset.x = 0;
			rpInfo.renderArea.offset.y = 0;
			rpInfo.renderArea.extent = extent;
			rpInfo.framebuffer = vDevice->swapchain->getFrameBuffer(swapchainImageIndex);

			commandPool->begin(commandBuffers[currentFrame], 0);
			//connect clear values
			rpInfo.clearValueCount = 1;
			rpInfo.pClearValues = &clearValue;

			vkCmdBeginRenderPass(commandPool->operator=(commandBuffers[currentFrame]), &rpInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandPool->operator=(commandBuffers[currentFrame]), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
			//draw indexed
			for (auto draw : drawCommands) fnDrawIndexed(draw.vertexArray, draw.descriptorIndex, draw.textures, draw.indexCount);

			vkCmdEndRenderPass(commandPool->operator=(commandBuffers[currentFrame]));
			//transition dst image

			transitionImageLayout(vDevice->swapchain->sceneViewportImages[currentFrame], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, commandPool->operator=(commandBuffers[currentFrame]));
			//transition src image
			transitionImageLayout(vDevice->swapchain->mSwapchain.get_images().value()[swapchainImageIndex], vDevice->swapchain->mSwapchain.image_format, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, commandPool->operator=(commandBuffers[currentFrame]));

			VkImageCopy imageCopy;
			imageCopy.dstOffset = { 0,0,0 };
			imageCopy.srcOffset = { 0,0,0 };

			imageCopy.extent = { vDevice->swapchain->mSwapchain.extent.width, vDevice->swapchain->mSwapchain.extent.height,1 };

			imageCopy.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopy.srcSubresource.mipLevel = 0;
			imageCopy.srcSubresource.baseArrayLayer = 0;
			imageCopy.srcSubresource.layerCount = 1;

			imageCopy.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopy.dstSubresource.mipLevel = 0;


			imageCopy.dstSubresource.baseArrayLayer = 0;
			imageCopy.dstSubresource.layerCount = 1;


			vkCmdCopyImage(commandPool->operator=(commandBuffers[currentFrame]), vDevice->swapchain->mSwapchain.get_images().value()[swapchainImageIndex], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vDevice->swapchain->sceneViewportImages[currentFrame], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopy);

			transitionImageLayout(vDevice->swapchain->sceneViewportImages[currentFrame], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, commandPool->operator=(commandBuffers[currentFrame]));

			transitionImageLayout(vDevice->swapchain->mSwapchain.get_images().value()[swapchainImageIndex], vDevice->swapchain->mSwapchain.image_format, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, commandPool->operator=(commandBuffers[currentFrame]));

			vkCmdBeginRenderPass(commandPool->operator=(commandBuffers[currentFrame]), &rpInfo, VK_SUBPASS_CONTENTS_INLINE);

			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandPool->operator=(commandBuffers[currentFrame]));
			vkCmdEndRenderPass(commandPool->operator=(commandBuffers[currentFrame]));



			//TODO gpu driven rendering (VkDrawIndirect);

			//finalize the command buffer (we can no longer add commands, but it can now be executed)
			commandPool->end(commandBuffers[currentFrame]);

		}
		void vulkanPipeline::begin()
		{
			LN_PROFILE_FUNCTION();
			ref<vulkanDevice> vDevice = std::dynamic_pointer_cast<vulkanDevice>(layout.device);
			descriptorsetIndex = 1;
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			//imgui commands
			ImGui::NewFrame();
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::DockSpaceOverViewport(viewport, ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoResize);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			if (ImGui::Begin("scene"));
			{
				ImVec2 scrollPos = ImGui::GetCursorScreenPos();
				ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
				ImVec2 mousePos = ImGui::GetMousePos();
				windowMousePos.x = mousePos.x - scrollPos.x;
				windowMousePos.y = mousePos.y - scrollPos.y;
				windowDimensions = { viewportPanelSize.x,viewportPanelSize.y };
				ImGui::Image(vDevice->swapchain->getViewportImage(currentFrame), viewportPanelSize);
			}
			
			ImGui::PopStyleVar(1);
			ImGui::End();
		}
		void vulkanPipeline::end() const
		{
			LN_PROFILE_FUNCTION();
			ImGui::Render();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

		}
		void vulkanPipeline::flush()
		{
			LN_PROFILE_FUNCTION();
			ref<vulkanDevice> vDevice = std::dynamic_pointer_cast<vulkanDevice>(layout.device);


			vkDeviceWaitIdle(vDevice->getDeviceHandles().device);
			vkWaitForFences(vDevice->getDeviceHandles().device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
			VkResult result = vkAcquireNextImageKHR(vDevice->getDeviceHandles().device, vDevice->swapchain->mSwapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &swapchainImageIndex);


			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				if (vDevice->window->getWidth() <= 0 || vDevice->window->getHeight() <= 0) return;
				//commandPool->resetCommandBuffer(commandBuffers[currentFrame]);
				destroyPipeline();
				createPipeLineLayout();
				for (auto commandBuffer : commandBuffers) commandPool->resetCommandBuffer(commandBuffer);
				vDevice->swapchain->recreateSwapchain();
				vDevice->createFramebuffers(renderPass);
				vDevice->swapchain->recreateViewport(maxFramesInFlight);

				return;
			}
			vkResetFences(vDevice->getDeviceHandles().device, 1, &inFlightFences[currentFrame]);

			imagesInFlight[swapchainImageIndex] = inFlightFences[currentFrame];
			commandPoolSubmitInfo submit = {};
			submit.pNext = nullptr;

			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
			VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
			submit.pWaitDstStageMask = waitStages;
			submit.pCommandBuffers = &commandBuffers[currentFrame];
			submit.commandBufferCount = 1;
			submit.waitSemaphoreCount = 1;
			submit.pWaitSemaphores = waitSemaphores;
			submit.signalSemaphoreCount = 1;
			submit.pSignalSemaphores = signalSemaphores;


			vkResetFences(vDevice->getDeviceHandles().device, 1, &inFlightFences[currentFrame]);
			commandPool->resetCommandBuffer(commandBuffers[currentFrame]);
			createCommands();
			commandPool->flush(presentQueue, 1, &submit, inFlightFences[currentFrame]);
			//submit command buffer to the queue and execute it.
			// _renderFence will now block until the graphic commands finish execution
			drawCommands.clear();


			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.pNext = nullptr;
			VkSwapchainKHR swapChains[] = { vDevice->swapchain->mSwapchain };
			presentInfo.pSwapchains = swapChains;
			presentInfo.swapchainCount = 1;

			presentInfo.pWaitSemaphores = signalSemaphores;
			presentInfo.waitSemaphoreCount = 1;

			presentInfo.pImageIndices = &swapchainImageIndex;
			presentInfo.pResults = nullptr;

			VkResult result2 = vkQueuePresentKHR(presentQueue, &presentInfo);
			_frameNumber += 1;

			if (result2 == VK_ERROR_OUT_OF_DATE_KHR || result2 == VK_SUBOPTIMAL_KHR)
			{
				initSyncStructures();
				vDevice->swapchain->recreateSwapchain();
				vDevice->createFramebuffers(renderPass);
				vDevice->swapchain->recreateViewport(maxFramesInFlight);
				return;
			}
			currentFrame = (currentFrame + 1) % maxFramesInFlight;

		}
		void vulkanPipeline::bindTextures(const std::vector<uint64_t> textureHandles, const uint64_t indexSet)
		{
			std::vector<VkDescriptorImageInfo> imageInfos;
			for (const uint64_t& textureHandle : textureHandles)
			{
				imageInfos.push_back({ VK_NULL_HANDLE, (VkImageView)textureHandle, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL });
			}
			descriptorSets[0]->write(indexSet, imageInfos.data());
		}
		void vulkanPipeline::clear()
		{
			LN_PROFILE_FUNCTION();
			drawCommands.clear();
		}
		void vulkanPipeline::createShaderStages()
		{
			LN_PROFILE_FUNCTION();
			shaderStages.resize(0);
			shaderModules.resize(0);
			for (const auto& shader : layout.pipelineShaders)
			{
				VkPipelineShaderStageCreateInfo stageInfo;
				stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				stageInfo.pNext = nullptr;
				VkShaderModule shaderModule;
				LN_CORE_INFO("creating vulkan shadermodule, result = {0}", createShaderModule(shader, &shaderModule));
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
				shaderStages.insert(shaderStages.begin(), stageInfo);
			}
		}
		void vulkanPipeline::createInputStates()
		{
			LN_PROFILE_FUNCTION();
			for (const auto& shader : layout.pipelineShaders) createBindingDescription(shader);
			for (const auto& shader : layout.pipelineShaders) createAttributeDescription(shader);
		}
		VkPipelineVertexInputStateCreateInfo vulkanPipeline::createVertexInputState(const ref<renderer::shader> shader)
		{
			LN_PROFILE_FUNCTION();
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
			LN_PROFILE_FUNCTION();
			if (shader->stage != renderer::shaderStageVertex) return;
			VkVertexInputBindingDescription bindingDescription;
			bindingDescription.stride = 0;
			inputDescriptions[shader->shaderName].bindings.resize(0);
			for (size_t i = shader->shaderLayout.size() - 1; i > 0; i--)
			{
				if ((shader->shaderLayout[i].type == renderer::Uniform) || (shader->shaderLayout[i].type == renderer::PushConstant) || (shader->shaderLayout[i].type == renderer::StorageBuffer) && shader->shaderLayout[i].resourceClass != renderer::stageInputs && bindingDescription.stride > shader->shaderLayout[i].stride) break;
				bindingDescription.binding = shader->shaderLayout[i].binding;
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				bindingDescription.stride = shader->shaderLayout[i].stride;
				inputDescriptions[shader->shaderName].bindings.push_back(bindingDescription);
				return;

			}
		}
		void vulkanPipeline::createAttributeDescription(const ref<renderer::shader> shader)
		{
			LN_PROFILE_FUNCTION();
			if (shader->stage != renderer::shaderStageVertex) return;
			inputDescriptions[shader->shaderName].attributes.resize(0);
			for (const auto& shaderResource : shader->shaderLayout)
			{
				if (!((shaderResource.type != renderer::Uniform) && (shaderResource.type != renderer::PushConstant) && (shaderResource.type != renderer::StorageBuffer) && shaderResource.resourceClass == renderer::stageInputs)) break;

				VkVertexInputAttributeDescription attributeDescription;
				attributeDescription.binding = shaderResource.binding;
				attributeDescription.location = shaderResource.location;
				attributeDescription.offset = shaderResource.offset;//TODO offset is wrong
				attributeDescription.format = getResourceFormat(shaderResource.type);
				inputDescriptions[shader->shaderName].attributes.push_back(attributeDescription);

			}
		}
		void vulkanPipeline::createPipeLineLayout()
		{
			LN_PROFILE_FUNCTION();
			ref<vulkanDevice> vDevice = std::dynamic_pointer_cast<vulkanDevice>(layout.device);
			vkCreatePipelineLayout(vDevice->getDeviceHandles().device, &pipelineLayoutCreateInfo(), nullptr, &pipelineLayout);
			createShaderStages();
			createInputStates();
			vertexInputStates.resize(0);
			for (const auto& shader : layout.pipelineShaders)
			{
				if (shader->stage == renderer::shaderStageVertex) vertexInputStates.push_back(createVertexInputState(shader));
			}
			initDefaultRenderpass();
			vDevice->createFramebuffers(renderPass);
			initSyncStructures();
			buildPipeline(vDevice->getDeviceHandles().device, renderPass);

		}

		VkResult vulkanPipeline::buildPipeline(VkDevice device, VkRenderPass pass)
		{
			LN_PROFILE_FUNCTION();
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

			VkDynamicState dynamicState[] = { VK_DYNAMIC_STATE_VIEWPORT , VK_DYNAMIC_STATE_SCISSOR };

			VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;
			dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicStateCreateInfo.dynamicStateCount = 1;
			dynamicStateCreateInfo.flags = 0;
			dynamicStateCreateInfo.pNext = nullptr;
			dynamicStateCreateInfo.pDynamicStates = dynamicState;



			VkPipelineColorBlendStateCreateInfo colorBlending = {};
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.pNext = nullptr;
			pipelineColorBlendAttachementState = colorBlendAttachmentState();
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &pipelineColorBlendAttachementState;
			colorBlending.blendConstants[0] = 0.0f;
			colorBlending.blendConstants[1] = 0.0f;
			colorBlending.blendConstants[2] = 0.0f;
			colorBlending.blendConstants[3] = 0.0f;
			//build the actual pipeline
			//we now use all of the info structs we have been writing into into this one to create the pipeline
			VkGraphicsPipelineCreateInfo pipelineInfo = {};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.pNext = nullptr;

			inputAssemblyStateCreateInfo = inputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
			pipelineRasterizationStateCreateInfo = rasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);
			pipelineMultisampleStateCreateInfo = multisamplingStateCreateInfo();
			pipelineInfo.stageCount = shaderStages.size();
			pipelineInfo.pStages = shaderStages.data();
			pipelineInfo.pVertexInputState = vertexInputStates.data();
			pipelineInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
			pipelineInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
			//pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
			pipelineInfo.pDepthStencilState = nullptr;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.layout = pipelineLayout;
			pipelineInfo.renderPass = pass;
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

			//it's easy to error out on create graphics pipeline, so we handle it a bit better than the common VK_CHECK case

			return vkCreateGraphicsPipelines(device, 0, 1, &pipelineInfo, nullptr, &pipeline);
		}

		VkResult vulkanPipeline::createShaderModule(ref<renderer::shader> shader, VkShaderModule* shaderModule)
		{
			LN_PROFILE_FUNCTION();
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
			LN_PROFILE_FUNCTION();
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
			LN_PROFILE_FUNCTION();
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
			LN_PROFILE_FUNCTION();
			VkPipelineRasterizationStateCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			info.pNext = nullptr;

			info.depthClampEnable = VK_FALSE;
			//discards all primitives before the rasterization stage if enabled which we don't want
			info.rasterizerDiscardEnable = VK_FALSE;

			info.polygonMode = polygonMode;
			info.lineWidth = 1.0f;
			//no backface cull
			info.cullMode = VK_CULL_MODE_BACK_BIT;
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
			LN_PROFILE_FUNCTION();
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
			LN_PROFILE_FUNCTION();
			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_TRUE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

			return colorBlendAttachment;
		}

		VkPipelineLayoutCreateInfo vulkanPipeline::pipelineLayoutCreateInfo()
		{
			LN_PROFILE_FUNCTION();
			VkPipelineLayoutCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			info.pNext = nullptr;

			//empty defaults
			info.flags = 0;
			info.setLayoutCount = 1;
			info.pSetLayouts = descriptorPool->getLayoutPtr();
			info.pushConstantRangeCount = 0;
			info.pPushConstantRanges = nullptr;
			return info;
		}
		void vulkanPipeline::initDefaultRenderpass()
		{
			LN_PROFILE_FUNCTION();
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
			color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;

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

			VkSubpassDependency dependency = {};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;

			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			VkRenderPassCreateInfo render_pass_info = {};
			render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

			//connect the color attachment to the info
			render_pass_info.attachmentCount = 1;
			render_pass_info.pAttachments = &color_attachment;
			//connect the subpass to the info
			render_pass_info.subpassCount = 1;
			render_pass_info.pSubpasses = &subpass;
			render_pass_info.dependencyCount = 1;
			render_pass_info.pDependencies = &dependency;

			vkCreateRenderPass(vDevice->getDeviceHandles().device, &render_pass_info, nullptr, &renderPass);

		}

		void vulkanPipeline::initSyncStructures() //TODO make seperate class from this
		{
			LN_PROFILE_FUNCTION();
			//create synchronization structures
			ref<vulkanDevice> vDevice = std::dynamic_pointer_cast<vulkanDevice>(layout.device);
			destroySyncStructures();
			imageAvailableSemaphores.resize(vDevice->swapchain->mSwapchain.image_count);
			renderFinishedSemaphores.resize(maxFramesInFlight);
			inFlightFences.resize(maxFramesInFlight);
			imagesInFlight.resize(vDevice->swapchain->mSwapchain.image_count, VK_NULL_HANDLE);

			VkSemaphoreCreateInfo semaphoreInfo{};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			for (size_t i = 0; i < maxFramesInFlight; i++)
			{
				vkCreateSemaphore(vDevice->getDeviceHandles().device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) ||
					vkCreateFence(vDevice->getDeviceHandles().device, &fenceInfo, nullptr, &inFlightFences[i]);
			}
			for (size_t i = 0; i < vDevice->swapchain->mSwapchain.image_count; i++) vkCreateSemaphore(vDevice->getDeviceHandles().device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);

		}

		void vulkanPipeline::destroySyncStructures()
		{
			LN_PROFILE_FUNCTION();
			ref<vulkanDevice> vDevice = std::dynamic_pointer_cast<vulkanDevice>(layout.device);
			// semaphore destruction
			if (!imageAvailableSemaphores.size())
			{
				for (auto imageAvailableSemaphore : imageAvailableSemaphores) vkDestroySemaphore(vDevice->getDeviceHandles().device, imageAvailableSemaphore, nullptr);
			}

			if (!renderFinishedSemaphores.size())
			{
				for (auto renderFinishedsmaphore : renderFinishedSemaphores) vkDestroySemaphore(vDevice->getDeviceHandles().device, renderFinishedsmaphore, nullptr);
			}
			// fence reconstruction
			if (!inFlightFences.size())
			{
				for (auto inFlightFence : inFlightFences) vkDestroyFence(vDevice->getDeviceHandles().device, inFlightFence, nullptr);
			}

			if (!imagesInFlight.size())
			{
				for (auto imageInFlight : imagesInFlight) vkDestroyFence(vDevice->getDeviceHandles().device, imageInFlight, nullptr);
			}
		}
		void vulkanPipeline::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandBuffer commandBufffer)
		{
			LN_PROFILE_FUNCTION();
			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = oldLayout;
			barrier.newLayout = newLayout;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = image;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;

			VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			switch (oldLayout)
			{
			case VK_IMAGE_LAYOUT_UNDEFINED:
				barrier.srcAccessMask = VK_ACCESS_NONE;
				sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				break;
			case VK_IMAGE_LAYOUT_PREINITIALIZED:
				barrier.srcAccessMask =
					VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				break;
			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				break;
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;
			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				break;
			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				break;
			}

			switch (newLayout)
			{
			case VK_IMAGE_LAYOUT_UNDEFINED:
				barrier.dstAccessMask = VK_ACCESS_NONE;
				destinationStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				break;
			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				break;
			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				barrier.srcAccessMask |= VK_ACCESS_TRANSFER_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				sourceStage |= VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				break;
			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				break;
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				barrier.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				destinationStage |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
				break;
			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				barrier.srcAccessMask =
					VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				break;
			}

			vkCmdPipelineBarrier(
				commandBufffer,
				sourceStage, destinationStage,
				0,
				0, nullptr,
				0, nullptr,
				1, &barrier);


		}
		void vulkanPipeline::drawIndexed(const ref<renderer::vertexArray>& vertexArray, std::vector<uint64_t> textures, int indexCount)
		{
			LN_PROFILE_FUNCTION();
			drawCommands.push_back({ vertexArray,currentFrame,textures,indexCount });
			if (!textures.size()) return;
			descriptorsetIndex++;
		}

		void vulkanPipeline::fnDrawIndexed(const ref<renderer::vertexArray>& vertexArray, const uint64_t& descriptorIndex, const std::vector<uint64_t>& textures, const int& indexCount)
		{
			LN_PROFILE_FUNCTION();
			VkDeviceSize offsets = 0;
			std::vector<VkBuffer> vulkanVertexBuffers;
			VkBuffer indexBuffer = std::dynamic_pointer_cast<vulkanIndexBuffer>(vertexArray->getIndexBuffer())->vkIndexBuffer;
			//cast base vertexBuffer to platform specific ref.
			for (ref<renderer::vertexBuffer> vertexBuffer : vertexArray->getVertexBuffers()) vulkanVertexBuffers.push_back(std::dynamic_pointer_cast<vulkanVertexBuffer>(vertexBuffer)->vkVertexBuffer);
			//extract platform specific buffer handle from platform specifi buffer ref.
			uint64_t currentIndex = descriptorIndex;
			if (currentFrame) currentIndex *= currentFrame + 1; //select descriptor set bank

			VkDescriptorImageInfo samplerInfo{ sampler->getHandle(),VK_NULL_HANDLE,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };

			std::vector<VkDescriptorImageInfo> imageInfos;
			for (size_t i = 0; i < 32; i++)
			{
				if (i < textures.size()) imageInfos.push_back({ VK_NULL_HANDLE,(VkImageView)textures[i],VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL });
				else  imageInfos.push_back({ VK_NULL_HANDLE,(VkImageView)textures[0],VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL });
			}
			descriptorSets[currentFrame]->write(0, &samplerInfo);
			descriptorSets[currentFrame]->write(1, imageInfos.data());
			vkCmdBindDescriptorSets(commandPool->operator=(commandBuffers[currentFrame]), VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame]->descriptorSet, 0, nullptr);
			vkCmdBindVertexBuffers(commandPool->operator=(commandBuffers[currentFrame]), 0, vulkanVertexBuffers.size(), vulkanVertexBuffers.data(), &offsets);
			vkCmdBindIndexBuffer(commandPool->operator=(commandBuffers[currentFrame]), indexBuffer, 0, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(commandPool->operator=(commandBuffers[currentFrame]), vertexArray->getIndexBuffer()->getCount(), 1, 0, 0, 0);
		}
	}
}