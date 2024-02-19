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
			renderPassBuilder& addSubPass(const subpassDescription description);
			renderPassBuilder& addCreateFlag(const VkRenderPassCreateFlagBits flag) { flags |= flag; return *this; };
			
			renderPassBuilder& addSubPassDependency(const subpassDescription srcSubpass, const subpassDescription dstSubpass,const VkPipelineStageFlags srcStageMask,const VkPipelineStageFlags dstStageMask,const VkAccessFlags srcAccessMask,const VkAccessFlags dstAccessMask,const VkDependencyFlags dependencyFlags);
			renderPassBuilder& addSubPassDependency(const subpassDescription srcSubpass, const subpassDescription dstSubpass,const subpassDependency dependancy);
			
			renderPassBuilder& addSubPassDependency(const subpassDescription srcSubpass, const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags);
			renderPassBuilder& addSubPassDependency(const subpassDescription srcSubpass, const subpassDependency dependancy);
			
			renderPassBuilder& addSubPassDependency(const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags);
			renderPassBuilder& addSubPassDependency(const subpassDependency dependency);
			
			renderPassBuilder& addClearColorValue(const float r ,const float g,const float b,const float a);
			renderPassBuilder& addClearColorValue(const int32_t r, const int32_t g, const int32_t b, const int32_t a);
			renderPassBuilder& addClearColorValue(const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a);

			renderPassBuilder& addDepthStencilValue(float depth, uint32_t stencil);
			
			ref<renderPass> build();
		protected:
			friend class device;
			renderPassBuilder(const VkDevice* device) { this->device = device; }
		private:
			static void addAttachements(const std::vector<attachement>& attachments, std::vector<VkAttachmentDescription>& descriptions);
			std::vector<VkAttachmentDescription> generateAttachementDescriptions();
			const VkSubpassDescription* generateSubpassDescriptions();
			std::vector<subpassDescription> subPasses; //engine
			std::vector<VkSubpassDescription> subpassDescriptions; //vulkan native

			std::vector<VkClearColorValue> clearColorValues;
			std::vector<VkClearDepthStencilValue> clearDepthStencilValues;
			std::vector<VkSubpassDependency> subpassDependencys;
			VkRenderPassCreateFlags flags = 0;
			const VkDevice* device;
		};
		
	}
}


