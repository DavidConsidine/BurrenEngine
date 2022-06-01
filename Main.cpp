#include <Windows.h>
#include <exception>
#include <iostream>

// callback function to process messages set to the window
LRESULT WndMessageProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    return 0;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
    const wchar_t wndClassName[] = L"Burren Engine";
    // Define the window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = DefWindowProc;    // must be set
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;           // must be set
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = wndClassName;    // must be set
    wc.hIconSm = nullptr;
    try
    {
        // Register window class
        if (RegisterClassEx(&wc) == 0)
        {
            throw std::exception{ "Failed to register Window class." };
        }
        // Create window instance
        HWND hwnd = CreateWindow(
            wndClassName, L"Burren Engine Title",
            WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
            CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr,
            hInstance, nullptr
        );
        if (hwnd == nullptr)
        {
            throw std::exception{ "Failed to create Window instance." };
        }
        ShowWindow(hwnd, nCmdShow);
        while (true){}
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    
    
    return 0;
}