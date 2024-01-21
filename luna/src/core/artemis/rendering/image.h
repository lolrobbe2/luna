#pragma once
#include <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis 
	{
		
		struct vmaAllocation; //forward declare.
		class allocator; //forward declare.
		class image
		{
		public:
		protected:
			image(const VkImage image, const vmaAllocation* p_allocation, const glm::vec2& extent ,const VkFormat& format, const glm::vec4 uv = {0.0f,0.0f,1.0f,1.0f});
		private:
			const VkImage _image;
			const vmaAllocation* p_allocation;
			const glm::vec2 extent;
			glm::vec2 uvStart;
			glm::vec2 uvEnd;
		};
	}
}


