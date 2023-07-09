#include "contentBrowserPanel.h"
#include <core/vulkan/rendering/vulkanTexture.h>
#include <core/platform/platformUtils.h>
#include <project/projectManager.h>
namespace luna
{
	std::map<std::string, bool> selectedMap;

	std::unordered_map<std::string,assets::assetType> allowedImportExtensions = {
		{".png",assets::texture},
		{".jpg",assets::texture},
		{".bmp",assets::texture},
		{".ttf",assets::font}
	};

	const char* assetTypeToString[] = {
	stringify(NONE),
	stringify(TEXTURE),
	stringify(FONT),
	};

	std::filesystem::path importPath;

	contentBrowserPanel::contentBrowserPanel()
	{
		m_BaseDirectory = std::filesystem::current_path();
		m_CurrentDirectory = m_BaseDirectory;

		loadNormalIcons();
		setNormalIcons();
		setNormalGuiIcons();

		loadSmallIcons();
		setSmallIcons();
		setSmallGuiIcons();

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
			ImGui::OpenPopup("asset manager");
			if (ImGui::BeginPopupModal("asset manager", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowSize(ImVec2(750, 375));
				ImVec2 windowPos = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetWindowPos({ windowPos.x - ImGui::GetWindowSize().x / 2, windowPos.y - ImGui::GetWindowSize().y / 2 });
				
				if (!assets::assetManager::isAssetHandleValid(importPath.filename().string())) {
					importPopup(openPopup);
				}
				else {
					showAssetInfo(importPath.filename().string());
				}
				ImGui::EndPopup();
			}
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

	ref<assets::asset> contentBrowserPanel::getSmallIcon(const std::filesystem::directory_entry directoryEntry, bool hovered)
	{
		if (directoryEntry.is_directory())
		{
			if (hovered) return smallDirectoryHoveredIcon;
			return smallDirectoryIcon;
		}
		else if (directoryEntry.path().extension().string() == ".png") {
			if (hovered) return  smallPngHoveredIcon;
			return smallPngIcon;
		}
		else if (directoryEntry.path().extension().string() == ".jpg") {
			if (hovered) return  smallJpgHoveredIcon;
			return smallJpgIcon;
		}
		else if (directoryEntry.path().extension().string() == ".ttf") {
			if (hovered) return  smallTtfHoveredIcon;
			return smallTtfIcon;
		}
		else if (directoryEntry.path().extension().string() == ".lscn") {
			if (hovered) return  smallLscnHoveredIcon;
			return smallLscnIcon;
		}
		if (hovered) return smallFileIconHovered;
		return smallFileIcon;
	}

	void contentBrowserPanel::importPopup(bool& openPopup)
	{
			ImGui::Text("would you like to import: %S" , importPath.filename().c_str());

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
				auto assetType = allowedImportExtensions.find(importPath.extension().string());
				if (assetType != allowedImportExtensions.end()) assets::assetManager::importAsset(importPath.string(), assetType->second);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetWindowFontScale(1);

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
					else if (allowedImportExtensions.find(directoryEntry.path().extension().string()) != allowedImportExtensions.end()) {
						openPopup = true;
						importPath = directoryEntry.path();
					}

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
		const ref<renderer::texture> icon = std::dynamic_pointer_cast<renderer::texture>(getSmallIcon(directoryEntry.entry,directoryEntry.hovered));

		
		
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

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (allowedImportExtensions.find(directoryEntry.entry.path().extension().string()) != allowedImportExtensions.end()) {
				openPopup = true;
				importPath = directoryEntry.entry.path();
			}
		}

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

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (allowedImportExtensions.find(directoryEntry.entry.path().extension().string()) != allowedImportExtensions.end()) {
				openPopup = true;
				importPath = directoryEntry.entry.path();
			}
		}

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
		assets::assetManager::setImportDirectory(platform::filesystem::getEngineRootPath());
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

		assets::assetManager::setImportDirectory("");
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
		assets::assetManager::importAsset("src/resources/small_icons/small_icon_png.png", assets::texture);
		assets::assetManager::importAsset("src/resources/small_icons/small_icon_png_hovered.png", assets::texture);

		assets::assetManager::importAsset("src/resources/small_icons/small_icon_jpg.png", assets::texture);
		assets::assetManager::importAsset("src/resources/small_icons/small_icon_jpg_hovered.png", assets::texture);

		assets::assetManager::importAsset("src/resources/small_icons/small_icon_ttf.png", assets::texture);
		assets::assetManager::importAsset("src/resources/small_icons/small_icon_ttf_hovered.png", assets::texture);

		assets::assetManager::importAsset("src/resources/small_icons/small_icon_lscn.png", assets::texture);
		assets::assetManager::importAsset("src/resources/small_icons/small_icon_lscn_hovered.png", assets::texture);

		assets::assetManager::importAsset("src/resources/small_icons/small_fileIcon.png", assets::texture);
		assets::assetManager::importAsset("src/resources/small_icons/small_fileIcon_hovered.png", assets::texture);
	}

