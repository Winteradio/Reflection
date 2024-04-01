#ifndef __TYPEMACRO_H__
#define __TYPEMACRO_H__

#include <type_traits>

#include "TypeInfo.h"

#define GENERATE( className ) \
    private : \
        friend SuperClassTypeDeduction; \
\
    public : \
        using SuperType = SuperClassTypeDeduction<className>::Type; \
        using ThisType = className; \
\
        static TypeInfo& GetStaticTypeInfo() \
        { \
            static TypeInfo typeinfo( TypeInfoInitializer<SuperType>::Init( #className ) ); \
            return typeinfo; \
        } \
\
        virtual TypeInfo& GetTypeInfo()\
        { \
            return m_TypeInfo; \
        } \
\
    private : \
        static inline TypeInfo& m_TypeInfo = GetStaticTypeInfo(); \
\

template< typename T, typename U = void >
struct SuperClassTypeDeduction
{
    using Type = void;
};

template< typename T >
struct SuperClassTypeDeduction< T, std::void_t<typename T::ThisType>>
{
    using Type = T::ThisType;
};

template< typename Super >
struct TypeInfoInitializer
{
    static TypeInfo Init( const char* className )
    {
        return TypeInfo( className, Super::GetStaticTypeInfo() );
    };
};

template<>
struct TypeInfoInitializer< void >
{
    static TypeInfo Init( const char* className )
    {
        return TypeInfo( className );
    }; 
};

#endif // __TYPEINFO_H__