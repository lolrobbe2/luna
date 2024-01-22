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
			buffer() = default;
			size_t getSize();
			void setData(void* p_src, size_t size);
			template<typename T>
			void setData(const T type);
			void* getData();
			template<typename T>
			T* getData();
			~buffer();
		protected:
			friend allocator;
			buffer(VkBuffer buffer,vmaAllocation* allocation,allocator* allocator);
		private:
			VkBuffer _buffer = VK_NULL_HANDLE;
			vmaAllocation* p_allocation = nullptr;
			allocator* p_allocator = nullptr;
		};
	}
}


