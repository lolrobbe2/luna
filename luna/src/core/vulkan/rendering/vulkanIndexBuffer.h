#pragma once
#include <core/rendering/buffer.h>
#include <core/vulkan/window/window.h>
namespace luna 
{
	namespace vulkan
	{
		/**
		 * @brief vulkan implemention of the index buffer.
		 */
		class vulkanIndexBuffer : public renderer::indexBuffer
		{
		public:
			/**
			 * @brief preFilled constructor.
			 * \param uint32_t* indices
			 */
			vulkanIndexBuffer(uint32_t* indices, uint32_t count);
			/**
			 * @brief creates an empty index buffer of size count.
			 * \param uint32_t count requested index amount. 
			 */
			vulkanIndexBuffer(uint32_t count);
			/**
			 * @brief index buffer destructor.
			 */
			virtual ~vulkanIndexBuffer();
			virtual void bind() const override;
			virtual void unbind() const override;
			/**
			 * @brief returns the index count.
			 */
			virtual uint32_t getCount() const override;
		protected:
			friend class vulkanPipeline;
			VkBuffer vkIndexBuffer; //vulkan index buffer handle
		};

	}
}

