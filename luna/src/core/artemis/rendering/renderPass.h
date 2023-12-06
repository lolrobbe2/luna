#pragma once
#include <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis 
	{
		class subPassBuilder
		{
		public:
			subPassBuilder setBindPoint(const VkPipelineBindPoint point) { description.pipelineBindPoint = point; return *this; }
			subPassBuilder addInputAttachement(const VkImageLayout layout);
			subPassBuilder addColorAttachement(const VkImageLayout layout);
			subPassBuilder addDepthStencilAttachement(const VkImageLayout layout);
			VkSubpassDescription build();
		private:
			std::vector<VkImageLayout> inputAttachements;
			std::vector<VkImageLayout> colorAttachements;
			std::vector<VkImageLayout> depthStencilAttachements;

			std::vector<VkImageLayout> preserveAttachements;
			VkSubpassDescription description;
		};
		

		/**
		* @brief easely build a renderpass
		*/
		class renderPassBuilder
		{
			renderPassBuilder addSubbPass() { return *this; }
		};
		class renderPass
		{
		public:
		protected:
			renderPass(const VkDevice* device);
		private:
			VkRenderPass m_renderPass;
		};
	}
}


