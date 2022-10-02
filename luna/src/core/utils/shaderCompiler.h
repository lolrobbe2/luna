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
			std::vector<char> source;
			std::string fileName;
			shaderc_shader_kind shaderKind; 
			compileLanguage language = SPIR_V; 
			shaderc_compile_options_t compileOptions;
		};
		class shaderCompiler
		{
		public:
			shaderCompiler();
			virtual ~shaderCompiler() {  };
			std::vector<uint32_t> compile(compileSpec compileSpec);
			

		private:
			bool reflect(const std::vector<uint32_t>& shaderData);
			std::string getResourceTypeName(const spirv_cross::Resource& resource, const spirv_cross::Compiler& compiler);
			bool init = false;
		};
	}
}


