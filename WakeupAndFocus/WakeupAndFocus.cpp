// WakeupAndFocus.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <Windows.h>

BOOL CALLBACK KillScreenSaverFunc(HWND hwnd, LPARAM lParam) {
	if (IsWindowVisible(hwnd)) PostMessage(hwnd, WM_CLOSE, 0, 0);
	return TRUE;
}

int _tmain(int argc, _TCHAR* argv[]) {
	if ((argc > 2) || (argc == 2 && _tcscmp(argv[1], L"/?") == 0)) {
		printf("Aktiviert das Display und das System und setzt den Vordergrund, die\r\n");
		printf("Eingabe und den Fokus auf den angegebenen Fenstertitel.\r\n");
		printf("Das Fenster wird dabei nicht explizit maximiert, sondern verbleibt\r\n");
		printf("in seinem letzten Zustand.\r\n");
		printf("\r\n");
		_tprintf(TEXT("%s [Fenstertitel]\r\n"), argv[0]);
		printf("\r\n");
		printf("  [Fenstertitel]\r\n");
		printf("                Bezeichnet den (optionalen) Fenstertitel, der aktiviert\r\n");
		printf("                werden soll.\r\n");
		return 0;
	}

	int ssRunning;
	SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &ssRunning, 0);

	if (ssRunning) {
		// Source https://support.microsoft.com/de-de/help/140723
		HDESK hdesk;
		hdesk = OpenDesktop(TEXT("Screen-saver"), 0, FALSE, DESKTOP_READOBJECTS | DESKTOP_WRITEOBJECTS);
		if (hdesk) {
			EnumDesktopWindows(hdesk, KillScreenSaverFunc, 0);
			CloseDesktop(hdesk);
		} else {
			// Windows 2000 and later: 
			// If there is no screen saver desktop, the screen saver 
			// is on the default desktop. Close it by sending a 
			// WM_CLOSE.
			PostMessage(GetForegroundWindow(), WM_CLOSE, 0, 0L); 
		}
	}

	SetThreadExecutionState(ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED);
	Sleep(1000);

	if (argc < 2) return 0;

	DWORD dwCurrentThread = GetCurrentThreadId();
	DWORD dwFGThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);

	AttachThreadInput(dwCurrentThread, dwFGThread, TRUE);

	HWND hwnd = FindWindow(NULL, argv[1]);

	if (hwnd != NULL) {
		SetForegroundWindow(hwnd);
		SetCapture(hwnd);
		SetFocus(hwnd);
		SetActiveWindow(hwnd);
		EnableWindow(hwnd, TRUE);
	}

	AttachThreadInput(dwCurrentThread, dwFGThread, FALSE);
	return 0;
}

