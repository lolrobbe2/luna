#pragma once
#ifndef _FENCE_
#define _FENCE_
#include <core/core.h>
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
			~fence() { if (_fence != VK_NULL_HANDLE && !noDestroy) wait(); vkDestroyFence(*p_device, _fence, nullptr); }
			void wait(bool waitForAll = true, uint64_t timeOut = UINT64_MAX);
			static void wait(std::vector<fence> fences, bool waitForAll = true, uint64_t timeOut = UINT64_MAX);
			void reset();
			static void reset(std::vector<fence> fences);
			/**
			* @brief when the deconstructor is called en noDestroy is true then the native fence (VkFence) will not be destroyed;
			*/
			fence& setNoDestroy(bool noDestroy = false) { noDestroy = true; return *this; }
		protected: 
			friend class device;
			fence(const VkDevice* device, const VkFenceCreateFlags flags);
		private:
			const VkDevice* p_device = nullptr;
			bool noDestroy = false;
			VkFence _fence = VK_NULL_HANDLE;
		};
	}
}


#endif