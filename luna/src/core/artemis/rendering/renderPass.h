#pragma once
#include <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis 
	{
		class subPassBuilder
		{
		public:
			subPassBuilder setBindPoint(const VkPipelineBindPoint point) { description.pipelineBindPoint = point; return *this; }
			subPassBuilder addInputAttachement(const VkImageLayout layout);
			subPassBuilder addColorAttachement(const VkImageLayout layout);
			subPassBuilder setDepthStencilAttachement(VkImageLayout layout);
			VkSubpassDescription build();
		private:
			std::vector<VkImageLayout> inputAttachements;
			std::vector<VkImageLayout> colorAttachements;
			VkImageLayout depthStencilAttachement;

			std::vector<VkImageLayout> preserveAttachements;
			VkSubpassDescription description;
		};
		

		/**
		* @brief easely build a renderpass
		*/
		class renderPassBuilder
		{
		public:
			renderPassBuilder(const VkDevice* device) { this->device = device; }
			renderPassBuilder addSubPass(const VkSubpassDescription description);
			renderPassBuilder addCreateFlag(const VkRenderPassCreateFlagBits flag) { flags |= flag; return *this; };
			renderPassBuilder addAttachementDescription(const VkAttachmentDescription description);
			renderPassBuilder addSubPassDependency(const VkSubpassDescription srcSubpass, const VkSubpassDescription dstSubpass,const VkPipelineStageFlags srcStageMask,const VkPipelineStageFlags dstStageMask,const VkAccessFlags srcAccessMask,const VkAccessFlags dstAccessMask,const VkDependencyFlags dependencyFlags);
			renderPassBuilder addSubPassDependency(const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags);
			renderPass build();
		private:
			std::vector<VkSubpassDescription> subPasses;
			std::vector<VkAttachmentDescription> attachementDescriptions;
			std::vector<VkSubpassDependency> subpassDependencys;
			VkRenderPassCreateFlags flags = 0;
			const VkDevice* device;
		};
		class renderPass
		{
		public:
		protected:
			friend class renderPassBuilder;
			renderPass(const VkDevice* device, const VkRenderPassCreateInfo* info);
		private:
			VkRenderPass m_renderPass;
			const VkDevice* device;
		};
	}
}


