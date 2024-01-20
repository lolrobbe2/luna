#pragma once
#include <core/artemis/rendering/builders/subPassBuilder.h>
namespace luna 
{
	namespace artemis 
	{		
		class renderPass
		{
		public:
		protected:
			friend class renderPassBuilder;
			renderPass(const VkDevice* device, const VkRenderPassCreateInfo* info);
		private:
			VkRenderPass m_renderPass = VK_NULL_HANDLE;
			const VkDevice* device;
		};
		/**
		* @brief easely build a renderpass
		*/
		class renderPassBuilder
		{
		public:
			renderPassBuilder(const VkDevice* device) { this->device = device; }
			renderPassBuilder& addSubPass(const subpassDescription description);
			renderPassBuilder& addCreateFlag(const VkRenderPassCreateFlagBits flag) { flags |= flag; return *this; };
			
			renderPassBuilder& addSubPassDependency(const subpassDescription srcSubpass, const subpassDescription dstSubpass,const VkPipelineStageFlags srcStageMask,const VkPipelineStageFlags dstStageMask,const VkAccessFlags srcAccessMask,const VkAccessFlags dstAccessMask,const VkDependencyFlags dependencyFlags);
			renderPassBuilder& addSubPassDependency(const subpassDescription srcSubpass, const subpassDescription dstSubpass,const subpassDependency dependancy);
			
			renderPassBuilder& addSubPassDependency(const subpassDescription srcSubpass, const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags);
			renderPassBuilder& addSubPassDependency(const subpassDescription srcSubpass, const subpassDependency dependancy);
			
			renderPassBuilder& addSubPassDependency(const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags);
			renderPassBuilder& addSubPassDependency(const subpassDependency dependency);
			renderPass build();
		private:
			static void addAttachements(const std::vector<attachement>& attachments, std::vector<VkAttachmentDescription>& descriptions);
			std::vector<VkAttachmentDescription> generateAttachementDescriptions();
			const VkSubpassDescription* generateSubpassDescriptions();

			std::vector<subpassDescription> subPasses; //engine
			std::vector<VkSubpassDescription> subpassDescriptions; //vulkan native

			std::vector<VkSubpassDependency> subpassDependencys;
			VkRenderPassCreateFlags flags = 0;
			const VkDevice* device;
		};
		
	}
}


