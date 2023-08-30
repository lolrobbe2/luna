#pragma once
#include <core/core.h>
#include <entt.h>
#include <type_traits>
#include <core/scene/baseComponents.h>

#ifndef LN_REGISTER_CLASS
#define LN_REGISTER_CLASS(mClass) objectDB::registerClass<mClass>();
#endif // !LN_REGISTER_CLASS
#ifndef LN_CLASS
#define LN_CLASS(mClass,mInherits) objectDB::addClass<mClass,mInherits>();
#endif // !LN_CLASS
#ifndef LN_EMIT_SIGNAL
#define  LN_EMIT_SIGNAL(signalName,...) this->emitSignal(signalName,__VA_ARGS__)
#endif // !LN_EMIT_SIGNAL


#ifndef LN_CLASS_STRINGIFY
/**
 * @brief creates stringid from class.
 */
#define LN_CLASS_STRINGIFY(mClass) objectDB::getClassName<mClass>();
#endif // !LN_CLASS_STRINGIFY
namespace luna
{
	class LN_API scene;
	/**
	 * @brief object class.
	 * @warning DO NOT TOUCH UNLESS YOU KNOW WHAT YOURE DOING!!!
	 */
	class LN_API object
	{
	public:
		object() = default;
		object(entt::entity handle, luna::scene* scene) : entityHandle(handle), scene(scene) {};
		object(uint64_t id, luna::scene* scene);
		virtual void init(scene* scene);
		virtual	void bindMethods(); 
		template <typename ... ArgsT>
		void emitSignal(const char* functionName, ArgsT && ... inArgs );
		/**
		* @brief connects a signal to a object, that ether being itself or another object.
		* 
		* @param uint64_t objectID (quikID entt::entity)
		*/
		void connectSignal(uint64_t objectID, std::string& functionName);
		std::vector<std::string> getSignalNames();
		template<typename T, typename... Args>
		T& addComponent(Args&&... args)
		{
			//LN_CORE_ASSERT(!hasComponent<T>(), "Node already has component!");
			T& component = scene->m_Registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
			//scene->onComponentAdded<T>(*this, component);
			return component;
		}
		template<typename T, typename... Args>
		T& addOrReplaceComponent(Args&&... args);

		template<typename T>
		T& getComponent()
		{
			return scene->m_Registry.get<T>(entityHandle);
		}

		template<typename T>
		bool hasComponent()
		{
			return scene->m_Registry.all_of<T>(entityHandle);
		}
		template<typename T>
		void removeComponent()
		{
			scene->m_Registry.remove<T>(entityHandle);
		}
		uuid getUUID() { return getComponent<idComponent>().id; }

	protected:
		friend class luna::scene;
		entt::entity entityHandle{ entt::null };
		scene* scene = nullptr;

	};
	/**
	 * @brief object database class.
	 * @warning DO NOT TOUCH UNLESS YOU KNOW WHAT YOURE DOING!!!
	 */
	class LN_API objectDB
	{
	public:
		struct classInfo
		{
			object* (*creation_func)() = nullptr;
			std::vector<classInfo*> children;
			classInfo* parentClass = nullptr;
			std::string className = "";
		};
		_ALWAYS_INLINE_ static bool isClassRegistered(std::string className)
		{
			return rootClassDatabase.find(className) != rootClassDatabase.end() || classDatabase.find(className) != classDatabase.end();
		}

		template<class T>
		_ALWAYS_INLINE_ static bool isClassRegistered() 
		{
			return isClassRegistered(getClassName<T>()); 
		};

		template<class T,class A>
		_ALWAYS_INLINE_ static void addClass()
		{
			classInfo* infoA = getPtr(getClassName<A>());
			classInfo* infoT = getPtr(getClassName<T>());
			if(!infoT) LN_REGISTER_CLASS(T)
			if(!infoA) LN_REGISTER_CLASS(A)
			if(!infoA) infoA = getPtr(getClassName<A>()); // parent node type
			if(!infoT) infoT = getPtr(getClassName<T>());
			//if (!infoA) return;
			
			//if (!infoT) return;
			infoT->parentClass = infoA;
			infoA->children.push_back(infoT);
			removeRootClass<T>();
		}
		
		template<class T>
		_ALWAYS_INLINE_ static void removeRootClass()
		{
			rootClassDatabase.erase(getClassName<T>());
		}

		template<class T>
		_ALWAYS_INLINE_ static void registerClass()
		{
			if (classDatabase.find(getClassName<T>()) != classDatabase.end()) return LN_CORE_WARN("class has already been registered: {0}", getClassName<T>());
			classInfo t;
			t.creation_func = &creator<T>;
			t.className = getClassName<T>();

			LN_CORE_INFO("class registered: {0}", t.className);
			classDatabase.insert({ getClassName<T>(), t});
			rootClassDatabase.insert({ getClassName<T>(), getPtr(t.className)});
		}
		/* start of c++ wizardry from GD */
		#define memnew(m_class) _post_initialize(new m_class)
		template <class T>
		static object* creator()
		{
			return memnew(T);
		}
		_ALWAYS_INLINE_ static void postinitialize_handler(void*) {}

		template <class T>
		_ALWAYS_INLINE_ static T* _post_initialize(T* p_obj) 
		{
			postinitialize_handler(p_obj);
			return p_obj;
		}
		/* end of c++ wizardry from GD*/

		static void createInstance(const std::string& className, scene* scene);

		_ALWAYS_INLINE_ static classInfo* getPtr(const std::string className) {
			auto searchResult = classDatabase.find(className);
			if (searchResult == classDatabase.end()) return nullptr;
			return &searchResult->second; 
		};

		template <class T>
		_ALWAYS_INLINE_ static std::string getClassName()
		{
			std::stringstream fullName(typeid(T).name());
			std::vector<std::string> seglist;
			std::string segment;
			while (std::getline(fullName, segment, ':')) seglist.push_back(segment);
			return seglist.back();
		}
		inline static std::unordered_map<std::string, classInfo*> rootClassDatabase; //stores base classes
		inline static std::unordered_map<std::string, classInfo> classDatabase;
	};
}

