#ifndef __REFLECTION_METHODINFO_H__
#define __REFLECTION_METHODINFO_H__

#include <string>

#include "Type/TypeCast.h"
#include "Method/MethodCall.h"

namespace Reflection
{
	/**
	 * @class	MethodInfo
	 * @brief	Class that holds information about a method in the reflection system.
	 * 			Provides functionality to invoke the method and access its metadata.
	 */
	class MethodInfo
	{
		public :
			/**
			 * @brief	Template struct for initializing MethodInfo instances.
			 * @tparam	Type   The owner type of the method.
			 * @tparam	Method The method type.
			 */
			template<typename Type, typename Method>
			struct Initializer
			{
				const TypeInfo* ownerType;
				const TypeInfo* methodType;
				const MethodBase* methodBase;

				Initializer(const MethodBase* method)
					: ownerType(TypeInfo::Get<Type>())
					, methodType(TypeInfo::Get<Method>())
					, methodBase(method)
				{}
			};

		public :
			/**
			 * @brief	Constructor for MethodInfo.
			 * @tparam	Type   The owner type of the method.
			 * @tparam	Method The method type.
			 * @param	initializer The initializer containing method metadata.
			 * @param	methodName  The name of the method.
			 */
			template<typename Type, typename Method>
			explicit MethodInfo(const Initializer<Type, Method>& initializer, const std::string& methodName)
				: m_ownerType(initializer.ownerType)
				, m_methodType(initializer.methodType)
				, m_methodBase(initializer.methodBase)
				, m_methodName(methodName)
			{
				TypeInfo* ownerType = const_cast<TypeInfo*>(m_ownerType);
				if (nullptr != ownerType)
				{
					ownerType->AddMethod(this);
				}
			}

			bool operator==(const MethodInfo& other) const;

		public :
			/**
			 * @brief	Invoke the method on the given instance with the provided arguments.
			 * @tparam	Return The return type of the method.
			 * @tparam	Class  The class type of the method.
			 * @tparam	Args   The argument types of the method.
			 * @param	instance A reference to the instance on which to invoke the method.
			 * @param	args     The arguments to pass to the method.
			 * @return	Return The result of the method invocation.
			 */
			template<typename Return, typename Class, typename... Args>
			Return Invoke(Class& instance, Args... args) const
			{
				void* pointer = reinterpret_cast<void*>(&instance);

				const MethodCall<Return, Args...>* methodCall = Cast<const MethodCall<Return, Args...>*>(m_methodBase);

				if constexpr (Utils::IsSame<Return, void>::value)
				{
					if (nullptr != methodCall)
					{
						methodCall->Invoke(pointer, std::forward<Args>(args)...);						
					}
				}
				else
				{
					if (nullptr != methodCall)
					{
						return methodCall->Invoke(pointer, std::forward<Args>(args)...);						
					}
					else
					{
						static Return null;
						return null;
					}
				}
			}

		public :
			const TypeInfo* GetOwnerType() const;
			const TypeInfo* GetMethodType() const;
			const std::string& GetMethodName() const;

		private :
			const TypeInfo* m_ownerType;
			const TypeInfo* m_methodType;
			const MethodBase* m_methodBase;
			const std::string m_methodName;
	};
};

#endif // __REFLECTION_METHODINFO_H__