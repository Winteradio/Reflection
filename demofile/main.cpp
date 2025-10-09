#include <iostream>
#include <unordered_map>
#include <LogProject/Log.h>
#include "Platform.h"
#include "TypeCast.h"

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
        GENERATE( Object )
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
            : m_pValue(const_cast<T*>(value))
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

        T& operator*()
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
        T* m_pValue;
};

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

    struct stHashGenerator
    {
        size_t operator()(const MetaData::TypeInfo& _typeInfo) const
        {
            return _typeInfo.GetTypeIndex();
        }
    };

    struct stCompare
    {
        bool operator()(const MetaData::TypeInfo& _lhs, const MetaData::TypeInfo& _rhs) const
        {
            return _lhs == _rhs;
        }
    };

    std::unordered_map<MetaData::TypeInfo, std::string, stHashGenerator, stCompare> map;

    map[*IObject::GetStaticTypeInfo()] = "IObject";
    map[*Object::GetStaticTypeInfo()] = "Object";
    map[*CObject::GetStaticTypeInfo()] = "CObject";
    map[*Hello::Object::GetStaticTypeInfo()] = "Hello Object";

    for (auto& [key, value] : map)
    {
        LOGINFO() << "Type Name : " << key.GetTypeName() << ", Type Index : " << key.GetTypeIndex() << ", Value : " << value;
    }
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