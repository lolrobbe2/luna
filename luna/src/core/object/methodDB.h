#pragma once
#ifndef _METHOD_DB_
#define _METHOD_DB_

#include <core/core.h>
#include <core/debug/uuid.h>
#include <core/variant/variant.h>
#include <core/debug/debugMacros.h>
#include <core/utils/hashedObjectStorage.h>

namespace luna
{

	class method
	{
	public:
		void setId(uint64_t newId) { id = newId; }
		uint64_t getId() const { id; } 
		_ALWAYS_INLINE_ void setClassName(const std::string& name) { m_class = name; }
		_ALWAYS_INLINE_ const std::string& getClassName() { return m_class; }
		_ALWAYS_INLINE_ virtual variant call(object* p_object, variant** args, int argCount) = 0;
		_ALWAYS_INLINE_ size_t getArgCount() const { return args.size(); }
	private:
		uint64_t id = 0;
		std::string m_class;
		std::vector<variant> defaultArgs;
		std::vector<variant::type> args;
		operator bool() { return id != 0; }
	};
#ifdef TYPED_METHOD_BIND
	template <typename T,typename R, typename... P>
#else
	template <typename MB_T, typename R, typename... P>
#endif
	class methodTR : public method
	{
		R(MB_T::* method)(P...) = nullptr;

		void (MB_T::* method)(P...) = nullptr;
		_ALWAYS_INLINE_ virtual variant call(object* p_object, variant** args, int argCount) override
		{
			//when the given argCOunt is smaller then the given argCount use calldv
			return argCount < getArgCount() ? callv(p_object, args, argCount) : calldv(p_object, args, argCount, defaultArgs);
		}
		template<typename... Args>
		_ALWAYS_INLINE_ variant call(MB_T* object, Args&&... args)
		{
			if (method) {
				// Call the stored method on the provided object and forward the arguments
				return (object->*method)(std::forward<Args>(args)...);
			}
		}
		_ALWAYS_INLINE_ variant callv(MB_T* object, variant** args, int argCount)
		{
			if (method) {
				// Ensure argCount matches the expected number of parameters
				LN_ERR_FAIL_COND_V_MSG(argCount != sizeof...(P), variant(), "Error: Argument count mismatch!");

				// Create a tuple from the array of variants
				return callFromVariants(object, args, std::index_sequence_for<P...>{});
			}
		}
		_ALWAYS_INLINE_ variant calldv(MB_T* object, variant** args, int argCount, const std::vector<variant>& defaultArgs)
		{
			constexpr size_t expectedArgCount = sizeof...(P);


			// If argCount is less than expected, use defaultArgs to fill in the rest
			LN_UNROLL_LOOP
				if (argCount < expectedArgCount)
				{
					std::vector<variant> finalArgs(expectedArgCount);

					// Copy the provided arguments into the finalArgs array
					LN_UNROLL_LOOP
						for (int i = 0; i < argCount; ++i)
							finalArgs[i] = *args[i];

					// Fill in the remaining arguments from defaultArgs
					LN_UNROLL_LOOP
						for (int i = argCount; i < expectedArgCount; ++i)
							finalArgs[i] = defaultArgs[i - argCount];  // Take values from defaultArgs

					// Convert the vector back to an array of pointers
					std::vector<variant*> finalArgPtrs(expectedArgCount);
					LN_UNROLL_LOOP
						for (int i = 0; i < expectedArgCount; ++i)
							finalArgPtrs[i] = &finalArgs[i];

					return callv(object, finalArgPtrs.data(), expectedArgCount);
				}

			return callv(object, args, argCount);

		}
		template<std::size_t... I>
		_ALWAYS_INLINE_ variant callFromVariants(MB_T* object, variant** args, std::index_sequence<I...>) {
			// Convert each Variant argument to the expected type and call the method
			return call(object, (args[I]->operator P())...);  // Extract and cast each Variant to P
		}
	};
#ifdef TYPED_METHOD_BIND
	template <typename T, typename... P>
#else
	template <typename MB_T,typename... P>
#endif
	class methodT : public method
	{
		void (MB_T::* method)(P...) = nullptr;
		_ALWAYS_INLINE_ virtual variant call(object* p_object, variant** args, int argCount) override
		{
			//when the given argCOunt is smaller then the given argCount use calldv
			return argCount < getArgCount() ? callv(p_object, args, argCount) : calldv(p_object, args, argCount, defaultArgs);
		}
		template<typename... Args>
		_ALWAYS_INLINE_ variant call(MB_T* object, Args&&... args) 
		{
			if (method) {
				// Call the stored method on the provided object and forward the arguments
				return (object->*method)(std::forward<Args>(args)...);
			}
		}
		_ALWAYS_INLINE_ variant callv(MB_T* object, variant** args, int argCount)
		{
			if (method) {
				// Ensure argCount matches the expected number of parameters
				LN_ERR_FAIL_COND_V_MSG(argCount != sizeof...(P),variant(), "Error: Argument count mismatch!");

				// Create a tuple from the array of variants
				return callFromVariants(object, args, std::index_sequence_for<P...>{});
			}
		}
		_ALWAYS_INLINE_ variant calldv(MB_T* object, variant** args, int argCount, const std::vector<variant>& defaultArgs)
		{
			constexpr size_t expectedArgCount = sizeof...(P);

			
			// If argCount is less than expected, use defaultArgs to fill in the rest
			LN_UNROLL_LOOP
			if (argCount < expectedArgCount) 
			{
				std::vector<variant> finalArgs(expectedArgCount);

				// Copy the provided arguments into the finalArgs array
				LN_UNROLL_LOOP
				for (int i = 0; i < argCount; ++i) 
					finalArgs[i] = *args[i];
			
				// Fill in the remaining arguments from defaultArgs
				LN_UNROLL_LOOP
				for (int i = argCount; i < expectedArgCount; ++i) 
					finalArgs[i] = defaultArgs[i - argCount];  // Take values from defaultArgs
				
				// Convert the vector back to an array of pointers
				std::vector<variant*> finalArgPtrs(expectedArgCount);
				LN_UNROLL_LOOP
				for (int i = 0; i < expectedArgCount; ++i) 
					finalArgPtrs[i] = &finalArgs[i];
				
				return callv(object, finalArgPtrs.data(), expectedArgCount);
			}

			return callv(object, args, argCount);
			
		}
		template<std::size_t... I>
		_ALWAYS_INLINE_ variant callFromVariants(MB_T* object, variant** args, std::index_sequence<I...>) {
			// Convert each Variant argument to the expected type and call the method
			return call(object, (args[I]->operator P())...);  // Extract and cast each Variant to P
		}

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
	/*holds info about a class's functions and signals*/
	struct classCallableInfo
	{
	public:
		_ALWAYS_INLINE_ methodDefinition*& operator[](const std::string& methodName) 
		{
			return methodDefMap[methodName];
		}
		method* getMethod(const std::string& methodName) 
		{
			return methodMap[methodName];
		}
	private:
		operator bool() { return className != ""; }
		std::string className = "";
		utils::hashedObjectStorage<std::string, methodDefinition*> methodDefMap;
		utils::hashedObjectStorage<std::string, method*> methodMap;

	};
	/**
	 * @brief holds all the internal and external functions + corresponding binding code.
	 */
	class methodDB
	{
	public:
		template<typename Class,typename R, typename... Args, typename... DefaultArgs>
		static void registerMethod(methodDefinition methodDef, R (Class::* p_method)(Args...), DefaultArgs&&... defaults)
		{
			method* methodInstance = createMethodBind(p_method);

			methodDef.m_class = methodInstance->getClassName();

			// Convert default arguments to variants
			std::vector<variant> defaultVariants = toVariants(std::forward<DefaultArgs>(defaults)...);
			methodInstance->setDefaultArgs(defaultVariants);  // Set default arguments in the base class

			if (classCallableStorage.hasValue(methodDef.m_class))
				classCallableStorage.setValue(methodDef.m_class, methodDef.m_name, methodInstance);
			else
			{
				classCallableInfo info;
				info[methodDef.m_name] = methodDef;
				classCallableStorage.putValue(methodDef.m_class, info);
			}
		}

