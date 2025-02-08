#ifndef __TYPE_MACRO_H__
#define __TYPE_MACRO_H__

#include "TypeUtils.h"

#if defined(_MSC_VER)
	#define FUNCSIG __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
	#define FUNCSIG __PRETTY_FUNCTION__
#else
	#define FUNCSIG __func__
#endif

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
            static MetaData::TypeInfo typeinfo(Type::Utils::TypeInitializer<SuperType>::Init(Type::Utils::ConvertToType(FUNCSIG))); \
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