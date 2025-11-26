#ifndef __REFLECTION_TYPEMANAGER_H__
#define __REFLECTION_TYPEMANAGER_H__

#include <unordered_map>

#include "Type/TypeInfo.h"

namespace Reflection
{
	/**
	 * @class	TypeManager
	 * @brief	Singleton class that manages type registrations and retrievals in the reflection system.
	 * 			Provides functionality to register types and access their TypeInfo instances.
	 */
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
			/**
			 * @brief	Registers a type T and returns its TypeInfo instance.
			 * @tparam	T The type to register.
			 * @return	TypeInfo* A pointer to the registered TypeInfo instance.
			 */
			template<typename T>
			TypeInfo* Register()
			{
				static TypeInfo::Initializer<T> initializer;
				static TypeInfo typeInfo(initializer);

				m_typeMap[typeInfo.GetTypeHash()] = &typeInfo;

				return &typeInfo;
			}

			/**
			 * @brief	Retrieves the TypeInfo instance for a registered type T.
			 * @tparam	T The type whose TypeInfo is to be retrieved.
			 * @return	const TypeInfo* A pointer to the TypeInfo instance of type T.
			 */
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