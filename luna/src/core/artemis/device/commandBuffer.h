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
		class frameBuffer;
		class buffer;
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
			_ALWAYS_INLINE_ void beginRenderPass(const ref<renderPass>& p_renderPass,const frameBuffer& frameBuffer);
			_ALWAYS_INLINE_ void bindDescriptorSets(const ref<pipeline> pipeline,const std::vector<descriptorSet>& descriptorSets);
			_ALWAYS_INLINE_ void bindDescriptorSet(const ref<pipeline> pipeline, const descriptorSet& descriptorSets);

			_ALWAYS_INLINE_ void endCurrentRenderPass();

			_ALWAYS_INLINE_ void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
			_ALWAYS_INLINE_ void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
			_ALWAYS_INLINE_ void bindIndexBuffer(const buffer& buffer, VkDeviceSize offset, VkIndexType indexType);
			_ALWAYS_INLINE_ void bindVertexBuffers(uint32_t firstBinding, std::vector<VkBuffer> buffers, const VkDeviceSize* p_offsets);
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


