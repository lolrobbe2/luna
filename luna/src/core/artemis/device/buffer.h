#pragma once
#include <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis
	{
		struct vmaAllocation; //forward declare.
		class allocator; 
		class LN_API buffer
		{
		public:
			size_t getSize();
		protected:
			friend allocator;
			buffer(VkBuffer buffer,vmaAllocation* allocation,allocator* allocator);
		private:
			VkBuffer _buffer = VK_NULL_HANDLE;
			vmaAllocation* p_allocation;
			allocator* p_allocator;
		};
	}
}


