#pragma once
#include <core/rendering/shader.h>
namespace luna
{
	namespace vulkan
	{
		class vulkanShader : public renderer::shader
		{
		public:
			vulkanShader(const std::string& filepath);
			virtual ~vulkanShader() {};
			void bind() const override;
			void unbind() const override;
			void createLayout() const override;
		private:
			renderer::shaderResource getShaderResource(spirv_cross::Resource shaderResource); 
		};
	}
}