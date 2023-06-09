#pragma once
#include <core/assets/assetManager.h>

namespace luna 
{
	class contentBrowserPanel
	{
	public:
		contentBrowserPanel();
		void onImGuiRender();
	private:
		ref<assets::asset> getIcon(const std::filesystem::directory_entry directoryEntry, bool hovered);

		void importPopup(bool& openPopup);


		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;
		ref<assets::asset> directoryIcon;
		ref<assets::asset> directoryHoveredIcon;

		ref<assets::asset> pngIcon;
		ref<assets::asset> pngHoveredIcon;

		ref<assets::asset> jpgIcon;
		ref<assets::asset> jpgHoveredIcon;

		ref<assets::asset> ttfIcon;
		ref<assets::asset> ttfHoveredIcon;

		ref<assets::asset> lscnIcon;
		ref<assets::asset> lscnHoveredIcon;

		ref<assets::asset> fileIcon;

	};
}


