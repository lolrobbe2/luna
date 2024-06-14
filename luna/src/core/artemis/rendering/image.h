#ifndef _IMAGE_
#define _IMAGE_

#include <core/platform/windows/windowsWindow.h>
#include <core/core.h>
namespace luna 
{
	namespace artemis 
	{
		typedef struct allocation; //forward declare.
		class allocator;
		class image
		{
		public:
			image() = default;
			_ALWAYS_INLINE_ operator bool() const { return _image == VK_NULL_HANDLE; }
			_ALWAYS_INLINE_ operator VkImage() const { return _image; }
			_ALWAYS_INLINE_ VkImage getImage() const { return _image; }
			_ALWAYS_INLINE_ operator VkImageView() const{ return _imageView; }
			_ALWAYS_INLINE_ VkImageView getView() const { return _imageView; }
			_ALWAYS_INLINE_ operator VkImageView*() const { return (VkImageView*)&_imageView; }
			_ALWAYS_INLINE_ VkImageView* getViewPtr() const { return (VkImageView*)&_imageView; }
			_ALWAYS_INLINE_ operator VkFormat() const { return format; }
			_ALWAYS_INLINE_ VkFormat getFormat() const { return format; }
			_ALWAYS_INLINE_ operator glm::vec2() const { return extent; }
			_ALWAYS_INLINE_ glm::vec2 getExtent() const { return extent; }
			_ALWAYS_INLINE_ operator VkExtent3D() const
			{ 
				VkExtent3D _extent;
				_extent.width = extent.x;
				_extent.height = extent.y;
				_extent.depth = 1;
				return _extent;
			}
			_ALWAYS_INLINE_ VkExtent3D getExtent3D() const
			{
				VkExtent3D _extent;
				_extent.width = extent.x;
				_extent.height = extent.y;
				_extent.depth = 1;
				return _extent;
			}
			_ALWAYS_INLINE_ operator VkOffset3D() const
			{
				VkOffset3D _offset;
				_offset.x = uvStart.x * extent.x;
				_offset.y = uvStart.y * extent.y;
				_offset.z = 0;
				return _offset;
			}
			_ALWAYS_INLINE_ VkOffset3D getOffset3D() const
			{
				VkOffset3D _offset;
				_offset.x = uvStart.x * extent.x;
				_offset.y = uvStart.y * extent.y;
				return _offset;
			}
			_ALWAYS_INLINE_ operator glm::vec4() const { return { uvStart.x,uvStart.y,uvEnd.x,uvEnd.y }; }
			_ALWAYS_INLINE_ glm::vec4 getUv() const { return { uvStart.x,uvStart.y,uvEnd.x,uvEnd.y }; }
			_ALWAYS_INLINE_ std::array<glm::vec2, 4> getUvCoords() const { return { uvStart, glm::vec2(uvEnd.x,uvStart.y),uvEnd,glm::vec2(uvStart.x,uvEnd.y) }; }
			_ALWAYS_INLINE_ operator std::array<glm::vec2, 4>() const { return { uvStart, glm::vec2(uvEnd.x,uvStart.y),uvEnd,glm::vec2(uvStart.x,uvEnd.y)}; }
			_ALWAYS_INLINE_ operator VkImageLayout() const { return currentLayout; }
			_ALWAYS_INLINE_ VkImageLayout getCurrentLayout() const { return currentLayout; }
			LN_API _ALWAYS_INLINE_ image& getSubImage(const glm::vec2& extent, const glm::vec4 uv = { 0.0f,0.0f,1.0f,1.0f });
			//TODO unload image;
		protected:
			friend class allocator;
			image(const VkImage image, const allocation* p_allocation, const glm::vec2& extent ,const VkFormat& format, const glm::vec4 uv = {0.0f,0.0f,1.0f,1.0f});
			image(const VkImage image, const VkImageView imageView, const allocation* p_allocation, const glm::vec2& extent, const VkFormat& format, const glm::vec4 uv = { 0.0f,0.0f,1.0f,1.0f });
		private:
			VkImage _image = VK_NULL_HANDLE;
			VkImageView _imageView = VK_NULL_HANDLE;
			VkImageLayout currentLayout = VK_IMAGE_LAYOUT_GENERAL;
			VkFormat format = VK_FORMAT_MAX_ENUM;
			const allocation* p_allocation = nullptr;
			glm::vec2 extent = {};
			glm::vec2 uvStart = {};
			glm::vec2 uvEnd = {};
		};
	}
}

#endif
