#ifndef __TYPE_MACRO_H__
#define __TYPE_MACRO_H__

#include "TypeUtils.h"

#define GENERATE( className ) \
    private : \
        friend Type::Utils::SuperTypeDetection; \
\
    public : \
        using SuperType = Type::Utils::SuperTypeDetection<className>::Type; \
        using ThisType = className; \
\
        static const MetaData::TypeInfo* GetStaticTypeInfo() \
        { \
            static MetaData::TypeInfo typeinfo(Type::Utils::TypeInitializer<SuperType>::Init(#className)); \
            return &typeinfo; \
        } \
\
        virtual const MetaData::TypeInfo* GetTypeInfo()\
        { \
            return TYPEINFO; \
        } \
\
    private : \
        static inline const MetaData::TypeInfo* TYPEINFO = GetStaticTypeInfo(); \
\

#endif // __TYPE_MACRO_H__