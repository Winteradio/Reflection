#include "Reflection/TypeManager.h"

namespace Reflection
{
	TypeManager::TypeManager()
		: m_typeMap()
	{}

	TypeManager::~TypeManager()
	{
		m_typeMap.clear();
	}

	TypeManager& TypeManager::GetHandle()
	{
		static TypeManager typeManager;
		return typeManager;
	}

	const TypeManager::TypeMap& TypeManager::GetTypeMap() const
	{
		return m_typeMap;
	}
}