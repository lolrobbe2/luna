#include "fence.h"
#include <core/debug/debugMacros.h>

namespace luna 
{
	namespace artemis 
	{
		fence::fence(const VkDevice* device,const VkFenceCreateFlags flags)
		{
			VkFenceCreateInfo info{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
			info.flags = flags;
			info.pNext = nullptr;
			VkResult res = vkCreateFence(*device, &info, nullptr, &_fence);
			LN_ERR_FAIL_COND_MSG(res != VK_SUCCESS, "[Artemis] an error occured while trying to create fence, VkResult: " + std::to_string(res));
			_device = device;
		}
	}
} 

