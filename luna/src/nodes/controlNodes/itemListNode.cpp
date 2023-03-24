#include "itemListNode.h"
#include <core/rendering/renderer2D.h>
namespace luna 
{
	namespace nodes
	{
		itemListNode::itemListNode(Node node) : controlNode(node,node){}

		itemListNode::itemListNode(entt::entity handle, luna::scene* scene) : controlNode(handle, scene){}
		
		void itemListNode::init(luna::scene* scene)
		{
			this->scene = scene;
			entityHandle = scene->create();
			addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(itemListNode);
			addComponent<tagComponent>().tag = LN_CLASS_STRINGIFY(itemListNode);
			addComponent<transformComponent>();
			addComponent<itemList>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
		}

		void itemListNode::shape(int pIdx) 
		{
			auto& itemList = getComponent<luna::itemList>();
			item& item = itemList.items[pIdx];
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

			shape(itemList.items.size() - 1);


			itemList.shapeChanged = true;
			//notify_property_list_changed();
			return itemId;
		}
		int itemListNode::addIconItem(const ref<renderer::texture>& pItem, bool selectable) 
		{
			item item;
			item.icon = pItem;
			item.selectable = selectable;
			auto& itemList = getComponent<luna::itemList>();
			itemList.items.push_back(item);
			int itemId = itemList.items.size() - 1;

			
			itemList.shapeChanged = true;
			
			return itemId;
		}
		void itemListNode::setItemText(int pIdx, const std::string& text) 
		{
			auto& itemList = getComponent<luna::itemList>();
			if (pIdx < 0) pIdx += getItemCount();
			if (itemList.items[pIdx].text == text) return;
			if (itemList.items.size() < pIdx) return;

			itemList.items[pIdx].text = text;
			shape(pIdx);
			itemList.shapeChanged = true;
		}

		std::string itemListNode::getItemText(int pIdx) 
		{
			auto& itemList = getComponent<luna::itemList>();
			return itemList.items[pIdx].text;
		}

		void itemListNode::setItemIcon(int pIdx, const ref<renderer::texture>& pIcon) 
		{
			auto& itemList = getComponent<luna::itemList>();
			if (pIdx < 0) pIdx += getItemCount();
			//ERR_FAIL_INDEX(pIdx, items.size());
			if (itemList.items[pIdx].icon == pIcon) return;
			itemList.items[pIdx].icon = pIcon;
			itemList.shapeChanged = true;
		}
		ref<renderer::texture> itemListNode::getItemIcon(int pIdx)
		{
			//ERR_FAIL_INDEX_V(pIdx, items.size(), ref<Texture2D>());
			auto& itemList = getComponent<luna::itemList>();
			return itemList.items[pIdx].icon;
		}
		glm::vec2 itemListNode::getItemIconRegion(int pIdx)
		{
			//ERR_FAIL_INDEX_V(pIdx, items.size(), Rect2());
			auto& itemList = getComponent<luna::itemList>();
			return itemList.items[pIdx].iconRegion;
		}
		void itemListNode::setItemCustomBgColor(int pIdx, const glm::vec4& pCustomBgColor)
		{
			if (pIdx < 0) pIdx += getItemCount();
			//ERR_FAIL_INDEX(pIdx, items.size());
			auto& itemList = getComponent<luna::itemList>();
			if (itemList.items[pIdx].customBg == pCustomBgColor) return;

			itemList.items[pIdx].customBg = pCustomBgColor;
			//
		}

		glm::vec4 itemListNode::getItemCustomBgColor(int pIdx)
		{
			//ERR_FAIL_INDEX_V(pIdx, items.size(), Color());
			auto& itemList = getComponent<luna::itemList>();
			return itemList.items[pIdx].customBg;
		}

		void itemListNode::setItemCustomFgColor(int pIdx, const glm::vec4& pCustomFgColor)
		{
			if (pIdx < 0) pIdx += getItemCount();
			//ERR_FAIL_INDEX(pIdx, items.size());
			auto& itemList = getComponent<luna::itemList>();

			if (itemList.items[pIdx].customFg == pCustomFgColor) return;

			itemList.items[pIdx].customFg = pCustomFgColor;
			//
		}

