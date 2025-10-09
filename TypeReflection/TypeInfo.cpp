#include "TypeInfo.h"

namespace MetaData
{
    TypeInfo::TypeInfo(const std::string& _strName, const size_t _typeIndex, const TypeInfo* _pSuper)
        : m_strTypeName(_strName)
        , m_pSuper(_pSuper)
        , m_typeIndex(_typeIndex)
    {}

    TypeInfo::TypeInfo(const TypeInfo& _rhs)
        : m_strTypeName(_rhs.m_strTypeName)
        , m_pSuper(_rhs.m_pSuper)
        , m_typeIndex(_rhs.m_typeIndex)
    {}

    TypeInfo::TypeInfo(TypeInfo&& _rhs)
        : m_strTypeName(_rhs.m_strTypeName)
        , m_pSuper(_rhs.m_pSuper)
        , m_typeIndex(_rhs.m_typeIndex)
    {}

    bool TypeInfo::operator==(const TypeInfo& _rhs) const
    {
        if (m_typeIndex == _rhs.m_typeIndex)
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

    const size_t TypeInfo::GetTypeIndex() const
    {
        return m_typeIndex;
    }
}
