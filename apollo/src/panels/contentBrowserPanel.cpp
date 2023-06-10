#include "contentBrowserPanel.h"
#include <core/vulkan/rendering/vulkanTexture.h>
namespace luna
{
	std::map<std::string, bool> selectedMap;

	contentBrowserPanel::contentBrowserPanel()
	{
		m_BaseDirectory = std::filesystem::current_path();
		m_CurrentDirectory = m_BaseDirectory;

		loadNormalIcons();
		setNormalIcons();
		setNormalGuiIcons();

		initAssetDir();
	}
	void contentBrowserPanel::onImGuiRender()
	{
		

		if (ImGui::Begin("content browser"))
		{
			
			if(ImGui::GetWindowWidth() > 530) largeIcons();
			else smallIcons();
		}
		ImGui::End();
		if (openPopup) {
			ImGui::OpenPopup("import asset");
			importPopup(openPopup);
		}
	}
	ref<assets::asset> contentBrowserPanel::getIcon(const std::filesystem::directory_entry directoryEntry, bool hovered)
	{
		if (directoryEntry.is_directory())
		{
			if (hovered) return directoryHoveredIcon;
			return directoryIcon;
		}
		else if (directoryEntry.path().extension().string() == ".png") {
			if (hovered) return  pngHoveredIcon;
			return pngIcon;
		}
		else if (directoryEntry.path().extension().string() == ".jpg") {
			if (hovered) return  jpgHoveredIcon;
			return jpgIcon;
		}
		else if (directoryEntry.path().extension().string() == ".ttf") {
			if (hovered) return  ttfHoveredIcon;
			return ttfIcon;
		}
		else if (directoryEntry.path().extension().string() == ".lscn") {
			if (hovered) return  lscnHoveredIcon;
			return lscnIcon;
		}
		return fileIcon;
	}

	void contentBrowserPanel::importPopup(bool& openPopup)
	{
		if (ImGui::BeginPopupModal("import asset", nullptr, ImGuiWindowFlags_NoMove))
		{
			ImGui::SetWindowSize(ImVec2(750, 375));
			ImVec2 windowPos = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetWindowPos({ windowPos.x - ImGui::GetWindowSize().x / 2, windowPos.y - ImGui::GetWindowSize().y / 2 });
			ImGui::SetCursorPos(ImVec2(20, 315));
			ImGui::SetWindowFontScale(2);
			if (ImGui::Button("no", ImVec2(200, 40)))
			{
				openPopup = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 210);
			if (ImGui::Button("import asset", ImVec2(200, 40)))
			{
				//LN_CORE_INFO("node added: {0}",m_ListSelected);
				openPopup = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetWindowFontScale(1);
			ImGui::EndPopup();
		}
	}

	void contentBrowserPanel::largeIcons() 
	{
		if (m_CurrentDirectory != m_BaseDirectory)
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{


			static std::string hovered;
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			if ((GetFileAttributesA(path.string().c_str()) & FILE_ATTRIBUTE_HIDDEN) == 0) {   //TODO make multi platform.
				//ImGui::PushID(filenameString.c_str());
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
				ref<vulkan::vulkanTexture> icon = std::dynamic_pointer_cast<vulkan::vulkanTexture>(getIcon(directoryEntry, hovered == filenameString));


				ImGui::ImageButton(icon->getGuiImageHandle(), { thumbnailSize, thumbnailSize }, { 0, 0 }, { 1, 1 });
				ImGui::PopStyleColor(1);
				ImGui::PopStyleColor(1);
				if (ImGui::IsItemHovered())
					hovered = filenameString;
				else if (hovered == filenameString)
					hovered = "";

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (directoryEntry.is_directory())
						m_CurrentDirectory /= path.filename();
					else openPopup = true;

				}
				ImGui::TextWrapped(filenameString.c_str());

				ImGui::NextColumn();
			}

		}
	}
	void contentBrowserPanel::smallIcons()
	{
		for (auto& directoryEntry : assetDir)
		{
			button(directoryEntry);
		}
	}
	void contentBrowserPanel::smallIconsDir(const std::filesystem::path& dir)
	{
		ImGui::Indent(10);
		for (auto& directoryEntry : assetDir)
		{
			button(directoryEntry);
		}
		ImGui::Unindent(10);

	}

