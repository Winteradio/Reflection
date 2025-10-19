#include <LogProject/Log.h>
#include <LogProject/LogPlatform.h>
#include "Reflection.h"

class IObject
{
    GENERATE(IObject);

    public :
        IObject()
            : m_Value(-1)
        {}

        virtual ~IObject() = default;

    public :
        PROPERTY(m_Value);
        int m_Value;
};

class ObjectA : public IObject
{
    GENERATE(ObjectA);

    public :
        ObjectA()
            : m_Value(1)
        {}

        virtual ~ObjectA() = default;

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
        ObjectB()
            : m_Value(2)
        {};

    public :
        METHOD(Hello);
        void Hello(const int value) 
        {
            LOGINFO() << "Member Hello " << value;
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
    ObjectHolder()
        : m_MyObject(new ObjectB())
    {
        m_MyObject->m_Value = 99;
    }

    ~ObjectHolder()
    {
        delete m_MyObject;
    }

public:
    PROPERTY(m_MyObject);
    ObjectB* m_MyObject;
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

        LOGINFO() << "  Property " 
                    << " Type : " << propertyInfo->GetPropertyType()->GetTypeName()
                    << " / Name : " << propertyInfo->GetPropertyName();
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
            ObjectB* value = propertyInfo->Get<ObjectB*>(holder);
            if (value != nullptr)
            {
                LOGINFO() << "ObjectHolder ObjectB Value : " << value->m_Value;
            }

            ObjectB newValue;
            newValue.m_Value = 150;

            propertyInfo->Set(holder, &newValue);

            LOGINFO() << "ObjectHolder Changed Value : " << holder.m_MyObject->m_Value;

            propertyInfo->Set(holder, value);
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