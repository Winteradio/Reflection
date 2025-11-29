#ifndef __REFLECTION_TYPEMANAGER_H__
#define __REFLECTION_TYPEMANAGER_H__

#include <unordered_map>
#include <mutex>

namespace Reflection
{
	class TypeInfo;

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

		public:
			/**
			 * @brief	Regist some type info.
			 * @param	The type info's pointer
			 */
			void Regist(const TypeInfo* typeInfo);

			/**
			 * @brief	Get the type info's that registed this instance.
			 * @return	The type info's map
			 */
			const TypeMap& GetTypeMap() const;

		private :
			TypeMap m_typeMap;
			std::mutex m_mutex;
	};
};

#endif // __REFLECTION_TYPEMANAGER_H__