#include "subPassBuilder.h"
namespace luna 
{
	namespace artemis 
	{

		subPassBuilder& subPassBuilder::addInputAttachement(attachement attachement,const VkImageLayout referenceLayout)
		{
			attachement.reference->layout = referenceLayout;
			description.inputAttachments.push_back(attachement);
			return *this;
		}

		subPassBuilder& subPassBuilder::addColorAttachement(attachement attachement,const VkImageLayout referenceLayout)
		{
			attachement.reference->layout= referenceLayout;
			description.colorAttachments.push_back(attachement);
			return *this;
		}

		subPassBuilder& subPassBuilder::setDepthStencilAttachement(const attachement attachement)
		{
			description.depthStencilAttachment = attachement;
			return *this;
		}

		subpassDescription subPassBuilder::build()
		{
			return description;
		}
	}
}