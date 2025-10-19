#include "Type/TypeInfo.h"

#include "Property/PropertyInfo.h"
#include "Method/MethodInfo.h"

namespace Reflection
{
	bool TypeInfo::operator==(const TypeInfo& other) const
	{
		if (m_typeHash != other.m_typeHash)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	
	const TypeInfo* TypeInfo::GetSuperType() const
	{
		return m_superType;
	}

	const size_t TypeInfo::GetTypeHash() const
	{
		return m_typeHash;
	}

	const std::string& TypeInfo::GetTypeName() const
	{
		return m_typeName;
	}

	void TypeInfo::AddProperty(const PropertyInfo* property)
	{
		if (nullptr == property)
		{
			return;
		}

		m_properties[property->GetPropertyName()] = property;
	}

	void TypeInfo::AddMethod(const MethodInfo* method)
	{
		if (nullptr == method)
		{
			return;
		}

		m_methods[method->GetMethodName()] = method;
	}

	const PropertyInfo* TypeInfo::GetProperty(const std::string& name) const
	{
		auto it = m_properties.find(name);
		if (it != m_properties.end())
		{
			return it->second;
		}

		return nullptr;
	}

	const MethodInfo* TypeInfo::GetMethod(const std::string& name) const
	{
		auto it = m_methods.find(name);
		if (it != m_methods.end())
		{
			return it->second;
		}

		return nullptr;
	}

	const TypeInfo::PropertyMap& TypeInfo::GetProperties() const
	{
		return m_properties;
	}

	const TypeInfo::MethodMap& TypeInfo::GetMethods() const
	{
		return m_methods;
	}
}
