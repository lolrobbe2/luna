#include "attachementBuilder.h"
namespace luna 
{
	namespace artemis 
	{
		attachementBuilder& attachementBuilder::setFlags(VkAttachmentDescriptionFlags flags)
		{
			_attachement.flags = flags;
			return *this;
		}
		attachementBuilder& attachementBuilder::setSamples(VkSampleCountFlagBits sampleCount)
		{
			_attachement.samples = sampleCount;
			return *this;
		}
		attachementBuilder& attachementBuilder::setOp(VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp)
		{
			_attachement.loadOp = loadOp;
			_attachement.storeOp = storeOp;
			return *this;
		}
		attachementBuilder& attachementBuilder::setStencilOp(VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp)
		{
			_attachement.stencilLoadOp = stencilLoadOp;
			_attachement.stencilStoreOp = stencilStoreOp;
			return *this;
		}
		attachementBuilder& attachementBuilder::setLayout(VkImageLayout initialLayout, VkImageLayout finalLayout)
		{
			_attachement.initialLayout = initialLayout;
			_attachement.finalLayout = finalLayout;
			return *this;
		}
		attachementBuilder& attachementBuilder::setClearColorValue(const float r, const float g, const float b, const float a)
		{
			// TODO: insert return statement here
		}
		attachementBuilder& attachementBuilder::setClearColorValue(const int32_t r, const int32_t g, const int32_t b, const int32_t a)
		{
			// TODO: insert return statement here
		}
		attachementBuilder& attachementBuilder::setClearColorValue(const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a)
		{
			// TODO: insert return statement here
		}
		attachementBuilder& attachementBuilder::setDepthStencilValue(const float depth, const uint32_t stencil)
		{
			// TODO: insert return statement here
		}
	}
}