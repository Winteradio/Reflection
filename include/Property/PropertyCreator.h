#ifndef __REFLECTION_PROPERTYCREATOR_H__
#define __REFLECTION_PROPERTYCREATOR_H__

#include "Property/PropertyInfo.h"
#include "Property/ContainerPropertyInfo.h"

namespace Reflection
{
	template<typename Property, Property property>
	struct PropertyCreator;

	template<typename Class, typename Property, Property Class::* address>
	struct PropertyCreator<Property Class::*, address>
	{
		PropertyInfo& Create()
		{

		}
	};
};

#endif // __REFLECTION_PROPERTYCREATOR_H__