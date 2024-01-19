#include "pipeline.h"
#include <core/debug/debugMacros.h>
namespace luna 
{
	namespace artemis 
	{
	
		pipeline::pipeline(const VkDevice* p_device, VkGraphicsPipelineCreateInfo createInfo) : p_device(p_device)
		{
			type = GRAPHICS;
			LN_ERR_FAIL_COND_MSG(*p_device == VK_NULL_HANDLE, "[Artmis] device handle was VK_NULL_HANDLE (invalid)");
			VkResult createRes = vkCreateGraphicsPipelines(*p_device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &_pipeline);
			LN_ERR_FAIL_COND_MSG(createRes != VK_SUCCESS, "[Artemis] an error occured during graphics pipeline creation, VkResult: " + VK_RESULT(createRes));
		}
		pipeline::pipeline(const VkDevice* p_device, VkComputePipelineCreateInfo createInfo) : p_device(p_device)
		{
		
			type = COMPUTE;
		}
}
}

