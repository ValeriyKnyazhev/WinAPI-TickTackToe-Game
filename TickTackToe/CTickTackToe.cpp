#ifndef UNICODE
#define UNICODE
#endif

#include "CTickTackToe.h"
#include "resource.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <windowsx.h>
#include <Windows.h>
#include <CommCtrl.h>

wchar_t *CTickTackToeWindow::nameClassWindow = L"MyClassWindow";
wchar_t *CTickTackToeWindow::nameWindow = L"Крестики нолики";

CTickTackToeWindow::CTickTackToeWindow()
{
	field = new Cell*[maxSizeOfBoard];
	for (int i = 0; i < maxSizeOfBoard; ++i)
	{
		field[i] = new Cell[maxSizeOfBoard];
	}

	initGame();
}

void CTickTackToeWindow::initGame()
{
	countMoves = 0;
	for (int x = 0; x < sizeOfBoard; x++)
		for (int y = 0; y < sizeOfBoard; y++)
		{
			field[x][y].figure = GAME_EMPTY;
			field[x][y].weight = 0;
			field[x][y].isWin = false;
			field[x][y].randShiftLeft = rand() % 5;
			field[x][y].randShiftTop = rand() % 5;
			field[x][y].randShiftRight = rand() % 5;
			field[x][y].randShiftDown = rand() % 5;
			if (y % 2)
				field[x][y].isBlackCell = x % 2;
			else
				field[x][y].isBlackCell = !(x % 2);
		}
	partEnd = false;
	if (!isFirst)
	{
		int x = rand() % sizeOfBoard;
		int y = rand() % sizeOfBoard;
		field[x][y].figure = GAME_COMPUTER;
		countMoves++;
	}
}

CTickTackToeWindow::~CTickTackToeWindow()
{
	for (int i = 0; i < maxSizeOfBoard; ++i)
	{
		delete []field[i];
	}
	delete []field;
}

bool CTickTackToeWindow::withinBorder(int x, int y)
{
	return (x >= 0 && x < sizeOfBoard && y >= 0 && y < sizeOfBoard);
}

double CTickTackToeWindow::findMaxLength(int x, int y, int dx, int dy, int color)
{
	double length = 0;
	int x0 = x, y0 = y;
	wchar_t buffer[10];
	_itow_s(x0 * 100 + y, buffer, 10);
	OutputDebugString(buffer);
	OutputDebugString(L"\n");
	if (field[x][y].figure == color)
	{
		length++;
	}
	x += dx;
	y += dy;
	while (withinBorder(x, y) && field[x][y].figure == color)
	{
		length++;
		x += dx; y += dy;
	}
	x = x0 - dx; y = y0 - dy;
	while (withinBorder(x, y) && field[x][y].figure == color)
	{
		length++;
		x -= dx; y -= dy;
	}
	if (color == GAME_PLAYER && length == 5)
	{
		int x0 = x, y0 = y;
		x += dx; y += dy;
		while (withinBorder(x, y) && field[x][y].figure == color)
		{
			field[x][y].isWin = true;
			x += dx; y += dy;
		}
		x = x0 - dx; y = y0 - dy;
		/*while (withinBorder(x, y) && field[x][y].figure == color)
		{
			field[x][y].isWin = true;
			x -= dx; y -= dy;
		}*/
		OutputDebugString(L"Field\r\n");
		for (int i = 0; i < sizeOfBoard; ++i) {
			for (int j = 0; j < sizeOfBoard; ++j) {
				wchar_t buff[10];
				int flag = 0;
				if (field[i][j].isWin) {
					flag = 1;
				}
				_itow_s(flag, buff, 10);
				OutputDebugString(buff);
				OutputDebugString(L" ");
			}
			OutputDebugString(L"\r\n");
		}
	}
	if (color == GAME_COMPUTER && length == 4)
	{	
		field[x0][y0].figure = color;
		x += dx; y += dy;
		int x0 = x, y0 = y;
		field[x][y].isWin = true;
		x += dx; y += dy;
		while (withinBorder(x, y) && field[x][y].figure == color)
		{
			field[x][y].isWin = true;
			x += dx; y += dy;
		}
		x = x0 - dx; y = y0 - dy;
		/*while (withinBorder(x, y) && field[x][y].figure == color)
		{
			field[x][y].isWin = true;
			x -= dx; y -= dy;
		}*/

		OutputDebugString(L"Field\r\n");
		for (int i = 0; i < sizeOfBoard; ++i) {
			for (int j = 0; j < sizeOfBoard; ++j) {
				wchar_t buff[10];
				int flag = 0;
				if (field[i][j].isWin) {
					flag = 1;
				}
				_itow_s(flag, buff, 10);
				OutputDebugString(buff);
				OutputDebugString(L" ");
			}
			OutputDebugString(L"\r\n");
		}
	}
	
	return length;
}


