#pragma once
#include <core/core.h>
namespace luna
{
	namespace renderer
	{
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