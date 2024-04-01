#include <iostream>
#include <LogProject/Log.h>
#include "Core.h"
#include "Platform.h"
#include "TypeCast.h"

#include <typeinfo>
#include <utility>

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

class NE_API IWrong
{
    GENERATE( IWrong )
};

int MAIN()
{
    Log::Info("%s", PLATFORM);
    Log::Info("%s", typeid( Object::ThisType ).name() );
    Log::Info("%s", typeid( Object::SuperType ).name() );

    Object* value = nullptr;

    IObject* ivalue = TypeCast<IObject*>( value );

    CObject* cvalue = TypeCast<CObject*>( value );

    IWrong* IWrongvalue = TypeCast<IWrong*>( value );

    Log::Print();

    return 0;
}