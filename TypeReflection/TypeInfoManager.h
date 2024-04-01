#ifndef __TYPEINFOMANAGER_H__
#define __TYPEINFOMANAGER_H__

#include "TypeInfo.h"

class TypeInfoManager
{
    private :
        TypeInfoManager();
        ~TypeInfoManager();

    public :
        TypeInfoManager& GetHandle();

    private :
        static TypeInfoManager* m_TypeInfoManagerPtr;

};

#endif // __TYPEINFOMANAGER_H__