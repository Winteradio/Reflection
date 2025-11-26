#ifndef __REFLECTION_PROPERTYMACRO_H__
#define __REFLECTION_PROPERTYMACRO_H__

#include "Property/PropertyInfo.h"

/**
 * @brief	Registers a property into the Reflection System.
 * @details	This macro creates a static struct instance. The constructor of this struct 
 * 			runs at **Static Initialization Time** (before main()), effectively registering 
 * 			the property information into the TypeManager automatically.
 * 			It also statically asserts that the property is not a reference type, 
 * 			as pointers to references are not valid in C++.
 * @param	Property The member variable name to register.
 */
#define PROPERTY( Property ) \
		struct RegisterProperty##Property \
		{ \
			RegisterProperty##Property() \
			{ \
				static_assert(!Reflection::Utils::IsReference<decltype(ThisType::Property)>::value, "Reflection::PROPERTY : The property cannot be a reference type."); \
				\
				static Reflection::PropertyInfo::Initializer<ThisType, decltype(ThisType::Property)> initializer(offsetof(ThisType, Property)); \
				static Reflection::PropertyInfo propertyInfo(initializer, std::string(#Property)); \
			}; \
		}; \
		\
		static inline const RegisterProperty##Property s_registerProperty##Property; \

#endif // __REFLECTION_PROPERTYMACRO_H__