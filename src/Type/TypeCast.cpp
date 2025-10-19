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

	bool IsChild(const TypeInfo* lhsType, const TypeInfo* rhsType)
	{
		if ((lhsType == nullptr) || (rhsType == nullptr))
		{
			return false;
		}

		const TypeInfo* superType = rhsType->GetSuperType();
		while (superType != nullptr)
		{
			if (*lhsType == *superType)
			{
				return true;
			}

			superType = superType->GetSuperType();
		}

		return false;
	}
}