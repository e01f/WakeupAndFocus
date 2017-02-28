// WakeupAndFocus.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <Windows.h>


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

