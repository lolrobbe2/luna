#pragma once
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
			void free();
			~descriptorSet() { free(); }
		protected:
			friend class descriptorPool;
			descriptorSet(const VkDescriptorSet descriptorSet, const VkDescriptorPool* p_descriptorPool, std::vector<VkWriteDescriptorSet>& descriptorWrites);
		private:
			VkDescriptorSet _descriptorSet;
			const VkDescriptorPool* p_descriptorPool;
			const VkDevice* p_device;
			std::vector<VkWriteDescriptorSet> descriptorWrites;
		};
	}
}


