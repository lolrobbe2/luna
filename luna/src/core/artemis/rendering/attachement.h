#pragma once
#include <core/artemis/rendering/swapchain.h>
namespace luna 
{
	namespace artemis 
	{
	

		class attachement
		{
		public:
			operator VkAttachmentReference() {
				return *reference;
			}
			operator VkAttachmentDescription()
			{
				return _attachement;
			}
			attachement() = default;
		protected:
			friend class attachementBuilder;
			friend class renderPassBuilder;
			friend class subPassBuilder;
			attachement(VkAttachmentDescription attachement);
			void setAttachementRef(uint64_t ref);
			void setReferenceLayout(VkImageLayout layout) { reference->layout = layout; }

		private:
			VkAttachmentDescription _attachement;
			VkImageLayout referenceLayout;
			ref<VkAttachmentReference> reference = createRef<VkAttachmentReference>();
		};
	}
}

