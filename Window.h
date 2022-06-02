#pragma once
#include <Windows.h>
#include <optional>

class Window
{
private:
	class WindowClass
	{
	public:
		static const wchar_t* GetName();
		static HINSTANCE GetInstance();

	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		static constexpr const wchar_t* className = L"Burren Engine Window Class";
		static WindowClass windowClass;
		HINSTANCE hInst;
	};
public:
	Window(const wchar_t* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	static std::optional<int> ProcessMessages();

private:
	static LRESULT SetupMessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT WndMessageThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WndMessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HWND hWnd;
};