#pragma once
#include <core/core.h>
#include <type_traits>

#ifndef LN_REGISTER_CLASS
#define LN_REGISTER_CLASS(mClass) objectDB::registerClass<mClass>();
#endif // !LN_REGISTER_CLASS
#ifndef LN_CLASS
#define LN_CLASS(mClass,mInherits) objectDB::addClass<mClass,mInherits>();
#endif // !LN_CLASS

namespace luna
{
	class scene;
	class LN_API object
	{
	public:
		virtual void init(scene* scene) = 0;
	};
	/**
	 * @brief object database class.
	 * @warn DO NOT TOUCH UNLESS YOU KNOW WHAT YOURE DOING!!!
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
		template<class T,class A>
		_ALWAYS_INLINE_ static void addClass()
		{
			LN_REGISTER_CLASS(T);
			LN_REGISTER_CLASS(A);
			classInfo* infoA = getPtr(getClasName<A>()); // parent node type
			if (!infoA) return;
			classInfo* infoT = getPtr(getClasName<T>()); 
			if (!infoT) return;
			infoT->parentClass = infoA;
			infoA->children.push_back(infoT);
			removeRootClass<T>();
		}
		
		template<class T>
		_ALWAYS_INLINE_ static void removeRootClass()
		{
			rootClassDatabase.erase(getClasName<T>());
		}

		template<class T>
		_ALWAYS_INLINE_ static void registerClass()
		{
			if (classDatabase.find(getClasName<T>()) != classDatabase.end()) return LN_CORE_WARN("class has already been registered: {0}", getClasName<T>());
			classInfo t;
			t.creation_func = &creator<T>;
			t.className = getClasName<T>();

			LN_CORE_INFO("class registered: {0}", t.className);
			classDatabase.insert({ getClasName<T>(), t});
			rootClassDatabase.insert({ getClasName<T>(), getPtr(t.className)});
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
		_ALWAYS_INLINE_ static std::string getClasName()
		{
			std::stringstream fullName(typeid(T).name());
			std::vector<std::string> seglist;
			std::string segment;
			while (std::getline(fullName, segment, ':')) seglist.push_back(segment);
			return seglist.back();
		}
		inline static std::unordered_map<std::string, classInfo*> rootClassDatabase; //stors base classes
		inline static std::unordered_map<std::string, classInfo> classDatabase;
	};
}

