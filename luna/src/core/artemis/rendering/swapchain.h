#pragma once
#include <core/platform/windows/windowsWindow.h>

namespace luna 
{
	namespace artemis 
	{
		class swapchain
		{
		public:
			void resize(uint32_t width, uint32_t height);
			~swapchain() { vkb::destroy_swapchain(m_swapchain); }
		protected:
			friend class device;
			swapchain(const vkb::Device* device, uint32_t width, uint32_t height, uint32_t imageCount);
		private:
			vkb::Swapchain m_swapchain;
			const vkb::Device* device;
		};
	}
}


