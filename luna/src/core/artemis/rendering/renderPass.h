#pragma once
#include <core/artemis/rendering/builders/subPassBuilder.h>
namespace luna 
{
	namespace artemis 
	{		
		class renderPass
		{
		public:
			operator VkRenderPass() const { return m_renderPass; }
			VkRenderPass getNative() const { return m_renderPass; }
			_ALWAYS_INLINE_ const VkClearValue* getClearValues() const { return clearValues.data(); }
			_ALWAYS_INLINE_ const uint32_t getClearValueCount() const { return clearValues.size(); }
		protected:
			friend class renderPassBuilder;
			renderPass(const VkDevice* device, const VkRenderPassCreateInfo* info,const std::vector<VkClearValue> clearValues);
		private:
			VkRenderPass m_renderPass = VK_NULL_HANDLE;
			std::vector<VkClearValue> clearValues;
			const VkDevice* device;
		};
		/**
		* @brief easely build a renderpass
		*/
		class renderPassBuilder
		{
		public:
			renderPassBuilder& addSubPass(const subpassDescription description);
			renderPassBuilder& addCreateFlag(const VkRenderPassCreateFlagBits flag) { flags |= flag; return *this; };
			
			renderPassBuilder& addSubPassDependency(const subpassDescription srcSubpass, const subpassDescription dstSubpass,const VkPipelineStageFlags srcStageMask,const VkPipelineStageFlags dstStageMask,const VkAccessFlags srcAccessMask,const VkAccessFlags dstAccessMask,const VkDependencyFlags dependencyFlags);
			renderPassBuilder& addSubPassDependency(const subpassDescription srcSubpass, const subpassDescription dstSubpass,const subpassDependency dependancy);
			
			renderPassBuilder& addSubPassDependency(const subpassDescription srcSubpass, const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags);
			renderPassBuilder& addSubPassDependency(const subpassDescription srcSubpass, const subpassDependency dependancy);
			
			renderPassBuilder& addSubPassDependency(const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags);
			renderPassBuilder& addSubPassDependency(const subpassDependency dependency);
		
			ref<renderPass> build();
		protected:
			friend class device;
			renderPassBuilder(const VkDevice* device) { this->device = device; }
		private:
			static void addAttachements(const std::vector<attachement>& attachments, std::vector<VkAttachmentDescription>& descriptions, std::vector<VkClearValue>& clearValues);
			std::vector<VkAttachmentDescription> generateAttachementDescriptions();
			const VkSubpassDescription* generateSubpassDescriptions();
			std::vector<VkClearValue> clearValues;
			std::vector<subpassDescription> subPasses; //engine
			std::vector<VkSubpassDescription> subpassDescriptions; //vulkan native

			std::vector<VkSubpassDependency> subpassDependencys;
			VkRenderPassCreateFlags flags = 0;
			const VkDevice* device;
		};
		
	}
}


