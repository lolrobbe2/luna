#pragma once
#include <core/vulkan/window/window.h>
#include <core/vulkan/device/vulkanDevice.h>
#include <core/rendering/shader.h>
namespace luna 
{
	namespace utils
	{
		/**
		 * @brief wrapper class around VkDescriptorSet.
		 */
		class vulkanDescriptorSet
		{
		public:
			vulkanDescriptorSet(const ref<vulkan::vulkanDevice>& device, const VkDescriptorSet& descriptoSet, const VkDescriptorPool& descriptorPool, const std::vector<VkWriteDescriptorSet>& descriptorWrite);
			~vulkanDescriptorSet() { destroy(); };
			void destroy();
			VkResult write(const uint32_t& descriptorIndex, void* pDescriptorInfo);
			VkDescriptorSet* handle() { return &descriptorSet; };
			VkDescriptorSet descriptorSet;
		private:
			VkDescriptorPool descriptorPool; //the pool the descriptorSet belongs to.
			std::vector<VkWriteDescriptorSet> descriptorWrites;
			ref<vulkan::vulkanDevice> device;
		};


		/**
		 * @brief vulkan descriptor pool and descriptorset wrapper class.
		 */
		class vulkanDescriptorPool
		{
		public: 
			/**
			 * @brief descriptor pool constructor.
			 * 
			 */
			vulkanDescriptorPool(const ref<vulkan::vulkanDevice>& pDevice,VkDescriptorPoolSize* pPoolSizes, const uint32_t& poolSizeCountconst,const std::vector<renderer::shaderResource>& shaderLayout, VkDescriptorSetLayoutCreateFlags flags);
			virtual ~vulkanDescriptorPool();
			VkResult createDescriptorSets(std::vector<ref<vulkanDescriptorSet>>& descriptorSets);
			VkDescriptorSetLayout getLayout() { return layout; };
			VkDescriptorSetLayout* getLayoutPtr() { return &layout; };
		private:
			VkDescriptorSetLayout createDescriptorLayouts(const std::vector<renderer::shaderResource>& shaderLayout, VkDescriptorSetLayoutCreateFlags flags);
			void createDescriptorWrites(const std::vector<renderer::shaderResource>& shaderLayout);
			ref<vulkan::vulkanDevice> device;
			VkDescriptorPool descriptorPool;
			VkDescriptorSetLayout layout;
			std::vector<VkWriteDescriptorSet> descriptorWrites;
			
		};


	}
}