int CTickTackToeWindow::cellPlayerWeight(int x, int y)
{
	int res1 = findMaxLength(x, y, 1, 0, GAME_PLAYER);
	int res2 = findMaxLength(x, y, 0, 1, GAME_PLAYER);
	int res3 = findMaxLength(x, y, 1, 1, GAME_PLAYER);
	int res4 = findMaxLength(x, y, 1, -1, GAME_PLAYER);
	if (res1 >= 5 || res2 >= 5 || res3 >= 5 || res4 >= 5)
	{
		this->partEnd = true;
		return MAXINT - 1;
	}
	if (res1 >= 4 || res2 >= 4 || res3 >= 4 || res4 >= 4)
	{
		return MAXINT - 2;
	}
	return res1*res1+ res2*res2 + res3*res3 + res4*res4;
}

int CTickTackToeWindow::cellComputerWeight(int x, int y)
{
	int res1 = findMaxLength(x, y, 1, 0, GAME_COMPUTER);
	int res2 = findMaxLength(x, y, 0, 1, GAME_COMPUTER);
	int res3 = findMaxLength(x, y, 1, 1, GAME_COMPUTER);
	int res4 = findMaxLength(x, y, 1, -1, GAME_COMPUTER);
	if (res1 >= 4 || res2 >= 4 || res3 >= 4 || res4 >= 4)
	{
		this->partEnd = true;
		return MAXINT;
	}
	//return max(res1, res2, res3, res4);
	return res1 + res2 + res3 + res4;
}

int CTickTackToeWindow::cellWeight(int x, int y)
{
	int weightComputer = cellComputerWeight(x, y);
	int weightPlayer = cellPlayerWeight(x, y);
	return max(weightComputer, weightPlayer);
}

int CTickTackToeWindow::weightNeighbors(int x, int y, int dx, int dy, int color)
{
	if (withinBorder(x + dx, y + dy))
		if (field[x + dx][y + dy].figure == color)
			return 2;
		else
			return 1;
	else
		return 0;
}

int CTickTackToeWindow::neighbors(int x, int y, int color)
{
	return weightNeighbors(x, y, 1, 1, color) +
		weightNeighbors(x, y, 0, 1, color) +
		weightNeighbors(x, y, -1, 1, color) +
		weightNeighbors(x, y, 1, 0, color) +
		weightNeighbors(x, y, -1, 0, color) +
		weightNeighbors(x, y, 1, -1, color) +
		weightNeighbors(x, y, 0, -1, color) +
		weightNeighbors(x, y, -1, -1, color);
}

