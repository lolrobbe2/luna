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
			/**
			 * @brief creates a vulkan shader from a filePath.
			 * 
			 * \param filepath the path to the source .GLSL file.
			 */
			vulkanShader(const std::string& filepath,const renderer::shaderStage& stage);
			virtual ~vulkanShader() {};
			void bind() const override;
			void unbind() const override;
			/**
			 * @brief creates the shader layout.
			 */
			void createLayout() override;
		private:
			/**
			 * @brief get the shderResource information and returns a corresponding engine shaderresource.
			 * 
			 * \param _shaderResource
			 * \param shaderSource
			 * \param typeClass
			 * \return 
			 */
			renderer::shaderResource getShaderResource(const spirv_cross::Resource& _shaderResource, const std::vector<uint32_t>& shaderSource,renderer::typeClass typeClass);

		private:
			friend class renderer::shader;
			
		};
	}
}