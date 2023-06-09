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
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;
		ref<assets::asset> directoryIcon;
		ref<assets::asset> fileIcon;

	};
}


