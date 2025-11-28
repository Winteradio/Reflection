#include <LogProject/Log.h>
#include <LogProject/LogPlatform.h>
#include "Reflection.h"

class IObject
{
	GENERATE(IObject);

	public :
		IObject() : m_Value(-1) {}
		virtual ~IObject() = default;

	public :
		virtual void Hello(const int value)
		{
			LOGINFO() << "Member IObject::Hello " << value;
		};

	public :
		PROPERTY(m_Value);
		int m_Value;

		// PROPERTY(m_RefValue);
		// int& m_RefValue = m_Value;

		// The Reference member variable is not supported in the reflection system.
		// The Reference type cannot be properly handled for offset calculation and memory management.
		// Hence, it is commented out to avoid potential issues.

		// This code occurs compilation error C2338: Reflection::PROPERTY : The property cannot be a reference type.
};

class ObjectA : public IObject
{
	GENERATE(ObjectA);

	public :
		ObjectA() : m_Value(1), m_Other(10)	{}
		virtual ~ObjectA() = default;

	public :
		virtual void Hello(const int value)
		{
			LOGINFO() << "Member ObjectA::Hello " << value;
		};

	public :
		PROPERTY(m_Value);
		int m_Value;

		PROPERTY(m_Other);
		int m_Other;
};

class ObjectB : public ObjectA
{
	GENERATE(ObjectB);

	public :
		ObjectB() : m_Value(2) {};

	public :
		METHOD(Hello);
		void Hello(const int value) final
		{
			LOGINFO() << "Member ObjectB::Hello " << value;
		};

		METHOD(ConstHello);
		void ConstHello(const int value1, const float value2) const 
		{
			LOGINFO() << "Member Const Hello " << value1 << " " << value2;
		};

		METHOD(StaticHello);
		static void StaticHello(const float value1) 
		{
			LOGINFO() << "Static Hello " << value1;
		};

		PROPERTY(m_Value);
		int m_Value;
};

class ObjectHolder
{
	GENERATE(ObjectHolder);

public:
	ObjectHolder(): m_MyObject(nullptr)	{}

public:
	PROPERTY(m_MyObject);
	ObjectA* m_MyObject;
};

namespace Test
{
	void Print(const Reflection::PropertyInfo* propertyInfo)
	{
		if (propertyInfo == nullptr)
		{
			LOGERROR() << "The property info is invalid";
			return;
		}

		if (propertyInfo->GetPropertyType() != propertyInfo->GetPropertyType()->GetPureType())
		{
			LOGINFO() << "  Property " 
						<< " Type : " << propertyInfo->GetPropertyType()->GetTypeName()
						<< " / Pure Type : " << propertyInfo->GetPropertyType()->GetPureType()->GetTypeName()
						<< " / Name : " << propertyInfo->GetPropertyName();
		}
		else
		{
			LOGINFO() << "  Property " 
						<< " Type : " << propertyInfo->GetPropertyType()->GetTypeName()
						<< " / Name : " << propertyInfo->GetPropertyName();
		}
	}

	void Print(const Reflection::MethodInfo* methodInfo)
	{
		if (methodInfo == nullptr)
		{
			LOGERROR() << "The method info is invalid";
			return;
		}

		LOGINFO() << "  Method "
					<< " Type : " << methodInfo->GetMethodType()->GetTypeName()
					<< " / Name : " << methodInfo->GetMethodName();
	}

	void Print(const Reflection::TypeInfo* typeInfo)
	{
		if (typeInfo == nullptr)
		{
			LOGERROR() << "The typeinfo is invalid";
			return;
		}

		if (typeInfo->GetSuperType() == nullptr)
		{
			LOGINFO() << "Type : " << typeInfo->GetTypeName()
						<< " / Super : none";
		}
		else
		{
			LOGINFO() << "Type : " << typeInfo->GetTypeName()
						<< " / Super : " << typeInfo->GetSuperType()->GetTypeName();
		}

		for (const auto& propertyPair : typeInfo->GetProperties())
		{
			Print(propertyPair.second);
		}

		for (const auto& methodPair : typeInfo->GetMethods())
		{
			Print(methodPair.second);
		}
	}

	void ChangeInstanceProperty()
	{
		IObject instance;

		const Reflection::PropertyInfo* propertyInfo = IObject::GetStaticTypeInfo()->GetProperty("m_Value");
		if (propertyInfo != nullptr)
		{
			int* value = propertyInfo->Get<int>(instance);
			if (value != nullptr)
			{
				LOGINFO() << "IObject Value : " << *value;
			}

			propertyInfo->Set(instance, 2);

			LOGINFO() << "IObject Changed Value : " << instance.m_Value;
		}
	}

	void ChangeConstantInstanceProperty()
	{
		const IObject constInstance;

		const Reflection::PropertyInfo* propertyInfo = IObject::GetStaticTypeInfo()->GetProperty("m_Value");
		if (propertyInfo != nullptr)
		{
			const int* value = propertyInfo->Get<int>(constInstance);
			if (value != nullptr)
			{
				LOGINFO() << "const IObject Value : " << *value;
			}
		}
	}

