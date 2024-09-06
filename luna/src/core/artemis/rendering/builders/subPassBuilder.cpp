#include "subPassBuilder.h"
namespace luna 
{
	namespace artemis 
	{

		subPassBuilder& subPassBuilder::addInputAttachement(attachement& attachement,const VkImageLayout referenceLayout)
		{
			
			description.inputAttachments.push_back(attachement);
			description.inputAttachments.back().reference->layout = referenceLayout;
			return *this;
		}

		subPassBuilder& subPassBuilder::addColorAttachement(attachement& attachement,const VkImageLayout referenceLayout)
		{
			description.colorAttachments.push_back(attachement);
			description.colorAttachments.back().reference->layout = referenceLayout;
			return *this;
		}

		subPassBuilder& subPassBuilder::setDepthStencilAttachement(const attachement& attachement)
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