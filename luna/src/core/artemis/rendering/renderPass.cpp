 #include "renderPass.h"
#include <core/debug/debugMacros.h>

namespace luna
{
	namespace artemis 
	{
		renderPass::renderPass(const VkDevice* device,const VkRenderPassCreateInfo* info)
		{
			VkResult res = vkCreateRenderPass(*device, info, nullptr, &m_renderPass);
			LN_ERR_FAIL_COND_MSG(res != VK_SUCCESS, "an error occured when creating renderPass, VkResult: " + std::to_string(res));
			this->device = device;
		}

	


		renderPassBuilder renderPassBuilder::addSubPass(const subpassDescription description)
		{
			subPasses.push_back(description);
			return *this;
		}

		renderPassBuilder renderPassBuilder::addSubPassDependency(const subpassDescription srcSubpass, const subpassDescription dstSubpass, const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags)
		{
			subpassDependency dependancy{ srcStageMask,dstStageMask,srcAccessMask,dstAccessMask,dependencyFlags };
			return addSubPassDependency(srcSubpass, dstSubpass,dependancy);
		}

		renderPassBuilder renderPassBuilder::addSubPassDependency(const subpassDescription srcSubpass, const subpassDescription dstSubpass, const subpassDependency dependancy)
		{
			auto searchRes1 = std::find(subPasses.begin(), subPasses.end(), srcSubpass);
			auto searchRes2 = std::find(subPasses.begin(), subPasses.end(), dstSubpass);
			VkSubpassDependency dependency;
			//dependency.srcSubpass = searchRes1 != subPasses.end() ? searchRes1 - subPasses.begin() + 1 : 0; //increment by one because VK_SUBPASS_EXTERNALL is 0 Unsigned!
			//dependency.dstSubpass = searchRes2 != subPasses.end() ? searchRes2 - subPasses.begin() + 1 : 0; //increment by one because VK_SUBPASS_EXTERNALL is 0 Unsigned!
			dependency.srcAccessMask = dependancy.srcAccessMask;
			dependency.dstAccessMask = dependancy.dstAccessMask;
			dependency.srcStageMask = dependancy.srcStageMask;
			dependency.dstStageMask = dependancy.dstStageMask;
			dependency.dependencyFlags = dependancy.dependencyFlags;
			subpassDependencys.push_back(dependency);
			return *this;
		}

		renderPassBuilder renderPassBuilder::addSubPassDependency(const subpassDescription srcSubpass, const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags)
		{
			subpassDependency dependancy{ srcStageMask,dstStageMask,srcAccessMask,dstAccessMask,dependencyFlags };
			return addSubPassDependency(srcSubpass, dependancy);
		}

		renderPassBuilder renderPassBuilder::addSubPassDependency(const subpassDescription srcSubpass, const subpassDependency dependency)
		{
			auto searchRes1 = std::find(subPasses.begin(), subPasses.end(), srcSubpass);

			VkSubpassDependency _dependency;
			//_dependency.srcSubpass = searchRes1 != subPasses.end() ? searchRes1 - subPasses.begin() + 1 : 0; //increment by one because VK_SUBPASS_EXTERNALL is 0 Unsigned!
			_dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
			_dependency.srcAccessMask = dependency.srcAccessMask;
			_dependency.dstAccessMask = dependency.dstAccessMask;
			_dependency.srcStageMask = dependency.srcStageMask;
			_dependency.dstStageMask = dependency.dstStageMask;
			_dependency.dependencyFlags = dependency.dependencyFlags;
			subpassDependencys.push_back(_dependency);
			return *this;
		}

		renderPassBuilder renderPassBuilder::addSubPassDependency(const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags)
		{
			subpassDependency dependancy{ srcStageMask,dstStageMask,srcAccessMask,dstAccessMask,dependencyFlags };
			return addSubPassDependency(dependancy);
		}

		renderPassBuilder renderPassBuilder::addSubPassDependency(const subpassDependency dependency)
		{
			VkSubpassDependency _dependency;
			_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			_dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
			_dependency.srcAccessMask = dependency.srcAccessMask;
			_dependency.dstAccessMask = dependency.dstAccessMask;
			_dependency.srcStageMask = dependency.srcStageMask;
			_dependency.dstStageMask = dependency.dstStageMask;
			_dependency.dependencyFlags = dependency.dependencyFlags;
			subpassDependencys.push_back(_dependency);
			return *this;
		}

		renderPass renderPassBuilder::build()
		{
			VkRenderPassCreateInfo info{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
			std::vector<VkAttachmentDescription> attachementDescriptions = generateAttachementDescriptions();
			info.attachmentCount = attachementDescriptions.size();
			info.dependencyCount = subpassDependencys.size();
			info.subpassCount = subPasses.size();

			info.pAttachments = attachementDescriptions.data();
			info.pDependencies = subpassDependencys.data();
			//info.pSubpasses = subPasses.data();
			return renderPass(device,&info);
		}

		void renderPassBuilder::addAttachements(const std::vector<attachement>& attachments, std::vector<VkAttachmentDescription>& descriptions)
		{
			for (attachement attachement : attachments)
			{
				
				if (attachement.reference->attachment == NULL_ATTACHEMENT_REF)
				{
					attachement.reference->attachment = descriptions.size();
					descriptions.push_back(attachement);
				}
			}
		}

		std::vector<VkAttachmentDescription> renderPassBuilder::generateAttachementDescriptions()
		{
			std::vector<VkAttachmentDescription> descriptions;
			for (subpassDescription subpass : subPasses)
			{
				addAttachements(subpass.colorAttachments, descriptions);
				addAttachements(subpass.inputAttachments, descriptions);
				addAttachements(subpass.resolveAttachments, descriptions);
				addAttachements({ subpass.depthStencilAttachment }, descriptions);
			}
			return descriptions;
		}

	}
}
