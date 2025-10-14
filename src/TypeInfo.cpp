#include "Reflection/TypeInfo.h"

namespace Reflection
{
    bool TypeInfo::operator==(const TypeInfo& other) const
    {
        if (m_typeHash != other.m_typeHash)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    
    const TypeInfo* TypeInfo::GetSuperType() const
    {
        return m_superType;
    }

    const size_t TypeInfo::GetTypeHash() const
    {
        return m_typeHash;
    }

    const std::string& TypeInfo::GetTypeName() const
    {
        return m_typeName;
    }

    void TypeInfo::AddProperty()
    {
        // TODO
    }

    void TypeInfo::AddMethod()
    {
        // TODO
    }
}
