#include "TypeManager.h"

namespace Type
{
	Manager::Manager()
	{}

	Manager::~Manager()
	{}

	Manager& Manager::GetHandle()
	{
		return TYPE_MANAGER;
	}

	Manager Manager::TYPE_MANAGER;
};