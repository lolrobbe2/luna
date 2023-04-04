#pragma once
#include <core/core.h>
#include <core/scene/scene.h>
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
			static void initMono();
			static bool loadAssembly(const std::filesystem::path& filepath);
			static bool loadAppAssembly(const std::filesystem::path& filepath);
			static MonoAssembly* loadCSharpAssembly(const std::string& assemblyPath);
			static uint8_t getFieldAccessibility(MonoClassField* field);
			static void printAssamblyTypes(MonoAssembly* assembly);
			static scene* getContext();
		private:

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