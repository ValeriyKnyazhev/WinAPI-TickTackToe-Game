#include <Windows.h>
#include "CTickTackToe.h"
#include <iostream>

int WINAPI wWinMain(
	HINSTANCE instance,
	HINSTANCE prevInstance,
	LPWSTR commandLine,
	int cmdShow)
{
	CTickTackToeWindow cWindow;
	if (!cWindow.RegisterClassA())
	{
		return -1;
	}

	if (!cWindow.Create())
	{
		return -1;
	}
	cWindow.Show(cmdShow);

	MSG message;
	BOOL getMessageResult = 0;
	

	while (GetMessage(&message, NULL, 0, 0)) {
		if (!::TranslateAccelerator(
			cWindow.GetHandle(),  // handle to receiving window 
			cWindow.GetHaccel(),    // handle to active accelerator table 
			&message))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	return 0;
}