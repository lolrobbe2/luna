#pragma once
#include <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis 
	{
		class sampler
		{	
		public: 
			operator VkSampler() const { return _sampler; }
			operator const VkSampler*() const { return &_sampler; }
			~sampler();
		protected:
			/**
			 * @brief sampler constructor.
			 * 
			 * \param device reference
			 * \param filters
			 * \param samplerAddressMode
			 */
			friend class device;
			sampler(const VkDevice* p_device, const VkFilter& filters, const VkSamplerAddressMode& samplerAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);
		private:
			const VkDevice* p_device;
			VkSampler _sampler = VK_NULL_HANDLE;
		};
	}
}


