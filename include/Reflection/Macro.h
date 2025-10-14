#ifndef __REFLECTION_MACRO_H__
#define __REFLECTION_MACRO_H__

#include "Reflection/TypeManager.h"
#include "Reflection/Utils.h"

#define GENERATE( Class ) \
    public : \
        using SuperType = Reflection::Utils::TypeDetector<Class>::Type; \
        using ThisType = Class; \
\
        static const Reflection::TypeInfo* GetStaticTypeInfo() \
        { \
            static Reflection::TypeInfo* typeInfo = Reflection::TypeManager::GetHandle().Register<Class>(); \
            return typeInfo; \
        } \
\
        virtual const Reflection::TypeInfo* GetTypeInfo() \
        { \
            return GetStaticTypeInfo(); \
        } \
\
    private : \
        static inline const Reflection::TypeInfo* s_typeInfo = GetStaticTypeInfo(); \

#define PROPERTY( Property ) \

#define METHOD( Method ) \

#endif // __REFLECTION_MACRO_H__