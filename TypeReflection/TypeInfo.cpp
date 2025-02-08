#include "TypeInfo.h"

namespace MetaData
{
    TypeInfo::TypeInfo(const std::string& _strName, const TypeInfo* _pSuper)
        : m_strTypeName(_strName)
        , m_pSuper(_pSuper)
    {}

    TypeInfo::TypeInfo(const TypeInfo& _rhs)
        : m_strTypeName(_rhs.m_strTypeName)
        , m_pSuper(_rhs.m_pSuper)
    {}

    TypeInfo::TypeInfo(TypeInfo&& _rhs)
        : m_strTypeName(_rhs.m_strTypeName)
        , m_pSuper(_rhs.m_pSuper)
    {}

    bool TypeInfo::operator==(const TypeInfo& _rhs) const
    {
        if ((m_strTypeName == _rhs.m_strTypeName) || (m_pSuper == _rhs.m_pSuper))
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

    const std::string& TypeInfo::GetTypeName() const
    {
        return m_strTypeName;
    }
}
