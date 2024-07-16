#pragma once
#include <core/scripting/monoType.h>
namespace luna
{
	namespace scripting 
	{
		class monoMethod;
		class monoParam
		{
		public:

			monoParam(MonoMethod* p_method,MonoType* p_type,int paramNumber,const char* p_name);
			//getters
			_ALWAYS_INLINE_ operator const monoType& () { return m_type; }
			_ALWAYS_INLINE_ const monoType& getType() { return m_type; }

			_ALWAYS_INLINE_ operator const std::string& () { return m_name; }
			_ALWAYS_INLINE_ const std::string& getName() { return m_name; }

			_ALWAYS_INLINE_ monoMethod getMethod();
		private:
			std::string m_name;
			int m_paramNumber;
			MonoMethod* p_method; //allows to get param method back; (don't use wrapper type for performance reasons,memory)
			monoType m_type;
		};
	}
}


