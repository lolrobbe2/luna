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
			float* float32 = clearValue.color.float32;
			float32[0] = r;
			float32[1] = g;
			float32[2] = b;
			float32[3] = a;
			return *this;
		}
		attachementBuilder& attachementBuilder::setClearColorValue(const int32_t r, const int32_t g, const int32_t b, const int32_t a)
		{
			int32_t* int32 = clearValue.color.int32;
			int32[0] = r;
			int32[1] = g;
			int32[2] = b;
			int32[3] = a;
			return *this;
		}
		attachementBuilder& attachementBuilder::setClearColorValue(const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a)
		{
			uint32_t* uint32 = clearValue.color.uint32;
			uint32[0] = r;
			uint32[1] = g;
			uint32[2] = b;
			uint32[3] = a;
			return *this;
		}
		attachementBuilder& attachementBuilder::setDepthStencilValue(const float depth, const uint32_t stencil)
		{
			clearValue.depthStencil.depth = depth;
			clearValue.depthStencil.stencil = stencil;
			return *this;
		}
	}
}