#include <core/core.h>
#include <shaderc/shaderc.h>
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
			compileLanguage language; 
			shaderc_compile_options_t compileOptions;
		};
		class shaderCompiler
		{
		public:
			shaderCompiler();
			virtual ~shaderCompiler() { shaderc_compiler_release(compiler); };
			std::vector<char> compile(compileSpec compileSpec);


		private:
			shaderc_compiler_t compiler;
			bool init = false;
		};
	}
}


