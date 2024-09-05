#include "commandBuffer.h"
#include <core/debug/debugMacros.h>
#include <core/artemis/rendering/pipeline.h>
#include <core/artemis/rendering/renderPass.h>
#include <core/artemis/rendering/swapchain.h>
#include <core/artemis/device/descriptorSet.h>
#include <core/artemis/rendering/frameBuffer.h>
#include <core/artemis/device/buffer.h>
#include <core/artemis/rendering/image.h>
namespace luna
{
	namespace artemis
	{
		commandBuffer::commandBuffer(const VkCommandPool* commandPool, const VkCommandBufferLevel& commandBufferLevel,const VkDevice* device)
		{
			VkCommandBufferAllocateInfo info{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO ,nullptr};
			info.commandBufferCount = 1;
			info.commandPool = *commandPool;
			info.level = commandBufferLevel;
			VkResult res = vkAllocateCommandBuffers(*device, &info, &m_commandBuffer);
			LN_ERR_FAIL_COND_MSG(res != VK_SUCCESS, "[Artemis] failed to allocate commandBuffer, VkResult: " + std::to_string(res));
			_commandPool = commandPool;
			_device = device;
		}
		bool commandBuffer::begin(const VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)
		{
			LN_ERR_FAIL_COND_V_MSG(recording,false, "you cannot call begin() on a commandBuffer that is allready recording");
			VkCommandBufferBeginInfo commandBufferBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
			commandBufferBeginInfo.flags = flags;
			commandBufferBeginInfo.pInheritanceInfo = nullptr;
			commandBufferBeginInfo.pNext = nullptr;
		
			vkResetCommandBuffer(m_commandBuffer, 0); //reset to make sure no chenanigans happens

			VkResult result = vkBeginCommandBuffer(m_commandBuffer, &commandBufferBeginInfo);
			LN_ERR_FAIL_COND_V_MSG(result != VK_SUCCESS, false, "could not start recording on commandBuffer, VkRsult: " + std::to_string(result));
			recording = true;
			return true;
		}
		void commandBuffer::end()
		{
			LN_ERR_FAIL_COND_MSG(!recording, "you cannot end recording a command buffer that has not begun recording! Consider using commandBuffer::begin function");
			VkResult result = vkEndCommandBuffer(m_commandBuffer);
			LN_ERR_FAIL_COND_MSG(result != VK_SUCCESS, "an error occured when stoping to record on the commandBuffer");
			recording = false;
		}
		void commandBuffer::lock()
		{
			commandBufferLock.lock();
		}
		void commandBuffer::unlock()
		{
			commandBufferLock.unlock();
		}
		void commandBuffer::dispatch(uint32_t groupCountX,uint32_t groupCountY,uint32_t groupCountZ)
		{
			vkCmdDispatch(m_commandBuffer, groupCountX, groupCountY, groupCountZ);
		}

		void commandBuffer::bindPipeline(ref<pipeline> pipeline)
		{
			vkCmdBindPipeline(m_commandBuffer, *pipeline, *pipeline);
		}

		void commandBuffer::beginRenderPass(const ref<renderPass>& p_renderPass,const frameBuffer& frameBuffer)
		{
			VkRenderPassBeginInfo info = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
			info.renderPass = *p_renderPass;
			info.renderArea = frameBuffer;
			info.framebuffer = frameBuffer;
			info.clearValueCount = p_renderPass->getClearValueCount();
			info.pClearValues = p_renderPass->getClearValues();
			vkCmdBeginRenderPass(m_commandBuffer,&info,VK_SUBPASS_CONTENTS_INLINE);
		}

		void commandBuffer::bindDescriptorSets(const ref<pipeline> pipeline,const std::vector<descriptorSet>& descriptorSets)
		{
			std::vector<VkDescriptorSet> nativeDescriptorSets;
			std::transform(descriptorSets.begin(), descriptorSets.end(), nativeDescriptorSets.begin(), [](const descriptorSet& set) -> VkDescriptorSet {return set; });
			vkCmdBindDescriptorSets(m_commandBuffer, *pipeline, *pipeline, 0, descriptorSets.size(),nativeDescriptorSets.data(), 0, nullptr);
		}

		void commandBuffer::bindDescriptorSet(const ref<pipeline> pipeline, const descriptorSet& descriptorSet)
		{
			vkCmdBindDescriptorSets(m_commandBuffer, *pipeline, *pipeline, 0, 1, descriptorSet, 0, nullptr);
		}

		void commandBuffer::endCurrentRenderPass()
		{
			vkCmdEndRenderPass(m_commandBuffer);
		}

		void commandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
		{
			vkCmdDraw(m_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
		}

		void commandBuffer::drawIndexed(uint32_t indexCount,uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
		{

			vkCmdDrawIndexed(m_commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
		}

		void commandBuffer::drawIndexedIndirect(buffer& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
		{
			vkCmdDrawIndexedIndirect(m_commandBuffer, buffer, offset, drawCount, stride);
		}

		void commandBuffer::drawIndexedIndirectCommand(const size_t index,buffer& buffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
		{
			VkDrawIndexedIndirectCommand drawCommand = { indexCount,instanceCount,firstIndex,vertexOffset,firstInstance };
			buffer.setDataIndex(index, drawCommand);
		}

		void commandBuffer::bindIndexBuffer(const buffer& buffer,VkDeviceSize offset,VkIndexType indexType)
		{
			vkCmdBindIndexBuffer(m_commandBuffer, buffer, offset, indexType);
		}

		void commandBuffer::bindVertexBuffers(uint32_t firstBinding, std::vector<VkBuffer> buffers, const VkDeviceSize* p_offsets)
		{
			vkCmdBindVertexBuffers(m_commandBuffer, firstBinding, buffers.size(), buffers.data(), p_offsets);
		}

		void commandBuffer::transitionImageLayout(image& image, const VkImageLayout oldLayout, const VkImageLayout newLayout)
		{
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

			VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_NONE;
			VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_NONE;
			//switch statement?
			if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_NONE;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_UNDEFINED)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_NONE;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_GENERAL)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_NONE;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
			} 
			else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_NONE;
				barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
				destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			{
				barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				destinationStage = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
			{
				barrier.srcAccessMask = VK_ACCESS_NONE;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}
			else
				LN_ERR_FAIL_MSG("[ARTEMIS] no appropriate transitionn found");
			vkCmdPipelineBarrier(m_commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
			image.setCurrentLayout(newLayout);
		}

		void commandBuffer::setViewport(VkViewport& viewport)
		{
			vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);
		}
		
		void commandBuffer::setScissor(VkRect2D scissor)
		{
			vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);
		}
	}
}
