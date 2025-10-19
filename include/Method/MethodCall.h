#ifndef __REFLECTION_METHODCALL_H__
#define __REFLECTION_METHODCALL_H__

#include "Utils.h"
#include "Type/TypeMacro.h"

namespace Reflection
{
	class MethodBase
	{
		GENERATE(MethodBase);

		public :
			MethodBase() = default;
			virtual ~MethodBase() = default;
	};

	template<typename Return, typename... Args>
	class MethodCall : public MethodBase
	{
		GENERATE(MethodCall);

		public :
			MethodCall() = default;
			virtual ~MethodCall() = default;

		public :
			virtual Return Invoke(void* pointer, Args... args) const = 0;
	};

	template<typename Return, typename Class, bool Const, typename... Args>
	class MemberMethodCall : public MethodCall<Return, Args...>
	{
		GENERATE(MemberMethodCall);

		public :
			using Function = typename Utils::Conditional<Const, Return(Class::*)(Args...) const, Return(Class::*)(Args...)>::Type;
			using ClassType = typename Utils::Conditional<Const, const Class, Class>::Type;

		public :
			MemberMethodCall(Function function)
				: m_function(function)
			{};

			virtual ~MemberMethodCall() = default;

		public :
			Return Invoke(void* pointer, Args... args) const override
			{
				ClassType* instance = reinterpret_cast<ClassType*>(pointer);
				return (instance->*m_function)(std::forward<Args>(args)...);
			}

		private :
			Function m_function;
	};

	template<typename Return, typename... Args>
	class StaticMethodCall : public MethodCall<Return, Args...>
	{
		GENERATE(StaticMethodCall);
		
		public :
			using Function = Return(*)(Args...);

		public :
			StaticMethodCall(Function function)
				: m_function(function)
			{};

			virtual ~StaticMethodCall() = default;

		public :
			Return Invoke(void* pointer, Args... args) const override
			{
				return m_function(std::forward<Args>(args)...);
			}

		private :
			Function m_function;
	};

	template<typename Func, Func func>
	struct MethodCreator;

	template<typename Return, typename Class, typename... Args, Return(Class::*func)(Args...)>
	struct MethodCreator<Return(Class::*)(Args...), func>
	{
		static const MethodBase* Create()
		{
			static MemberMethodCall<Return, Class, false, Args...> methodCall(func);
			return &methodCall;
		}
	};

	template<typename Return, typename Class, typename... Args, Return(Class::*func)(Args...) const>
	struct MethodCreator<Return(Class::*)(Args...) const, func>
	{
		static const MethodBase* Create()
		{
			static MemberMethodCall<Return, Class, true, Args...> methodCall(func);
			return &methodCall;
		}
	};

	template<typename Return, typename... Args, Return(*func)(Args...)>
	struct MethodCreator<Return(*)(Args...), func>
	{
		static const MethodBase* Create()
		{
			static StaticMethodCall<Return, Args...> methodCall(func);
			return &methodCall;
		}
	};
};

#endif // __REFLECTION_METHODCALL_H__