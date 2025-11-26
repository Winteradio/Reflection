#ifndef __REFLECTION_CAST_H__
#define __REFLECTION_CAST_H__

#include "Utils.h"
#include "Type/TypeManager.h"

namespace Reflection
{
	/**
	 * @brief	Compares whether two TypeInfo pointers represent the same Raw Type.
	 * @details	This function checks for strict equality by comparing the TypeHash of Raw Types,
	 *			including pointer, reference, and const qualifiers.
	 * @param	lhsType	The left TypeInfo pointer.
	 * @param	rhsType	The right TypeInfo pointer.
	 * @return	bool True if the two TypeInfos are identical, false otherwise.
	 */
	bool IsSame(const TypeInfo* lhsType, const TypeInfo* rhsType);

	/**
	 * @brief	Checks if the TypeInfo of the two template types T and U are identical.
	 * @tparam	T The first type to compare.
	 * @tparam	U The second type to compare.
	 * @return	bool True if the TypeInfos of the two types are identical.
	 */
	bool IsChild(const TypeInfo* parentType, const TypeInfo* childType);

	/**
	 * @brief	Checks if the TypeInfo of the two template types T and U are identical.
	 * @tparam	T The first type to compare.
	 * @tparam	U The second type to compare.
	 * @return	bool True if the TypeInfos of the two types are identical.
	 */
	template<typename T, typename U>
	bool IsSame()
	{
		const TypeInfo* TType = TypeManager::GetHandle().GetTypeInfo<T>();
		const TypeInfo* UType = TypeManager::GetHandle().GetTypeInfo<U>();

		return IsSame(TType, UType);
	}

	/**
	 * @brief	Confirms the inheritance relationship to see if T is a parent of U.
	 * @tparam	T The type to check as the parent (Parent Type).
	 * @tparam	U The type to check as the child (Child Type).
	 * @return	bool True if T is the superior type of U.
	 */
	template<typename T, typename U>
	bool IsChild()
	{
		const TypeInfo* TType = TypeManager::GetHandle().GetTypeInfo<T>();
		const TypeInfo* UType = TypeManager::GetHandle().GetTypeInfo<U>();

		return IsChild(TType, UType);
	}

	/**
	 * @brief	Performs safe casting between pointer types using the reflection system.
	 * @details	This function checks for inheritance and actual runtime type to ensure casting safety,
	 * 			primarily enhancing and replacing dynamic_cast functionality.
	 * @tparam	T The target pointer type to cast to (Output Type, e.g., ObjectA*).
	 * @tparam	U The input pointer type provided (Input Type, e.g., ObjectB*).
	 * @param	pointer The address of the object to be cast.
	 * @return	T Returns a pointer of the target type T if the cast is valid, and nullptr if it fails.
	 */
	template<typename T, typename U>
	T Cast(U pointer)
	{
		// [Compile-time check] The output type T and input type U must both be pointer types.
		static_assert(Utils::IsPointer<T>::value && Utils::IsPointer<U>::value,
					"Reflection::Cast<T, U> : The T and U must be pointer type.");

		// Define the Pure Types (remove pointers).
		using TType = Utils::RemovePointer_t<T>;
		using UType = Utils::RemovePointer_t<U>;

		// Check the input pointer is valid.
		if (pointer == nullptr)
		{
			return nullptr;
		}

		// [Stage 1: Compile-time / Static Casting Check]
	    // Check if the Pure Types are identical or if the cast is a safe compile-time upcast (UType -> TType).
		if (IsSame<TType, UType>() || IsChild<TType, UType>())
		{
			return static_cast<T>(pointer);
		}

		// [Stage 2: Runtime / Dynamic Casting Check]
    	// The static check failed, now perform a dynamic check against the actual runtime object type.

		// Get TypeInfo for the target type (T)
		const TypeInfo* outputType = TypeManager::GetHandle().GetTypeInfo<TType>();
		
		// Get the TypeInfo of the actual instance that the pointer is pointing to.
		const TypeInfo* inputType = pointer->GetTypeInfo();

		// Check if the actual instance type (inputType) is the same as the target type (outputType)
    	// OR if the actual instance type is a child of the target type (safe downcasting).
		if (IsSame(outputType, inputType) || IsChild(outputType, inputType))
		{
			return static_cast<T>(pointer);
		}

		// All validation failed, return nullptr.		
		return nullptr;
	}
};

#endif // __REFLECTION_CAST_H__