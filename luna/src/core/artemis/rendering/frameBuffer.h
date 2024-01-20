#pragma once
#include <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis 
	{
		class frameBuffer
		{
		protected:
			frameBuffer(const VkDevice* device);
		private:
			VkFramebuffer m_frameBuffer = VK_NULL_HANDLE;
		};
	}
}


