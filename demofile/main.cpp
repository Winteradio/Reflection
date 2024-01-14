#include <iostream>
#include <LogProject/Log.h>

#ifdef _WIN32
#include <windows.h>
    #pragma comment(linker, "/entry:WinMainCRTStartup")
    #pragma comment(linker, "/subsystem:console")

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    Log::Info("Windows");
#else
int main()
{
    Log::Info("Linux");
#endif

    Log::Print();
    return 0;
}