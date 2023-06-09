#include "contentBrowserPanel.h"
#include <core/vulkan/rendering/vulkanTexture.h>
namespace luna
{

	contentBrowserPanel::contentBrowserPanel()
	{
		m_BaseDirectory = std::filesystem::current_path();
		m_CurrentDirectory = m_BaseDirectory;
		assets::assetManager::importAsset("src/resources/directoryIcon.png",assets::texture);
		assets::assetManager::importAsset("src/resources/directoryIconHovered.png", assets::texture);
		
		assets::assetManager::importAsset("src/resources/icon_png.png", assets::texture);
		assets::assetManager::importAsset("src/resources/icon_png_hovered.png", assets::texture);
		
		assets::assetManager::importAsset("src/resources/icon_ttf.png", assets::texture);
		assets::assetManager::importAsset("src/resources/icon_ttf_hovered.png", assets::texture);

		assets::assetManager::importAsset("src/resources/fileIcon.png", assets::texture);


		directoryIcon = assets::assetManager::getAsset("directoryIcon.png");
		directoryHoveredIcon = assets::assetManager::getAsset("directoryIconHovered.png");
		
		pngIcon = assets::assetManager::getAsset("icon_png.png");
		pngHoveredIcon = assets::assetManager::getAsset("icon_png_hovered.png");

		ttfIcon = assets::assetManager::getAsset("icon_ttf.png");
		ttfHoveredIcon = assets::assetManager::getAsset("icon_ttf_hovered.png");
		
		fileIcon = assets::assetManager::getAsset("fileIcon.png");
		
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(directoryIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(directoryHoveredIcon)->createGuiImage();

		std::dynamic_pointer_cast<vulkan::vulkanTexture>(pngIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(pngHoveredIcon)->createGuiImage();

		std::dynamic_pointer_cast<vulkan::vulkanTexture>(ttfIcon)->createGuiImage();
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(ttfHoveredIcon)->createGuiImage();
		
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(fileIcon)->createGuiImage();
	}
	void contentBrowserPanel::onImGuiRender()
	{
		if (ImGui::Begin("content browser"))
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

				ImGui::PushID(filenameString.c_str());

				ref<vulkan::vulkanTexture> icon = std::dynamic_pointer_cast<vulkan::vulkanTexture>(getIcon(directoryEntry, hovered == filenameString));


				ImGui::ImageButton(icon->getGuiImageHandle(), {thumbnailSize, thumbnailSize}, {0, 0}, {1, 1});
				if (ImGui::IsItemHovered()) 
					hovered = filenameString;
				else if (hovered == filenameString)
					hovered = "";

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (directoryEntry.is_directory())
						m_CurrentDirectory /= path.filename();

				}
				ImGui::TextWrapped(filenameString.c_str());

				ImGui::NextColumn();

				ImGui::PopID();
			}
		}
		ImGui::End();
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
		else if (directoryEntry.path().extension().string() == ".ttf") {
			if (hovered) return  ttfHoveredIcon;
			return ttfIcon;
		}
		return fileIcon;
	}
}
