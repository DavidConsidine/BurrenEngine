#include "Window.h"

Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass() noexcept
    :
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

const wchar_t* Window::WindowClass::GetName() noexcept
{
    return className;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return windowClass.hInst;
}

Window::Window(int width, int height, const wchar_t* name)
    :
    width(width),
    height(height)
{
    RECT wndRect{};
    wndRect.left = 100;
    wndRect.right = wndRect.left + width;
    wndRect.top = 100;
    wndRect.bottom = wndRect.top + height;
    if (AdjustWindowRect(&wndRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
    {
        throw std::exception("adjust window rect failed");
    }
    hWnd = CreateWindow(
        WindowClass::GetName(), name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top,
        nullptr, nullptr, WindowClass::GetInstance(), this
    );

    if (hWnd == nullptr)
    {
        throw std::exception("Window handle is null");
    }
    ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
    DestroyWindow(hWnd);
}

std::optional<int> Window::ProcessMessages() noexcept
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
        throw std::exception("set window text failed");
    }
}

LRESULT Window::SetupMessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
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

LRESULT Window::WndMessageThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->WndMessageProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::WndMessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
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
        //PAINTSTRUCT ps = {};
        //HDC hdc = BeginPaint(hWnd, &ps);
        //FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        //EndPaint(hWnd, &ps);
        break;
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
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftPressed(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftReleased(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightPressed(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightReleased(pt.x, pt.y);
        break;
    }
    case WM_MBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnMiddlePressed(pt.x, pt.y);
        break;
    }
    case WM_MBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnMiddleReleased(pt.x, pt.y);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        mouse.OnWheelDelta(pt.x, pt.y, delta);
        break;
    }
    case WM_MOUSEMOVE:
    {
        POINTS pt = MAKEPOINTS(lParam);
        //mouse.OnMouseMove(pt.x, pt.y);
        break;
    }
    /* Mouse messages: END */
    }
    // fallback to default message handling
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}