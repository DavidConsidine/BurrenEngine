#pragma once
#include <Windows.h>
#include <optional>
#include "Mouse.h"

class Window
{
private:
	class WindowClass
	{
	public:
		static const wchar_t* GetName()noexcept;
		static HINSTANCE GetInstance()noexcept;

	private:
		WindowClass()noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		static constexpr const wchar_t* className = L"Burren Engine Window Class";
		static WindowClass windowClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const wchar_t* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	static std::optional<int> ProcessMessages() noexcept;
	
	Mouse mouse;

private:
	void SetTitleText(HWND hWnd, const wchar_t* newTitle);
	static LRESULT SetupMessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT WndMessageThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)noexcept;
	LRESULT WndMessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)noexcept;

	int width;
	int height;
	HWND hWnd;
	
};