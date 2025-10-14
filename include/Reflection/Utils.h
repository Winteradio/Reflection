#ifndef __REFLECTION_UTILS_H__
#define __REFLECTION_UTILS_H__

#include <string>
#include <atomic>

namespace Reflection
{
	namespace Utils
	{
		template<typename T, typename U>
		struct IsSame
		{
			static constexpr bool value = false;
		};

		template<typename T>
		struct IsSame<T, T>
		{
			static constexpr bool value = true;
		};

		template<bool T>
		struct IsEnabled;

		template<bool T>
		using IsEnabled_t = typename IsEnabled<T>::Type;

		template<typename...>
		struct TypeWrapper
		{
			using Type = void;
		};

		template<typename T, typename U = void>
		struct TypeDetector
		{
			using Type = void;
		};

		template<typename T>
		struct TypeDetector<T, typename TypeWrapper<typename T::ThisType>::Type>
		{
			using Type = typename T::ThisType;
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
	};
};
#endif // __REFLECTION_UTILS_H__