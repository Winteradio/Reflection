#ifndef __REFLECTION_METHODMACRO_H__
#define __REFLECTION_METHODMACRO_H__

#include "Macro.h"
#include "Method/MethodInfo.h"

/**
 * @def		METHOD( Method )
 * @brief	Registers a member method into the Reflection System.
 * @details	This macro creates an internal static struct `RegisterMethod`. 
 * 			The constructor of this struct runs at **Static Initialization Time** (before the `main()` function), 
 * 			ensuring that the method metadata is automatically registered into its owner `TypeInfo`.
 * 			It also utilizes `MethodCreator` to automatically generate a 
 * 			**Type-Erased Invoker (Thunk)**. This allows the method to be 
 * 			dynamically invoked at runtime via `MethodInfo::Invoke`, regardless 
 * 			of the specific function signature.
 * @param	Method The name of the member function to register (without quotes).
 */
#define METHOD( Method ) \
		struct RegisterMethod##Method \
		{ \
			RegisterMethod##Method() \
			{ \
				static const Reflection::MethodBase* methodBase = Reflection::MethodCreator<decltype(&ThisType::Method), &ThisType::Method>::Create(); \
				static Reflection::MethodInfo::Initializer<ThisType, decltype(&ThisType::Method)> initializer(methodBase); \
				static Reflection::MethodInfo methodInfo(initializer, std::string(#Method)); \
			}; \
		}; \
		\
		static inline const RegisterMethod##Method s_registerMethod##Method __STATIC_USED__; \

#endif // __REFLECTION_METHODMACRO_H__