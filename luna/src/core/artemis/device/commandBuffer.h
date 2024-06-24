#ifndef _COMMAND_BUFFER_
#define _COMMAND_BUFFER_
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
		class commandBuffer
		{
		public:
			commandBuffer() {}
			bool begin(const VkCommandBufferUsageFlags flags);
			void end();
			void lock();
			void unlock();
			bool isRecording() const { return recording; }
			void reset(const VkCommandBufferResetFlags flags = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT) { vkResetCommandBuffer(m_commandBuffer, flags); }
			VkCommandBuffer native() const { return m_commandBuffer; }
			void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

			void bindPipeline(const ref<pipeline> pipeline);
			void beginRenderPass(const ref<renderPass>& p_renderPass,const frameBuffer& frameBuffer);
			void bindDescriptorSets(const ref<pipeline> pipeline,const std::vector<descriptorSet>& descriptorSets);
			void bindDescriptorSet(const ref<pipeline> pipeline, const descriptorSet& descriptorSets);

			void endCurrentRenderPass();

			void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
			void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
			void bindIndexBuffer(const buffer& buffer, VkDeviceSize offset, VkIndexType indexType);
			void bindVertexBuffers(uint32_t firstBinding, std::vector<VkBuffer> buffers, const VkDeviceSize* p_offsets);
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


#endif // !_COMMAND_BUFFER_
