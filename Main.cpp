#include <exception>
#include <iostream>
#include "Window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
    Window wnd{ 800, 600, L"Burren Engine" };
    
    try
    {
        while (true)
        {
            if (const auto exitCode = wnd.ProcessMessages())
            {
                return *exitCode;
            }
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    
    
    return 0;
}