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
			~fence() { if(_fence != VK_NULL_HANDLE && !noDestroy)vkDestroyFence(*p_device, _fence, nullptr); }
			_ALWAYS_INLINE_ void wait(bool waitForAll = true, uint64_t timeOut = UINT64_MAX);
			_ALWAYS_INLINE_ static void wait(std::vector<fence> fences, bool waitForAll = true, uint64_t timeOut = UINT64_MAX);
			_ALWAYS_INLINE_ void reset();
			_ALWAYS_INLINE_ static void reset(std::vector<fence> fences);
			/**
			* @brief when the deconstructor is called en noDestroy is true then the native fence (VkFence) will not be destroyed;
			*/
			_ALWAYS_INLINE_ fence& setNoDestroy(bool noDestroy = false) { noDestroy = true; return *this; }
		protected: 
			friend class device;
			fence(const VkDevice* device, const VkFenceCreateFlags flags);
		private:
			const VkDevice* p_device;
			bool noDestroy = false;
			VkFence _fence = VK_NULL_HANDLE;
		};
	}
}


