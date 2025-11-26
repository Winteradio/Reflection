#ifndef __REFLECTION_TYPEINFO_H__
#define __REFLECTION_TYPEINFO_H__

#include <typeinfo>
#include <unordered_map>
#include <string>

#include "Utils.h"

namespace Reflection
{
	class PropertyInfo;
	class MethodInfo;

	/**
	 * @class	TypeInfo
	 * @brief	Class that holds type information in the reflection system.
	 * 			Provides functionality to access type metadata, properties, and methods.
	 */
	class TypeInfo
	{
		public :
			/**
			 * @brief	Template struct for initializing TypeInfo instances.
			 * @tparam	T The type for which to create TypeInfo.
			 */
			using PropertyMap = std::unordered_map<std::string, const PropertyInfo*>;

			/**
			 * @brief	Template struct for initializing TypeInfo instances.
			 * @tparam	T The type for which to create TypeInfo.
			 */
			using MethodMap = std::unordered_map<std::string, const MethodInfo*>;

		public:
			/**
			 * @brief	Template type to validate if T has a SuperType defined.
			 * @tparam	T The type to check for SuperType.
			 */
			template<typename T>
			using ValidSuper = typename Utils::IsEnabled<!Utils::IsSame<typename T::SuperType, void>::value>::Type;

			/**
			 * @brief	Template struct for initializing TypeInfo instances.
			 * @tparam	T The type for which to create TypeInfo.
			 * @tparam	U An optional parameter to detect SuperType.
			 */
			template<typename T, typename U = void>
			struct Initializer
			{
				const TypeInfo* superType;
				const size_t typeHash;
				const std::string typeName;

				Initializer()
					: superType(nullptr)
					, typeHash(typeid(T).hash_code())
					, typeName(typeid(T).name())
				{}
			};

			/**
			 * @brief	Template struct specialization for types with a SuperType defined.
			 * @tparam	T The type for which to create TypeInfo.
			 */
			template<typename T>
			struct Initializer<T, ValidSuper<T>>
			{
				const TypeInfo* superType;
				const size_t typeHash;
				const std::string typeName;

				Initializer()
					: superType(T::SuperType::GetStaticTypeInfo())
					, typeHash(typeid(T).hash_code())
					, typeName(typeid(T).name())
				{}
			};

		public :
			/**
			 * @brief	Constructor for TypeInfo.
			 * @tparam	T The type for which to create TypeInfo.
			 * @param	initializer The initializer containing type metadata.
			 */
			template<typename T>
			explicit TypeInfo(const Initializer<T>& initializer)
				: m_superType(initializer.superType)
				, m_typeHash(initializer.typeHash)
				, m_typeName(initializer.typeName)
				, m_properties()
				, m_methods()
			{
				if constexpr (Utils::IsPointer<T>::value || Utils::IsReference<T>::value || Utils::IsConst<T>::value)
				{
					m_pureType = TypeManager::GetHandle().GetTypeInfo<Utils::PureType_t<T>>();
				}
				else
				{
					m_pureType = this;
				}
			}

			bool operator==(const TypeInfo& other) const;

		public :
			const TypeInfo* GetSuperType() const;
			const TypeInfo* GetPureType() const;
			const size_t GetTypeHash() const;
			const std::string& GetTypeName() const;

			void AddProperty(const PropertyInfo* property);
			void AddMethod(const MethodInfo* method);

			const PropertyInfo* GetProperty(const std::string& name) const;
			const MethodInfo* GetMethod(const std::string& name) const;

			const PropertyMap& GetProperties() const;
			const MethodMap& GetMethods() const;

		private :
			const TypeInfo*		m_superType;
			const TypeInfo*		m_pureType;
			const size_t		m_typeHash;
			const std::string	m_typeName;

			PropertyMap			m_properties;
			MethodMap			m_methods;
	};
};

#endif // __REFLECTION_TYPEINFO_H__
