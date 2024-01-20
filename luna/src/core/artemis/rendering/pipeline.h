#pragma once
#include <core/artemis/rendering/shader.h>

namespace luna 
{
	namespace artemis 
	{
		enum pipelineType
		{
			NONE,
			GRAPHICS,
			COMPUTE
		};
	
		class pipeline
		{
		public:
			pipeline();
			pipeline(const VkDevice* p_device, VkGraphicsPipelineCreateInfo createInfo);
			pipeline(const VkDevice* p_device, VkComputePipelineCreateInfo createInfo);
		private:
			pipelineType type;
			VkPipeline _pipeline = VK_NULL_HANDLE;
			const VkDevice* p_device;
		};
	}
}


