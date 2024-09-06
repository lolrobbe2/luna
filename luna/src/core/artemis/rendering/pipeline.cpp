#include "pipeline.h"
#include <core/debug/debugMacros.h>
namespace luna 
{
	namespace artemis
	{
		pipeline::pipeline()
		{
			type = NONE;
		}
		pipeline::pipeline(const VkDevice* p_device, VkGraphicsPipelineCreateInfo createInfo) : p_device(p_device)
		{
			type = GRAPHICS;
			LN_ERR_FAIL_COND_MSG(*p_device == VK_NULL_HANDLE, "[Artmis] device handle was VK_NULL_HANDLE (invalid)");
			VkResult createRes = vkCreateGraphicsPipelines(*p_device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &_pipeline);
			LN_ERR_FAIL_COND_MSG(createRes != VK_SUCCESS, "[Artemis] an error occured during graphics pipeline creation, VkResult: " + VK_RESULT(createRes));
			layout = createInfo.layout;
			bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		}
		pipeline::pipeline(const VkDevice* p_device, VkComputePipelineCreateInfo createInfo) : p_device(p_device)
		{
			type = COMPUTE;
			LN_ERR_FAIL_COND_MSG(*p_device == VK_NULL_HANDLE, "[Artmis] device handle was VK_NULL_HANDLE (invalid)");
			VkResult createRes = vkCreateComputePipelines(*p_device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &_pipeline);
			LN_ERR_FAIL_COND_MSG(createRes != VK_SUCCESS, "[Artemis] an error occured during compute pipeline creation, VkResult: " + VK_RESULT(createRes));
			layout = createInfo.layout;
			bindPoint = VK_PIPELINE_BIND_POINT_COMPUTE;

		}

	

	}
}

