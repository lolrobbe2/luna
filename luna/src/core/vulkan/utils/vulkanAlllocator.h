#pragma once
#include <vk_mem_alloc.h>
#include <core/utils/vectorCache.h>
namespace luna
{
	namespace utils
	{
		class vulkanAllocator
		{
		public:
			vulkanAllocator();
		private:
			struct allocation
			{

			};
			vectorCache<allocation> allocationCache;
		};
		
	}
}

