#pragma once
#include <core/vulkan/window/window.h>
#include <core/vulkan/device/vulkanDevice.h>
#include <core/rendering/shader.h>
namespace luna 
{
	namespace utils
	{
		/**
		 * @brief vulkan descriptor pool and descriptorset wrapper class.
		 */
		class vulkanDescriptorPool
		{
		public: 
			/**
			 * @brief descroptor pool constructor.
			 * 
			 */
			vulkanDescriptorPool(const ref<vulkan::vulkanDevice>& pDevice,VkDescriptorPoolSize* pPoolSizes, const uint32_t& poolSizeCountconst,const std::vector<renderer::shaderResource>& shaderLayout, VkDescriptorSetLayoutCreateFlags flags);
			virtual ~vulkanDescriptorPool() = default;
			VkResult createDescriptorSets(VkDescriptorSet* pDescriptorSets,uint64_t amount);
		private:
			VkDescriptorSetLayout* createDescriptorLayouts(const std::vector<renderer::shaderResource>& shaderLayout, VkDescriptorSetLayoutCreateFlags flags);
			ref<vulkan::vulkanDevice> device;
			VkDescriptorPool descriptorPool;
		};

		/**
		 * @brief wrapper class around VkDescriptorSet.
		 */
		class vulkanDescriptorSet 
		{
		public:
			vulkanDescriptorSet(const ref<vulkan::vulkanDevice>& device,const VkDescriptorSet& descriptoSet, const VkDescriptorPool& descriptorPool,const VkWriteDescriptorSet& descriptorWrite);

			void destroy();
			VkResult write();
		private:
			VkDescriptorSet descriptorSet;
			VkDescriptorPool descriptorPool; //the pool the descriptorSet belongs to.
			VkWriteDescriptorSet descriptorWrite;
			ref<vulkan::vulkanDevice> device;
		};
	}
}


