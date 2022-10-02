#pragma once
#include <core/rendering/shader.h>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
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
			void createLayout() override;
		private:
			renderer::shaderResource getShaderResource(const spirv_cross::Resource& _shaderResource, const std::vector<uint32_t>& shaderSource,renderer::typeClass typeClass);

		private:
			friend class renderer::shader;
			std::vector<uint32_t>shaderSource;
		};
	}
}