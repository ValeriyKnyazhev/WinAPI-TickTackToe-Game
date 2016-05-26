#pragma once
#include <windows.h>

class CTickTackToeWindow {
public:
	struct Cell
	{
		bool isBlackCell;
		bool isWin;
		int figure;
		int weight;
		int randShiftLeft;
		int randShiftTop;
		int randShiftRight;
		int randShiftDown;
	};

	CTickTackToeWindow();
	~CTickTackToeWindow();

	static bool RegisterClassA();

	bool Create();
	void Show(int cmdShow);

	HWND GetHandle() const;
	HACCEL GetHaccel() const;

protected:
	void OnCreate();
	void onNewGame();
	void onDestroy();
	void onPaint();
	void onClose();
	void drawToe(HDC, int, int, HBRUSH, HPEN);
	void drawNought(HDC, int, int, HPEN);
	bool withinBorder(int x, int y);
	double findMaxLength(int x, int y, int dx, int dy, int color);
	int cellComputerWeight(int x, int y);
	int cellPlayerWeight(int x, int y);
	int cellWeight(int x, int y);
	int weightNeighbors(int x, int y, int dx, int dy, int color);
	int neighbors(int x, int y, int color);
	void moveComputer();
	void onButton(int, int);
	void initGame();

	void OnDlgInit(HWND handle);
	void OnDlgOk(HWND handle);
	void OnDlgCancel(HWND handle);
	void OnDlgCheck(HWND handle, UINT checkBoxID);
	void OnDlgChangeColor(HWND handle, UINT buttonID);
	void OnDlgScroll(HWND handle);
	static BOOL __stdcall dialogProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static wchar_t* nameClassWindow;
	static wchar_t* nameWindow;

	Cell** field = NULL;
	bool partEnd;
	void draw(HDC dc);
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	HWND handle;
	HACCEL haccel;
	HWND handleDialog;

	COLORREF colorToe = RGB(0, 0, 150);
	COLORREF colorCross = RGB(150, 0, 0);
	COLORREF colorWin = RGB(0, 150, 0);
	COLORREF tmpColorToe = RGB(0, 0, 150);
	COLORREF tmpColorCross = RGB(150, 0, 0);
	COLORREF tmpColorWin = RGB(0, 150, 0);

	int sizeOfBoard = 10;
	int tmpSizeOfBoard = 10;
	const int minSizeOfBoard = 8;
	const int maxSizeOfBoard = 15;
	int sizeOfCell = 50;
	int tmpSizeOfCell = 50;
	const int minSizeOfCell = 30;
	const int maxSizeOfCell = 60;

	const int GAME_COMPUTER = 1;
	const int GAME_PLAYER = 2;
	const int GAME_EMPTY = 0;
	const int MARGIN = 2;

	int countMoves = 0;
	bool isCross = true;
	bool tmpIsCross = true;
	bool isFirst = true;
	bool tmpIsFirst = true;

};
