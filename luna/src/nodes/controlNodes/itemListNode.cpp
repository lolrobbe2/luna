#include "itemListNode.h"
namespace luna 
{
	namespace nodes
	{
		void itemListNode::init(luna::scene* scene)
		{
			this->scene = scene;
			entityHandle = scene->create();
			addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(itemListNode);
			addComponent<tagComponent>().tag = LN_CLASS_STRINGIFY(itemListNode);
			addComponent<itemList>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
		}
		int itemListNode::addItem(const std::string& pItem, const ref<renderer::texture>& pTexture, bool selectable)
		{
			item item;
			item.icon = pTexture;
			item.text = pItem;
			item.selectable = selectable;
			auto& itemList = getComponent<luna::itemList>();
			itemList.items.push_back(item);
			int itemId = itemList.items.size() - 1;

			//_shape(items.size() - 1);


			itemList.shapeChanged = true;
			//notify_property_list_changed();
			return itemId;
		}

	}
}
