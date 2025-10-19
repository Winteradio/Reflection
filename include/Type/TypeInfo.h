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

	class TypeInfo
	{
		public :
			using PropertyMap = std::unordered_map<std::string, const PropertyInfo*>;
			using MethodMap = std::unordered_map<std::string, const MethodInfo*>;

		public:
			template<typename T>
			using ValidSuper = typename Utils::IsEnabled<!Utils::IsSame<typename T::SuperType, void>::value>::Type;

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
			template<typename T>
			explicit TypeInfo(const Initializer<T>& initializer)
				: m_superType(initializer.superType)
				, m_typeHash(initializer.typeHash)
				, m_typeName(initializer.typeName)
				, m_properties()
				, m_methods()
			{}

			bool operator==(const TypeInfo& other) const;

		public :
			const TypeInfo* GetSuperType() const;
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
			const size_t		m_typeHash;
			const std::string	m_typeName;

			PropertyMap			m_properties;
			MethodMap			m_methods;
	};
};

#endif // __REFLECTION_TYPEINFO_H__
