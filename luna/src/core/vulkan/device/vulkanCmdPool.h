#pragma once
#include <core/vulkan/window/window.h>
namespace luna
{
	namespace vulkan
	{
		
		typedef uint64_t virtualCmdBuffer;
		struct vulkanCmdPoolSpec
		{
			VkDevice device;
			VkCommandPoolCreateFlags    flags;
			uint32_t queueFamilyIndex;
		};
		struct commandPoolSubmitInfo
		{
			const void* pNext;
			uint32_t                       waitSemaphoreCount;
			const VkSemaphore* pWaitSemaphores;
			const VkPipelineStageFlags* pWaitDstStageMask;
			uint32_t                       commandBufferCount;
			const virtualCmdBuffer* pCommandBuffers;
			uint32_t                       signalSemaphoreCount;
			const VkSemaphore* pSignalSemaphores;
		};
		class vulkanCmdPool
		{
		public:
			VkCommandBuffer operator=(const virtualCmdBuffer commandBuffer) { return virtualBuffers.find(commandBuffer)->second.first; };
			
			/**
			* @brief vulkanCmdPool constructor
			* @param vulkanCmdPoolSpec commandPoolSpecefication;
			*/
			vulkanCmdPool(const vulkanCmdPoolSpec& commandBufferSPec);
			/**
			* @brief commandPool destructor, does nothing at the moment
			*/
			virtual ~vulkanCmdPool(){};
			/**
			* @brief createNewBuffer creates a proteceted commandBuffer that is bound to the pool it is created with.
			* @param virtualCmdBuffer* a command buffer array pointer or std::vector pointer.
			* @param const uint32_t& commandBufferCount number of commandBuffers to be created.
			* @param const VkCommandBufferLevel& commandBufferLevel specifies the commandBuffer type.
			*/
			VkResult createNewBuffer(virtualCmdBuffer* commandBuffer, const uint32_t& commandBufferCount, const VkCommandBufferLevel& commandBufferLevel);
			/**
			* @brief starts recording a commandBuffer.
			* @param virtualCommandBuffer commandBuffer handle.
			* @param const VkCommandBufferUsageFlags& usageFlags
			*/
			VkResult begin(virtualCmdBuffer commandBuffer,const VkCommandBufferUsageFlags& usageFlags);
			/**
			* @brief stops recording the commandBuffer.
			* @param virtualCommandBuffer commandBuffer handle.
			*/
			VkResult end(virtualCmdBuffer commandBuffer);
			/**
			* @brief flush executes all flushed/submitted commandBuffers.
			* @param VkQueue queue
			* @param submiCount the amount of commandPoolSubmitInfos 
			* @param const commandPoolSubmitInfo* pCommandPoolSubmitInfo
			*/
			VkResult flush(VkQueue queue, uint64_t submitCount, const commandPoolSubmitInfo* pCommandPoolSubmitInfo, VkFence waitFence);
		private:
			std::unordered_map<virtualCmdBuffer, std::pair<VkCommandBuffer, VkCommandBufferUsageFlags>> virtualBuffers;
			VkCommandPool commandPool;
			vulkanCmdPoolSpec sCommandPoolSpec;
			std::vector<VkCommandBuffer>commandBuffers;
		};
		

	}
}


