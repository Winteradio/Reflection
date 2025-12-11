#ifndef __REFLECTION_PROPERTYCREATOR_H__
#define __REFLECTION_PROPERTYCREATOR_H__

#include "Property/PropertyInfo.h"
#include "Property/ContainerPropertyInfo.h"
#include "Utils.h"

namespace Reflection
{
	template<typename Pointer, Pointer pointer, typename = void>
	struct PropertyCreator
	{
		static const PropertyInfo* Create(const std::string& propertyName)
		{
			using Class = typename Utils::MemberTraits<Pointer>::ClassType;
			using Property = typename Utils::MemberTraits<Pointer>::PropertyType;

			static size_t offset = reinterpret_cast<size_t>(&(static_cast<Class*>(nullptr)->*pointer));
			static PropertyInfo::Initializer<Class, Property> initializer(offset);
			static Reflection::PropertyInfo propertyInfo(initializer, propertyName);

			return &propertyInfo;
		}
	};

	template<typename Class, typename Property, Property Class::* pointer>
	struct PropertyCreator<Property Class::*, pointer, typename Utils::IsEnabled_t<Utils::IsArray<Property>>>
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
	struct PropertyCreator<Property Class::*, pointer, typename Utils::IsEnabled_t<Utils::IsSet<Property>>>
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
	struct PropertyCreator<Property Class::*, pointer, typename Utils::IsEnabled_t<Utils::IsMap<Property>>>
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