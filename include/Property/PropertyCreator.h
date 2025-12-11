#ifndef __REFLECTION_PROPERTYCREATOR_H__
#define __REFLECTION_PROPERTYCREATOR_H__

#include "Property/PropertyInfo.h"
#include "Property/ContainerPropertyInfo.h"
#include "Utils.h"

namespace Reflection
{
	template<typename T, typename = void>
	struct HasIterator
	{
		static constexpr bool value = false;
	};

	template<typename T>
	struct HasIterator<T, typename Utils::TypeWrapper<typename T::Iterator>::Type>
	{
		static constexpr bool value = true;
	};

	template<typename T, typename = void>
	struct HasKey
	{
		static constexpr bool value = false;
	};

	template<typename T>
	struct HasKey<T, typename Utils::TypeWrapper<typename T::KeyType>::Type>
	{
		static constexpr bool value = true;
	};

	template<typename T, typename = void>
	struct HasValue
	{
		static constexpr bool value = false;
	};

	template<typename T>
	struct HasValue<T, typename Utils::TypeWrapper<typename T::ValueType>::Type>
	{
		static constexpr bool value = true;
	};

	template<typename T>
	constexpr bool IsArray = HasIterator<T>::value && !HasKey<T>::value && !HasValue<T>::value;

	template<typename T>
	constexpr bool IsSet = HasIterator<T>::value && HasKey<T>::value && !HasValue<T>::value;

	template<typename T>
	constexpr bool IsMap = HasIterator<T>::value && HasKey<T>::value && HasValue<T>::value;

	template<typename T>
	struct MemberTraits;

	template<typename Class, typename Property>
	struct MemberTraits<Property Class::*>
	{
		using ClassType = Class;
		using PropertyType = Property;
	};

	template<typename Pointer, Pointer pointer, typename = void>
	struct PropertyCreator
	{
		static const PropertyInfo* Create(const std::string& propertyName)
		{
			using Class = typename MemberTraits<Pointer>::ClassType;
			using Property = typename MemberTraits<Pointer>::PropertyType;

			static size_t offset = reinterpret_cast<size_t>(&(static_cast<Class*>(nullptr)->*pointer));
			static PropertyInfo::Initializer<Class, Property> initializer(offset);
			static Reflection::PropertyInfo propertyInfo(initializer, propertyName);

			return &propertyInfo;
		}
	};

	template<typename Class, typename Property, Property Class::* pointer>
	struct PropertyCreator<Property Class::*, pointer, typename Utils::IsEnabled_t<IsArray<Property>>>
	{
		static const PropertyInfo* Create(const std::string& propertyName)
		{
			static size_t offset = reinterpret_cast<size_t>(&(static_cast<Class*>(nullptr)->*pointer));
			static ArrayPropertyInfo::Initializer<Class, Property> initializer(offset);
			static Reflection::ArrayPropertyInfo propertyInfo(initializer, propertyName);

			return &propertyInfo;
		}
	};

	template<typename Class, typename Property, Property Class::* pointer>
	struct PropertyCreator<Property Class::*, pointer, typename Utils::IsEnabled_t<IsSet<Property>>>
	{
		static const PropertyInfo* Create(const std::string& propertyName)
		{
			static size_t offset = reinterpret_cast<size_t>(&(static_cast<Class*>(nullptr)->*pointer));
			static SetPropertyInfo::Initializer<Class, Property> initializer(offset);
			static Reflection::SetPropertyInfo propertyInfo(initializer, propertyName);

			return &propertyInfo;
		}
	};

	template<typename Class, typename Property, Property Class::* pointer>
	struct PropertyCreator<Property Class::*, pointer, typename Utils::IsEnabled_t<IsMap<Property>>>
	{
		static const PropertyInfo* Create(const std::string& propertyName)
		{
			static size_t offset = reinterpret_cast<size_t>(&(static_cast<Class*>(nullptr)->*pointer));
			static MapPropertyInfo::Initializer<Class, Property> initializer(offset);
			static Reflection::MapPropertyInfo propertyInfo(initializer, propertyName);

			return &propertyInfo;
		}
	};
};

#endif // __REFLECTION_PROPERTYCREATOR_H__