#include "contentBrowserPanel.h"
#include <core/vulkan/rendering/vulkanTexture.h>
namespace luna
{
	contentBrowserPanel::contentBrowserPanel()
	{
		m_BaseDirectory = std::filesystem::current_path();
		m_CurrentDirectory = m_BaseDirectory;
		assets::assetManager::importAsset("src/resources/directoryIcon.png",assets::texture);
		assets::assetManager::importAsset("src/resources/fileIcon.png", assets::texture);

		directoryIcon = assets::assetManager::getAsset("directoryIcon.png");
		fileIcon = assets::assetManager::getAsset("fileIcon.png");
		
		std::dynamic_pointer_cast<vulkan::vulkanTexture>(directoryIcon)->createGuiImage();
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
				const auto& path = directoryEntry.path();
				std::string filenameString = path.filename().string();

				ImGui::PushID(filenameString.c_str());

				ref<vulkan::vulkanTexture> icon = directoryEntry.is_directory() ? std::dynamic_pointer_cast<vulkan::vulkanTexture>(directoryIcon) : std::dynamic_pointer_cast<vulkan::vulkanTexture>(fileIcon);


				ImGui::ImageButton(icon->getGuiImageHandle(), {thumbnailSize, thumbnailSize}, {1, 0}, {0, 1});
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
}
