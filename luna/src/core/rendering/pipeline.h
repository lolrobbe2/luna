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
			/**
			 * @brief creates graphics a pipeline.
			 * 
			 * \param layout
			 */
			virtual void createPipeline(const pipelineLayout& layout) = 0;
			/**
			 * @brief destroys the graphics pipeline.
			 * 
			 */
			virtual void destroyPipeline() = 0;
			/**
			 * @brief starts recording pipeline input.
			 * 
			 */
			virtual void begin() const = 0;
			/**
			 * @brief ends recording end.
			 * 
			 */
			virtual void end() const = 0;
			/**
			 * @brief flush the renderer pipline and render a frame.
			 * 
			 */
			virtual void flush() = 0;
			int test;
		private:

		};
	}
}
