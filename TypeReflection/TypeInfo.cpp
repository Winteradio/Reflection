#include "TypeInfo.h"

TypeInfo::TypeInfo( const char* className ) 
    : m_Name( className )
    , m_SuperPtr( nullptr )
{}

TypeInfo::TypeInfo( const char* className, TypeInfo& super  ) 
    : m_Name( className )
    , m_SuperPtr( &super )
{}

TypeInfo::TypeInfo( const TypeInfo& Other ) 
    : m_Name( Other.m_Name )
    , m_SuperPtr( Other.m_SuperPtr )
{}

TypeInfo::TypeInfo( TypeInfo&& Other ) 
    : m_Name( Other.m_Name )
    , m_SuperPtr( Other.m_SuperPtr )
{}

TypeInfo::~TypeInfo()
{}

bool TypeInfo::operator==( const TypeInfo& Other ) const 
{
    return m_Name == Other.m_Name;
}

void TypeInfo::SetSuper( TypeInfo& Super )
{
    m_SuperPtr = &Super;
}

TypeInfo* TypeInfo::GetSuper() { return m_SuperPtr; }

