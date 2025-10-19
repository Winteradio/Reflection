#ifndef __REFLECTION_CAST_H__
#define __REFLECTION_CAST_H__

#include "Utils.h"
#include "Type/TypeManager.h"

namespace Reflection
{
    bool IsSame(const TypeInfo* lhsType, const TypeInfo* rhsType);
    bool IsChild(const TypeInfo* lhsType, const TypeInfo* rhsType);

    template<typename T, typename U>
    bool IsSame()
    {
        const TypeInfo* TType = TypeManager::GetHandle().GetTypeInfo<T>();
        const TypeInfo* UType = TypeManager::GetHandle().GetTypeInfo<U>();

        return IsSame(TType, UType);
    }

    template<typename T, typename U>
    bool IsChild()
    {
        const TypeInfo* TType = TypeManager::GetHandle().GetTypeInfo<T>();
        const TypeInfo* UType = TypeManager::GetHandle().GetTypeInfo<U>();

        return IsChild(TType, UType);
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

        if (true == IsSame<TType, UType>())
        {
            return static_cast<T>(pointer);
        }

        if (true == IsChild<TType, UType>())
        {
            return static_cast<T>(pointer);
        }

        const TypeInfo* typeInfo = TypeManager::GetHandle().GetTypeInfo<TType>();
        const TypeInfo* pointerInfo = pointer->GetTypeInfo();
        if (IsSame(typeInfo, pointerInfo) || IsChild(typeInfo, pointerInfo))
        {
            return static_cast<T>(pointer);
        }

        return nullptr;
    }
};

#endif // __REFLECTION_CAST_H__