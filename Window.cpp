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

void Window::SetTitleText(HWND hWnd, const wchar_t* newTitle)
{
    if (SetWindowText(hWnd, newTitle) == 0)
    {
        // something went wrong
    }
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
    bool bLButtonPressed = false;
    bool bRButtonPressed = false;
    bool bMButtonPressed = false;
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
    /* Keyboard messages: START */
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: // handles "F10" key and "ALT+ any key" presses
        if ((lParam & 0x40000000))
        {
            SetTitleText(hWnd, L"Repeating");
        }
        else
        {
            SetTitleText(hWnd, L"Pressed");
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        SetTitleText(hWnd, L"Released");
        break;
    case WM_CHAR:
        break;
    /* Keyboard messages: END */
    /* Mouse messages: START */
    case WM_LBUTTONDOWN:
    {
        MAKEPOINTS(lParam);
        bLButtonPressed = true;
        SetTitleText(hWnd, L"LB pressed");
        break;
    }
    case WM_LBUTTONUP:
    {
        bLButtonPressed = false;
        MAKEPOINTS(lParam);
        SetTitleText(hWnd, L"LB released");
        break;
    }
    case WM_RBUTTONDOWN:
    {
        bRButtonPressed = true;
        MAKEPOINTS(lParam);
        SetTitleText(hWnd, L"RB pressed");
        break;
    }
    case WM_RBUTTONUP:
    {
        bRButtonPressed = false;
        MAKEPOINTS(lParam);
        SetTitleText(hWnd, L"RB released");
        break;
    }
    case WM_MBUTTONDOWN:
    {
        bMButtonPressed = true;
        MAKEPOINTS(lParam);
        SetTitleText(hWnd, L"MB pressed");
        break;
    }
    case WM_MBUTTONUP:
    {
        bMButtonPressed = false;
        MAKEPOINTS(lParam);
        SetTitleText(hWnd, L"MB released");
        break;
    }
    case WM_MOUSEWHEEL:
    {
        MAKEPOINTS(lParam);
        GET_KEYSTATE_WPARAM(wParam);
        GET_WHEEL_DELTA_WPARAM(wParam);
        SetTitleText(hWnd, L"mouse wheel delta");
        break;
    }
    case WM_MOUSEMOVE:
    {
        GET_KEYSTATE_WPARAM(wParam);
        MAKEPOINTS(lParam);
        SetTitleText(hWnd, L"mouse move");
        break;
    }
    /* Mouse messages: END */
    }
    // fallback to default message handling
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}