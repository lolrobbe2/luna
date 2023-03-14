#pragma once
#include <nodes/controlNodes/controlNode.h>
namespace luna
{
	namespace nodes
	{
		class itemListNode : public controlNode
		{
			itemListNode() = default;
			virtual ~itemListNode() = default;

			itemListNode(entt::entity handle, luna::scene* scene);
			itemListNode(luna::scene* scene);
			virtual void init(luna::scene* scene) override;

			int addItem(const std::string& pItem, const ref<renderer::texture>& pTexture = ref<renderer::texture>(), bool selectable = true);
			int addIconItem(const ref<renderer::texture>& pItem, bool selectable = true);


			void setItemText(int pIdx, const std::string& text);
			std::string getItemText(int pIdx);

			void setItemIcon(int pIdx, const ref<renderer::texture>& pIcon);

			ref<renderer::texture> getItemIcon(int p_idx);

			glm::vec2 getItemIconRegion(int pIdx);

			void setItemLanguage(int pIdx, const std::string& language);
			std::string getItemLanguage(int pIdx);

			void select(int Idx, bool single = true);
			void deselect(int pIdx);
			void deselect_all();
			bool isSelected(int pIdx);

			void setCurrent(int current);
			int getCurrent();

			void moveItem(int fromIdx, int toIdx);

			void setItemCount(int count);

			int getItemCount() const;

			void removeItem(int idx);

			void setItemCustomBgColor(int pIdx, const glm::vec4& pCustomBgColor);
			glm::vec4 getItemCustomBgColor(int pIdx);

			void setItemCustomFgColor(int pIdx, const glm::vec4& pCustomFgColor);
			glm::vec4 getItemCustomFgColor(int pIdx);

			void setItemTagIcon(int p_idx, const ref<renderer::texture>& pTagIcon);

			void setItemSelectable(int pIdx, bool selectable);

			bool isItemSelectable(int pIdx);

			void setItemDisabled(int pIdx, bool disabled);

			bool isItemDisabled(int pIdx);

			void clear();

			std::vector<int> getSelectedItems();

			bool isAnythingSelected();

			void shape(int p_idx);
		};
	}
}

