#pragma once
#include <core/core.h>
#include <type_traits>


#define LN_REGISTER_CLASS(node) objectDB::registerClass<node>();

namespace luna
{
	class object
	{

	};
	class LN_API objectDB
	{
	public:
		struct classInfo
		{
			object* (*creation_func)() = nullptr;
		};
		
		
		template<class T>
		static void registerClass()
		{
			
			classInfo t;
			t.creation_func = &creator<T>;
			std::stringstream fullName(typeid(T).name());
			std::vector<std::string> seglist;
			std::string segment;
			while (std::getline(fullName, segment, ':'))
			{
				seglist.push_back(segment);
			}
			LN_CORE_INFO("class registered: {}", seglist.back());
			classDatabase.insert({ seglist.back(), t});
			//classDatabase.insert( std::string(typeid(T).name()),std::function<void>(&create<T>());
		}
		/* start of c++ wizardry */
		#define memnew(m_class) _post_initialize(new ("") m_class)
		template <class T>
		static object* creator()
		{
			return (object*)memnew(T);
		}
		_ALWAYS_INLINE_ static void postinitialize_handler(void*) {}

		template <class T>
		_ALWAYS_INLINE_ static T* _post_initialize(T* p_obj) {
			postinitialize_handler(p_obj);
			return p_obj;
		}
		/* end of c++ wizardry */


		static classInfo* getPtr(const std::string className) { return &classDatabase.find(className)->second; };
		inline static std::unordered_map<std::string, classInfo> classDatabase;
	};
}

