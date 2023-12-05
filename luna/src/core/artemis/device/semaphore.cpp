#include "semaphore.h"
#include <core/debug/debugMacros.h>
namespace luna
{
	namespace artemis 
	{
		semaphore::semaphore(const VkDevice* device,const VkSemaphoreCreateFlags flags)
		{
			VkSemaphoreCreateInfo info{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
			info.flags = flags;
			info.pNext = nullptr;
			LN_ERR_FAIL_COND_MSG(*device == VK_NULL_HANDLE, "[Artemis] failed to create semaphore because device has not been initialized or has been invalidated!");
			VkResult res = vkCreateSemaphore(*device, &info, nullptr, &_semaphore);
			LN_ERR_FAIL_COND_MSG(res != VK_SUCCESS, "[Artemis] failed to create semaphore, VkResult: " + std::to_string(res));
			this->device = device;
		}
	}
}