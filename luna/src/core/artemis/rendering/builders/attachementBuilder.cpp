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
	}
}