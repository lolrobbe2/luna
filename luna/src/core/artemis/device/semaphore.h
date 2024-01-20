#pragma once
#include <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis 
	{
		class semaphore
		{
		public:
			~semaphore() { vkDestroySemaphore(*device, _semaphore, nullptr); }
			operator const VkSemaphore() const { return _semaphore; }
			operator const VkSemaphore*() const { return &_semaphore; }
			const VkSemaphore native() { return _semaphore; }
		protected:
			friend class device;
			semaphore(const VkDevice* device, const VkSemaphoreCreateFlags flags);
			const VkDevice* device; //weak_ref to device handle does not need to call delete.
			VkSemaphore _semaphore = VK_NULL_HANDLE;
		};
	}
}


