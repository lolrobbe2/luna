#include <core/core.h>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
namespace luna
{
	namespace utils
	{
		enum compileLanguage
		{
			SPIR = 0,
			SPIR_V = 1,
			GLSL = 2
		};
		struct compileSpec
		{
			bool reflect = false;
			std::vector<char> source;
			std::string fileName;
			shaderc_shader_kind shaderKind; 
			compileLanguage language = SPIR_V; 
			shaderc_compile_options_t compileOptions;
		};
		class shaderCompiler
		{
		public:
			/**
			 * @brief constructor does nothing at the moment.
			 * 
			 */
			shaderCompiler();
			virtual ~shaderCompiler() {  };
			/**
			 * @brief compile a shader from source GLSL to specified.
			 * 
			 * \param compileSpec struct with compile specifications
			 * \return compiled shadersource.
			 */
			std::vector<uint32_t> compile(compileSpec compileSpec);
			

		private:
			/**
			 * @brief reflects on the compiled shader and shows a text based dissasambly of the shader.
			 * 
			 * \param shaderData compiled shaderData.
			 * \param reflect reflect yes or no.
			 * \return param reflect.
			 */
			bool reflect(const std::vector<uint32_t>& shaderData, bool reflect);
			/**
			 * @brief returns the resource type name ex mat4 returns "Mat4".
			 * 
			 * \param resource to get the typename of.
			 * \param compiler the compiler wich contains the compiled shader.
			 * \return the type name of the resource as an std::string.
			 */
			std::string getResourceTypeName(const spirv_cross::Resource& resource, const spirv_cross::Compiler& compiler);
		};
	}
}


