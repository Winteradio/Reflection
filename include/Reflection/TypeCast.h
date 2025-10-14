#ifndef __REFLECTION_CAST_H__
#define __REFLECTION_CAST_H__

#include "Reflection/Utils.h"
#include "Reflection/TypeInfo.h"

namespace Reflection
{
    template<typename T, typename U>
    bool IsSame()
    {
        const TypeInfo* TType = T::GetStaticTypeInfo();
        const TypeInfo* UType = U::GetStaticTypeInfo();

        if ((TType == nullptr) || (UType == nullptr))
        {
            return false;
        }
        else
        {
            return (*TType) == (*UType);
        }
    }

    template<typename T, typename U>
    bool IsChild()
    {
        const TypeInfo* TTypeInfo = T::GetStaticTypeInfo();
        const TypeInfo* UTypeInfo = U::GetStaticTypeInfo();

        while ()
    }
}
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
        const TypeInfo* TType = T::GetStaticTypeInfo();
        const TypeInfo* UType = U::GetStaticTypeInfo();

        if ((TType == nullptr) || (UType == nullptr))
        {
            return false;
        }

        const TypeInfo* superType = nullptr;

        do
        {
            superType = UType->GetSuperType();

            if ((superType != nullptr) && (*TType == *superType))
            {
                return true;
            }
        }
        while (superType != nullptr);

        return false;
    }

    template<typename T, typename U>
    T Cast(U pointer)
    {
        using TType = Utils::RemovePointer_t<T>;
        using UType = Utils::RemovePointer_t<U>;

        if (pointer == nullptr)
        {
            return nullptr;
        }

        if ((true == IsSame<TType, UType>()) ||
            (true == IsChild<TType, UType>()) ||
            (pointer->GetTypeInfo() == TType::GetStaticTypeInfo()))
        {
            return reinterpret_cast<T>(pointer);
        }

        return nullptr;
    }
};

#endif // __REFLECTION_CAST_H__