	void ChangePointerProperty()
	{
		ObjectHolder holder;

		const Reflection::PropertyInfo* propertyInfo = ObjectHolder::GetStaticTypeInfo()->GetProperty("m_MyObject");
		if (propertyInfo != nullptr)
		{
			LOGINFO() << "ObjectHolder change value(nullptr) to the Object B*";
			ObjectB objectB;
			propertyInfo->Set(holder, &objectB);
			holder.m_MyObject->Hello(456);

			LOGINFO() << "ObjectHolder change value(ObjectB*) to the Object A*";
			ObjectA objectA;
			propertyInfo->Set(holder, &objectA);
			holder.m_MyObject->Hello(234);

			LOGINFO() << "ObjectHolder get value(ObjectA*) as the IObject*";
			ObjectA* pointerA = propertyInfo->Get<ObjectA*>(holder);
			if (pointerA != nullptr)
			{
				pointerA->Hello(123);
			}
		}
	}

	void GetPropertyVirtualFunction()
	{
		ObjectHolder holder;

		const Reflection::PropertyInfo* propertyInfo = ObjectHolder::GetStaticTypeInfo()->GetProperty("m_MyObject");
		if (propertyInfo != nullptr)
		{
			ObjectA* parentType = propertyInfo->Get<ObjectA*>(holder);
			if (parentType != nullptr)
			{
				LOGINFO() << "ObjectHolder Get ObjectA Value : " << parentType->m_Value;
				parentType->Hello(123);
			}

			ObjectB* childType = propertyInfo->Get<ObjectB*>(holder);
			if (childType != nullptr)
			{
				LOGINFO() << "ObjectHolder Get ObjectB Value : " << childType->m_Value;
				childType->Hello(123);
			}
		}
	}

	void Print()
	{
		LOGINFO() << "[ Print Type Info ]";

		for (const auto& typePair : Reflection::TypeManager::GetHandle().GetTypeMap())
		{
			Print(typePair.second);
		}

		LOGINFO() << " ";
	}

	void Property()
	{
		LOGINFO() << "[ Test Property ]";
		
		ChangeInstanceProperty();
		ChangeConstantInstanceProperty();
		ChangePointerProperty();
		GetPropertyVirtualFunction();

		LOGINFO() << " ";
	}

	void Cast()
	{
		LOGINFO() << "[ Test Type Casting ]";

		IObject* objectA = new ObjectA;
		IObject* objectB = new ObjectB;

		ObjectB* wrongB = Reflection::Cast<ObjectB*>(objectA);
		if (nullptr == wrongB)
		{
			LOGINFO() << "Failed to cast the ObjectA(ObjectA) to ObjectB";
		}

		ObjectB* castB = Reflection::Cast<ObjectB*>(objectB);
		if (nullptr != castB)
		{
			LOGINFO() << "Succeed to cast the IObject(ObjectB) to the ObjectB : " << castB->m_Value;
		}

		ObjectA* castA = Reflection::Cast<ObjectA*>(objectB);
		if (nullptr != castA)
		{
			LOGINFO() << "Succeed to cast the ObjectB(ObjectB) to the ObjectA : " << castA->m_Value;
		}

		/**
		 * Compile Error
		int value = 100;
		IObject* castInt = Reflection::Cast<IObject*>(&value);
		if (nullptr == castInt)
		{
			LOGINFO() << "Failed to cast the int(100) to IObject*";
		}
		*/

		LOGINFO() << " ";
	}

	void Invoke()
	{
		LOGINFO() << "[ Test Method Invoke ]";

		ObjectB value;

		const Reflection::TypeInfo* typeInfo = value.GetTypeInfo();
		if (nullptr == typeInfo)
		{
			return;
		}

		const Reflection::MethodInfo* hello = typeInfo->GetMethod("Hello");
		const Reflection::MethodInfo* constHello = typeInfo->GetMethod("ConstHello");
		const Reflection::MethodInfo* staticHello = typeInfo->GetMethod("StaticHello");

		if (nullptr != hello)
		{
			hello->Invoke<void>(value, 10);
		}

		if (nullptr != constHello)
		{
			constHello->Invoke<void>(value, 10, 0.5f);
		}

		if (nullptr != staticHello)
		{
			staticHello->Invoke<void>(value, 0.5f);
		}

		LOGINFO() << " ";
	}
};

int MAIN()
{
	Log::Init(1024, Log::Enum::eMode_Print, Log::Enum::eLevel_Time | Log::Enum::eLevel_Type);
	LOGINFO() << "|| Reflection Test ||";
	LOGINFO() << " ";

	Test::Print();
	Test::Cast();
	Test::Property();
	Test::Invoke();

	system("pause");

	return 0;
}