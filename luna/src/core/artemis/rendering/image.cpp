#include "image.h"
#include <core/artemis/device/buffer.h>
namespace luna 
{
	namespace artemis 
	{
		image::image(const VkImage image, const vmaAllocation* p_allocation, const glm::vec2& extent, const VkFormat& format, const glm::vec4 uv)
		{
			_image = image;
			this->format = format;
			this->p_allocation = p_allocation;
			this->extent = extent;
			this->uvStart = { uv.x,uv.y };
			this->uvEnd = { uv.z,uv.w };
		}
		image::image(const VkImage image, const VkImageView imageView, const vmaAllocation* p_allocation, const glm::vec2& extent, const VkFormat& format, const glm::vec4 uv) : image(image,p_allocation,extent,format,uv)
		{
			_imageView = imageView;
		}
	}
}

