#ifndef __TYPE_CAST_H__
#define __TYPE_CAST_H__

#include "TypeUtils.h"
#include "TypeMacro.h"

namespace Type
{
    template<typename T, typename U>
    bool IsSame()
    {
        return (*T::GetStaticTypeInfo()) == (*U::GetStaticTypeInfo());
    }

    template<typename T, typename U>
    bool IsChild()
    {
        const MetaData::TypeInfo* super = U::GetStaticTypeInfo()->GetSuper();

        while(super != nullptr)
        {
            if (*T::GetStaticTypeInfo() == *super) 
            {
                break;
            }
            else
            {
                super = super->GetSuper();
            }
        }

        return super != nullptr;
    }

    template<typename T, typename U>
    T Cast(U Ptr)
    {
        using TType = Utils::RemovePointer_t<T>;
        using UType = Utils::RemovePointer_t<U>;

        if ( Ptr == nullptr )
        {
            return nullptr;
        }

        if ( IsSame<TType, UType>() )
        {
            return reinterpret_cast<T>( Ptr );
        }

        if ( IsChild<TType, UType>() )
        {
            return reinterpret_cast<T>( Ptr );
        }

        if ( TType::GetStaticTypeInfo() == Ptr->GetTypeInfo() )
        {
            return reinterpret_cast<T>( Ptr );
        }

        return nullptr;
    }
};

#endif // __TYPE_CAST_H__