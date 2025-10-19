#ifndef __REFLECTION_METHODMACRO_H__
#define __REFLECTION_METHODMACRO_H__

#include "Method/MethodInfo.h"

#define METHOD( Method ) \
        struct RegisterMethod##Method \
        { \
            RegisterMethod##Method() \
            { \
                static const Reflection::MethodBase* methodBase = Reflection::MethodCreator<decltype(&ThisType::Method), &ThisType::Method>::Create(); \
                static Reflection::MethodInfo::Initializer<ThisType, decltype(&ThisType::Method)> initializer(methodBase); \
                static Reflection::MethodInfo methodInfo(initializer, std::string(#Method)); \
            }; \
        }; \
        \
        static inline const RegisterMethod##Method s_registerMethod##Method; \

#endif // __REFLECTION_METHODMACRO_H__