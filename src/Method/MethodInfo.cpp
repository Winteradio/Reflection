#include "Method/MethodInfo.h"

namespace Reflection
{
	bool MethodInfo::operator==(const MethodInfo& other) const
	{
		if (m_ownerType != other.m_ownerType)
		{
			return false;
		}
		else if (m_methodType != other.m_methodType)
		{
			return false;
		}
		else if (m_methodName != other.m_methodName)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	const TypeInfo* MethodInfo::GetOwnerType() const
	{
		return m_ownerType;
	}

	const TypeInfo* MethodInfo::GetMethodType() const
	{
		return m_methodType;
	}

	const std::string& MethodInfo::GetMethodName() const
	{
		return m_methodName;
	}
}