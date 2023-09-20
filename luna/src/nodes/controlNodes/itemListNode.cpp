#include "itemListNode.h"
#include <core/rendering/renderer2D.h>
/* DISCLAIMER ALOT of code has been shamelesly copied from godot src*/
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

		void itemListNode::draw()
		{
			
			/*
			auto& transform = getComponent<transformComponent>();
			auto& itemListComponent = getComponent<itemList>();
			glm::vec3 translation{ 0.0f,0.0f,0.0f };
			glm::vec3 customTransform = { 0.0f,0.0f,0.0f };
			static glm::vec2 glyphDimensions;
			if (itemListComponent.font) glyphDimensions = glm::vec2(itemListComponent.font->getGlyph('A')->getWidth(), itemListComponent.font->getGlyph('A')->getHeight());
			glm::vec2 advance{};
			advance.x = (glyphDimensions.x / renderer::renderer::getSceneDimensions().x) + transform.translation.x;
			advance.y = (glyphDimensions.y / renderer::renderer::getSceneDimensions().y) + transform.translation.y;
			for (item& item : itemListComponent.items)
			{
				glm::vec2 size{ 15,3 };
				size.x *= transform.scale.x;
				size.y *= transform.scale.y;
				if (renderer::renderer2D::drawQuad(translation + transform.translation, size + glm::vec2(0.01f), item.customBg))
				{
					item.selectable = false;
					return;
				}
				else item.selectable = true;

				renderer::renderer2D::drawQuad(translation + transform.translation, size, item.customFg);
				customTransform = translation;
				customTransform = customTransform - glm::vec3(size.x / 2, -size.y / 4.0f, 0.0f);
				if (itemListComponent.font) renderer::renderer2D::drawLabel(customTransform + transform.translation, { transform.scale.x,transform.scale.y }, itemListComponent.font, item.text);
				item.rectCache.start = size;
				item.rectCache.position = translation;
				translation.y += size.y;
			}
			*/
			/*
			_check_shape_changed();

			int scroll_bar_minwidth = scroll_bar->get_minimum_size().x;
			scroll_bar->set_anchor_and_offset(SIDE_LEFT, ANCHOR_END, -scroll_bar_minwidth);
			scroll_bar->set_anchor_and_offset(SIDE_RIGHT, ANCHOR_END, 0);
			scroll_bar->set_anchor_and_offset(SIDE_TOP, ANCHOR_BEGIN, theme_cache.panel_style->get_margin(SIDE_TOP));
			scroll_bar->set_anchor_and_offset(SIDE_BOTTOM, ANCHOR_END, -theme_cache.panel_style->get_margin(SIDE_BOTTOM));

			Size2 size = get_size();
			int width = size.width - theme_cache.panel_style->get_minimum_size().width;
			if (scroll_bar->is_visible()) {
				width -= scroll_bar_minwidth;
			}

			draw_style_box(theme_cache.panel_style, Rect2(Point2(), size));

			Ref<StyleBox> sbsel;
			Ref<StyleBox> cursor;

			if (has_focus()) {
				sbsel = theme_cache.selected_focus_style;
				cursor = theme_cache.cursor_focus_style;
			}
			else {
				sbsel = theme_cache.selected_style;
				cursor = theme_cache.cursor_style;
			}
			bool rtl = is_layout_rtl();

			if (has_focus()) {
				RenderingServer::get_singleton()->canvas_item_add_clip_ignore(get_canvas_item(), true);
				draw_style_box(theme_cache.focus_style, Rect2(Point2(), size));
				RenderingServer::get_singleton()->canvas_item_add_clip_ignore(get_canvas_item(), false);
			}

			// Ensure_selected_visible needs to be checked before we draw the list.
			if (ensure_selected_visible && current >= 0 && current < items.size()) {
				Rect2 r = items[current].rect_cache;
				int from = scroll_bar->get_value();
				int to = from + scroll_bar->get_page();

				if (r.position.y < from) {
					scroll_bar->set_value(r.position.y);
				}
				else if (r.position.y + r.size.y > to) {
					scroll_bar->set_value(r.position.y + r.size.y - (to - from));
				}
			}

			ensure_selected_visible = false;

			Vector2 base_ofs = theme_cache.panel_style->get_offset();
			base_ofs.y -= int(scroll_bar->get_value());

			// Define a visible frame to check against and optimize drawing.
			const Rect2 clip(-base_ofs, size);

			// Do a binary search to find the first separator that is below clip_position.y.
			int first_visible_separator = 0;
			{
				int lo = 0;
				int hi = separators.size();
				while (lo < hi) {
					const int mid = (lo + hi) / 2;
					if (separators[mid] < clip.position.y) {
						lo = mid + 1;
					}
					else {
						hi = mid;
					}
				}
				first_visible_separator = lo;
			}

			// Draw visible separators.
			for (int i = first_visible_separator; i < separators.size(); i++) {
				if (separators[i] > clip.position.y + clip.size.y) {
					break; // done
				}

				const int y = base_ofs.y + separators[i];
				draw_line(Vector2(theme_cache.panel_style->get_margin(SIDE_LEFT), y), Vector2(width, y), theme_cache.guide_color);
			}

			// Do a binary search to find the first item whose rect reaches below clip.position.y.
			int first_item_visible;
			{
				int lo = 0;
				int hi = items.size();
				while (lo < hi) {
					const int mid = (lo + hi) / 2;
					const Rect2& rcache = items[mid].rect_cache;
					if (rcache.position.y + rcache.size.y < clip.position.y) {
						lo = mid + 1;
					}
					else {
						hi = mid;
					}
				}

				// We might end up with an item in columns 2, 3, etc, but we need the one from the first column.
				// We can also end up in a state where lo reached hi, and so no items can be rendered; we skip that.
				while (lo < hi && lo > 0 && items[lo].column > 0) {
					lo -= 1;
				}

				first_item_visible = lo;
			}

			// Draw visible items.
			for (int i = first_item_visible; i < items.size(); i++) {
				Rect2 rcache = items[i].rect_cache;

				if (rcache.position.y > clip.position.y + clip.size.y) {
					break; // done
				}

				if (!clip.intersects(rcache)) {
					continue;
				}

				if (current_columns == 1) {
					rcache.size.width = width - rcache.position.x;
				}

				bool should_draw_selected_bg = items[i].selected;
				bool should_draw_hovered_bg = hovered == i && !items[i].selected;
				bool should_draw_custom_bg = items[i].custom_bg.a > 0.001;

				if (should_draw_selected_bg || should_draw_hovered_bg || should_draw_custom_bg) {
					Rect2 r = rcache;
					r.position += base_ofs;
					r.position.y -= theme_cache.v_separation / 2;
					r.size.y += theme_cache.v_separation;
					r.position.x -= theme_cache.h_separation / 2;
					r.size.x += theme_cache.h_separation;

					if (rtl) {
						r.position.x = size.width - r.position.x - r.size.x;
					}

					if (should_draw_selected_bg) {
						draw_style_box(sbsel, r);
					}
					if (should_draw_hovered_bg) {
						draw_style_box(theme_cache.hovered_style, r);
					}
					if (should_draw_custom_bg) {
						draw_rect(r, items[i].custom_bg);
					}
				}

				Vector2 text_ofs;
				if (items[i].icon.is_valid()) {
					Size2 icon_size;
					//= _adjust_to_max_size(items[i].get_icon_size(),fixed_icon_size) * icon_scale;

					if (fixed_icon_size.x > 0 && fixed_icon_size.y > 0) {
						icon_size = fixed_icon_size * icon_scale;
					}
					else {
						icon_size = items[i].get_icon_size() * icon_scale;
					}

					Vector2 icon_ofs;

					Point2 pos = items[i].rect_cache.position + icon_ofs + base_ofs;

					if (icon_mode == ICON_MODE_TOP) {
						pos.x += Math::floor((items[i].rect_cache.size.width - icon_size.width) / 2);
						pos.y += theme_cache.icon_margin;
						text_ofs.y = icon_size.height + theme_cache.icon_margin * 2;
					}
					else {
						pos.y += Math::floor((items[i].rect_cache.size.height - icon_size.height) / 2);
						text_ofs.x = icon_size.width + theme_cache.icon_margin;
					}

					Rect2 draw_rect = Rect2(pos, icon_size);

					if (fixed_icon_size.x > 0 && fixed_icon_size.y > 0) {
						Rect2 adj = _adjust_to_max_size(items[i].get_icon_size() * icon_scale, icon_size);
						draw_rect.position += adj.position;
						draw_rect.size = adj.size;
					}

					Color icon_modulate = items[i].icon_modulate;
					if (items[i].disabled) {
						icon_modulate.a *= 0.5;
					}

					// If the icon is transposed, we have to switch the size so that it is drawn correctly
					if (items[i].icon_transposed) {
						Size2 size_tmp = draw_rect.size;
						draw_rect.size.x = size_tmp.y;
						draw_rect.size.y = size_tmp.x;
					}

					Rect2 region = (items[i].icon_region.size.x == 0 || items[i].icon_region.size.y == 0) ? Rect2(Vector2(), items[i].icon->get_size()) : Rect2(items[i].icon_region);

					if (rtl) {
						draw_rect.position.x = size.width - draw_rect.position.x - draw_rect.size.x;
					}
					draw_texture_rect_region(items[i].icon, draw_rect, region, icon_modulate, items[i].icon_transposed);
				}

				if (items[i].tag_icon.is_valid()) {
					Point2 draw_pos = items[i].rect_cache.position;
					if (rtl) {
						draw_pos.x = size.width - draw_pos.x - items[i].tag_icon->get_width();
					}
					draw_texture(items[i].tag_icon, draw_pos + base_ofs);
				}

				if (!items[i].text.is_empty()) {
					int max_len = -1;

					Vector2 size2 = items[i].text_buf->get_size();
					if (fixed_column_width) {
						max_len = fixed_column_width;
					}
					else if (same_column_width) {
						max_len = items[i].rect_cache.size.x;
					}
					else {
						max_len = size2.x;
					}

					Color txt_modulate;
					if (items[i].selected) {
						txt_modulate = theme_cache.font_selected_color;
					}
					else if (hovered == i) {
						txt_modulate = theme_cache.font_hovered_color;
					}
					else if (items[i].custom_fg != Color()) {
						txt_modulate = items[i].custom_fg;
					}
					else {
						txt_modulate = theme_cache.font_color;
					}

					if (items[i].disabled) {
						txt_modulate.a *= 0.5;
					}

					if (icon_mode == ICON_MODE_TOP && max_text_lines > 0) {
						text_ofs += base_ofs;
						text_ofs += items[i].rect_cache.position;

						if (rtl) {
							text_ofs.x = size.width - text_ofs.x - max_len;
						}

						items.write[i].text_buf->set_alignment(HORIZONTAL_ALIGNMENT_CENTER);

						if (theme_cache.font_outline_size > 0 && theme_cache.font_outline_color.a > 0) {
							items[i].text_buf->draw_outline(get_canvas_item(), text_ofs, theme_cache.font_outline_size, theme_cache.font_outline_color);
						}

						items[i].text_buf->draw(get_canvas_item(), text_ofs, txt_modulate);
					}
					else {
						if (fixed_column_width > 0) {
							size2.x = MIN(size2.x, fixed_column_width);
						}

						if (icon_mode == ICON_MODE_TOP) {
							text_ofs.x += (items[i].rect_cache.size.width - size2.x) / 2;
						}
						else {
							text_ofs.y += (items[i].rect_cache.size.height - size2.y) / 2;
						}

						text_ofs += base_ofs;
						text_ofs += items[i].rect_cache.position;

						float text_w = width - text_ofs.x;
						items.write[i].text_buf->set_width(text_w);

						if (rtl) {
							text_ofs.x = size.width - width;
							items.write[i].text_buf->set_alignment(HORIZONTAL_ALIGNMENT_RIGHT);
						}
						else {
							items.write[i].text_buf->set_alignment(HORIZONTAL_ALIGNMENT_LEFT);
						}

						if (theme_cache.font_outline_size > 0 && theme_cache.font_outline_color.a > 0) {
							items[i].text_buf->draw_outline(get_canvas_item(), text_ofs, theme_cache.font_outline_size, theme_cache.font_outline_color);
						}

						if (width - text_ofs.x > 0) {
							items[i].text_buf->draw(get_canvas_item(), text_ofs, txt_modulate);
						}
					}
				}

				if (select_mode == SELECT_MULTI && i == current) {
					Rect2 r = rcache;
					r.position += base_ofs;
					r.position.y -= theme_cache.v_separation / 2;
					r.size.y += theme_cache.v_separation;
					r.position.x -= theme_cache.h_separation / 2;
					r.size.x += theme_cache.h_separation;

					if (rtl) {
						r.position.x = size.width - r.position.x - r.size.x;
					}

					draw_style_box(cursor, r);
				}
				*/
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
		void itemListNode::checkShapeChanged() {
			if (!getComponent<itemList>().shapeChanged) {
				return;
			}
			/*
			int scroll_bar_minwidth = scroll_bar->get_minimum_size().x;
			Size2 size = get_size();
			float max_column_width = 0.0;

			//1- compute item minimum sizes
			for (int i = 0; i < items.size(); i++) {
				Size2 minsize;
				if (items[i].icon.is_valid()) {
					if (fixed_icon_size.x > 0 && fixed_icon_size.y > 0) {
						minsize = fixed_icon_size * icon_scale;
					}
					else {
						minsize = items[i].get_icon_size() * icon_scale;
					}

					if (!items[i].text.is_empty()) {
						if (icon_mode == ICON_MODE_TOP) {
							minsize.y += theme_cache.icon_margin;
						}
						else {
							minsize.x += theme_cache.icon_margin;
						}
					}
				}

				if (!items[i].text.is_empty()) {
					int max_width = -1;
					if (fixed_column_width) {
						max_width = fixed_column_width;
					}
					else if (same_column_width) {
						max_width = items[i].rect_cache.size.x;
					}
					items.write[i].text_buf->set_width(max_width);
					Size2 s = items[i].text_buf->get_size();

					if (icon_mode == ICON_MODE_TOP) {
						minsize.x = MAX(minsize.x, s.width);
						if (max_text_lines > 0) {
							minsize.y += s.height + theme_cache.line_separation * max_text_lines;
						}
						else {
							minsize.y += s.height;
						}

					}
					else {
						minsize.y = MAX(minsize.y, s.height);
						minsize.x += s.width;
					}
				}

				if (fixed_column_width > 0) {
					minsize.x = fixed_column_width;
				}
				max_column_width = MAX(max_column_width, minsize.x);

				// elements need to adapt to the selected size
				minsize.y += theme_cache.v_separation;
				minsize.x += theme_cache.h_separation;
				items.write[i].rect_cache.size = minsize;
				items.write[i].min_rect_cache.size = minsize;
			}

			int fit_size = size.x - theme_cache.panel_style->get_minimum_size().width - scroll_bar_minwidth;

			//2-attempt best fit
			current_columns = 0x7FFFFFFF;
			if (max_columns > 0) {
				current_columns = max_columns;
			}

			// Repeat until all items fit.
			while (true) {
				bool all_fit = true;
				Vector2 ofs;
				int col = 0;
				int max_h = 0;

				separators.clear();

				for (int i = 0; i < items.size(); i++) {
					if (current_columns > 1 && items[i].rect_cache.size.width + ofs.x > fit_size) {
						// Went past.
						current_columns = MAX(col, 1);
						all_fit = false;
						break;
					}

					if (same_column_width) {
						items.write[i].rect_cache.size.x = max_column_width;
					}
					items.write[i].rect_cache.position = ofs;

					max_h = MAX(max_h, items[i].rect_cache.size.y);
					ofs.x += items[i].rect_cache.size.x + theme_cache.h_separation;

					items.write[i].column = col;
					col++;
					if (col == current_columns) {
						if (i < items.size() - 1) {
							separators.push_back(ofs.y + max_h + theme_cache.v_separation / 2);
						}

						for (int j = i; j >= 0 && col > 0; j--, col--) {
							items.write[j].rect_cache.size.y = max_h;
						}

						ofs.x = 0;
						ofs.y += max_h + theme_cache.v_separation;
						col = 0;
						max_h = 0;
					}
				}

				for (int j = items.size() - 1; j >= 0 && col > 0; j--, col--) {
					items.write[j].rect_cache.size.y = max_h;
				}

				if (all_fit) {
					float page = MAX(0, size.height - theme_cache.panel_style->get_minimum_size().height);
					float max = MAX(page, ofs.y + max_h);
					if (auto_height) {
						auto_height_value = ofs.y + max_h + theme_cache.panel_style->get_minimum_size().height;
					}
					scroll_bar->set_max(max);
					scroll_bar->set_page(page);
					if (max <= page) {
						scroll_bar->set_value(0);
						scroll_bar->hide();
					}
					else {
						scroll_bar->show();

						if (do_autoscroll_to_bottom) {
							scroll_bar->set_value(max);
						}
					}
					break;
				}
			}

			update_minimum_size();
			shape_changed = false;
			*/
		}
		
	}
}
