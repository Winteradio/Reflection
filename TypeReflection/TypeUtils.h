#ifndef __TYPE_UTILS_H__
#define __TYPE_UTILS_H__

#include "TypeInfo.h"

namespace Type
{
	namespace Utils
	{
		template<typename...>
		struct EmptyType
		{
			using Type = void;
		};

		template<typename... Args>
		using VOID_TYPE = typename EmptyType<Args...>::Type;

		template<typename T, typename U = void>
		struct SuperTypeDetection
		{
			using Type = void;
		};

		template<typename T>
		struct SuperTypeDetection<T, VOID_TYPE<typename T::ThisType>>
		{
			using Type = typename T::ThisType;
		};

		template<typename T>
		struct TypeInitializer
		{
			static MetaData::TypeInfo Init(const char* className )
			{
				return MetaData::TypeInfo( className, T::GetStaticTypeInfo() );
			};
		};

		template<>
		struct TypeInitializer<void>
		{
			static MetaData::TypeInfo Init(const char* className)
			{
				return MetaData::TypeInfo(className);
			}
		};

		template<typename T>
		struct RemovePointer
		{
			using Type = T;
		};

		template<typename T>
		struct RemovePointer<T*>
		{
			using Type = T;
		};

		template<typename T>
		struct RemovePointer<T* const>
		{
			using Type = T;
		};

		template<typename T>
		struct RemovePointer<T* volatile>
		{
			using Type = T;
		};

		template<typename T>
		struct RemovePointer<T* const volatile>
		{
			using Type = T;
		};

		template<typename T>
		using RemovePointer_t = typename RemovePointer<T>::Type;
	};
};
#endif // __TYPE_UTILS_H__