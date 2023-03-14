#pragma once
#include <nodes/controlNodes/controlNode.h>
namespace luna
{
	namespace nodes
	{
		class itemListNode : public controlNode
		{
			int addItem(const std::string& pItem, const ref<renderer::texture>& pTexture = ref<renderer::texture>(), bool selectable = true);
			int addIconItem(const ref<renderer::texture>& pItem, bool selectable = true);

			void setItemtText(int pIdx, const std::string& text);
			std::string getItemText(int pIdx) const;

			void setItemLanguage(int pIdx, const std::string& language);
			std::string getItemLanguage(int pIdx) const;

			void select(int Idx, bool single = true);
			void deselect(int pIdx);
			void deselect_all();
			bool isSelected(int pIdx) const;

			void setCurrent(int current);
			int getCurrent() const;

			void moveItem(int fromIdx, int toIdx);

			void setItemCount(int count);
			int getItemCount() const;
			void removeItem(int idx);

			void clear();
		};
	}
}

