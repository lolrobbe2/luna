#pragma once
#include <core/vulkan/window/window.h>


namespace luna
{
	namespace renderer
	{
		/**
		 * @brief the device class is the code representation of the physical device or GPU.
		 * @code
		 *  ref<device> device = std::make_shared(new device());
		 * @endcode
		 */
		class device
		{
		public:
			virtual ~device() {};
			virtual void createContext() = 0;
			virtual void destroyContext()  = 0;
			/**
			 * @brief this is the reference to the main window.
			 */
			ref<vulkan::window> window;
		private:
			
			
		};
	}
}
