#pragma once
#include <core/rendering/buffer.h>
#include <core/vulkan/window/window.h>
namespace luna 
{
	namespace vulkan
	{
		class vulkanIndexBuffer : public renderer::indexBuffer
		{
		public:
			vulkanIndexBuffer(uint32_t* indices, uint32_t count);
			vulkanIndexBuffer(uint32_t count);
			virtual ~vulkanIndexBuffer();
			virtual void bind() const override;
			virtual void unbind() const override;

			virtual uint32_t getCount() const override;
		protected:
			friend class vulkanPipeline;
			VkBuffer vkIndexBuffer;
		};

	}
}

