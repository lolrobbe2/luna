#pragma once
#ifndef _METHOD_DB_
#define _METHOD_DB_

#include <core/core.h>
#include <core/debug/uuid.h>
#include <core/variant/variant.h>
namespace luna
{

	class method
	{
	public:
		void setId(uint64_t newId) { id = newId; }
		uint64_t getId() { id; }
		_ALWAYS_INLINE_ void setClassName(const std::string& name) { m_class = name; }
		_ALWAYS_INLINE_ const std::string& getClassName(const std::string& name) { return m_class; }
	private:
		uint64_t id = 0;
		std::string m_class;
		std::vector<variant> args;
		operator bool() { return id != 0; }
	};
#ifdef TYPED_METHOD_BIND
	template <typename T, typename... P>
#else
	template <typename... P>
#endif
	class methodT : public method
	{
		void (MB_T::* method)(P...) = nullptr;

	};
	struct methodDefinition
	{
		uint64_t id = 0;
		std::string m_class;
		std::string m_name;
		std::vector<std::string> args;
		operator bool() { return id != 0; }
	};
	_ALWAYS_INLINE_ static std::string camelToPascal(const std::string& pascalText)
	{
		std::string camelText = pascalText;
		camelText[0] = std::toupper(pascalText[0]);
		return camelText;
	}
	/**
	 * @brief holds all the internal and external functions + corresponding binding code.
	 */
	class methodDB
	{
	public:
		template <typename T, typename... P>
		method* createMethodBind(void (T::* p_method)(P...)) {
#ifdef TYPED_METHOD_BIND
			method* a = memnew((MethodBindT<T, P...>)(p_method));
#else
			method* a = memnew((methodT<P...>)(reinterpret_cast<void (MB_T::*)(P...)>(p_method)));
#endif
			a->setClassName(T::className());
			return a;
		}
		template<typename Class, typename... Args>
		methodDefinition createMethodDefinition(Class* p_object,const char* p_methodName, Args&&... p_args)
		{
			// Ensure Class is derived from 'object' at any level in the inheritance chain
			return createMethodDefinition<Class>(p_methodName,std::forward<Args>(p_args)...);
		}
		template<typename Class, typename... Args>
		methodDefinition createMethodDefinition(const char* p_methodName, Args&&... p_args)
		{
			// Ensure Class is derived from 'object' at any level in the inheritance chain
			static_assert(std::is_base_of<object, Class>::value, "[MethodDB] Class must derive from 'object'");
			return createMethodDefinition(Class::getClassName(), p_methodName, std::forward<Args>(p_args)..., sizeof...(p_args));
		}

		template<typename... Args>
		methodDefinition createMethodDefinition(const char* p_className, const char* p_methodName, Args&&... p_args)
		{
			return createMethodDefinition(p_className, p_methodName, std::forward<Args>(p_args)..., sizeof...(p_args));
		}
		methodDefinition createMethodDefinition(const char* p_className, const char* p_methodName, const char* const** p_args, uint32_t p_argcount);
		static void init();
		
	};
}
#endif
