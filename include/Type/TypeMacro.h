#ifndef __REFLECTION_TYPEMACRO_H__
#define __REFLECTION_TYPEMACRO_H__

#include "Type/TypeManager.h"

/**
 * @def		GENERATE( Class )
 * @brief	Injects essential reflection infrastructure into a class.
 * @details	This macro must be included inside the declaration of any class 
 * 			that intends to support the Reflection System.
 * 			It performs the following key tasks:
 * 			1. **Type Aliases**: Defines `SuperType` and `ThisType` to enable 
 * 				template meta-programming (SFINAE) for detecting inheritance relationships.
 * 			2. **Static Accessor**: Implements `GetStaticTypeInfo()` to allow retrieval 
 * 				of `TypeInfo` without an instance.
 * 			3. **Polymorphism Support**: Overrides the virtual `GetTypeInfo()` method, 
 * 				enabling retrieval of the correct child `TypeInfo` even from a `BasePtr`.
 * 			4. **Automatic Registration**: Instantiates a static `s_typeInfo` variable, 
 * 				forcing the class to be registered with the `TypeManager` at **Static Initialization Time**.
 * @param	Class The name of the current class (e.g., Monster).
 */
#define GENERATE( Class ) \
	public : \
		using SuperType = typename Reflection::Utils::TypeDetector<Class>::Type; \
		using ThisType = Class; \
\
		static const Reflection::TypeInfo* GetStaticTypeInfo() \
		{ \
			static const Reflection::TypeInfo* typeInfo = Reflection::TypeManager::GetHandle().GetTypeInfo<Class>(); \
			return typeInfo; \
		} \
\
		virtual const Reflection::TypeInfo* GetTypeInfo() const \
		{ \
			return GetStaticTypeInfo(); \
		} \
\
	private : \
		static inline const Reflection::TypeInfo* s_typeInfo = GetStaticTypeInfo(); \

#endif // __REFLECTIONTYPE_MACRO_H__