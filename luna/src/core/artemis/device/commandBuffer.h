#pragma once
#include  <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis 
	{
		class pipeline;
		class swapchain;
		class descriptorSet;
		class renderPass;
		class LN_API commandBuffer
		{
		public:
			_ALWAYS_INLINE_ bool begin(const VkCommandBufferUsageFlags flags);
			_ALWAYS_INLINE_ void end();
			_ALWAYS_INLINE_ void lock();
			_ALWAYS_INLINE_ void unlock();
			bool isRecording() { return recording; }
			_ALWAYS_INLINE_ void reset(const VkCommandBufferResetFlags flags = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT) { vkResetCommandBuffer(m_commandBuffer, flags); }
			_ALWAYS_INLINE_ VkCommandBuffer native() { return m_commandBuffer; }
			_ALWAYS_INLINE_ void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

			_ALWAYS_INLINE_ void bindPipeline(const ref<pipeline> pipeline);
			_ALWAYS_INLINE_ void beginRenderPass(const ref<renderPass>& p_renderPass,const ref<swapchain> p_swapchain);
			_ALWAYS_INLINE_ void bindDescriptorSets(const ref<pipeline> pipeline,const std::vector<descriptorSet>& descriptorSets);
			_ALWAYS_INLINE_ void bindDescriptorSet(const ref<pipeline> pipeline, const descriptorSet& descriptorSets);

			operator VkCommandBuffer() const { return m_commandBuffer; }
			operator VkCommandBuffer*() { return &m_commandBuffer; }
			~commandBuffer() { vkFreeCommandBuffers(*_device, *_commandPool, 1, &m_commandBuffer); }
		private:
			std::mutex commandBufferLock;
			bool recording = false;
			const VkDevice* _device = nullptr;
			const VkCommandPool* _commandPool = nullptr;
			VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
		protected:
			friend class commandPool;
			commandBuffer(const VkCommandPool* commandPool, const VkCommandBufferLevel& commandBufferLevel, const VkDevice* device);

		};
	}
}


