#pragma once
#include <core/rendering/device.h>
#include <core/rendering/shader.h>
namespace luna
{
	namespace renderer
	{
	
		struct pipelineLayout
		{
			std::vector<ref<shader>> pipelineShaders;
			ref<device> device;
		};
		class pipeline 
		{
		public:
			virtual ~pipeline() {};
			virtual void createPipeline(const pipelineLayout& layout) = 0;
			virtual void begin() const = 0;
			virtual void end() const = 0;
			int test;
		private:

		};
	}
}