void CTickTackToeWindow::moveComputer()
{
	int maxWeight = -1;
	std::vector<std::pair<int, int>> resultOfWeight;
	int posx = 0, posy = 0;
	for (int x = 0; x < sizeOfBoard; x++)
		for (int y = 0; y < sizeOfBoard; y++)
		{
			if (!partEnd && field[x][y].figure == GAME_EMPTY)
			{
				field[x][y].weight = cellWeight(x, y);
				if (field[x][y].weight == maxWeight)
				{
					resultOfWeight.push_back(std::pair<int, int>(x, y));
				}
				if (field[x][y].weight > maxWeight)
				{
					maxWeight = field[x][y].weight;
					resultOfWeight.clear();
					resultOfWeight.push_back(std::pair<int, int>(x, y));
				}
			}
		}
	if (!(resultOfWeight.size() == 1))
	{
		std::vector<std::pair<int, int>> resultOfneighbors;
		int maxNeighbors = -1;
		for (int i = 0; i<resultOfWeight.size(); i++)
		{
			int x = resultOfWeight[0].first;
			int y = resultOfWeight[0].second;
			int countNeighbors = neighbors(x, y, GAME_COMPUTER);
			if (countNeighbors == maxNeighbors)
			{
				resultOfneighbors.push_back(resultOfWeight[0]);
			}
			if (countNeighbors > maxNeighbors)
			{
				countNeighbors = maxNeighbors;
				resultOfneighbors.clear();
				resultOfneighbors.push_back(std::pair<int, int>(x, y));
			}
		}
		if (!(resultOfneighbors.size() == 1))
		{
			field[resultOfneighbors[0].first][resultOfneighbors[0].second].figure = GAME_COMPUTER;
		}
		else
		{
			field[resultOfneighbors[0].first][resultOfneighbors[0].second].figure = GAME_COMPUTER;
		}
	}
	else
		field[resultOfWeight[0].first][resultOfWeight[0].second].figure = GAME_COMPUTER;
}

