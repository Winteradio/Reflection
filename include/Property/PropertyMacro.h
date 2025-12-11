#ifndef __REFLECTION_PROPERTYMACRO_H__
#define __REFLECTION_PROPERTYMACRO_H__

#include "Property/PropertyInfo.h"
#include "Property/PropertyCreator.h"

/**
 * @brief	Registers a property into the Reflection System.
 * @details	This macro creates a static struct instance whose constructor runs at **Static Initialization Time** (before main()), 
 * 			automatically registering the property into the owner TypeInfo.
 * 			**Offset Calculation Detail:** 
 * 				The offset is calculated using `reinterpret_cast<size_t>(&(reinterpret_cast<ThisType*>(0)->Property))`.
 * 				This technique is used to safely compute the member offset for **non-standard-layout types** * 
 * 				(e.g., classes with virtual functions) where the standard `offsetof` macro is conditionally supported.
 * 			It also statically asserts that the property is not a reference type, as pointers to references are not valid in C++.
 * @param	Property The member variable name to register.
 */

#define PROPERTY( Property ) \
		struct RegisterProperty##Property \
		{ \
			RegisterProperty##Property() \
			{ \
				static_assert(!Reflection::Utils::IsReference<decltype(ThisType::Property)>::value, "Reflection::PROPERTY : The property cannot be a reference type."); \
				static std::string propertyName = std::string(#Property); \
				static const Reflection::PropertyInfo* property = Reflection::PropertyCreator<decltype(&ThisType::Property), &ThisType::Property>::Create(propertyName); \
				assert(nullptr != property && "The property is invalid."); \
			}; \
		}; \
		\
		static inline const RegisterProperty##Property s_registerProperty##Property; \

#endif // __REFLECTION_PROPERTYMACRO_H__