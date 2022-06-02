#include "Window.h"

Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass():
    hInst(GetModuleHandle(nullptr))
{
    // Define the window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = SetupMessageProc;    // must be set
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();           // must be set
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();    // must be set
    wc.hIconSm = nullptr;
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(GetName(), GetInstance());
}

const wchar_t* Window::WindowClass::GetName()
{
    return className;
}

HINSTANCE Window::WindowClass::GetInstance()
{
    return windowClass.hInst;
}

Window::Window(const wchar_t* name)
{
    hWnd = CreateWindow(
        WindowClass::GetName(), name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr,
        WindowClass::GetInstance(), nullptr
    );

    if (hWnd == nullptr)
    {
        // throw error
    }
    ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
    DestroyWindow(hWnd);
}

std::optional<int> Window::ProcessMessages()
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))    // non-blocking
    {
        if (msg.message == WM_QUIT)
        {
            return static_cast<int>(msg.wParam);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return {};
}

LRESULT Window::SetupMessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::WndMessageThunk));
        return pWnd->WndMessageProc(hWnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::WndMessageThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->WndMessageProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::WndMessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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