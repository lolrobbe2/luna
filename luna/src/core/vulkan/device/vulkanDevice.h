#pragma once
#include <core/rendering/device.h>
#include <vulkan/vulkan.h>
namespace luna
{
	namespace renderer
	{
		class vulkanDevice : public device
		{
		public:
			vulkanDevice();
			virtual ~vulkanDevice();
			void createContext() override;
			void destroyContext() override;
		private:
			VkResult createInstance();
			/*helper helper functions*/
			VkResult checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
			const std::vector<const char*>& getRequiredExtensions();

		private:
			
			struct deviceHandles
			{
				VkApplicationInfo appInfo{};
				VkInstance instance;
				VkDevice device;
			};

			deviceHandles deviceHandle;
		};
	}
}