		glm::vec4 itemListNode::getItemCustomFgColor(int pIdx)
		{
			//ERR_FAIL_INDEX_V(pIdx, items.size(), Color());
			auto& itemList = getComponent<luna::itemList>();

			return itemList.items[pIdx].customFg;
		}

		void itemListNode::setItemTagIcon(int p_idx, const ref<renderer::texture>& pTagIcon) 
		{
			if (p_idx < 0) p_idx += getItemCount();
			//ERR_FAIL_INDEX(p_idx, items.size());
			auto& itemList = getComponent<luna::itemList>();

			if (itemList.items[p_idx].tagIcon == pTagIcon) return;

			itemList.items[p_idx].tagIcon = pTagIcon;
			itemList.shapeChanged = true;
		}

		void itemListNode::setItemSelectable(int pIdx, bool selectable) 
		{
			if (pIdx < 0) pIdx += getItemCount();
			//ERR_FAIL_INDEX(p_idx, items.size());
			auto& itemList = getComponent<luna::itemList>();
			itemList.items[pIdx].selectable = selectable;
		}

		bool itemListNode::isItemSelectable(int pIdx)
		{
			//ERR_FAIL_INDEX_V(p_idx, items.size(), false);
			auto& itemList = getComponent<luna::itemList>();

			return itemList.items[pIdx].selectable;
		}

		void itemListNode::setItemDisabled(int pIdx, bool disabled) 
		{
			if (pIdx < 0) pIdx += getItemCount();
			auto& itemList = getComponent<luna::itemList>();

			//ERR_FAIL_INDEX(pIdx, items.size());

			if (itemList.items[pIdx].disabled == disabled) return;

			itemList.items[pIdx].disabled = disabled;
			//
		}

		bool itemListNode::isItemDisabled(int pIdx)
		{
			//ERR_FAIL_INDEX_V(pIdx, items.size(), false);
			auto& itemList = getComponent<luna::itemList>();
			return itemList.items[pIdx].disabled;
		}

		void itemListNode::select(int pIdx, bool pSingle) {
			//ERR_FAIL_INDEX(p_idx, items.size());
			auto& itemList = getComponent<luna::itemList>();

			if (pSingle || itemList.selectMode == luna::itemList::SELECT_SINGLE) {
				if (!itemList.items[pIdx].selectable || itemList.items[pIdx].disabled) {
					return;
				}

				for (int i = 0; i < itemList.items.size(); i++) {
					itemList.items[i].selected = pIdx == i;
				}

				itemList.current = pIdx;
				itemList.ensureSelectedVisible = false;
			}
			else {
				if (itemList.items[pIdx].selectable && !itemList.items[pIdx].disabled) {
					itemList.items[pIdx].selected = true;
				}
			}
			
		}

		int itemListNode::getItemAtPosition(const glm::vec2& _pos, bool exact = false)
		{
			auto& itemList = getComponent<luna::itemList>();
			auto& transform = getComponent<luna::transformComponent>();
			
			return 0;
		}

		void itemListNode::deselect(int p_idx) {
			//ERR_FAIL_INDEX(p_idx, items.size());
			auto& itemList = getComponent<luna::itemList>();

			if (itemList.selectMode != luna::itemList::SELECT_MULTI) {
				itemList.items[p_idx].selected = false;
				itemList.current = -1;
			}
			else {
				itemList.items[p_idx].selected = false;
			}
			
		}

		void itemListNode::deselect_all() {
			auto& itemList = getComponent<luna::itemList>();

			if (itemList.items.size() < 1) {
				return;
			}

			for (int i = 0; i < itemList.items.size(); i++) {
				itemList.items[i].selected = false;
			}
			itemList.current = -1;
			
		}

		bool itemListNode::isSelected(int pIdx) 
		{
			//ERR_FAIL_INDEX_V(p_idx, items.size(), false);
			auto& itemList = getComponent<luna::itemList>();

			return itemList.items[pIdx].selected;
		}

		void itemListNode::setCurrent(int p_current) 
		{
			//ERR_FAIL_INDEX(p_current, items.size());
			auto& itemList = getComponent<luna::itemList>();

			if (itemList.current == p_current) return;
			if (itemList.selectMode == itemList::SELECT_SINGLE) select(p_current, true);
			else itemList.current = p_current;
		}

		int itemListNode::getCurrent()
		{
			auto& itemList = getComponent<luna::itemList>();

			return itemList.current;
		}