		template <typename T, typename R, typename... P>
		static method* createMethodBind(R(T::* p_method)(P...)) {
#ifdef TYPED_METHOD_BIND
			method* a = memnew((MethodBindT<T, R, P...>)(p_method));
#else
			method* a = memnew((methodT<R, P...>)(reinterpret_cast<R(MB_T::*)(P...)>(p_method)));
#endif
			a->setClassName(T::className());
			return a;
		}

		template<typename Class, typename... Args>
		static methodDefinition createMethodDefinition(Class* p_object,const char* p_methodName, Args&&... p_args)
		{
			// Ensure Class is derived from 'object' at any level in the inheritance chain
			return createMethodDefinition<Class>(p_methodName,std::forward<Args>(p_args)...);
		}
		template<typename Class, typename... Args>
		static methodDefinition createMethodDefinition(const char* p_methodName, Args&&... p_args)
		{
			// Ensure Class is derived from 'object' at any level in the inheritance chain
			static_assert(std::is_base_of<object, Class>::value, "[MethodDB] Class must derive from 'object'");
			return createMethodDefinition(Class::getClassName(), p_methodName, std::forward<Args>(p_args)..., sizeof...(p_args));
		}

		template<typename... Args>

		static methodDefinition createMethodDefinition(const char* p_methodName, Args&&... p_args)
		{
			return createMethodDefinition("", p_methodName, std::forward<Args>(p_args)...);
		}
		template<typename... Args>

		static methodDefinition createMethodDefinition(const char* p_className, const char* p_methodName, Args&&... p_args)
		{
			return createMethodDefinition(p_className, p_methodName, std::forward<Args>(p_args)..., sizeof...(p_args));
		}
		static methodDefinition createMethodDefinition(const char* p_className, const char* p_methodName, const char* const** p_args, uint32_t p_argcount);
		
		template <typename... Args>
		constexpr std::vector<variant> toVariants(Args&&... args) {
			// Static assertion to ensure all arguments are derived from 'variant'
			static_assert((std::is_base_of<variant, std::decay_t<Args>>::value && ...),
				"[MethodDB] All arguments must be derived from 'variant'");

			// Creating a vector of variants using perfect forwarding
			// C++20 makes constexpr std::vector possible
			return { std::forward<Args>(args)... };
		}

		static void init();
	private:
		utils::hashedObjectStorage<std::string, classCallableInfo> classCallableStorage;
	};
	template<typename... Args>
	static methodDefinition METHOD_DEF(const char* p_methodName, Args&&... p_args) {
		return methodDB::createMethodDefinition(p_methodName, std::forward<Args>(p_args))
	}
}
#endif
