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
		struct signal
		{
			std::string signalName;
			MonoMethod* signalMethod;
		};
		/**
		 * @brief a root class represents the C# node implementation from wich the instantiated class inhertits.
		 */
		class rootClass
		{
		public:
			rootClass() = default;
			rootClass(MonoClass* childClass);
			MonoArray* createArray(const size_t arraySize);
			virtual ~rootClass() = default;
			operator MonoClass* () { return root; };
		private:
			MonoClass* root;
		};

		class scriptClass
		{
		public:
			scriptClass() = default;
			scriptClass(MonoClass* childClass,MonoClass* baseClass);
			virtual ~scriptClass() = default;
			MonoObject* instance();
			void queueFree();

			void process(float deltaTime);
			void invokeSignal(std::string& signalName, void* obj, void** params);
			MonoMethod* constructor = nullptr;
			MonoMethod* readyMethod = nullptr;
			MonoMethod* processMethod = nullptr;
			MonoMethod* physicsProcessMethod = nullptr;
		
			MonoClass* childClass;
			MonoClass* baseClass;
			std::vector<signal> availableSignals;
			std::vector<signal> implementedSignals;
		};



		class LN_API scriptingEngine
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
			_ALWAYS_INLINE_ static std::string pascalToCamel(const std::string& pascalText)
			{
				std::string camelText = pascalText;
				camelText[0] = std::tolower(pascalText[0]);
				return camelText;
			}

			_ALWAYS_INLINE_ static std::string camelToPascal(const std::string& pascalText)
			{
				std::string camelText = pascalText;
				camelText[0] = std::toupper(pascalText[0]);
				return camelText;
			}

			static void init();
			static void shutdown();
			static void initMono();
			static bool loadAssembly(const std::filesystem::path& filepath);
			static bool loadAppAssembly(const std::filesystem::path& filepath);
			static MonoAssembly* loadCSharpAssembly(const std::string& assemblyPath);
			static uint8_t getFieldAccessibility(MonoClassField* field);
			static void printAssamblyTypes(MonoAssembly* assembly);
			static void loadCoreClasses();
			static void loadAppClasses();
			static void reloadAssembly();
			static std::vector<const char*> getAppClassNames() {
				std::vector<const char*> appClassNames;
				for (auto const& appClass : appClasses) { appClassNames.emplace_back(appClass.first.c_str()); };
				return appClassNames;
			}

			static std::vector<const char*> getCoreClassNames() {
				std::vector<const char*> coreClassNames;
				for (auto const& coreClass : rootClasses) { coreClassNames.emplace_back(coreClass.first.c_str()); };
				return coreClassNames;
			}
			static scene* getContext();
			static void secContext(scene* scene);
			void createInstance(const std::string& className, uuid entityId);
			static MonoObject* instanciate(MonoClass* monoClass);
			static scriptClass* getScriptClass(const std::string& className) { return appClasses.find(className)->second; }
			template<class type>
			static MonoArray* createArray(const size_t size);
			static MonoString* createMonoString(const std::string& string);
			static std::vector<signal> getSignalsFromClass(MonoClass* monoClass);
			static bool hasSignalAttribute(MonoClass* monoClass);
			static bool classHasAttributes(MonoClass* monoClass);
		private:
			inline static std::map<std::string, rootClass> rootClasses;
			inline static std::map<std::string, scriptClass*> appClasses;

		};

		template<class type>
		MonoArray* scriptingEngine::createArray(const size_t size)
		{
			std::string className = LN_CLASS_STRINGIFY(type);
			className = pascalToCamel(className);
			if (rootClasses.find(className) == rootClasses.end()) return nullptr;
			return rootClasses[className].createArray(size);
		}

	}
}