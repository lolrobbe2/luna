#include "descriptorPoolBuilder.h"
#include <core/artemis/rendering/shader.h>
namespace luna 
{
	namespace artemis 
	{
		descriptorPoolBuilder::descriptorPoolBuilder(const VkDevice* device, const ref<shader> shader)
		{
			p_device = device;

		}
	}
}