void CTickTackToeWindow::onButton(int xPos, int yPos) {
	if (!partEnd)
	{
		RECT rect;
		int x = xPos / sizeOfCell;
		int y = yPos / sizeOfCell;
		if (field[x][y].figure == GAME_EMPTY) {
			field[x][y].figure = GAME_PLAYER;
			cellPlayerWeight(x, y);
			countMoves++;
			if (partEnd) {
				::MessageBox(0, L"Вы выиграли", L"WIN!", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				if (isFirst || countMoves < sizeOfBoard*sizeOfBoard)
				{
					moveComputer();
					countMoves++;
				}
				if (partEnd) {
					::MessageBox(0, L"Вы проиграли", L"LOSE!", MB_OK | MB_ICONEXCLAMATION);
				}
			}
		}
		if (countMoves == sizeOfBoard*sizeOfBoard)
		{
			::MessageBox(0, L"Ничья", L"DRAW!", MB_OK | MB_ICONEXCLAMATION);
		}
		if (GetClientRect(handle, &rect)) {
			InvalidateRect(handle, &rect, false);
		}
	}
}

bool CTickTackToeWindow::RegisterClassA() {
	WNDCLASSEX tag;
	tag.cbSize = sizeof(WNDCLASSEX);
	tag.style = CS_HREDRAW | CS_VREDRAW;
	tag.lpfnWndProc = windowProc;
	tag.cbClsExtra = 0;
	tag.cbWndExtra = 0;
	tag.hIcon = (HICON)::LoadImage(::GetModuleHandle(0), (LPCWSTR)IDI_ICON1, IMAGE_ICON, 128, 128, LR_DEFAULTCOLOR);
	tag.hIconSm = (HICON)::LoadImage(::GetModuleHandle(0), (LPCWSTR)IDI_ICON1, IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	tag.hCursor = LoadCursor(NULL, IDC_ARROW);
	tag.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	tag.lpszMenuName = LPCWSTR(IDR_MENU1);
	tag.lpszClassName = nameClassWindow;
	tag.hInstance = GetModuleHandle(0);
	return ::RegisterClassEx(&tag);
}

bool CTickTackToeWindow::Create() {
	::CreateWindow(
		nameClassWindow,
		nameWindow,
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		sizeOfCell*sizeOfBoard,
		sizeOfCell*sizeOfBoard,
		0,
		0,
		GetModuleHandle(0),
		this);

	if (handle == 0) {
		::MessageBox(0, L"Can't got handle", L"ERROOR!", MB_OK | MB_ICONEXCLAMATION);
	}

	SetWindowPos(handle, HWND_TOP, 0, 0, sizeOfCell*sizeOfBoard + minSizeOfCell / 2 + 1, sizeOfCell*sizeOfBoard + maxSizeOfCell - 1, SWP_NOMOVE);

	return handle != 0;
}

void CTickTackToeWindow::Show(int cmdShow) {
	::ShowWindow(handle, cmdShow);
	::UpdateWindow(handle);
}

void CTickTackToeWindow::onDestroy() {
	PostQuitMessage(0);
}

void CTickTackToeWindow::onPaint() {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(handle, &ps);
	HDC newHdc = ::CreateCompatibleDC(hdc);
	RECT rect;
	GetClientRect(handle, &rect);

	HBITMAP bitmap = ::CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
	HGDIOBJ oldbitmap = ::SelectObject(newHdc, bitmap);

	draw(newHdc);

	BitBlt(hdc, 0, 0, rect.right, rect.bottom, newHdc, 0, 0, SRCCOPY);

	::SelectObject(newHdc, oldbitmap);
	::DeleteObject(bitmap);
	::DeleteDC(newHdc);
	::DeleteObject(oldbitmap);

	::EndPaint(handle, &ps);
}

void CTickTackToeWindow::onClose() {
	switch (MessageBox(handle, reinterpret_cast<LPCWSTR>(L"Вы уверены?"), reinterpret_cast<LPCWSTR>(L"Выйти из игры"), MB_ICONWARNING | MB_YESNO))
	{
	case IDNO:
		break;
	case IDYES:
		onDestroy();
		break;
	}
}

void CTickTackToeWindow::onNewGame() {
	initGame();
	SetWindowPos(handle, HWND_TOP, 0, 0, sizeOfCell*sizeOfBoard + minSizeOfCell/2 + 1, sizeOfCell*sizeOfBoard + maxSizeOfCell - 1, SWP_NOMOVE);
	RECT rect;
	if (GetClientRect(handle, &rect)) {
		InvalidateRect(handle, &rect, false);
	}
	
}

void CTickTackToeWindow::drawToe(HDC dc, int x, int y, HBRUSH bckgrBrush, HPEN focusBrush) {
	::SelectObject(dc, focusBrush);
	::SelectObject(dc, bckgrBrush);
	Ellipse(dc, x*sizeOfCell + MARGIN + field[x][y].randShiftLeft, y*sizeOfCell + MARGIN + field[x][y].randShiftTop,
		(x + 1)*sizeOfCell - MARGIN - field[x][y].randShiftRight, (y + 1)*sizeOfCell - MARGIN - field[x][y].randShiftDown);
}

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	return LineTo(hdc, x2, y2);
}

void CTickTackToeWindow::drawNought(HDC dc, int x, int y, HPEN hpen) {
	::SelectObject(dc, hpen);
	Line(dc, x*sizeOfCell + 2*field[x][y].randShiftLeft, y*sizeOfCell + 2*field[x][y].randShiftTop, 
		(x + 1)*sizeOfCell - 2*field[x][y].randShiftRight, (y + 1)*sizeOfCell - 2*field[x][y].randShiftDown);
	Line(dc, (x + 1)*sizeOfCell - field[x][y].randShiftLeft, y*sizeOfCell + field[x][y].randShiftTop, 
		x*sizeOfCell + field[x][y].randShiftRight, (y + 1)*sizeOfCell - field[x][y].randShiftDown);
}

LRESULT __stdcall CTickTackToeWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_NCCREATE) {
		CTickTackToeWindow* window =
			reinterpret_cast<CTickTackToeWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		::SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG>(window));
		window->handle = handle;
		return DefWindowProc(handle, message, wParam, lParam);
	}

	CTickTackToeWindow* window = reinterpret_cast<CTickTackToeWindow*>(::GetWindowLongPtr(handle, GWLP_USERDATA));

	int Id;
	switch (message) {
	case WM_CREATE:
	{
		window->OnCreate();
		return 0;
	}
	case WM_DESTROY:
	{
		window->onDestroy();
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		window->onButton(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	case WM_PAINT:
	{
		window->onPaint();
		return 0;
	}
	case WM_CLOSE:
	{
		::DestroyWindow(handle);
		return 0;
	}
	case WM_COMMAND:
	{
		switch (HIWORD(wParam)) {
		case 1:
		{
			switch (LOWORD(wParam)) {
			case ID_ACCELERATOREXIT:
			{
				::PostQuitMessage(0);
				return 0;
			}
			}
			break;
		}
		case 0:
		{
			Id = LOWORD(wParam);
			switch (Id) {
			case ID_NEW_GAME:
			{
				window->onNewGame();
				break;
			}
			case ID_SETTINGS:
			{
				::DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG1), handle, CTickTackToeWindow::dialogProc);
				break;
			}
			case ID_EXIT:
			{
				window->onClose();
				break;
			}
			default:
				break;
			}
		}

		}
	}
	}
	return ::DefWindowProc(handle, message, wParam, lParam);
	
}

