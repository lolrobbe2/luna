#pragma once
#include <core/core.h>
namespace luna
{
	namespace renderer
	{
		enum typeId
		{
			Void = 19,
			Bool = 20,
			Int = 21,
			FLoat = 22,
			Vector = 23,
			Matrix = 24,
			Image = 25,
			Sampler = 26,
			SampledImage = 27,
			Array = 28,
			RuntimeArray = 29,
			Struct = 30,
			Pointer = 32
		};
		class shader
		{
		public:

			virtual ~shader() = default;
			virtual void bind() const = 0;
			virtual void unbind() const = 0;
			static ref<shader> create(const std::string& filepath);
			static ref<shader> create(const std::vector<uint8_t> shaderSrc);
			std::string shaderName;
		private:
			
		};

	}
}