#include "Property/PropertyInfo.h"

namespace Reflection
{
	bool PropertyInfo::operator==(const PropertyInfo& other) const
	{
		if (m_ownerType != other.m_ownerType)
		{
			return false;
		}
		else if (m_propertyType != other.m_propertyType)
		{
			return false;
		}
		else if (m_propertyOffset != other.m_propertyOffset)
		{
			return false;
		}
		else if (m_propertyName != other.m_propertyName)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	const TypeInfo* PropertyInfo::GetOwnerType() const
	{
		return m_ownerType;
	}

	const TypeInfo* PropertyInfo::GetPropertyType() const
	{
		return m_propertyType;
	}

	const size_t PropertyInfo::GetPropertyOffset() const
	{
		return m_propertyOffset;
	}

	const std::string& PropertyInfo::GetPropertyName() const
	{
		return m_propertyName;
	}
}