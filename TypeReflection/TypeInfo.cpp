#include "TypeInfo.h"

namespace MetaData
{
    TypeInfo::TypeInfo(const char* _pName, const TypeInfo* _pSuper)
        : m_pTypeName(_pName)
        , m_pSuper(_pSuper)
    {}

    TypeInfo::TypeInfo(const TypeInfo& _rhs)
        : m_pTypeName(_rhs.m_pTypeName)
        , m_pSuper(_rhs.m_pSuper)
    {}

    TypeInfo::TypeInfo(TypeInfo&& _rhs)
        : m_pTypeName(_rhs.m_pTypeName)
        , m_pSuper(_rhs.m_pSuper)
    {}

    bool TypeInfo::operator==(const TypeInfo& _rhs) const
    {
        if ((m_pTypeName == _rhs.m_pTypeName) || (m_pSuper == _rhs.m_pSuper))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void TypeInfo::SetSuper(const TypeInfo* _pSuper)
    {
        m_pSuper = _pSuper;
    }

    const TypeInfo* TypeInfo::GetSuper() const
    {
        return m_pSuper;
    }

    const char* TypeInfo::GetTypeName() const
    {
        return m_pTypeName;
    }
}
