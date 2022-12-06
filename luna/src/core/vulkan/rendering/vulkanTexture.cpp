#include "vulkanTexture.h"
#include <core/vulkan/utils/vulkanAllocator.h>
#include <lnpch.h>
namespace luna
{
	namespace vulkan
	{
		vulkanTexture::vulkanTexture(const std::string& filePath)
		{
			std::ifstream textureFile(filePath);
			if(textureFile.is_open() && textureFile.good())
			{
				int width, height, channels;
				stbi_uc* image =  stbi_load(filePath.c_str(), &width, &height, &channels, 4);
				utils::vulkanAllocator::createBuffer(&buffer,sizeof(*image), VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT, VMA_MEMORY_USAGE_GPU_ONLY, VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_WITHIN_BUDGET_BIT);
				utils::vulkanAllocator::createImage(&imageHandle, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT, VMA_MEMORY_USAGE_GPU_ONLY,{(unsigned int)width,(unsigned int)height,0},VK_FORMAT_R8G8B8A8_SRGB);
				//read file
				return;
			}
			LN_CORE_CRITICAL("could not open texture file at: {0}", filePath);
			return;
		}
		vulkanTexture::vulkanTexture(void* texelData, const glm::vec2& dimensions)
		{
			utils::vulkanAllocator::createBuffer(&buffer, dimensions.x * dimensions.y, VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT, VMA_MEMORY_USAGE_GPU_ONLY, VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_WITHIN_BUDGET_BIT);
			utils::vulkanAllocator::createImage(&imageHandle, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT, VMA_MEMORY_USAGE_GPU_ONLY, { (unsigned int)width,(unsigned int)height,0 }, VK_FORMAT_R8G8B8A8_SRGB);
			setData(texelData,dimensions.x * dimensions.y * 4);
			LN_CORE_INFO("not fully implemented");
		}
		uint32_t vulkanTexture::getWidth() const
		{
			return width;
		}
		uint32_t vulkanTexture::getHeight() const
		{
			return height;
		}
		const std::string& vulkanTexture::getPath() const
		{
			// TODO: insert return statement here
			return "";
		}
		void vulkanTexture::setData(void* data, uint32_t size)
		{

		}
		void vulkanTexture::bind(uint32_t slot) const
		{

		}
		bool vulkanTexture::isLoaded() const
		{
			return false;
		}
	}
}
