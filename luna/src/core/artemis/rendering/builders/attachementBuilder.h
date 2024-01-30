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
			attachementBuilder& setClearColorValue(const float r, const float g, const float b, const float a);
			attachementBuilder& setClearColorValue(const int32_t r, const int32_t g, const int32_t b, const int32_t a);
			attachementBuilder& setClearColorValue(const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a);
			attachementBuilder& setDepthStencilValue(const float depth, const uint32_t stencil);

			attachement build() {
				attachement att = attachement(_attachement);
				return att;
			}
		private:
			VkAttachmentDescription _attachement{};
			VkImageLayout referenceLayout;
			VkClearValue clearValue;
			ref<swapchain> swapchain;
		};
	}
}