	void contentBrowserPanel::setSmallIcons()
	{
		smallDirectoryIcon = assets::assetManager::getAsset("directoryIcon.png");
		smallDirectoryHoveredIcon = assets::assetManager::getAsset("directoryIconHovered.png");

		smallPngIcon = assets::assetManager::getAsset("small_icon_png.png");
		smallPngHoveredIcon = assets::assetManager::getAsset("small_icon_png_hovered.png");

		smallJpgIcon = assets::assetManager::getAsset("small_icon_jpg.png");
		smallJpgHoveredIcon = assets::assetManager::getAsset("small_icon_jpg_hovered.png");

		smallTtfIcon = assets::assetManager::getAsset("small_icon_ttf.png");
		smallTtfHoveredIcon = assets::assetManager::getAsset("small_icon_ttf_hovered.png");

		smallLscnIcon = assets::assetManager::getAsset("small_icon_lscn.png");
		smallLscnHoveredIcon = assets::assetManager::getAsset("small_icon_lscn_hovered.png");

		smallFileIcon = assets::assetManager::getAsset("small_fileIcon.png");
		smallFileIconHovered = assets::assetManager::getAsset("small_fileIcon_hovered.png");
	}
	void contentBrowserPanel::setSmallGuiIcons()
	{
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(smallDirectoryIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(smallDirectoryHoveredIcon)->createGuiImage();

		std::dynamic_pointer_cast<vulkan::vulkanTexture>(smallPngIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(smallPngHoveredIcon)->createGuiImage();

		std::dynamic_pointer_cast<vulkan::vulkanTexture>(smallJpgIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(smallJpgHoveredIcon)->createGuiImage();

		std::dynamic_pointer_cast<vulkan::vulkanTexture>(smallTtfIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(smallTtfHoveredIcon)->createGuiImage();

		std::dynamic_pointer_cast<vulkan::vulkanTexture>(smallLscnIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(smallLscnHoveredIcon)->createGuiImage();

		std::dynamic_pointer_cast<vulkan::vulkanTexture>(smallFileIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(smallFileIconHovered)->createGuiImage();
	}
	void contentBrowserPanel::showAssetInfo(const std::string& filename)
	{
		assets::assetMetadata* metaData = assets::assetManager::getAssetMetadata(filename);
		std::string sizeText;
#pragma region basicMetadata
		if (ImGui::BeginTable(filename.c_str(), 2, ImGuiTableFlags_Borders))
		{
			ImGui::SetWindowFontScale(1.2);

			ImGui::TableNextColumn();
			ImGui::TableHeader("type");
			ImGui::TableNextColumn();
			ImGui::TableHeader("value");
			ImGui::TableNextColumn();
			ImGui::Text("handle");
			ImGui::TableNextColumn();
			ImGui::Text(std::to_string(metaData->handle).c_str());
			ImGui::TableNextColumn();
			ImGui::Text("name");
			ImGui::TableNextColumn();
			ImGui::Text(metaData->name);
			ImGui::TableNextColumn();
			ImGui::Text("filepath");
			ImGui::TableNextColumn();
			ImGui::Text(metaData->filePath);
			ImGui::TableNextColumn();
			ImGui::Text("asset-type");
			ImGui::TableNextColumn();
			ImGui::Text(assetTypeToString[metaData->assetType]);
			ImGui::TableNextColumn();
			ImGui::Text("size on disk");
			ImGui::TableNextColumn();
			uint64_t size = metaData->fileSizeBytes;
			float adjustedSize;
			if (size < 1000) 
			{
				adjustedSize = size;
				sizeText = "%f B";
			}
			else if (size > 1000 && size < 1000000) 
			{
				adjustedSize = (float)size / 1000.0f;
				sizeText = "%.2f ";
				sizeText += " KB";
			}
			else if (size > 1000000 && size < 1000000000)
			{
				adjustedSize = (float)size / 1000000.0f;
				sizeText = "%.2f ";
				sizeText += " MB";
			} 
			else
			{
				adjustedSize = (float)size / 1000000000.0f;
				sizeText = "%.2f ";
				sizeText += " GB";
			}
			ImGui::Text(sizeText.c_str(),adjustedSize);


			ImGui::EndTable();
		}
#pragma endregion	
#pragma region specificData
		if (ImGui::BeginTable(filename.c_str(), 2, ImGuiTableFlags_Borders))
		{
			ImGui::SetWindowFontScale(1.2);

			ImGui::TableNextColumn();
			ImGui::TableHeader("type");
			ImGui::TableNextColumn();
			ImGui::TableHeader("value");
			ImGui::TableNextColumn();
			switch (metaData->assetType)
			{
			case assets::texture:
			{
				assets::textureAssetMetadata* textureMetadata = (assets::textureAssetMetadata*)metaData;
				ImGui::Text("width");
				ImGui::TableNextColumn();
				ImGui::Text(std::to_string(textureMetadata->width).c_str());
				ImGui::TableNextColumn();
				ImGui::Text("height");
				ImGui::TableNextColumn();
				ImGui::Text(std::to_string(textureMetadata->height).c_str());
				ImGui::TableNextColumn();
				ImGui::Text("channels");
				ImGui::TableNextColumn();
				ImGui::Text(std::to_string(textureMetadata->channels).c_str());
				ImGui::TableNextColumn();
				ImGui::Text("imageDataSize (uncompressed)");
				ImGui::TableNextColumn();
				uint64_t size = textureMetadata->imageSize;
				float adjustedSize;
				if (size < 1000)
				{
					adjustedSize = size;
					sizeText = "%f B";
					
				}
				else if (size > 1000 && size < 1000000)
				{
					adjustedSize = (float)size / 1000.0f;
					sizeText = "%.2f ";
					sizeText += " KB";
				}
				else if (size > 1000000 && size < 1000000000)
				{
					adjustedSize = (float)size / 1000000.0f;
					sizeText = "%.2f ";
					sizeText += " MB";
				}
				else
				{
					adjustedSize = (float)size / 1000000000.0f;
					sizeText = "%.2f ";
					sizeText += " GB";
				}
				ImGui::Text(sizeText.c_str(), adjustedSize);
			}
			case assets::font:
			{
				assets::fontAssetMetadata* fontMetadata = (assets::fontAssetMetadata*)metaData;
				ImGui::Text("width");
				ImGui::TableNextColumn();
				ImGui::Text(std::to_string(fontMetadata->width).c_str());
				ImGui::TableNextColumn();
				ImGui::Text("height");
				ImGui::TableNextColumn();
				ImGui::Text(std::to_string(fontMetadata->height).c_str());
			}
			default:
				break;
			}
			ImGui::EndTable();
			if (ImGui::Button("exit",ImVec2(730,20))) {
				openPopup = false;
			}
		}
#pragma endregion 
		ImGui::SetWindowFontScale(1);

	}
}
