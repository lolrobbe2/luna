#include "allocator.h"
#include <core/utils/objectStorage.h>
#include <vk_mem_alloc.h>
#include <core/debug/debugMacros.h>

namespace luna 
{
	namespace artemis 
	{
		
		struct vmaAllocation
		{
			VmaAllocation allocation;
			VmaAllocationInfo allocationInfo;
			vmaAllocation(VmaAllocation allocation, VmaAllocationInfo allocationInfo) : allocation(allocation),allocationInfo(allocationInfo) {}
		};
		struct transferCommand
		{
			uint64_t bufferOffset;
			VkBuffer sourceBuffer;
			VkImage VulkanImage;
			VkFormat ImageFormat;
			glm::vec2 subImageHeight;
			glm::vec3 dimensions;
			glm::vec3 offset;
		};
		struct allocatorData
		{
			VmaAllocator allocator; //allocator handle.
			std::vector<transferCommand> transferCommands; //TODO make attomic var
			ref<commandPool> transferPool;
		};
		buffer& allocator::allocateBuffer(const size_t bufferSize, const memoryUsage memUsage, VkBufferUsageFlags bufferUsage)
		{
			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			
			bufferInfo.size = bufferSize;
			bufferInfo.usage = bufferUsage;

			VmaAllocationCreateInfo vmaAllocInfo = {};
			vmaAllocInfo.usage = (VmaMemoryUsage)memUsage;
			VmaAllocation allocation;
			VmaAllocationInfo info;
			VkBuffer _buffer;
			VkResult createRes = vmaCreateBuffer(p_data->allocator, &bufferInfo, &vmaAllocInfo, &_buffer, &allocation, &info);
			LN_ERR_FAIL_COND_V_MSG(createRes != VK_SUCCESS, buffer(VK_NULL_HANDLE,nullptr,nullptr), "[Artemis] an error occured whilst allocating a buffer, VkResult: " + VK_RESULT(createRes));
			return *(new buffer(_buffer, new vmaAllocation(allocation, info), this));
		}
		image& allocator::allocateImage()
		{
			// TODO: insert return statement here
		}
		void allocator::deallocate(const VkBuffer buffer,vmaAllocation* p_allocation)
		{
			LN_ERR_FAIL_COND_MSG(buffer == VK_NULL_HANDLE, "[Artemis] cannot deallocate a buffer thats has already been detroyed!");
			LN_ERR_FAIL_COND_MSG(p_allocation == nullptr, "[Artemis] allocation was invalid (nullptr)");
			vmaDestroyBuffer(p_data->allocator, buffer, p_allocation->allocation);
			delete p_allocation; //invalidate p_allocation
		}
		void allocator::deallocate(const VkImage image, vmaAllocation* p_allocation)
		{
			LN_ERR_FAIL_COND_MSG(image == VK_NULL_HANDLE, "[Artemis] cannot deallocate a image thats has already been detroyed!");
			LN_ERR_FAIL_COND_MSG(p_allocation == nullptr, "[Artemis] allocation was invalid (nullptr)");
			vmaDestroyImage(p_data->allocator, image, p_allocation->allocation);
			delete p_allocation; //invalidate p_allocation
		}
		allocator::allocator(const VkDevice* p_device, const VkInstance* p_instance, const VkPhysicalDevice* p_physicalDevice, const uint32_t apiVersion,const ref<commandPool> transferPool)
		{
			LN_ERR_FAIL_COND_MSG(transferPool == nullptr, "[Artemis] transferPool cann not be nullPointer!");
			allocatorData* p_allocatorData = new allocatorData();
			VmaAllocatorCreateInfo allocatorCreateInfo;
			allocatorCreateInfo.instance = *p_instance;
			allocatorCreateInfo.physicalDevice = *p_physicalDevice;
			allocatorCreateInfo.device = *p_device;

			allocatorCreateInfo.pAllocationCallbacks = nullptr;
			allocatorCreateInfo.pDeviceMemoryCallbacks = nullptr;
			allocatorCreateInfo.pVulkanFunctions = nullptr;

			allocatorCreateInfo.pTypeExternalMemoryHandleTypes = nullptr;
			allocatorCreateInfo.pHeapSizeLimit = nullptr;
			allocatorCreateInfo.preferredLargeHeapBlockSize = 0;

			VkPhysicalDeviceMemoryProperties memoryProperties;
			vkGetPhysicalDeviceMemoryProperties(*p_physicalDevice, &memoryProperties);
			allocatorCreateInfo.vulkanApiVersion = apiVersion;

			VkResult createRes = vmaCreateAllocator(&allocatorCreateInfo, &p_allocatorData->allocator);
			LN_ERR_FAIL_COND_MSG(createRes != VK_SUCCESS, "[Artemis] an error occured during allocator creation");
			
			p_allocatorData->transferPool = transferPool;
			p_data = ref<allocatorData>(p_allocatorData);
		}
		size_t allocator::getSize(vmaAllocation* allocation)
		{
			return allocation->allocationInfo.size;
		}
	}
}