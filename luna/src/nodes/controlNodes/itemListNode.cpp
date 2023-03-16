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
			glm::vec2 pos = _pos;
			/*
			pos -= theme_cache.panel_style->get_offset();
			pos.y += scroll_bar->get_value();
			*/
			/*
			if (is_layout_rtl()) {
				pos.x = get_size().width - pos.x;
			}
			*/
			int closest = -1;
			int closest_dist = 0x7FFFFFFF;

			for (int i = 0; i < itemList.items.size(); i++) {
				item::rectangle rectangle = itemList.items[i].rectCache;
				if (i % itemList.currentColumns == itemList.currentColumns - 1) {
					rectangle.setWidth(transform.scale.x - rectangle.position.x); // Make sure you can still select the last item when clicking past the column.
				}

				if (rectangle.hasPoint(pos)) {
					closest = i;
					break;
				}

				float dist = rectangle.distanceTo(pos);
				if (!exact && dist < closest_dist) {
					closest = i;
					closest_dist = dist;
				}
			}

			return closest;
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

#define CAN_SELECT(i) (items[i].selectable && !items[i].disabled)
#define IS_SAME_ROW(i, row) (i / current_columns == row)

			//double prev_scroll = scroll_bar->get_value();

			eventDispatcher dispatcher(pEvent);
			dispatcher.dispatch<mouseMovedEvent>(LN_BIND_EVENT_FN(itemListNode::mouseMotionEvent));
			if (pEvent.IsInCategory(luna::eventCategoryMouseButton) || pEvent.IsInCategory(luna::eventCategoryKeyboard)) itemListNode::mouseEvent(pEvent);
			/*
			if (p_event->is_pressed() && items.size() > 0) {
				if (p_event->is_action("ui_up", true)) {
					if (!search_string.is_empty()) {
						uint64_t now = OS::get_singleton()->get_ticks_msec();
						uint64_t diff = now - search_time_msec;

						if (diff < uint64_t(GLOBAL_GET("gui/timers/incremental_search_max_interval_msec")) * 2) {
							for (int i = current - 1; i >= 0; i--) {
								if (CAN_SELECT(i) && items[i].text.begins_with(search_string)) {
									set_current(i);
									ensure_current_is_visible();
									if (select_mode == SELECT_SINGLE) {
										emit_signal(SNAME("item_selected"), current);
									}

									break;
								}
							}
							accept_event();
							return;
						}
					}

					if (current >= current_columns) {
						int next = current - current_columns;
						while (next >= 0 && !CAN_SELECT(next)) {
							next = next - current_columns;
						}
						if (next < 0) {
							accept_event();
							return;
						}
						set_current(next);
						ensure_current_is_visible();
						if (select_mode == SELECT_SINGLE) {
							emit_signal(SNAME("item_selected"), current);
						}
						accept_event();
					}
				}
				else if (p_event->is_action("ui_down", true)) {
					if (!search_string.is_empty()) {
						uint64_t now = OS::get_singleton()->get_ticks_msec();
						uint64_t diff = now - search_time_msec;

						if (diff < uint64_t(GLOBAL_GET("gui/timers/incremental_search_max_interval_msec")) * 2) {
							for (int i = current + 1; i < items.size(); i++) {
								if (CAN_SELECT(i) && items[i].text.begins_with(search_string)) {
									set_current(i);
									ensure_current_is_visible();
									if (select_mode == SELECT_SINGLE) {
										emit_signal(SNAME("item_selected"), current);
									}
									break;
								}
							}
							accept_event();
							return;
						}
					}

					if (current < items.size() - current_columns) {
						int next = current + current_columns;
						while (next < items.size() && !CAN_SELECT(next)) {
							next = next + current_columns;
						}
						if (next >= items.size()) {
							accept_event();
							return;
						}
						set_current(next);
						ensure_current_is_visible();
						if (select_mode == SELECT_SINGLE) {
							emit_signal(SNAME("item_selected"), current);
						}
						accept_event();
					}
				}
				else if (p_event->is_action("ui_page_up", true)) {
					search_string = ""; //any mousepress cancels

					for (int i = 4; i > 0; i--) {
						int index = current - current_columns * i;
						if (index >= 0 && index < items.size() && CAN_SELECT(index)) {
							set_current(index);
							ensure_current_is_visible();
							if (select_mode == SELECT_SINGLE) {
								emit_signal(SNAME("item_selected"), current);
							}
							accept_event();
							break;
						}
					}
				}
				else if (p_event->is_action("ui_page_down", true)) {
					search_string = ""; //any mousepress cancels

					for (int i = 4; i > 0; i--) {
						int index = current + current_columns * i;
						if (index >= 0 && index < items.size() && CAN_SELECT(index)) {
							set_current(index);
							ensure_current_is_visible();
							if (select_mode == SELECT_SINGLE) {
								emit_signal(SNAME("item_selected"), current);
							}
							accept_event();

							break;
						}
					}
				}
				else if (p_event->is_action("ui_left", true)) {
					search_string = ""; //any mousepress cancels

					if (current % current_columns != 0) {
						int current_row = current / current_columns;
						int next = current - 1;
						while (next >= 0 && !CAN_SELECT(next)) {
							next = next - 1;
						}
						if (next < 0 || !IS_SAME_ROW(next, current_row)) {
							accept_event();
							return;
						}
						set_current(next);
						ensure_current_is_visible();
						if (select_mode == SELECT_SINGLE) {
							emit_signal(SNAME("item_selected"), current);
						}
						accept_event();
					}
				}
				else if (p_event->is_action("ui_right", true)) {
					search_string = ""; //any mousepress cancels

					if (current % current_columns != (current_columns - 1) && current + 1 < items.size()) {
						int current_row = current / current_columns;
						int next = current + 1;
						while (next < items.size() && !CAN_SELECT(next)) {
							next = next + 1;
						}
						if (items.size() <= next || !IS_SAME_ROW(next, current_row)) {
							accept_event();
							return;
						}
						set_current(next);
						ensure_current_is_visible();
						if (select_mode == SELECT_SINGLE) {
							emit_signal(SNAME("item_selected"), current);
						}
						accept_event();
					}
				}
				else if (p_event->is_action("ui_cancel", true)) {
					search_string = "";
				}
				else if (p_event->is_action("ui_select", true) && select_mode == SELECT_MULTI) {
					if (current >= 0 && current < items.size()) {
						if (items[current].selectable && !items[current].disabled && !items[current].selected) {
							select(current, false);
							emit_signal(SNAME("multi_selected"), current, true);
						}
						else if (items[current].selected) {
							deselect(current);
							emit_signal(SNAME("multi_selected"), current, false);
						}
					}
				}
				else if (p_event->is_action("ui_accept", true)) {
					search_string = ""; //any mousepress cancels

					if (current >= 0 && current < items.size()) {
						emit_signal(SNAME("item_activated"), current);
					}
				}
				else {
					
					ref<InputEventKey> k = p_event;

					if (k.is_valid() && k->get_unicode()) {
						uint64_t now = OS::get_singleton()->get_ticks_msec();
						uint64_t diff = now - search_time_msec;
						uint64_t max_interval = uint64_t(GLOBAL_GET("gui/timers/incremental_search_max_interval_msec"));
						search_time_msec = now;

						if (diff > max_interval) {
							search_string = "";
						}

						if (String::chr(k->get_unicode()) != search_string) {
							search_string += String::chr(k->get_unicode());
						}

						for (int i = current + 1; i <= items.size(); i++) {
							if (i == items.size()) {
								if (current == 0 || current == -1) {
									break;
								}
								else {
									i = 0;
								}
							}

							if (i == current) {
								break;
							}

							if (items[i].text.findn(search_string) == 0) {
								setCurrent(i);
								ensureCurrentIsVisible();
								if (select_mode == SELECT_SINGLE) {
									//emit_signal(SNAME("item_selected"), current);
								}
								break;
							}
						}
					}
				}
			}
			*/
			/*
			ref<InputEventPanGesture> pan_gesture = p_event;
			if (pan_gesture.is_valid()) {
				scroll_bar->set_value(scroll_bar->get_value() + scroll_bar->get_page() * pan_gesture->get_delta().y / 8);
			}

			if (scroll_bar->get_value() != prev_scroll) {
				accept_event(); //accept event if scroll changed
			}
			*/
			#undef CAN_SELECT
			#undef IS_SAME_ROW
			return false;
		}
		bool itemListNode::mouseMotionEvent(mouseMovedEvent& Event)
		{
			auto& itemList = getComponent<luna::itemList>();
			if (itemList.deferSelectSingle >= 0) {
				itemList.deferSelectSingle = -1;
				return false;
			}
		}

		bool itemListNode::mouseEvent(Event& Event)
		{
			
			auto& itemList = getComponent<luna::itemList>();
			//TODO improve code!
			luna::mouseButtonPressedEvent& mouseButtonEvent = (luna::mouseButtonPressedEvent&)Event;
			//if (Event.getEventType() != eventType::MouseButtonPressed) mouseButtonEvent = luna::mouseButtonPressedEvent(Mouse::MOUSE_CODE_MAX_ENUM,false);
			luna::mouseMovedEvent& mouseMovedEvent = (luna::mouseMovedEvent&)Event;
			luna::keyPressedEvent& pressedEvent = (luna::keyPressedEvent&)Event;
;			if (itemList.deferSelectSingle >= 0 && mouseButtonEvent.getMouseButton() == Mouse::ButtonLeft) {
				select(itemList.deferSelectSingle, true);

				//emit_signal(SNAME("multi_selected"), itemList.deferSelectSingle, true);
				itemList.deferSelectSingle = -1;
				return true;
			}

			if (mouseButtonEvent.getMouseButton() == (Mouse::ButtonLast || Mouse::ButtonLeft || Mouse::ButtonMiddle || Mouse::ButtonRight)) {
				itemList.searchString = ""; //any mousepress cancels
				glm::vec2 pos;
				//pos -= theme_cache.panel_style->get_offset();
				//pos.y += scroll_bar->get_value(); no scrollbar

				/*
				if (is_layout_rtl()) {
					pos.x = get_size().width - pos.x;
				}
				*/
				int closest = getItemAtPosition(renderer::renderer::getSceneMousePos() / renderer::renderer::getSceneDimensions(), true);

				if (closest != -1 && (mouseButtonEvent.getMouseButton() == Mouse::ButtonLeft || (itemList.allowRmbSelect && mouseButtonEvent.getMouseButton() == Mouse::ButtonRight))) {
					int i = closest;

					if (itemList.selectMode == itemList::SELECT_MULTI && itemList.items[i].selected && (pressedEvent.getkeyCode() == input::LeftControl || pressedEvent.getkeyCode() == input::RightControl)) {
						deselect(i);
						//emit_signal(SNAME("multi_selected"), i, false);

					}
					else if (itemList.selectMode == itemList::SELECT_MULTI && (pressedEvent.getkeyCode() == input::LeftShift || pressedEvent.getkeyCode() == input::RightShift) && itemList.current >= 0 && itemList.current < itemList.items.size() && itemList.current != i) {
						int from = itemList.current;
						int to = i;
						if (i < itemList.current) {
								moveItem(from, to);
								moveItem(to, from);
						}
						for (int j = from; j <= to; j++) {
							if (!itemList.items[j].selectable) {
								continue;
							}
							bool selected = !itemList.items[j].selected;
							select(j, false);
							if (selected) {
								//emit_signal(SNAME("multi_selected"), j, true);
							}
						}
						//emit_signal(SNAME("item_clicked"), i, get_local_mouse_position(), mb->get_button_index());

					}
					else {
						//todo dubleclicks
						if (!mouseButtonEvent.getMouseButton() == Mouse::ButtonLeft && mouseButtonEvent.isDoubleClick() && pressedEvent.getkeyCode() != (input::LeftControl || input::RightControl) && itemList.selectMode == itemList::SELECT_MULTI && itemList.items[i].selectable && !itemList.items[i].disabled && itemList.items[i].selected && mouseButtonEvent.getMouseButton() == Mouse::ButtonLeft) {
							itemList.deferSelectSingle = i;
							return true;
						}

						if (!itemList.items[i].selected || itemList.allowReselect) {
							select(i, itemList.selectMode == itemList::SELECT_SINGLE || pressedEvent.getkeyCode() != (input::LeftControl || input::RightControl));

							if (itemList.selectMode == itemList::SELECT_SINGLE) {
								//emit_signal(SNAME("item_selected"), i);
							}
							else {
								//emit_signal(SNAME("multi_selected"), i, true);
							}
						}

						//emit_signal(SNAME("item_clicked"), i, get_local_mouse_position(), mb->get_button_index());

						if (mouseButtonEvent.getMouseButton() == Mouse::ButtonLeft && mouseButtonEvent.isDoubleClick()) {
							//emit_signal(SNAME("item_activated"), i);
						}
					}

					return false;
				}
				else if (closest != -1) {
					//emit_signal(SNAME("item_clicked"), closest, get_local_mouse_position(), mb->get_button_index());
				}
				else {
					// Since closest is null, more likely we clicked on empty space, so send signal to interested controls. Allows, for example, implement items deselecting.
					//emit_signal(SNAME("empty_clicked"), get_local_mouse_position(), mb->get_button_index());
				}
			}
			//TODO
			//scrollbar not yet supported!! 
			/*
			if (mb.is_valid() && mb->get_button_index() == MouseButton::WHEEL_UP && mb->is_pressed()) {
				scroll_bar->set_value(scroll_bar->get_value() - scroll_bar->get_page() * mb->get_factor() / 8);
			}
			if (mb.is_valid() && mb->get_button_index() == MouseButton::WHEEL_DOWN && mb->is_pressed()) {
				scroll_bar->set_value(scroll_bar->get_value() + scroll_bar->get_page() * mb->get_factor() / 8);
			}
			*/
		}
	}
}
