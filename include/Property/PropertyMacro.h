#ifndef __REFLECTION_PROPERTYMACRO_H__
#define __REFLECTION_PROPERTYMACRO_H__

#include "Property/PropertyInfo.h"

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