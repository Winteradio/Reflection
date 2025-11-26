#ifndef __REFLECTION_METHODCALL_H__
#define __REFLECTION_METHODCALL_H__

#include "Utils.h"
#include "Type/TypeMacro.h"

namespace Reflection
{
	/**
	 * @class	MethodBase
	 * @brief	Base class for method calls in the reflection system.
	 * 			Provides a common interface for invoking methods dynamically.
	 */
	class MethodBase
	{
		/**
		 * @brief	Macro to generate type information for the MethodBase class.
		 * @details	This macro is assumed to be defined in "Type/TypeMacro.h" and is used
		 *			to facilitate reflection capabilities for the class.
		 */
		GENERATE(MethodBase);

		public :
			MethodBase() = default;
			virtual ~MethodBase() = default;
	};

	/**
	 * @class	MethodCall
	 * @brief	Template class for method calls with specific return types and arguments.
	 * 			Inherits from MethodBase and provides an interface for invoking methods.
	 * @tparam	Return The return type of the method.
	 * @tparam	Args   The argument types of the method.
	 */
	template<typename Return, typename... Args>
	class MethodCall : public MethodBase
	{
		/**
		 * @brief	Macro to generate type information for the MethodCall class.
		 * @details	This macro is assumed to be defined in "Type/TypeMacro.h"
		 * 			and is used to facilitate reflection capabilities for the class.
		 */
		GENERATE(MethodCall);

		public :
			MethodCall() = default;
			virtual ~MethodCall() = default;

		public :
			/** 
			 * @brief	Invoke the method with the given pointer and arguments.
			 * @param	pointer A pointer to the instance on which to invoke the method.
			 * @param	args    The arguments to pass to the method.
			 * @return	Return The result of the method invocation.
			*/
			virtual Return Invoke(void* pointer, Args... args) const = 0;
	};

	/**
	 * @class	MemberMethodCall
	 * @brief	Template class for member method calls.
	 * 			Inherits from MethodCall and provides an implementation for invoking member methods.
	 * @tparam	Return The return type of the method.
	 * @tparam	Class  The class type of the member method.
	 * @tparam	Const  A boolean indicating whether the method is const.
	 * @tparam	Args   The argument types of the method.
	 */
	template<typename Return, typename Class, bool Const, typename... Args>
	class MemberMethodCall : public MethodCall<Return, Args...>
	{
		/**
		 * @brief	Macro to generate type information for the MemberMethodCall class.
		 * @details	This macro is assumed to be defined in "Type/TypeMacro.h"
		 * 			and is used to facilitate reflection capabilities for the class.
		 */
		GENERATE(MemberMethodCall);

		public :
			/**
			 * @brief	Defines the function type for the member method, considering constness.
			 * @tparam	Return The return type of the method.
			 * @tparam	Class  The class type of the member method.
			 * @tparam	Const  A boolean indicating whether the method is const.
			 * @tparam	Args   The argument types of the method.
			 */
			using Function = typename Utils::Conditional<Const, Return(Class::*)(Args...) const, Return(Class::*)(Args...)>::Type;
			
			/**
			 * @brief	Defines the class type, considering constness.
			 * @tparam	Class  The class type of the member method.
			 * @tparam	Const  A boolean indicating whether the method is const.
			 * @tparam	Return The return type of the method.
			 * @tparam	Args   The argument types of the method.
			*/
			using ClassType = typename Utils::Conditional<Const, const Class, Class>::Type;

		public :
			/**
			 * @brief	Constructor for MemberMethodCall.
			 * @param	function The member function pointer to be invoked.
			 */
			MemberMethodCall(Function function)
				: m_function(function)
			{};

			virtual ~MemberMethodCall() = default;

		public :
			/**
			 * @brief	Invoke the member method on the given instance pointer with the provided arguments.
			 * @param	pointer A pointer to the instance on which to invoke the method.
			 * @param	args    The arguments to pass to the method.
			 * @return	Return The result of the method invocation.
			 */
			Return Invoke(void* pointer, Args... args) const override
			{
				ClassType* instance = reinterpret_cast<ClassType*>(pointer);
				return (instance->*m_function)(std::forward<Args>(args)...);
			}

		private :
			/**
			 * @brief	The member function pointer to be invoked.
			 * @tparam	Return The return type of the method.
			 * @tparam	Class  The class type of the member method.
			 * @tparam	Const  A boolean indicating whether the method is const.
			 * @tparam	Args   The argument types of the method.
			 */
			Function m_function;
	};

