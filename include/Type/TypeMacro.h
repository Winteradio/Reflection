#ifndef __REFLECTION_TYPEMACRO_H__
#define __REFLECTION_TYPEMACRO_H__

#include "Type/TypeManager.h"

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