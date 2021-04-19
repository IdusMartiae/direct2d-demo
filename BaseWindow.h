#pragma once

#include "Common.h"

template <class DERIVED_TYPE>
class BaseWindow
{
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE* pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED_TYPE*)(pCreate->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hwnd = hWnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		}
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
	BaseWindow() : m_hwnd(NULL) {}

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle		= 0,
		int windowX			= WINDOW_X,
		int windowY			= WINDOW_Y,
		int wndWidth		= WINDOW_WIDTH,
		int wndHeight		= WINDOW_HEIGHT,
		HWND hWndParent		= 0,
		HMENU hMenu			= 0
	)
	{
		WNDCLASS wndClass = {0};

		wndClass.lpszClassName = ClassName();
		wndClass.lpfnWndProc = DERIVED_TYPE::WndProc;
		wndClass.hInstance = GetModuleHandleW(NULL);

		RegisterClassW(&wndClass);

		m_hwnd = CreateWindowExW(
			dwExStyle, ClassName(), lpWindowName, dwStyle,
			windowX, windowY, wndWidth, wndHeight,
			hWndParent, hMenu, GetModuleHandleW(NULL), this
		);

		return (m_hwnd ? TRUE : FALSE);
	}

	HWND Window() const { return m_hwnd;  }

	HWND m_hwnd;

protected:

	virtual PCWSTR ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	
	
};

