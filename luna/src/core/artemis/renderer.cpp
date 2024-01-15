#include "renderer.h"
#include "rendering/builders/attachementBuilder.h"
#include <core/utils/shaderLibrary.h>
namespace luna 
{
	namespace artemis 
	{
		renderer::renderer(const ref<vulkan::window>& window)
		{
			LN_PROFILE_FUNCTION();
			c_device = device(window);
			p_swapChain = c_device.getSwapchain();
			p_graphicsCommandPool = c_device.getCommandPool(vkb::QueueType::graphics);
			p_transferCommandPool = c_device.getCommandPool(vkb::QueueType::transfer);

			attachementBuilder attachementBuilder{p_swapChain};
			attachementBuilder
				.setOp(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE)
				.setSamples()
				.setLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
			
			attachement attachement = attachementBuilder.build();

			subPassBuilder subPassBuilder;
			subPassBuilder
				.addColorAttachement(attachement)
				.setBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS);
			subpassDescription subPassDescriptionInfo = subPassBuilder.build();
			
			subpassDependency dependancy {0,VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,0};

			renderPassBuilder _renderPassBuilder(c_device);
			
			_renderPassBuilder
				.addSubPass(subPassDescriptionInfo)
				.addSubPassDependency(dependancy);
			renderPass renderPass = _renderPassBuilder.build();

			descriptorPoolBuilder poolBuilder = c_device.getDescriptorPoolBuilder(shaderLibrary::get("fragment.glsl"));;
			poolBuilder.setSamplerAmount(100).setStorageImageAmount(32*100);

			descriptorPool pool = poolBuilder.build();

			descriptorSet& set = pool.allocateDescriptorSet();
			
			std::vector<descriptorSet>& sets = pool.allocateDescriptorSets(98);
		}
	}
}