#pragma once
#include <core/core.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
namespace luna
{
	namespace scripting
	{
		class scriptingEngine
		{
		public: 
			enum class accessibility : uint8_t
			{
				None = 0,
				Private = BIT(0),
				Internal = BIT(1),
				Protected = BIT(2),
				Public = BIT(3)
			};
			static void init();
			static void shutdown();
			static MonoAssembly* loadCSharpAssembly(const std::string& assemblyPath);
		private:
			inline static MonoDomain* s_RootDomain;
			inline static MonoDomain* s_AppDomain;
		};
	}
}