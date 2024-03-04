#include "fence.h"
#include <core/debug/debugMacros.h>

namespace luna 
{
	namespace artemis 
	{
		void fence::wait(bool waitForAll, uint64_t timeOut)
		{
			vkWaitForFences(*p_device, 1, &_fence, waitForAll ? VK_TRUE : VK_FALSE, timeOut);
		}
		void fence::wait(std::vector<fence> fences, bool waitForAll, uint64_t timeOut)
		{
			std::vector<VkFence> nativeFences;
			nativeFences.resize(fences.size(), fences[nativeFences.size()].setNoDestroy(true).native());
			vkWaitForFences(*fences[0].p_device, nativeFences.size(), nativeFences.data(), waitForAll, timeOut);
		}
		void fence::reset()
		{
			vkResetFences(*p_device, 1, &_fence);
		}
		void fence::reset(std::vector<fence> fences)
		{
			std::vector<VkFence> nativeFences;
			nativeFences.resize(fences.size(), fences[nativeFences.size()].setNoDestroy(true).native());
			vkResetFences(*fences[0].p_device, fences.size(),nativeFences.data());
		}
		fence::fence(const VkDevice* device,const VkFenceCreateFlags flags)
		{
			VkFenceCreateInfo info{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
			info.flags = flags;
			info.pNext = nullptr;
			VkResult res = vkCreateFence(*device, &info, nullptr, &_fence);
			LN_ERR_FAIL_COND_MSG(res != VK_SUCCESS, "[Artemis] an error occured while trying to create fence, VkResult: " + VK_RESULT(res));
			p_device = device;
		}
	}
} 

