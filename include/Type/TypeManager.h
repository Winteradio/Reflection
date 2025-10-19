#ifndef __REFLECTION_TYPEMANAGER_H__
#define __REFLECTION_TYPEMANAGER_H__

#include <unordered_map>

#include "Type/TypeInfo.h"

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
			const TypeInfo* GetTypeInfo()
			{
				const size_t typeHash = typeid(T).hash_code();

				auto itr = m_typeMap.find(typeHash);
				if (itr != m_typeMap.end())
				{
					return itr->second;
				}
				else
				{
					return Register<T>();
				}
			}

		private:
			const std::string GetTypeName(const char* name);

		private :
			TypeMap m_typeMap;
	};
};

#endif // __REFLECTION_TYPEMANAGER_H__