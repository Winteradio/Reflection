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
			 * @brief	Retrieves the TypeInfo instance for a given type T (Self-Registration API).
			 * @details	This function implements the **Self-Registration Pattern** by defining 
			 * 			a static TypeInfo object internally. 
			 * 			The constructor of this static object ensures the type is registered with 
			 * 			the TypeManager at static initialization time (Lazy Initialization).
			 * 			The function guarantees **runtime immutability** by always returning 
			 * 			a constant pointer (`const TypeInfo*`) to the single, static instance.
			 * @tparam	T The type for which to retrieve the TypeInfo.
			 * @return	const TypeInfo* A pointer to the unique, read-only TypeInfo instance.
			 */
			template<typename T>
			static const TypeInfo* Get()
			{
				static TypeInfo::Initializer<T> initializer;
				static TypeInfo typeInfo(initializer);

				return &typeInfo;
			}

		public :
			/**
			 * @brief	Constructor for TypeInfo.
			 * @tparam	T The type for which to create TypeInfo.
			 * @param	initializer The initializer containing type metadata.
			 */
			template<typename T>
			explicit TypeInfo(const Initializer<T>& initializer)
				: m_properties()
				, m_methods()
				, m_typeName(initializer.typeName)
				, m_typeHash(initializer.typeHash)
				, m_superType(initializer.superType)
				, m_pureType(this)
			{
				if constexpr (Utils::IsPointer<T>::value || Utils::IsReference<T>::value || Utils::IsConst<T>::value)
				{
					m_pureType = TypeInfo::Get<Utils::PureType_t<T>>();
				}

				Regist();
			}

			bool operator==(const TypeInfo& other) const;

		public :
			const PropertyMap& GetProperties() const;
			const MethodMap& GetMethods() const;

			const PropertyInfo* GetProperty(const std::string& name) const;
			const MethodInfo* GetMethod(const std::string& name) const;

			/**
			 * @brief	[Internal Use Only] Registers a property into this TypeInfo instance.
			 * @details This function is designed to be called **only during the static initialization phase** * 
			 * 			(via the owner's PropertyInfo constructor), 
			 * 			where the TypeInfo object is temporarily mutable for setup. 
			 * 			**Note: It does not check for constness.**
			 * @param	property The property information pointer.
			 */
			void AddProperty(const PropertyInfo* property);

			/**
			 * @brief	[Internal Use Only] Registers a method into this TypeInfo instance.
			 * @details Similar to AddProperty, this is called **only during the static initialization phase**. 
			 * @param	method The method information pointer.
			 */
			void AddMethod(const MethodInfo* method);

			const std::string& GetTypeName() const;
			size_t GetTypeHash() const;
			const TypeInfo* GetSuperType() const;
			const TypeInfo* GetPureType() const;

		private:
			/**
			 * @brief	Registers the TypeInfo instance with the global TypeManager.
			 * @details This is called by the TypeInfo constructor during static initialization.
			 */
			void Regist();

		private :
			PropertyMap			m_properties;
			MethodMap			m_methods;

			const std::string	m_typeName;
			const size_t		m_typeHash;
			const TypeInfo*		m_superType;
			const TypeInfo*		m_pureType;
	};
};

#endif // __REFLECTION_TYPEINFO_H__