		void itemListNode::moveItem(int pFromIdx, int pToIdx) {
			//ERR_FAIL_INDEX(p_from_idx, items.size());
			//ERR_FAIL_INDEX(p_to_idx, items.size());
			auto& itemList = getComponent<luna::itemList>();

			if (isAnythingSelected() && getSelectedItems()[0] == pFromIdx) {
				itemList.current = pToIdx;
			}

			item item = itemList.items[pFromIdx];
			itemList.items.erase(itemList.items.begin() + pFromIdx);
			itemList.items.insert(itemList.items.begin() + pToIdx, item);

			
			itemList.shapeChanged = true;
			
		}

		void itemListNode::setItemCount(int count) 
		{
			if(count < 0) return LN_CORE_ERROR("count was negative: {0}",count);
			auto& itemList = getComponent<luna::itemList>();
			if (itemList.items.size() == count) return;

			itemList.items.resize(count);
			
			itemList.shapeChanged = true;
			//notify_property_list_changed();
		}
		int itemListNode::getItemCount()
		{
			return getComponent<luna::itemList>().items.size();
		}
		std::vector<int> itemListNode::getSelectedItems()
		{
			std::vector<int> selected;
			auto& itemList = getComponent<luna::itemList>();
			auto& items = itemList.items;
			for (int i = 0; i < itemList.items.size(); i++) {
				if (items[i].selected) {
					selected.push_back(i);
					if (itemList.selectMode == itemList::SELECT_SINGLE) {
						break;
					}
				}
			}
			return selected;
		}

		bool itemListNode::isAnythingSelected()
		{
			auto& itemList = getComponent<luna::itemList>();
			for (int i = 0; i < itemList.items.size(); i++) {
				if (itemList.items[i].selected) return true;
			}
			return false;
		}
		//event dispatch
		bool itemListNode::guiInputEvent(Event& pEvent) 
		{
			//ERR_FAIL_COND(p_event.is_null());


			//double prev_scroll = scroll_bar->get_value();
			LN_PROFILE_FUNCTION();
			eventDispatcher dispatcher(pEvent);
			dispatcher.dispatch<mouseMovedEvent>(LN_BIND_EVENT_FN(itemListNode::mouseMotionEvent));
			if (pEvent.IsInCategory(luna::eventCategoryMouseButton) || pEvent.IsInCategory(luna::eventCategoryKeyboard)) itemListNode::mouseEvent(pEvent);
		
			return false;
		}
		bool itemListNode::mouseMotionEvent(mouseMovedEvent& Event)
		{
			auto& itemList = getComponent<luna::itemList>();
			for (size_t i = 0; i < itemList.items.size(); i++)
			{
				item& item = itemList.items[i];
				if (i == itemList.current && !itemList.items[itemList.current].selected) itemList.items[itemList.current].customFg = {255.0f, 105.0f, 180.0f ,255.0f};
				else if(!itemList.items[i].selected && i!= itemList.current)item.customFg = { 255.0f, 182.0f, 193.0f,255.0f };
			}
			return false;
		}

		bool itemListNode::mouseEvent(Event& Event)
		{
			
			auto& itemList = getComponent<luna::itemList>();
			//TODO improve code!
			luna::mouseButtonPressedEvent& mouseButtonEvent = (luna::mouseButtonPressedEvent&)Event;
			//if (Event.getEventType() != eventType::MouseButtonPressed) mouseButtonEvent = luna::mouseButtonPressedEvent(Mouse::MOUSE_CODE_MAX_ENUM,false);
			luna::mouseMovedEvent& mouseMovedEvent = (luna::mouseMovedEvent&)Event;
			luna::keyPressedEvent& pressedEvent = (luna::keyPressedEvent&)Event;
			if (itemList.current != -1 && mouseButtonEvent.getMouseButton() == Mouse::ButtonLeft) 
			{
				itemList.items[itemList.current].selected = true;
				itemList.items[itemList.current].customFg = { 255.0f, 0.0f, 255.0f,255.0f };
			}
			for(item& item : itemList.items)
			{
				if(!item.hover && mouseButtonEvent.getMouseButton() == Mouse::ButtonLeft)
				{
					item.selected = false;
					item.customFg = { 255.0f, 182.0f, 193.0f,255.0f };
				}
			}
			
			return false;
		}
	}
}