	bool contentBrowserPanel::button(assetDirectory& directoryEntry,int indent)
	{
		const ref<renderer::texture> icon = std::dynamic_pointer_cast<renderer::texture>(getIcon(directoryEntry.entry,directoryEntry.hovered));

		
		
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

		if (directoryEntry.hovered) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.259, 0.588, 0.98,1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.259, 0.588, 0.98,1.0f });
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.059f, 0.059f, 0.059f,1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.059f, 0.059f, 0.059f,1.0f });
		}
		if (ImGui::ImageButton(icon->getGuiImageHandle(), { 40,40 })) {
			directoryEntry.open = !directoryEntry.open;
		}

		bool hovered;
		if (ImGui::IsItemHovered()) hovered = true;
		else hovered = false;

		ImGui::SameLine(indent+55);

		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2{ 0.0f,0.5f });
		if (ImGui::Button(directoryEntry.entry.path().filename().string().c_str(),{ImGui::GetWindowWidth(),45})) {
			directoryEntry.open = !directoryEntry.open;
		}
		ImGui::PopStyleVar();

		if (ImGui::IsItemHovered() || hovered) {
			directoryEntry.hovered = true;
		}
		else directoryEntry.hovered = false;
		ImGui::PopStyleColor(2);
		if(directoryEntry.open)
		{
			ImGui::Indent(30);
			for (auto& childDirectoryEntry : directoryEntry.childDir)
			{
				button(childDirectoryEntry,indent+30);
			}
			ImGui::Unindent(30);
		}
		return false;
	}
	void contentBrowserPanel::initAssetDir()
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_BaseDirectory))
		{
			if ((GetFileAttributesA(directoryEntry.path().string().c_str()) & FILE_ATTRIBUTE_HIDDEN) == 0) {
				assetDirectory dir;
				if (directoryEntry.is_directory()) initChildAssetDir(directoryEntry.path(), dir);
				dir.entry = directoryEntry;
				assetDir.push_back(dir);
			}
		}
	}
	void contentBrowserPanel::initChildAssetDir(const std::filesystem::path& childDirPath,assetDirectory& parentDir)
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(childDirPath))
		{
			if ((GetFileAttributesA(directoryEntry.path().string().c_str()) & FILE_ATTRIBUTE_HIDDEN) == 0) {
				assetDirectory dir;
				if (directoryEntry.is_directory()) initChildAssetDir(directoryEntry.path(),dir);
				dir.entry = directoryEntry;
				parentDir.childDir.push_back(dir);
			}
		}
	}

	void contentBrowserPanel::loadNormalIcons()
	{

		assets::assetManager::importAsset("src/resources/normal/directoryIcon.png", assets::texture);
		assets::assetManager::importAsset("src/resources/normal/directoryIconHovered.png", assets::texture);

		assets::assetManager::importAsset("src/resources/normal/icon_png.png", assets::texture);
		assets::assetManager::importAsset("src/resources/normal/icon_png_hovered.png", assets::texture);

		assets::assetManager::importAsset("src/resources/normal/icon_jpg.png", assets::texture);
		assets::assetManager::importAsset("src/resources/normal/icon_jpg_hovered.png", assets::texture);

		assets::assetManager::importAsset("src/resources/normal/icon_ttf.png", assets::texture);
		assets::assetManager::importAsset("src/resources/normal/icon_ttf_hovered.png", assets::texture);

		assets::assetManager::importAsset("src/resources/normal/icon_lscn.png", assets::texture);
		assets::assetManager::importAsset("src/resources/normal/icon_lscn_hovered.png", assets::texture);

		assets::assetManager::importAsset("src/resources/normal/fileIcon.png", assets::texture);


	}

	void contentBrowserPanel::setNormalIcons()
	{
		directoryIcon = assets::assetManager::getAsset("directoryIcon.png");
		directoryHoveredIcon = assets::assetManager::getAsset("directoryIconHovered.png");

		pngIcon = assets::assetManager::getAsset("icon_png.png");
		pngHoveredIcon = assets::assetManager::getAsset("icon_png_hovered.png");

		jpgIcon = assets::assetManager::getAsset("icon_jpg.png");
		jpgHoveredIcon = assets::assetManager::getAsset("icon_jpg_hovered.png");

		ttfIcon = assets::assetManager::getAsset("icon_ttf.png");
		ttfHoveredIcon = assets::assetManager::getAsset("icon_ttf_hovered.png");

		lscnIcon = assets::assetManager::getAsset("icon_lscn.png");
		lscnHoveredIcon = assets::assetManager::getAsset("icon_lscn_hovered.png");

		fileIcon = assets::assetManager::getAsset("fileIcon.png");
	}

	void contentBrowserPanel::setNormalGuiIcons()
	{
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(directoryIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(directoryHoveredIcon)->createGuiImage();

		std::dynamic_pointer_cast<vulkan::vulkanTexture>(pngIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(pngHoveredIcon)->createGuiImage();

		std::dynamic_pointer_cast<vulkan::vulkanTexture>(jpgIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(jpgHoveredIcon)->createGuiImage();

		std::dynamic_pointer_cast<vulkan::vulkanTexture>(ttfIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(ttfHoveredIcon)->createGuiImage();

		std::dynamic_pointer_cast<vulkan::vulkanTexture>(lscnIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(lscnHoveredIcon)->createGuiImage();

		std::dynamic_pointer_cast<vulkan::vulkanTexture>(fileIcon)->createGuiImage();
	}

	void contentBrowserPanel::loadSmallIcons()
	{

	}
}
