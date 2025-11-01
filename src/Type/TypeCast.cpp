#include "Type/TypeCast.h"

namespace Reflection
{
	bool IsSame(const TypeInfo* lhsType, const TypeInfo* rhsType)
	{
		if ((lhsType == nullptr) || (rhsType == nullptr))
		{
			return false;
		}
		else
		{
			return *lhsType == *rhsType;
		}
	}

	bool IsChild(const TypeInfo* parentType, const TypeInfo* childType)
	{
		if ((parentType == nullptr) || (childType == nullptr))
		{
			return false;
		}

		const TypeInfo* superType = childType->GetSuperType();
		while (superType != nullptr)
		{
			if (*parentType == *superType)
			{
				return true;
			}

			superType = superType->GetSuperType();
		}

		return false;
	}
}