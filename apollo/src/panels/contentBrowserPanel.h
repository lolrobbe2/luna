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
		ref<assets::asset> getSmallIcon(const std::filesystem::directory_entry directoryEntry, bool hovered);
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
		void setSmallIcons();
		void setSmallGuiIcons();

		void showAssetInfo(const std::string& filename);

		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;
#pragma region normalIcons
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
#pragma endregion
#pragma region smallIcons
		ref<assets::asset> smallDirectoryIcon;
		ref<assets::asset> smallDirectoryHoveredIcon;

		ref<assets::asset> smallPngIcon;
		ref<assets::asset> smallPngHoveredIcon;

		ref<assets::asset> smallJpgIcon;
		ref<assets::asset> smallJpgHoveredIcon;

		ref<assets::asset> smallTtfIcon;
		ref<assets::asset> smallTtfHoveredIcon;

		ref<assets::asset> smallLscnIcon;
		ref<assets::asset> smallLscnHoveredIcon;

		ref<assets::asset> smallFileIcon;
		ref<assets::asset> smallFileIconHovered;
#pragma endregion
		std::filesystem::path currentImportSelected;

		bool openPopup = false;

		std::vector<assetDirectory> assetDir;
	};
}


