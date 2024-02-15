 #include "renderPass.h"
#include <core/debug/debugMacros.h>

namespace luna
{
	namespace artemis 
	{
		renderPass::renderPass(const VkDevice* device,const VkRenderPassCreateInfo* info)
		{
			VkResult res = vkCreateRenderPass(*device, info, nullptr, &m_renderPass);
			LN_ERR_FAIL_COND_MSG(res != VK_SUCCESS, "an error occured when creating renderPass, VkResult: " + VK_RESULT(res));
			this->device = device;
		}

	


		renderPassBuilder& renderPassBuilder::addSubPass(const subpassDescription description)
		{
			subPasses.push_back(description);
			return *this;
		}

		renderPassBuilder& renderPassBuilder::addSubPassDependency(const subpassDescription srcSubpass, const subpassDescription dstSubpass, const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags)
		{
			subpassDependency dependancy{ srcStageMask,dstStageMask,srcAccessMask,dstAccessMask,dependencyFlags };
			return addSubPassDependency(srcSubpass, dstSubpass,dependancy);
		}

		renderPassBuilder& renderPassBuilder::addSubPassDependency(const subpassDescription srcSubpass, const subpassDescription dstSubpass, const subpassDependency dependancy)
		{
			auto searchRes1 = std::find(subPasses.begin(), subPasses.end(), srcSubpass);
			auto searchRes2 = std::find(subPasses.begin(), subPasses.end(), dstSubpass);
			VkSubpassDependency dependency;
			dependency.srcSubpass = searchRes1 != subPasses.end() ? searchRes1 - subPasses.begin() + 1 : 0; //increment by one because VK_SUBPASS_EXTERNALL is 0 Unsigned!
			dependency.dstSubpass = searchRes2 != subPasses.end() ? searchRes2 - subPasses.begin() + 1 : 0; //increment by one because VK_SUBPASS_EXTERNALL is 0 Unsigned!
			dependency.srcAccessMask = dependancy.srcAccessMask;
			dependency.dstAccessMask = dependancy.dstAccessMask;
			dependency.srcStageMask = dependancy.srcStageMask;
			dependency.dstStageMask = dependancy.dstStageMask;
			dependency.dependencyFlags = dependancy.dependencyFlags;
			subpassDependencys.push_back(dependency);
			return *this;
		}

		renderPassBuilder& renderPassBuilder::addSubPassDependency(const subpassDescription srcSubpass, const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags)
		{
			subpassDependency dependancy{ srcStageMask,dstStageMask,srcAccessMask,dstAccessMask,dependencyFlags };
			return addSubPassDependency(srcSubpass, dependancy);
		}

		renderPassBuilder& renderPassBuilder::addSubPassDependency(const subpassDescription srcSubpass, const subpassDependency dependency)
		{
			auto searchRes1 = std::find(subPasses.begin(), subPasses.end(), srcSubpass);

			VkSubpassDependency _dependency;
			_dependency.srcSubpass = searchRes1 != subPasses.end() ? searchRes1 - subPasses.begin() + 1 : 0; //increment by one because VK_SUBPASS_EXTERNALL is 0 Unsigned!
			_dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
			_dependency.srcAccessMask = dependency.srcAccessMask;
			_dependency.dstAccessMask = dependency.dstAccessMask;
			_dependency.srcStageMask = dependency.srcStageMask;
			_dependency.dstStageMask = dependency.dstStageMask;
			_dependency.dependencyFlags = dependency.dependencyFlags;
			subpassDependencys.push_back(_dependency);
			return *this;
		}

		renderPassBuilder& renderPassBuilder::addSubPassDependency(const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags)
		{
			subpassDependency dependancy{ srcStageMask,dstStageMask,srcAccessMask,dstAccessMask,dependencyFlags };
			return addSubPassDependency(dependancy);
		}

		renderPassBuilder& renderPassBuilder::addSubPassDependency(const subpassDependency dependency)
		{
			VkSubpassDependency _dependency;
			_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			_dependency.dstSubpass = 0;
			_dependency.srcAccessMask = dependency.srcAccessMask;
			_dependency.dstAccessMask = dependency.dstAccessMask;
			_dependency.srcStageMask = dependency.srcStageMask;
			_dependency.dstStageMask = dependency.dstStageMask;
			_dependency.dependencyFlags = dependency.dependencyFlags;
			subpassDependencys.push_back(_dependency);
			return *this;
		}

		renderPassBuilder& renderPassBuilder::addClearColorValue(const float r, const float g, const float b, const float a)
		{
			VkClearColorValue clearColor;
			clearColor.float32[0] = r;
			clearColor.float32[1] = g;
			clearColor.float32[2] = b;
			clearColor.float32[3] = a;
			clearColorValues.push_back(clearColor);
			return *this;
		}

		renderPassBuilder& renderPassBuilder::addClearColorValue(const int32_t r, const int32_t g, const int32_t b, const int32_t a)
		{
			VkClearColorValue clearColor;
			clearColor.int32[0] = r;
			clearColor.int32[1] = g;
			clearColor.int32[2] = b;
			clearColor.int32[3] = a;
			clearColorValues.push_back(clearColor);
			return *this;
		}

		renderPassBuilder& renderPassBuilder::addClearColorValue(const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a)
		{
			VkClearColorValue clearColor;
			clearColor.uint32[0] = r;
			clearColor.uint32[1] = g;
			clearColor.uint32[2] = b;
			clearColor.uint32[3] = a;
			clearColorValues.push_back(clearColor);
			return *this;
		}

		renderPassBuilder& renderPassBuilder::addDepthStencilValue(float depth, uint32_t stencil)
		{
			VkClearDepthStencilValue depthStencilValue;
			depthStencilValue.depth = depth;
			depthStencilValue.stencil = stencil;
			clearDepthStencilValues.push_back(depthStencilValue);
			return *this;
		}

		ref<renderPass> renderPassBuilder::build()
		{
			VkRenderPassCreateInfo info{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
			std::vector<VkAttachmentDescription> attachementDescriptions = generateAttachementDescriptions();
			info.attachmentCount = attachementDescriptions.size();
			info.dependencyCount = subpassDependencys.size();
			info.subpassCount = subPasses.size();

			info.pAttachments = attachementDescriptions.data();
			info.pDependencies = subpassDependencys.data();
			info.pSubpasses = generateSubpassDescriptions();
			return ref<renderPass>(new renderPass(device,&info));
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

		const VkSubpassDescription* renderPassBuilder::generateSubpassDescriptions()
		{
			subpassDescriptions.resize(0);
			for(subpassDescription subpass : subPasses)
			{
				subpassDescriptions.push_back(subpass);
			}
			return subpassDescriptions.data();
		}
	}
}
