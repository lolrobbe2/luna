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
			
			_ALWAYS_INLINE_ VkPipeline getNative() { return _pipeline; }
			operator VkPipeline() const { return _pipeline; }
			_ALWAYS_INLINE_ VkPipelineBindPoint getBindPoint() const { return bindPoint; }
			operator VkPipelineBindPoint() const { return bindPoint; }
			_ALWAYS_INLINE_ VkPipelineLayout getLayout() { return layout; }
			operator VkPipelineLayout() { return layout; }
		private:
			VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_MAX_ENUM; //store bindPoint for quikAccessTime
			VkPipelineLayout layout;
			pipelineType type;
			VkPipeline _pipeline = VK_NULL_HANDLE;
			const VkDevice* p_device;
		};
	}
}