	/**
	 * @class	StaticMethodCall
	 * @brief	Template class for static method calls.
	 * 			Inherits from MethodCall and provides an implementation for invoking static methods.
	 * @tparam	Return The return type of the method.
	 * @tparam	Args   The argument types of the method.
	 */
	template<typename Return, typename... Args>
	class StaticMethodCall : public MethodCall<Return, Args...>
	{
		/**
		 * @brief	Macro to generate type information for the StaticMethodCall class.
		 * @details	This macro is assumed to be defined in "Type/TypeMacro.h"
		 * 			and is used to facilitate reflection capabilities for the class.
		 */
		GENERATE(StaticMethodCall);
		
		public :
			/**
			 * @brief	Defines the function type for the static method.
			 * @tparam	Return The return type of the method.
			 */
			using Function = Return(*)(Args...);

		public :
			/**
			 * @brief	Constructor for StaticMethodCall.
			 * @param	function The static function pointer to be invoked.
			 */
			StaticMethodCall(Function function)
				: m_function(function)
			{};

			virtual ~StaticMethodCall() = default;

		public :
			/**
			 * @brief	Invoke the static method with the provided arguments.
			 * @param	pointer A pointer to the instance (not used for static methods).
			 * @param	args    The arguments to pass to the method.
			 */
			Return Invoke(void* pointer, Args... args) const override
			{
				return m_function(std::forward<Args>(args)...);
			}

		private :
			/**
			 * @brief	The static function pointer to be invoked.
			 * @tparam	Return The return type of the method.
			 * @tparam	Args   The argument types of the method.
			 */
			Function m_function;
	};

	/**
	 * @class	MethodCreator
	 * @brief	Factory helper struct to generate Type-Erased MethodCall instances.
	 * @details	This struct uses partial template specialization to deduce the return type, 
	 * 			class type, argument types, and const-ness of a function pointer.
	 * 			It then instantiates the appropriate MethodCall subclass (Member/Static/Const)
	 * 			to abstract away the specific function signature.
	 * @tparam	Func The function type signature.
	 * @tparam	func The compile-time function pointer constant.
	 */
	template<typename Func, Func func>
	struct MethodCreator;

	/**
	 * @brief	Specialization of MethodCreator for member functions.
	 * @tparam	Return The return type of the method.
	 * @tparam	Class  The class type of the member method.
	 * @tparam	Args   The argument types of the method.
	 * @tparam	func  The member function pointer.
	 */
	template<typename Return, typename Class, typename... Args, Return(Class::*func)(Args...)>
	struct MethodCreator<Return(Class::*)(Args...), func>
	{
		/**
		 * @brief	Creates a MethodBase instance for the member method.
		 * @return	const MethodBase* A pointer to the created MethodBase instance.
		 */
		static const MethodBase* Create()
		{
			static MemberMethodCall<Return, Class, false, Args...> methodCall(func);
			return &methodCall;
		}
	};

	/**
	 * @brief	Specialization of MethodCreator for const member functions.
	 * @tparam	Return The return type of the method.
	 * @tparam	Class  The class type of the member method.
	 * @tparam	Args   The argument types of the method.
	 * @tparam	func  The const member function pointer.
	 */
	template<typename Return, typename Class, typename... Args, Return(Class::*func)(Args...) const>
	struct MethodCreator<Return(Class::*)(Args...) const, func>
	{
		/**
		 * @brief	Creates a MethodBase instance for the const member method.
		 * @return	const MethodBase* A pointer to the created MethodBase instance.
		 */
		static const MethodBase* Create()
		{
			static MemberMethodCall<Return, Class, true, Args...> methodCall(func);
			return &methodCall;
		}
	};

	/**
	 * @brief	Specialization of MethodCreator for static functions.
	 * @tparam	Return The return type of the method.
	 * @tparam	Args   The argument types of the method.
	 * @tparam	func  The static function pointer.
	 */
	template<typename Return, typename... Args, Return(*func)(Args...)>
	struct MethodCreator<Return(*)(Args...), func>
	{
		/**
		 * @brief	Creates a MethodBase instance for the static method.
		 * @return	const MethodBase* A pointer to the created MethodBase instance.
		 */
		static const MethodBase* Create()
		{
			static StaticMethodCall<Return, Args...> methodCall(func);
			return &methodCall;
		}
	};
};

#endif // __REFLECTION_METHODCALL_H__