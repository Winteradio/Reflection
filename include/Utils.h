#ifndef __REFLECTION_UTILS_H__
#define __REFLECTION_UTILS_H__

#include <string>

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

		template<>
		struct IsEnabled<true>
		{
			using Type = void;
		};

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
		struct RemoveReference
		{
			using Type = T;
		};

		template<typename T>
		struct RemoveReference<T&>
		{
			using Type = T;
		};

		template<typename T>
		struct RemoveReference<const T&>
		{
			using Type = T;
		};

		template<typename T>
		struct RemoveReference<volatile T&>
		{
			using Type = T;
		};

		template<typename T>
		struct RemoveReference<const volatile T&>
		{
			using Type = T;
		};

		template<typename T>
		struct RemoveConst
		{
			using Type = T;
		};

		template<typename T>
		struct RemoveConst<const T>
		{
			using Type = T;
		};

		template<typename T>
		using RemovePointer_t = typename RemovePointer<T>::Type;

		template<typename T>
		using RemoveReference_t = typename RemoveReference<T>::Type;

		template<typename T>
		using RemoveConst_t = typename RemoveConst<T>::Type;

		template<typename T>
		using PureType_t = RemovePointer_t<RemoveReference_t<RemoveConst_t<T>>>;

		template<typename T>
		struct IsPointer
		{
			static constexpr bool value = !IsSame<T, RemovePointer<T>::Type>::value;
		};

		template<typename T>
		struct IsReference
		{
			static constexpr bool value = !IsSame<T, RemoveReference<T>::Type>::value;
		};

		template<typename T>
		struct IsConst
		{
			static constexpr bool value = !IsSame<T, RemoveConst<T>::Type>::value;
		};

		template<bool Condition, typename T, typename U>
		struct Conditional
		{
			using Type = T;
		};

		template<typename T, typename U>
		struct Conditional<false, T, U>
		{
			using Type = U;
		};
	};
};
#endif // __REFLECTION_UTILS_H__