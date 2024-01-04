#include "subPassBuilder.h"
namespace luna 
{
	namespace artemis 
	{

		subPassBuilder subPassBuilder::addInputAttachement(const attachement attachement)
		{
			inputAttachements.push_back(attachement);
			return *this;
		}

		subPassBuilder subPassBuilder::addColorAttachement(const attachement attachement)
		{
			colorAttachements.push_back(attachement);
			return *this;
		}

		subPassBuilder subPassBuilder::setDepthStencilAttachement(const attachement attachement)
		{
			depthStencilAttachement = attachement;
			return *this;
		}

		subpassDescription subPassBuilder::build()
		{
			return description;
		}
	}
}