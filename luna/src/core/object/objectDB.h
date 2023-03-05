#pragma once
#include <core/core.h>
#include <type_traits>


#define LN_REGISTER_CLASS(object) objectDB::registerClass<object>();

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
		};
		
		
		template<class T>
		_ALWAYS_INLINE_ static void registerClass()
		{
			
			classInfo t;
			t.creation_func = &creator<T>;
			std::stringstream fullName(typeid(T).name());
			LN_CORE_INFO("class registered: {0}", getClasName<T>());
			classDatabase.insert({ getClasName<T>(), t});
		}
		/* start of c++ wizardry */
		#define memnew(m_class) _post_initialize(new m_class)
		template <class T>
		static object* creator()
		{
			return memnew(T);
		}
		_ALWAYS_INLINE_ static void postinitialize_handler(void*) {}

		template <class T>
		_ALWAYS_INLINE_ static T* _post_initialize(T* p_obj) {
			postinitialize_handler(p_obj);
			return p_obj;
		}
		/* end of c++ wizardry */

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
			while (std::getline(fullName, segment, ':'))
			{
				seglist.push_back(segment);
			}
			return seglist.back();
		}
		inline static std::unordered_map<std::string, classInfo> classDatabase;
	};
}

