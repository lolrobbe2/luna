#pragma once
#include <core/rendering/shader.h>
namespace luna
{
	namespace vulkan
	{
		class vulkanShader : public renderer::shader
		{
			vulkanShader(const std::string& filepath);
			virtual ~vulkanShader() {};
			void bind();
			void unbind();
		};
	}
}


