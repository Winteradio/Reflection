#ifndef __REFLECTION_PROPERTYINFO_H__
#define __REFLECTION_PROPERTYINFO_H__

#include <string>

#include "Type/TypeCast.h"

namespace Reflection
{
	class PropertyInfo
	{
		public :
			template<typename Type, typename Property>
			struct Initializer
			{
				TypeInfo* ownerType;
				TypeInfo* propertyType;
				const size_t propertyOffset;

				Initializer(const size_t offset)
					: ownerType(TypeManager::GetHandle().Register<Type>())
					, propertyType(TypeManager::GetHandle().Register<Property>())
					, propertyOffset(offset)
				{}
			};

		public :
			template<typename Type, typename Property>
			explicit PropertyInfo(const Initializer<Type, Property>& initializer, const std::string& propertyName)
				: m_ownerType(initializer.ownerType)
				, m_propertyType(initializer.propertyType)
				, m_propertyOffset(initializer.propertyOffset)
				, m_propertyName(propertyName)
			{
				if (nullptr != initializer.ownerType)
				{
					initializer.ownerType->AddProperty(this);
				}
			}

			bool operator==(const PropertyInfo& other) const;

		public :
			template<typename T, typename U>
			Utils::RemovePointer_t<T>* Get(U& instance) const
			{
				return const_cast<Utils::RemovePointer_t<T>*>(GetImpl<T>(instance));
			}

			template<typename T, typename U>
			const Utils::RemovePointer_t<T>* Get(const U& instance) const
			{
				return GetImpl<T>(instance);
			}

			template<typename T, typename U>
			void Set(U& instance, const T& value) const
			{
				const TypeInfo* otherType = TypeManager::GetHandle().GetTypeInfo<T>();
				if (!IsSame(m_propertyType, otherType))
				{
					return;
				}

				char* base = reinterpret_cast<char*>(&instance);
				char* address = base + m_propertyOffset;

				T& property = *reinterpret_cast<T*>(address);
				property = value;
			}

		private :
			template<typename T, typename U>
			const Utils::RemovePointer_t<T>* GetImpl(const U& instance) const
			{
				const TypeInfo* otherType = TypeManager::GetHandle().GetTypeInfo<T>();
				if (!IsSame(m_propertyType, otherType))
				{
					return nullptr;
				}

				const char* base = reinterpret_cast<const char*>(&instance);
				const char* address = base + m_propertyOffset;

				if constexpr (Utils::IsPointer<T>::value)
				{
					const T pointer = *reinterpret_cast<const T*>(address);

					return pointer;
				}
				else
				{
					const T& instance = *reinterpret_cast<const T*>(address);

					return &instance;
				}
			}

		public :
			const TypeInfo* GetOwnerType() const;
			const TypeInfo* GetPropertyType() const;
			const size_t GetPropertyOffset() const;
			const std::string& GetPropertyName() const;

		private :
			const TypeInfo* m_ownerType;
			const TypeInfo* m_propertyType;
			const size_t m_propertyOffset;
			const std::string m_propertyName;
	};
};

#endif // __REFLECTION_PROPERTYINFO_H__