#ifndef __REFLECTION_CONTAINERPROPERTYINFO_H__
#define __REFLECTION_CONTAINERPROPERTYINFO_H__

#include "Property/PropertyInfo.h"

namespace Reflection
{
	class ContainerPropertyInfo : public PropertyInfo
	{
		GENERATE(ContainerPropertyInfo);
	};

	class ArrayPropertyInfo : public ContainerPropertyInfo
	{
		GENERATE(ArrayPropertyInfo);
	};

	class SetPropertyInfo : public ContainerPropertyInfo
	{
		GENERATE(SetPropertyInfo);
	};

	class MapPropertyInfo : public ContainerPropertyInfo
	{
		GENERATE(MapPropertyInfo);
	};
};

#endif // __REFLECTION_CONTAINERPROPERTYINFO_H__