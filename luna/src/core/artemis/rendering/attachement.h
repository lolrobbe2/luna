#pragma once
#include <core/artemis/rendering/swapchain.h>
namespace luna 
{
	namespace artemis 
	{
	

		class attachement
		{
		public:
			operator VkAttachmentReference() const
			{
				return *reference;
			}
			operator VkAttachmentDescription() const
			{
				return _attachement;
			}
			operator VkClearValue() const 
			{
				return clearValue;
			}
			attachement() = default;
		protected:
			friend class attachementBuilder;
			friend class renderPassBuilder;
			friend class subPassBuilder;
			attachement(VkAttachmentDescription attachement, VkClearValue clearValue);
			void setAttachementRef(uint64_t ref);
			void setReferenceLayout(VkImageLayout layout) { reference->layout = layout; }
		private:
			VkAttachmentDescription _attachement;
			VkImageLayout referenceLayout;
			ref<VkAttachmentReference> reference = createRef<VkAttachmentReference>();
			VkClearValue clearValue;
		};
	}
}

