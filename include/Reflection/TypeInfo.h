#ifndef __REFLECTION_TYPEINFO_H__
#define __REFLECTION_TYPEINFO_H__

#include <typeinfo>
#include <string>

#include "Reflection/Utils.h"

namespace Reflection
{
	class TypeInfo
	{
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
			{}

			bool operator==(const TypeInfo& other) const;

		public :
			const TypeInfo* GetSuperType() const;
			const size_t GetTypeHash() const;
			const std::string& GetTypeName() const;

			void AddProperty();
			void AddMethod();

		private :
			const TypeInfo*		m_superType;
			const size_t		m_typeHash;
			const std::string	m_typeName;
	};
};

#endif // __REFLECTION_TYPEINFO_H__
