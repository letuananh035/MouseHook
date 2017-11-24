// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <Windows.h>
HWND hWndHook;
HHOOK mouseHook;
HHOOK keyboardHook;
BOOL Run = false;
HINSTANCE hInstance;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	hInstance = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


LRESULT CALLBACK MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
void SendRDown(POINT p)
{
	HWND hWnd = WindowFromPoint(p);
	SetForegroundWindow(hWnd);
	RECT rct;
	if (!GetWindowRect(hWnd, &rct))
	{
		return;
	}
	int newx = p.x - rct.left;
	int newy = p.y - rct.top;
	POINT pWin;
	pWin.x = newx;
	pWin.y = newy;
	ScreenToClient(hWnd, &pWin);
	PostMessage(hWnd, (int)WM_RBUTTONDOWN, MK_RBUTTON, newy * 65536 + newx);
	//SendMessage(hWnd, BM_CLICK, MK_LBUTTON, NULL);
}
void SendRUp(POINT p)
{
	HWND hWnd = WindowFromPoint(p);
	SetForegroundWindow(hWnd);
	RECT rct;
	if (!GetWindowRect(hWnd, &rct))
	{
		return;
	}
	int newx = p.x - rct.left;
	int newy = p.y - rct.top;
	POINT pWin;
	pWin.x = newx;
	pWin.y = newy;
	ScreenToClient(hWnd, &pWin);
	PostMessage(hWnd, (int)WM_RBUTTONUP, 0, newy * 65536 + newx);
}

void SendLDown(POINT p)
{
	HWND hWnd = WindowFromPoint(p);
	SetForegroundWindow(hWnd);
	RECT rct;
	if (!GetWindowRect(hWnd, &rct))
	{
		return;
	}
	int newx = p.x - rct.left;
	int newy = p.y - rct.top;
	POINT pWin;
	pWin.x = newx;
	pWin.y = newy;
	ScreenToClient(hWnd, &pWin);
	PostMessage(hWnd, (int)WM_LBUTTONDOWN, MK_LBUTTON, newy * 65536 + newx);
}
void SendLUp(POINT p)
{
	HWND hWnd = WindowFromPoint(p);
	SetForegroundWindow(hWnd);
	RECT rct;
	if (!GetWindowRect(hWnd, &rct))
	{
		return;
	}
	int newx = p.x - rct.left;
	int newy = p.y - rct.top;
	POINT pWin;
	pWin.x = newx;
	pWin.y = newy;
	ScreenToClient(hWnd, &pWin);
	PostMessage(hWnd, (int)WM_LBUTTONUP, 0, newy * 65536 + newx);

}

INT InitMouseHook(HWND hwndYourWindow)
{
	hWndHook = hwndYourWindow;
	if (!(mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0)))
	{
		MessageBox(hwndYourWindow, L"Failed to install mouse hook!", L"Error", NULL);
		return 0;
	}

	if (!(keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0)))
	{
		MessageBox(hwndYourWindow, L"Failed to install keyboard hook!", L"Error", NULL);
		return 0;
	}
	return 1;
}

LRESULT CALLBACK MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	POINT pos_cursor;
	HWND hwndClick = NULL;
	if (nCode >= 0)
	{
		switch (wParam)
		{
		
		case WM_RBUTTONDOWN:
			if (Run == true){
				GetCursorPos(&pos_cursor);
				SendRDown(pos_cursor);
				return 1;
			}
			break;
		case WM_RBUTTONUP:
			if (Run == true){
				GetCursorPos(&pos_cursor);
				SendLUp(pos_cursor);
				return 1;
			}
			break;
		case WM_LBUTTONDOWN:{
			if (Run == true){
				GetCursorPos(&pos_cursor);
				SendLDown(pos_cursor);
				return 1;
			}
			break;
		}
		case WM_LBUTTONUP:
			if (Run == true){
				GetCursorPos(&pos_cursor);
				SendRUp(pos_cursor);
				return 1;
			}
			break;
		}
	}
	return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		switch (wParam)
		{
		case WM_KEYDOWN:{
				PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
				if (p->vkCode == VK_F6){
					Run = !Run;
					return 1;
				}
				break;
			}
		}
	}
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}


INT UninstallMouseHook()
{
	if (mouseHook != NULL)
	{
		UnhookWindowsHookEx(mouseHook);
	}
	if (keyboardHook != NULL)
	{
		UnhookWindowsHookEx(keyboardHook);
	}
	return 1;
}
