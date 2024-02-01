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
			VkAccessFlags           srcAccessMask;
			VkAccessFlags           dstAccessMask;
			VkPipelineStageFlags    srcStageMask;
			VkPipelineStageFlags    dstStageMask;
			VkDependencyFlags       dependencyFlags;
		} subpassDependency;

		typedef struct subpassDescription {
			VkSubpassDescriptionFlags       flags;
			VkPipelineBindPoint             pipelineBindPoint;
			std::vector<attachement> inputAttachments;
			std::vector<attachement> colorAttachments;
			std::vector<attachement> resolveAttachments;
			attachement depthStencilAttachment;
			uint32_t                        preserveAttachmentCount;
			const uint32_t* pPreserveAttachments;
		
			bool operator==(const subpassDescription& rhs) const {
				// Compare relevant members for equality with *this (lhs) and rhs
				return (flags == rhs.flags &&
					pipelineBindPoint == rhs.pipelineBindPoint);
			}
			operator std::vector<VkClearValue>() const 
			{
				std::vector<VkClearValue> clearValues;
				for each (attachement attachement in colorAttachments)
				{
					clearValues.emplace_back(attachement);
				}
				for each (attachement attachement in inputAttachments)
				{
					clearValues.emplace_back(attachement);
				}
				for each (attachement attachement in resolveAttachments)
				{
					clearValues.emplace_back(attachement);
				}
			}
			operator VkSubpassDescription() 
			{
				VkSubpassDescription description{};
				description.flags = flags;
				description.pipelineBindPoint = pipelineBindPoint;
				std::vector<VkAttachmentReference> _inputAttachements;
				for(attachement attachement : inputAttachments) 
				{
					_inputAttachements.push_back(attachement);
				}

				std::vector<VkAttachmentReference> _colorAttachements;
				for (attachement attachement : colorAttachments)
				{
					_colorAttachements.push_back(attachement);
				}
				description.pInputAttachments = _inputAttachements.data();
				description.inputAttachmentCount = _inputAttachements.size();
				description.pColorAttachments = _colorAttachements.data();
				description.colorAttachmentCount = _colorAttachements.size();
				return description;
			}


		} subpassDescription;

		class subPassBuilder
		{
		public:
			subPassBuilder() = default;
			subPassBuilder& setBindPoint(const VkPipelineBindPoint point) { description.pipelineBindPoint = point; return *this; }
			subPassBuilder& addInputAttachement(attachement attachement, const VkImageLayout referenceLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL);
			subPassBuilder& addColorAttachement(attachement attachement, const VkImageLayout referenceLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			subPassBuilder& setDepthStencilAttachement(const attachement attachement);
			subpassDescription build();
		private:
			std::vector<attachement> inputAttachements;
			std::vector<attachement> colorAttachements;
			attachement depthStencilAttachement = attachement();

			std::vector<VkImageLayout> preserveAttachements;
			subpassDescription description = subpassDescription();
		};
	}
}

