#include "buffer.h"
#include <core/artemis/device/allocator.h>
namespace luna 
{
	namespace artemis 
	{
		buffer::buffer(VkBuffer buffer, vmaAllocation* allocation, allocator* allocator)
		{
			if (!allocator)return;
			_buffer = buffer;
			p_allocation = allocation;
			p_allocator = allocator;
		}
		size_t buffer::getSize()
		{
			return p_allocator->getSize(p_allocation);
		}
	}
}