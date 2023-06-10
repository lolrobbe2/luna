#pragma once
#include <core/assets/assetManager.h>

namespace luna 
{
	struct assetDirectory
	{
	public:
		bool open = false;
		bool hovered = false;
		std::filesystem::directory_entry entry;
		std::vector<assetDirectory> childDir;
	};
	class contentBrowserPanel
	{

	public:
		contentBrowserPanel();
		void onImGuiRender();
	private:
		ref<assets::asset> getIcon(const std::filesystem::directory_entry directoryEntry, bool hovered);

		void importPopup(bool& openPopup);

		void largeIcons();
		void smallIcons();
		void smallIconsDir(const std::filesystem::path& dir);

		bool button(assetDirectory& directoryEntry, int indent = 0);

		void initAssetDir();
		void initChildAssetDir(const std::filesystem::path& childDirPath,assetDirectory& dir);

		void loadNormalIcons();
		void setNormalIcons();
		void setNormalGuiIcons();

		void loadSmallIcons();

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

		std::filesystem::path currentImportSelected;

		bool openPopup = false;

		std::vector<assetDirectory> assetDir;
	};
}


