#pragma once
#include <core/assets/publicTypes/image.h>
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
		ref<assets::image> getIcon(const std::filesystem::directory_entry directoryEntry, bool hovered);
		ref<assets::image> getSmallIcon(const std::filesystem::directory_entry directoryEntry, bool hovered);
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
		ref<assets::image> directoryIcon;
		ref<assets::image> directoryHoveredIcon;

		ref<assets::image> pngIcon;
		ref<assets::image> pngHoveredIcon;

		ref<assets::image> jpgIcon;
		ref<assets::image> jpgHoveredIcon;

		ref<assets::image> ttfIcon;
		ref<assets::image> ttfHoveredIcon;

		ref<assets::image> lscnIcon;
		ref<assets::image> lscnHoveredIcon;

		ref<assets::image> fileIcon;
#pragma endregion
#pragma region smallIcons
		ref<assets::image> smallDirectoryIcon;
		ref<assets::image> smallDirectoryHoveredIcon;

		ref<assets::image> smallPngIcon;
		ref<assets::image> smallPngHoveredIcon;

		ref<assets::image> smallJpgIcon;
		ref<assets::image> smallJpgHoveredIcon;

		ref<assets::image> smallTtfIcon;
		ref<assets::image> smallTtfHoveredIcon;

		ref<assets::image> smallLscnIcon;
		ref<assets::image> smallLscnHoveredIcon;

		ref<assets::image> smallFileIcon;
		ref<assets::image> smallFileIconHovered;
#pragma endregion
		std::filesystem::path currentImportSelected;

		bool openPopup = false;

		std::vector<assetDirectory> assetDir;
	};
}