void CTickTackToeWindow::OnCreate()
{
	haccel = LoadAccelerators(GetModuleHandle(0), MAKEINTRESOURCE(ID_ACCELERATOREXIT));
}

void CTickTackToeWindow::draw(HDC dc) {
	RECT mainRect;

	GetClientRect(handle, &mainRect);
	HBRUSH brush = CreateSolidBrush(RGB(250, 250, 250));
	FillRect(dc, &mainRect, brush);
	RECT myrect;
	HBRUSH cellBrush;
	cellBrush = ::CreateSolidBrush(RGB(200, 200, 200));
	HPEN hpenCross = CreatePen(PS_SOLID, 5, colorCross);
	HPEN hpenToe = CreatePen(PS_SOLID, 5, colorToe);
	HPEN hpenWin = CreatePen(PS_SOLID, 5, colorWin);
	for (int x = 0; x < sizeOfBoard; x++)
		for (int y = 0; y < sizeOfBoard; y++)
		{
			if (field[x][y].isBlackCell) {
				myrect.right = x*sizeOfCell;
				myrect.top = y*sizeOfCell;
				myrect.left = (x + 1)*sizeOfCell;
				myrect.bottom = (y + 1)*sizeOfCell;
				FillRect(dc, &myrect, cellBrush);
			}
			
		}
	for (int x = 0; x < sizeOfBoard; x++)
		for (int y = 0; y < sizeOfBoard; y++)
		{
			if (field[x][y].isWin == true)
			{
				if (field[x][y].figure == GAME_COMPUTER) {
					if (isCross)
					{
						if (field[x][y].isBlackCell)
							drawToe(dc, x, y, cellBrush, hpenWin);
						else
							drawToe(dc, x, y, brush, hpenWin);
					}
					else {
						drawNought(dc, x, y, hpenWin);
					}

				}
				if (field[x][y].figure == GAME_PLAYER) {
					if (isCross)
					{
						drawNought(dc, x, y, hpenWin);
					}
					else {
						if (field[x][y].isBlackCell)
							drawToe(dc, x, y, cellBrush, hpenWin);
						else
							drawToe(dc, x, y, brush, hpenWin);
					}
				}
			}
			else {
				if (field[x][y].figure == GAME_COMPUTER) {
					if (isCross)
					{
						if (field[x][y].isBlackCell)
							drawToe(dc, x, y, cellBrush, hpenToe);
						else
							drawToe(dc, x, y, brush, hpenToe);
					}
					else {
						drawNought(dc, x, y, hpenCross);
					}

				}
				if (field[x][y].figure == GAME_PLAYER) {
					if (isCross)
					{
						drawNought(dc, x, y, hpenCross);
					}
					else {
						if (field[x][y].isBlackCell)
							drawToe(dc, x, y, cellBrush, hpenToe);
						else
							drawToe(dc, x, y, brush, hpenToe);
					}
				}
			}
		}
	::DeleteObject(brush);
	::DeleteObject(cellBrush);
	::DeleteObject(hpenCross);
	::DeleteObject(hpenToe);
	::DeleteObject(hpenWin);
}

