#pragma once
#include <core/artemis/rendering/attachement.h>

namespace luna 
{
	namespace artemis
	{
		/*
		* @brief helper type for the renderPass builder same as VkSubpassDependency,
		* @brief but without src/dst subPass as this is determined by the renderPass builder.
		*/
		typedef struct subpassDependency {
			VkPipelineStageFlags    srcStageMask;
			VkPipelineStageFlags    dstStageMask;
			VkAccessFlags           srcAccessMask;
			VkAccessFlags           dstAccessMask;
			VkDependencyFlags       dependencyFlags;
		} subpassDependency;

		typedef struct subpassDescription {
			VkSubpassDescriptionFlags       flags;
			VkPipelineBindPoint             pipelineBindPoint;
			const std::vector<attachement> inputAttachments;
			const std::vector<attachement> colorAttachments;
			const std::vector<attachement> resolveAttachments;
			const attachement depthStencilAttachment;
			uint32_t                        preserveAttachmentCount;
			const uint32_t* pPreserveAttachments;
		
			bool operator==(const subpassDescription& rhs) const {
				// Compare relevant members for equality with *this (lhs) and rhs
				return (flags == rhs.flags &&
					pipelineBindPoint == rhs.pipelineBindPoint);
			}
		} subpassDescription;

		class subPassBuilder
		{
		public:
			subPassBuilder setBindPoint(const VkPipelineBindPoint point) { description.pipelineBindPoint = point; return *this; }
			subPassBuilder addInputAttachement(const attachement attachement);
			subPassBuilder addColorAttachement(const attachement attachement);
			subPassBuilder setDepthStencilAttachement(const attachement attachement);
			subpassDescription build();
		private:
			std::vector<attachement> inputAttachements;
			std::vector<attachement> colorAttachements;
			attachement depthStencilAttachement;

			std::vector<VkImageLayout> preserveAttachements;
			subpassDescription description;
		};
	}
}

