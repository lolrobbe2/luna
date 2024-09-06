#pragma once
#ifndef _DESCRIPTOR_SET_
#define _DESCRIPTOR_SET_

#include <core/artemis/rendering/shader.h>
namespace luna 
{
	namespace artemis 
	{
		/// <summary>
		/// remebember descriptorSet& not descriptorSet otherwise the descriptorSet will be destroyed before you can use it
		/// </summary>
		class descriptorSet
		{
		public: 
			_ALWAYS_INLINE_ VkDescriptorSet getNative() const { return m_descriptorSet; }
			operator VkDescriptorSet() const { return m_descriptorSet; }
			_ALWAYS_INLINE_ const VkDescriptorSet* getNativePtr() const { return &m_descriptorSet; }
			void update();
			operator const VkDescriptorSet*() const { return &m_descriptorSet; }
			void write(const uint32_t& descriptorIndex,const void* pDescriptorInfo);
			void free();
			descriptorSet() = default;
			~descriptorSet() { free(); }
		protected:
			friend class descriptorPool;
			descriptorSet(const VkDevice* p_device,const VkDescriptorSet descriptorSet, const VkDescriptorPool* p_descriptorPool, std::vector<VkWriteDescriptorSet>& descriptorWrites);
		private:
			VkDescriptorSet m_descriptorSet = VK_NULL_HANDLE;
			const VkDescriptorPool* p_descriptorPool = nullptr;
			const VkDevice* p_device = nullptr;
			std::vector<VkWriteDescriptorSet> descriptorWrites;

		};
	}
}
#endif // !_DESCRIPTOR_SET_