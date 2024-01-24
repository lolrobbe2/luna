#pragma once
#include  <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis 
	{
		class commandBuffer
		{
		public:
			_ALWAYS_INLINE_ bool begin(const VkCommandBufferUsageFlags flags);
			_ALWAYS_INLINE_ void end();
			bool isRecording() { return recording; }
			_ALWAYS_INLINE_ void reset(const VkCommandBufferResetFlags flags = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT) { vkResetCommandBuffer(m_commandBuffer, flags); }
			_ALWAYS_INLINE_ VkCommandBuffer native() { return m_commandBuffer; }
			operator VkCommandBuffer() const { return m_commandBuffer; }
			operator VkCommandBuffer*() { return &m_commandBuffer; }
			~commandBuffer() { vkFreeCommandBuffers(*_device, *_commandPool, 1, &m_commandBuffer); }
		private:
			bool recording = false;
			const VkDevice* _device;
			const VkCommandPool* _commandPool;
			VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
		protected:
			friend class commandPool;
			commandBuffer(const VkCommandPool* commandPool, const VkCommandBufferLevel& commandBufferLevel, const VkDevice* device);

		};
	}
}


