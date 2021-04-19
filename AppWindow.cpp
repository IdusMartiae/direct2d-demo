#include "AppWindow.h"

int wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR CmdLine, int CmdShow)
{
	DrawController drawController;
	
	if (!drawController.Create(WINDOW_TITLE, WS_OVERLAPPEDWINDOW))
	{
		return 0;
	}
	
	ShowWindow(drawController.Window(), CmdShow);

	MSG message;

	DWORD	startTimer;
	DWORD	updateFrequency = DELTA_TIME;

	while (drawController.timerRunning) {

		startTimer = GetTickCount();
		(startTimer % 1) == 0 ? drawController.Update() : Sleep(0);

		while (PeekMessageW(&message, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessageW(&message, NULL, 0, 0))
			{
					TranslateMessage(&message);
					DispatchMessageW(&message);
			}
		};

	}
	

	return 0;
}
