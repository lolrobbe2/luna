#pragma once
#include <core/artemis/rendering/attachement.h>
namespace luna 
{
	namespace artemis 
	{
		class attachementBuilder
		{
		public:
			attachementBuilder(const ref<swapchain> swapchain) : swapchain(swapchain)
			{
				_attachement.format = *swapchain;
			}
			attachementBuilder& setFlags(VkAttachmentDescriptionFlags flags);
			attachementBuilder& setSamples(VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT);
			attachementBuilder& setOp(VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp);
			attachementBuilder& setStencilOp(VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp);
			attachementBuilder& setLayout(VkImageLayout initialLayout, VkImageLayout finalLayout);
			attachement build() {
				attachement att = attachement(_attachement);
				return att;
			}
		private:
			VkAttachmentDescription _attachement{};
			VkImageLayout referenceLayout;
			ref<swapchain> swapchain;
		};
	}
}


