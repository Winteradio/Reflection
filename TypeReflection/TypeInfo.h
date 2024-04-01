#ifndef __TYPEINFO_H__
#define __TYPEINFO_H__

#include <stdio.h>

class TypeInfo
{
    public :
        TypeInfo() = delete;
        explicit TypeInfo( const char* className );
        explicit TypeInfo( const char* className, TypeInfo& super );
        explicit TypeInfo( const TypeInfo& Other );
        explicit TypeInfo( TypeInfo&& Other );
        virtual ~TypeInfo();

    public :
        bool operator==( const TypeInfo& Other ) const;
    
    public :
        void SetSuper( TypeInfo& Super );
        TypeInfo* GetSuper();

    private :
        const char* m_Name = nullptr;
        TypeInfo* m_SuperPtr = nullptr;
};

#endif // __TYPEINFO_H__
