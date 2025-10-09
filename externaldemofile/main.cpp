#include <iostream>
#include <LogProject/Log.h>
#include <ReflectionProject/Platform.h>
#include <ReflectionProject/TypeMacro.h>
#include <ReflectionProject/TypeCast.h>

#define NE_API

class NE_API IObject
{
    GENERATE( IObject )
};

class NE_API Object : public IObject
{
    GENERATE( Object )
};

class NE_API CObject : public Object
{
    GENERATE( CObject )

    public :
        int Value = 2;
};

namespace Hello
{
    class NE_API Object : public IObject
    {
        GENERATE( Hello::Object )
    };
};

template<typename T>
class Operator
{
    public :
        Operator()
        {
            LOGINFO() << "Operator Constructor";
        }

        Operator(const T* value)
            : m_pValue( value )
        {
            LOGINFO() << "Operator Constructor";
        }

        ~Operator()
        {
            LOGINFO() << "Operator Destructor";
        }

        Operator& operator=(const Operator<T>& other)
        {
            LOGINFO() << "Operator =";
            m_pValue = other.m_pValue;
            return *this;
        }

        const T& operator*()
        {
            LOGINFO() << "Operator *";
            return *m_pValue;
        }

        T* operator->()
        {
            LOGINFO() << "Operator ->";
            return m_pValue;
        }

    private :
        const T* m_pValue;
}

class Test
{
    public :
        Operator<CObject> m_pCObject;
};

int MAIN()
{
    Log::Init(1024, Log::Enum::eMode_Print, Log::Enum::eLevel_Time | Log::Enum::eLevel_Type);
    LOGINFO() << "Platform : " << PLATFORM;
    LOGINFO() << "Object This Type : " << Hello::Object::GetStaticTypeInfo()->GetTypeName(); 
    LOGINFO() << "Object Super Type : " << Hello::Object::SuperType::GetStaticTypeInfo()->GetTypeName();

    Object* value = new Object();

    IObject* ivalue = Type::Cast<IObject*>( value );

    CObject* cvalue = Type::Cast<CObject*>( value );

    LOGINFO() << "IObject Type Index : " << IObject::GetStaticTypeInfo()->GetTypeIndex();
    LOGINFO() << "Object Type Index : " << Object::GetStaticTypeInfo()->GetTypeIndex();
    LOGINFO() << "CObject Type Index : " << CObject::GetStaticTypeInfo()->GetTypeIndex();
    LOGINFO() << "Hello Object Type Index : " << Hello::Object::GetStaticTypeInfo()->GetTypeIndex();

    Operator<Test> test(new Test());

    test->m_pCObject = Operator<CObject>(new CObject());

    /* Compile Error
    class NE_API IWrong
    {};

    class NE_API Wrong : public IWrong
    {};

    Wrong* wrong = new Wrong();

    IWrong* IWrongvalue = Type::Cast<IWrong*>( wrong );
    */

    system("pause");

    return 0;
}