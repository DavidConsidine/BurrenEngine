#include <Windows.h>
#include <exception>
#include <iostream>

// callback function to process messages set to the window
LRESULT WndMessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // sample window messages
    switch (uMsg)
    {
    case WM_CLOSE:  // msg signalling window should terminate
        if (MessageBox(hWnd, L"Close Window?", L"Burren Engine", MB_OKCANCEL) == IDOK)
        {
            DestroyWindow(hWnd);
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:  // msg signalling window must repaint a portion of it's client area
    {
        PAINTSTRUCT ps = {};
        HDC hdc = BeginPaint(hWnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_SIZE:   // msg signalling window size has changed
        break;
    case WM_KILLFOCUS:  // msg signalling window is losing keyboard focus
        break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        break;
    case WM_CHAR:
        break;
    case WM_LBUTTONDOWN:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_RBUTTONDOWN:
        break;
    case WM_RBUTTONUP:
        break;
    case WM_MBUTTONDOWN:
        break;
    case WM_MBUTTONUP:
        break;
    case WM_MOUSEWHEEL:
        break;
    case WM_MOUSEMOVE:
        break;
    }
    // fallback to default message handling
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
    const wchar_t wndClassName[] = L"Burren Engine";
    // Define the window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndMessageProc;    // must be set
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

        // Handle message loop
        MSG msg{};
        while (GetMessage(&msg, nullptr, 0, 0) != 0)    // Blocks until there is a message to get from the message queue
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    
    
    return 0;
}