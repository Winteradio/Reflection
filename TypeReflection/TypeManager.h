#ifndef __TYPE_MANAGER_H__
#define __TYPE_MANAGER_H__

namespace Type
{
	class Manager
	{
		private :
			Manager();
			~Manager();

		public :
			static Manager& GetHandle();

		private :
			static Manager TYPE_MANAGER;
	};
};

#define GET_TYPE_MANAGER Type::Manager::GetHandle();

#endif // __TYPE_MANAGER_H__