HWND CTickTackToeWindow::GetHandle() const
{
	return handle;
}

HACCEL CTickTackToeWindow::GetHaccel() const
{
	return haccel;
}

BOOL __stdcall CTickTackToeWindow::dialogProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {

	CTickTackToeWindow* window = reinterpret_cast<CTickTackToeWindow*>(::GetWindowLongPtr(::GetParent(handle),
		GWLP_USERDATA));

	switch (message) {
	case WM_INITDIALOG:
	{
		window->OnDlgInit(handle);
		return TRUE;
	}
	case WM_HSCROLL:
	{
		window->OnDlgScroll(handle);
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDCANCEL:
		{
			window->OnDlgCancel(handle);
			return TRUE;
		}
		case IDOK:
		{
			window->OnDlgOk(handle);
			return TRUE;
		}
		case IDC_BUTTON1:
		{
			window->OnDlgChangeColor(handle, IDC_BUTTON1);
			return TRUE;
		}
		case IDC_BUTTON2:
		{
			window->OnDlgChangeColor(handle, IDC_BUTTON2);
			return TRUE;
		}
		case IDC_BUTTON3:
		{
			window->OnDlgChangeColor(handle, IDC_BUTTON3);
			return TRUE;
		}
		case IDC_CHECK1:
		{
			window->OnDlgCheck(handle, IDC_CHECK1);
			return TRUE;
		}
		case IDC_CHECK2:
		{
			window->OnDlgCheck(handle, IDC_CHECK2);
			return TRUE;
		}
		}
	}
	}

	return FALSE;
}

void CTickTackToeWindow::OnDlgOk(HWND handle)
{
	CTickTackToeWindow* window = reinterpret_cast<CTickTackToeWindow*>(::GetWindowLongPtr(GetParent(handle),
		GWLP_USERDATA));

	window->colorCross = window->tmpColorCross;
	window->colorToe = window->tmpColorToe;
	window->isCross = window->tmpIsCross;
	if (window->isFirst != window->tmpIsFirst)
	{
		window->isFirst = window->tmpIsFirst;
		window->onNewGame();
	}
	window->colorWin = window->tmpColorWin;
	if (window->sizeOfCell != window->tmpSizeOfCell)
	{
		window->sizeOfCell = window->tmpSizeOfCell;
		SetWindowPos(window->handle, HWND_TOP, 0, 0, sizeOfCell*sizeOfBoard + minSizeOfCell / 2 + 1, sizeOfCell*sizeOfBoard + maxSizeOfCell - 1, SWP_NOMOVE);
		RECT rect;
		if (GetClientRect(window->handle, &rect)) {
			InvalidateRect(window->handle, &rect, TRUE);
		}
	}
	if (window->sizeOfBoard != window->tmpSizeOfBoard)
	{
		window->sizeOfBoard = window->tmpSizeOfBoard;
		window->onNewGame();
	}

	::EndDialog(handle, 0);
	::InvalidateRect(window->handle, NULL, 1);
}

void CTickTackToeWindow::OnDlgCancel(HWND handle)
{
	CTickTackToeWindow* window = reinterpret_cast<CTickTackToeWindow*>(::GetWindowLongPtr(GetParent(handle),
		GWLP_USERDATA));

	window->tmpColorCross = window->colorCross;
	window->tmpColorToe = window->colorToe;
	window->tmpIsCross = window->isCross;
	window->tmpIsFirst = window->isFirst;
	window->tmpColorWin = window->colorWin;
	window->tmpSizeOfBoard = window->sizeOfBoard;
	window->tmpSizeOfCell = window->sizeOfCell;

	::EndDialog(handle, 0);
}

