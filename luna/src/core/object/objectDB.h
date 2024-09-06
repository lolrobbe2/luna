#ifndef _OBJECT_DB_
#define _OBJECT_DB_
#include <core/core.h>
#include <entt.h>
#include <type_traits>
#include <core/scene/baseComponents.h>
#include <core/scene/scene.h>
#ifndef LN_REGISTER_OBJECT
#define LN_REGISTER_OBJECT(mClass) objectDB::registerClass<mClass>();
#endif // !LN_REGISTER_CLASS
#include <core/debug/debugMacros.h>
#ifndef LN_REGISTER_CLASS
#define LN_REGISTER_CLASS(mClass,mInherits) objectDB::addClass<mClass,mInherits>();
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

#ifndef LN_CLASS_TYPE_NAME
#define LN_CLASS_TYPE_NAME(mClass) getComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(mClass)
#endif

#ifndef LN_NOTIFICATION_FUNC
#define LN_NOTIFICATION_FUNC() getComponent<idComponent>().notificationFunc = [this](const notificationType type) { this->notification(type); }
#endif

#ifndef OBJ_GET
//gets an object in the current context given an entt::entity named objectId
#define OBJ_GET(className) className(objectId,scripting::scriptingEngine::getContext())
#endif // OBJ_GET

#ifndef OBJ_ROOT
/*
* @brief adds the basic object functions when private inheritence has been used!
*/
#define OBJ_ROOT(className) \
template<typename T, typename... Args> \
T& addComponent(Args&&... args) {return className::addComponent<T>(std::forward<Args&&>()...);} \
template<typename T, typename... Args> \
T& addOrReplaceComponent(Args&&... args) \
{ \
	return className::addOrReplaceComponent<T>(std::forward<Args&&>()...); \
} \
template<typename T>\
T& getComponent()\
{\
	return className::getComponent<T>();\
}
#endif // !OBJ_ROOT

namespace luna
{
	class LN_API object;
	template <class T>
	static object* creator()
	{
		return memnew(T);
	}
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

#endif