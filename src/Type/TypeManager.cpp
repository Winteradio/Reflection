#include "Type/TypeManager.h"

#include "Type/TypeInfo.h"

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

	void TypeManager::Regist(const TypeInfo* typeInfo)
	{
		if (nullptr == typeInfo)
		{
			return;
		}

		std::lock_guard<std::mutex> lock(m_mutex);
		
		m_typeMap.insert({typeInfo->GetTypeHash(), typeInfo});
	}

	const TypeManager::TypeMap& TypeManager::GetTypeMap() const
	{
		return m_typeMap;
	}
}