void CTickTackToeWindow::OnDlgChangeColor(HWND handle, UINT buttonID)
{
	CTickTackToeWindow* window = reinterpret_cast<CTickTackToeWindow*>(::GetWindowLongPtr(GetParent(handle),
		GWLP_USERDATA));

	CHOOSECOLOR cc;

	static COLORREF arrayCustomColor[16];

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = handle;
	cc.lpCustColors = (LPDWORD)arrayCustomColor;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (::ChooseColor(&cc) == TRUE)
	{
		switch (buttonID) {
		case IDC_BUTTON1:
		{
			window->tmpColorCross = cc.rgbResult;
			break;
		}
		case IDC_BUTTON2:
		{
			window->tmpColorToe = cc.rgbResult;
			break;
		}
		case IDC_BUTTON3:
		{
			window->tmpColorWin = cc.rgbResult;
			break;
		}
		}
	}
}

void CTickTackToeWindow::OnDlgCheck(HWND handle, UINT checkBoxID)
{
	CTickTackToeWindow* window = reinterpret_cast<CTickTackToeWindow*>(::GetWindowLongPtr(GetParent(handle),
		GWLP_USERDATA));
	switch (checkBoxID) {
	case IDC_CHECK1:
	{
		HWND previewCheckBox = ::GetDlgItem(handle, IDC_CHECK1);
		window->tmpIsCross = (BOOL) ::SendMessage(previewCheckBox, BM_GETCHECK, 0, 0);
		DeleteObject(previewCheckBox);
		break;
	}
	case IDC_CHECK2:
	{
		HWND previewCheckBox = ::GetDlgItem(handle, IDC_CHECK2);
		window->tmpIsFirst = (BOOL) ::SendMessage(previewCheckBox, BM_GETCHECK, 0, 0);
		DeleteObject(previewCheckBox);
		break;
	}
	}
	
}

void CTickTackToeWindow::OnDlgInit(HWND handle)
{
	CTickTackToeWindow* window = reinterpret_cast<CTickTackToeWindow*>(::GetWindowLongPtr(GetParent(handle),
		GWLP_USERDATA));
	
	HWND handleCheckBox1 = ::GetDlgItem(handle, IDC_CHECK1);
	if (window->isCross) {
		::SendMessage(handleCheckBox1, BM_SETCHECK, BST_CHECKED, 0);
	}
	else {
		::SendMessage(handleCheckBox1, BM_SETCHECK, BST_UNCHECKED, 0);
	}
	DeleteObject(handleCheckBox1);

	HWND handleCheckBox2 = ::GetDlgItem(handle, IDC_CHECK2);
	if (window->isFirst) {
		::SendMessage(handleCheckBox2, BM_SETCHECK, BST_CHECKED, 0);
	}
	else {
		::SendMessage(handleCheckBox2, BM_SETCHECK, BST_UNCHECKED, 0);
	}
	DeleteObject(handleCheckBox2);

	HWND handleSizeOfBoardSlider = ::GetDlgItem(handle, IDC_SLIDER1);
	::SendMessage(handleSizeOfBoardSlider, TBM_SETRANGE, TRUE, MAKELONG(minSizeOfBoard, maxSizeOfBoard));
	::SendMessage(handleSizeOfBoardSlider, TBM_SETPOS, TRUE, window->sizeOfBoard);

	HWND handleSizeOfCellSlider = ::GetDlgItem(handle, IDC_SLIDER2);
	::SendMessage(handleSizeOfCellSlider, TBM_SETRANGE, TRUE, MAKELONG(minSizeOfCell, maxSizeOfCell));
	::SendMessage(handleSizeOfCellSlider, TBM_SETPOS, TRUE, window->sizeOfCell);

}

void CTickTackToeWindow::OnDlgScroll(HWND handle) {
	CTickTackToeWindow* window = reinterpret_cast<CTickTackToeWindow*>(::GetWindowLongPtr(GetParent(handle),
		GWLP_USERDATA));

	int positionScrollSizeOfBoard = ::SendMessage(::GetDlgItem(handle, IDC_SLIDER1), TBM_GETPOS, 0, 0);
	int positionScrollSizeOfCell = ::SendMessage(::GetDlgItem(handle, IDC_SLIDER2), TBM_GETPOS, 0, 0);

	window->tmpSizeOfBoard = positionScrollSizeOfBoard;
	window->tmpSizeOfCell = positionScrollSizeOfCell;
}