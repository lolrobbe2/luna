#pragma once
#include <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis 
	{
		struct vmaAllocation; //forward declare.
		class allocator;
		class image
		{
		public:
			image() = default;
			
			operator VkImage() const { return _image; }
			VkImage getImage() const { return _image; }
			operator VkImageView() const{ return _imageView; }
			VkImageView getView() const { return _imageView; }
			operator VkFormat() const { return format; }
			VkFormat getFormat() const { return format; }
			operator glm::vec2() const { return extent; }
			glm::vec2 getExtent() const { return extent; }
			operator glm::vec4() const { return { uvStart.x,uvStart.y,uvEnd.x,uvEnd.y }; }
			glm::vec4 getUv() const { return { uvStart.x,uvStart.y,uvEnd.x,uvEnd.y }; }
		protected:
			friend class allocator;
			image(const VkImage image, const vmaAllocation* p_allocation, const glm::vec2& extent ,const VkFormat& format, const glm::vec4 uv = {0.0f,0.0f,1.0f,1.0f});
			image(const VkImage image, const VkImageView imageView, const vmaAllocation* p_allocation, const glm::vec2& extent, const VkFormat& format, const glm::vec4 uv = { 0.0f,0.0f,1.0f,1.0f });
		private:
			VkImage _image = VK_NULL_HANDLE;
			VkImageView _imageView = VK_NULL_HANDLE;
			VkFormat format = VK_FORMAT_MAX_ENUM;
			const vmaAllocation* p_allocation = nullptr;
			glm::vec2 extent;
			glm::vec2 uvStart;
			glm::vec2 uvEnd;
		};
	}
}


