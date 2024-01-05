#include "attachement.h"
namespace luna 
{
	namespace artemis 
	{
		attachement::attachement(VkAttachmentDescription attachement)
		{
			_attachement = attachement;
			reference = createRef<VkAttachmentReference>();
			reference->attachment = NULL_ATTACHEMENT_REF;
		}

		void attachement::setAttachementRef(uint64_t ref)
		{
			reference->attachment = reference->attachment != NULL_ATTACHEMENT_REF ? ref : reference->attachment; //if an attachement refernce has already been assigned the same a attachment reference is kept else ref is wrong.
		}
	}
}