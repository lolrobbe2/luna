#include "commandBuffer.h"
#include <core/debug/debugMacros.h>
#include <core/artemis/rendering/pipeline.h>
#include <core/artemis/rendering/renderPass.h>
#include <core/artemis/rendering/swapchain.h>
#include <core/artemis/device/descriptorSet.h>
#include <core/artemis/rendering/frameBuffer.h>
#include <core/artemis/device/buffer.h>
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

		void commandBuffer::bindPipeline(const ref<pipeline> pipeline)
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

		void commandBuffer::bindIndexBuffer(const buffer& buffer,VkDeviceSize offset,VkIndexType indexType)
		{
			vkCmdBindIndexBuffer(m_commandBuffer, buffer, offset, indexType);
		}

		void commandBuffer::bindVertexBuffers(uint32_t firstBinding, std::vector<VkBuffer> buffers, const VkDeviceSize* p_offsets)
		{
			vkCmdBindVertexBuffers(m_commandBuffer, firstBinding, buffers.size(), buffers.data(), p_offsets);
		}

		
		
	}
}
