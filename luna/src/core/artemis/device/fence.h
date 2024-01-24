#pragma once
#include <core/artemis/device/semaphore.h>

namespace luna 
{
	namespace artemis 
	{
		class fence
		{
		public:
			fence() = default;
			operator const VkFence() const { return _fence; }
			operator const VkFence*() const { return &_fence; }
			const VkFence native() { return _fence; }
			~fence() { vkDestroyFence(*_device, _fence, nullptr); }
		protected: 
			friend class device;
			fence(const VkDevice* device, const VkFenceCreateFlags flags);
		private:
			const VkDevice* _device;
			VkFence _fence = VK_NULL_HANDLE;
		};
	}
}


