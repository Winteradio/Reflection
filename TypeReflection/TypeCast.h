#ifndef __TYPECAST_H__
#define __TYPECAST_H__

#include "TypeMacro.h"

template< typename To, typename From >
bool IsSame()
{
    return To::GetStaticTypeInfo() == From::GetStaticTypeInfo();
}

template< typename To, typename From >
bool IsChild()
{
    TypeInfo* super = From::GetStaticTypeInfo().GetSuper();

    while( super != nullptr )
    {
        if ( To::GetStaticTypeInfo() == *super ) break;

        super = super->GetSuper();
    }

    return super != nullptr;
}

template< typename To, typename From >
To TypeCast( From Ptr )
{
    if ( Ptr == nullptr )
    {
        return nullptr;
    }

    using ToType = std::remove_pointer<To>::type;
    using FromType = std::remove_pointer<From>::type;

    if ( IsSame<ToType,FromType>() )
    {
        return reinterpret_cast< To >( Ptr );
    }

    if ( IsChild<ToType,FromType>() )
    {
        return reinterpret_cast< To >( Ptr );
    }

    if ( ToType::GetStaticTypeInfo() == Ptr->GetTypeInfo() )
    {
        return reinterpret_cast< To >( Ptr );
    }

    return nullptr;
}

#endif // __TYPECAST_H__