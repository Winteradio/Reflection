#ifndef __REFLECTION_METHODINFO_H__
#define __REFLECTION_METHODINFO_H__

#include <string>

#include "Type/TypeCast.h"
#include "Method/MethodCall.h"

namespace Reflection
{
	class MethodInfo
	{
		public :
			template<typename Type, typename Method>
			struct Initializer
			{
				TypeInfo* ownerType;
				TypeInfo* methodType;
				const MethodBase* methodBase;

				Initializer(const MethodBase* method)
					: ownerType(TypeManager::GetHandle().Register<Type>())
					, methodType(TypeManager::GetHandle().Register<Method>())
					, methodBase(method)
				{}
			};

		public :
			template<typename Type, typename Method>
			explicit MethodInfo(const Initializer<Type, Method>& initializer, const std::string& methodName)
				: m_ownerType(initializer.ownerType)
				, m_methodType(initializer.methodType)
				, m_methodBase(initializer.methodBase)
				, m_methodName(methodName)
			{
				if (nullptr != initializer.ownerType)
				{
					initializer.ownerType->AddMethod(this);
				}
			}

			bool operator==(const MethodInfo& other) const;

		public :
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