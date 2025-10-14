#ifndef __REFLECTION_TYPEMANAGER_H__
#define __REFLECTION_TYPEMANAGER_H__

#include <unordered_map>
#include <string>

#include "Reflection/TypeInfo.h"

namespace Reflection
{
	class TypeManager
	{
		private : 
			using TypeMap = std::unordered_map<size_t, const TypeInfo*>;

			TypeManager();
			~TypeManager();

		public :
			static TypeManager& GetHandle();

			const TypeMap& GetTypeMap() const;

		public:
			template<typename T>
			TypeInfo* Register()
			{
				static TypeInfo::Initializer<T> initializer;
				static TypeInfo typeInfo(initializer);

				m_typeMap[typeInfo.GetTypeHash()] = &typeInfo;

				return &typeInfo;
			}

			template<typename T>
			TypeInfo* GetTypeInfo()
			{
				const std::string strTypeName = typeid(T).name();

				const auto itr = m_typeMap.find(strTypeName);
				if (itr != m_typeMap.end())
				{
					return itr->second;
				}
				else
				{
					return Register<T>();
				}
			}

		private :
			TypeMap m_typeMap;
	};
};

#endif // __REFLECTION_TYPEMANAGER_H__