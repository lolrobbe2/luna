#pragma once
#include <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis
	{
		typedef struct allocation; //forward declare.
		class allocator; 
		class LN_API buffer
		{
		public:
			buffer() = default;
			const size_t getSize() const;
			void setData(void* p_src, size_t size);
			template<typename T>
			void setData(const T type);
			void* getData();
			template<typename T>
			T* getData();
			~buffer();

			operator VkBuffer() const { return _buffer; }
			VkBuffer getBuffer() const { return _buffer; }
		protected:
			friend allocator;
			buffer(VkBuffer buffer,allocation* allocation,allocator* allocator);
		private:
			VkBuffer _buffer = VK_NULL_HANDLE;
			allocation* p_allocation = nullptr;
			allocator* p_allocator = nullptr;
		};
	}
}


