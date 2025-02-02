#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifdef _WIN32
    #include <windows.h>
        #pragma comment(linker, "/entry:WinMainCRTStartup")
        #pragma comment(linker, "/subsystem:console")

    #define MAIN() CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
    #define PLATFORM "Windows"

#elif __linux__
    #define MAIN() main()
    #define PLATFORM "Linux"

#endif

#endif // __PLATFORM_H__