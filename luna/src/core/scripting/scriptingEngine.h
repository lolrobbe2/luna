#pragma once
#include <core/core.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>

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
			static uint8_t getFieldAccessibility(MonoClassField* field);
			static void printAssamblyTypes(MonoAssembly* assembly);

			_ALWAYS_INLINE_ static MonoImage* getImage() { return s_AppImage; };
		private:
			inline static MonoDomain* s_RootDomain;
			inline static MonoDomain* s_AppDomain;
			inline static MonoClass* s_AppClass;
			inline static MonoImage* s_AppImage;
		};

		/**
		 * @brief a root class represents the C# node implementation from wich the instantiated class inhertits.
		 */
		class rootClass
		{
		public:
			rootClass() = default;
			rootClass(MonoClass* childClass);
			~rootClass();

		private:
			MonoClass* root;
			std::map<std::string, MonoMethod*> methods;
			std::map<std::string, MonoMethod*> signals;
		};

		class scriptClass
		{
		public:
			scriptClass() = default;
			scriptClass(std::string nodeName);
			~scriptClass();

		private:
			MonoClass* childClass;
			MonoClass* baseClass;

			MonoMethod* readyMethod = nullptr;
			MonoMethod* processMethod = nullptr;
			MonoMethod* physicsProcessMethod = nullptr;
		};
	}
}