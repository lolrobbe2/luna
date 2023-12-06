#pragma once
#include <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis 
	{
		class frameBuffer
		{
		protected:
			frameBuffer();
		private:
			VkFramebuffer m_frameBuffer;
		};
	}
}


