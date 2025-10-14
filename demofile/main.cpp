#include <iostream>
#include <unordered_map>
#include <LogProject/Log.h>
#include <LogProject/LogPlatform.h>
#include "Reflection/Reflection.h"

class IObject
{
    GENERATE(IObject);
};

class ObjectA : public IObject
{
    GENERATE(ObjectA);
};

class ObjectB : public ObjectA
{
    GENERATE(ObjectB);
};

class ObjectC : public ObjectB
{
    GENERATE(ObjectC);
};

int MAIN()
{
    Log::Init(1024, Log::Enum::eMode_Print, Log::Enum::eLevel_Time | Log::Enum::eLevel_Type);
    LOGINFO() << "Reflection Test";

    LOGINFO() << "IObject Type Name : " << IObject::GetStaticTypeInfo()->GetTypeName();
    LOGINFO() << "ObjectA Type Name : " << ObjectA::GetStaticTypeInfo()->GetTypeName();

    for (const auto& typePair : Reflection::TypeManager::GetHandle().GetTypeMap())
    {
        const Reflection::TypeInfo* typeInfo = typePair.second;
        if (typeInfo != nullptr)
        {
            LOGINFO() << "Type Name : " << typeInfo->GetTypeName();
        }
    }

    system("pause");

    return 0;
}