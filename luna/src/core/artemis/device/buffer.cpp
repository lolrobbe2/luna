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
		void buffer::setData(void* p_src, size_t size)
		{
			memcpy_s(getData(), getSize(), p_src, size);
		}
		void* buffer::getData()
		{
			return p_allocator->getData(p_allocation);
		}
		template<typename T>
		void buffer::setData(const T type)
		{
			setData(&type, sizeof(type));
		}
		template<typename T>
		T* buffer::getData()
		{
			return (T*)getData();
		}
		buffer::~buffer()
		{
			p_allocator->deallocate(_buffer, p_allocation);
		}
	}
}