#ifndef __TYPE_UTILS_H__
#define __TYPE_UTILS_H__

#include "TypeInfo.h"

#include <string>
#include <atomic>

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
			static MetaData::TypeInfo Init(const std::string& _className, const size_t _typeIndex)
			{
				return MetaData::TypeInfo(_className, _typeIndex, T::GetStaticTypeInfo());
			};
		};

		template<>
		struct TypeInitializer<void>
		{
			static MetaData::TypeInfo Init(const std::string& _className, const size_t _typeIndex)
			{
				return MetaData::TypeInfo(_className, _typeIndex);
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

		const std::string ConvertToType(const char* _pFuncSignature);

		extern std::atomic<size_t> GLOBAL_TYPE_INDEX;
	};
};
#endif // __TYPE_UTILS_H__