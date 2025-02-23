#include <iostream>
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