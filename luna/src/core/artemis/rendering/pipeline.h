#pragma once
#include <core/artemis/rendering/shader.h>

namespace luna 
{
	namespace artemis 
	{
		enum pipelineType
		{
			GRAPHICS,
			COMPUTE
		};
	
		class pipeline
		{
		public:
			pipeline(const VkDevice* p_device, VkGraphicsPipelineCreateInfo createInfo);
			pipeline(const VkDevice* p_device, VkComputePipelineCreateInfo createInfo);
		private:
			pipelineType type;
			VkPipeline _pipeline;
			const VkDevice* p_device;
		};
	}
}


