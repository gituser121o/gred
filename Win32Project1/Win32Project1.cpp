// Win32Project1.cpp : Defines the entry point for the application.
//
//TODO:
//сохранение
//загрузка
//масштабирование
//перемещение
//стирание
//спрей
//текст
//ломаная
//многоугольник
//параметры ручки/кисти
//больше цветов
// *скролиг
//эффекты

#include "stdafx.h"
#include "Win32Project1.h"
#include "WinGDI.h"
#include <string.h>
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32PROJECT1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

struct colors
{
	BYTE red;
	BYTE green;
	BYTE blue;
	int x;
	int y;
};

struct instr
{
	int x;
	int y;
};

int currentColorNumber=0;
COLORREF currentColor;
int colorCount = 8;
int xpos, ypos;
int colorSize = 20;
int instrumentSize = 20;
int instrumentXOffset;
int innstrumentYOffset;
bool flag;
int top;
int right;
int menu;      //left menu border
int bottom;
bool pause;
colors color[8];
int instrumentBarOffset;
int colorOffset;
int instrumentCount = 6;
int currentInstrument;
instr instruments[8];
HBITMAP bitmaps[5000];
int curBitmap=-1;
int bmpCount = 0;


void DrawColorBar(HDC hdc)
{
	RECT rect;
	HBRUSH br = CreateSolidBrush(currentColor);
	rect.top = 10;
	rect.bottom = 60;
	rect.right = right-27;
	rect.left = right - 77;
	FillRect(hdc, &rect, br);
}



void DrawMenu(HDC hdc,HWND hWnd)
{
	color[0].red = 0;
	color[0].green = 0;
	color[0].blue = 0;
	color[0].x = 0;
	color[0].y = 0;

	color[1].red = 255;
	color[1].green = 255;
	color[1].blue = 255;
	color[1].x = colorSize + 2;
	color[1].y = 0;

	color[2].red = 255;
	color[2].green = 0;
	color[2].blue = 0;
	color[2].x = 0;
	color[2].y = colorSize + 2;

	color[3].red = 0;
	color[3].green = 255;
	color[3].blue = 0;
	color[3].x = colorSize+2;
	color[3].y = colorSize+2;

	color[4].red = 0;
	color[4].green = 0;
	color[4].blue = 255;
	color[4].x = 0;
	color[4].y = 2 * (colorSize + 2);

	color[5].red = 255;
	color[5].green = 255;
	color[5].blue = 0;
	color[5].x = colorSize + 2;
	color[5].y = 2 * (colorSize + 2);

	color[6].red = 255;
	color[6].green = 182;
	color[6].blue = 193; 
	color[6].x = 0;
	color[6].y = 3 * (colorSize + 2);

	color[7].red = 128;
	color[7].green = 0;
	color[7].blue = 128;
	color[7].x = colorSize + 2;
	color[7].y = 3 * (colorSize + 2);

	instruments[0].x = 0;
	instruments[0].y = 0;

	instruments[1].x = instrumentSize + 2;
	instruments[1].y = 0;

	instruments[2].x = 0;
	instruments[2].y = instrumentSize + 2;

	instruments[3].x = instrumentSize + 2;
	instruments[3].y = instrumentSize + 2;

	instruments[4].x = 0;
	instruments[4].y = 2*(instrumentSize + 2);

	instruments[5].x = (instrumentSize + 2);
	instruments[5].y = 2 * (instrumentSize + 2);

	currentColor = RGB(color[currentColorNumber].red, color[currentColorNumber].green, color[currentColorNumber].blue);
	RECT win;
	GetClientRect(hWnd, &win);
	Rectangle(hdc, win.right - 90, win.top - 5, win.right, win.bottom + 5);
	RECT rect;
	rect.bottom = win.bottom + 5;
	rect.top = win.top - 5;
	rect.left = win.right - 105;
	rect.right = win.right;
	HBRUSH br = CreateSolidBrush(RGB(235, 235, 235));
	FillRect(hdc, &rect, br);
	rect.top = win.top + 65;
	rect.left = win.right - 75;
	colorOffset = rect.left;
	rect.right = win.right - 30;
	rect.bottom = 65+(colorSize+2)*(colorCount/2);
	instrumentBarOffset = rect.bottom + 30;
	//DeleteObject(br);
	br = CreateSolidBrush(RGB(245, 245, 245));
	//Rectangle(hdc, rect.left,rect.top,rect.right,rect.bottom);
	FillRect(hdc, &rect, br);
	right = win.right;
	top = win.top;
	bottom = win.bottom;
	instrumentXOffset = win.right - 73;
	for (int i = 0; i < colorCount; i++)
	{
		br = CreateSolidBrush(RGB(color[i].red, color[i].green, color[i].blue));
		rect.top = win.top + 67 + color[i].y;
		rect.left = win.right - 73 + color[i].x;
		rect.right = win.right - 73+colorSize + color[i].x;
		rect.bottom = win.top + 67+colorSize + color[i].y;
		FillRect(hdc, &rect, br);
	}
	menu = right - 105;
	DrawColorBar(hdc);
	rect.top = instrumentBarOffset;
	rect.bottom = win.bottom;
	rect.left = colorOffset;
	rect.right = win.right-30;
	br = CreateSolidBrush(RGB(245, 245, 245));
	FillRect(hdc, &rect, br);
	for (int i = 0; i < instrumentCount; i++)
	{
		rect.top = instrumentBarOffset + 1 + instruments[i].y;
		rect.bottom = instrumentBarOffset + 1 + instruments[i].y+instrumentSize;
		rect.left = instrumentXOffset + 1 + instruments[i].x;
		rect.right = instrumentXOffset + 1 + instruments[i].x+instrumentSize;
		br = CreateSolidBrush(RGB(100, 100, 100));
		FillRect(hdc, &rect, br);
	}
}


void CheckColor(int x, int y,HWND hWnd)
{
	if (((y < top+67) || (y>top+230)) || (x < right - 80) || (x>right-25))
		return;
	
	for (int i = 0; i < colorCount; i++)
	{
		if ((y >=top+ 67 + color[i].y) && (y <= 67+colorSize + color[i].y) && (x >=right- 73 + color[i].x) && (x <=right- 73+colorSize + color[i].x))
		{
			currentColorNumber = i;
			HDC hdc = GetDC(hWnd);
			currentColor = RGB(color[i].red, color[i].green, color[i].blue);
			DrawColorBar(hdc);
			ReleaseDC(hWnd, hdc);			
		}
	}
}

void CheckInstruments(int x, int y, HWND hWnd)
{
	for (int i = 0; i < instrumentCount; i++)
	{
		if ((y >= instrumentBarOffset + instruments[i].y) && (y <= instrumentBarOffset + instruments[i].y + instrumentSize) && (x >= instrumentXOffset + instruments[i].x) && (x <= instrumentXOffset + instruments[i].x + instrumentSize))
		{
			currentInstrument = i;			
		}
	}
}
void CreateManifest(char* s)
{

}


HBITMAP hBitmap = NULL;
void CrBitmap(HDC hdc)
{
	if (curBitmap < 0) curBitmap = 0;
	curBitmap++;
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP btmp = CreateCompatibleBitmap(hdc, menu, bottom);
	//SelectObject(memDC, btmp);
	HANDLE oldBitmap = SelectObject(memDC, btmp);
	BitBlt(memDC, 0, 0, menu, bottom, hdc, 0, 0, SRCCOPY);
	bitmaps[curBitmap] = btmp;
	

	SelectObject(memDC, oldBitmap);
	
}
bool cz = false;
void LdBitmap(HDC hdc,HWND hWnd)
{

	HDC hdcMem = CreateCompatibleDC(hdc);	
	if (cz)
	{
		if (curBitmap > 1) curBitmap--;
		
	}
	//if (curBitmap < 0) curBitmap = 0;
	//if (curBitmap < 0) curBitmap = 5;
	
		hBitmap = bitmaps[curBitmap];
	HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmap);
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(bitmap), &bitmap);
	BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
	
	
}

/*void FillArea(int x, int y, COLORREF n, COLORREF old,HWND hWnd)
{
	HDC	hdc = GetDC(hWnd);
	if (((x<0) || (x>right)) || (y<0) || (y>bottom)) return;
	COLORREF cur = GetPixel(hdc, x, y);
	if (cur == old)
	{
		SetPixel(hdc, x, y, n);
		FillArea(x - 1, y, n, old, hWnd);
		FillArea(x - 1, y-1, n, old, hWnd);
		FillArea(x - 1, y+1, n, old, hWnd);
		FillArea(x, y-1, n, old, hWnd);
		FillArea(x , y+1, n, old, hWnd);
		FillArea(x + 1, y + 1, n, old, hWnd);
		FillArea(x + 1, y , n, old, hWnd);
		FillArea(x + 1, y - 1, n, old, hWnd);
	}
	ReleaseDC(hWnd, hdc);
}*/

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hdcMem;
	HBITMAP hb;
	HANDLE han;
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		if ((GetAsyncKeyState(VK_CONTROL)) && (GetAsyncKeyState(0x5A))){
			cz = true;
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
			//hdc = GetDC(hWnd);
		//	LdBitmap(hdc, hWnd);
	}
		break;
	case WM_LBUTTONDOWN:
		xpos = LOWORD(lParam);
		ypos = HIWORD(lParam);
		if (xpos < menu) flag = true;
		else
		{
			//hdc = GetDC(hWnd);
			CheckColor(xpos, ypos, hWnd);
			if (ypos >= instrumentBarOffset)
				CheckInstruments(xpos, ypos, hWnd);
		}
		if (currentInstrument == 5)
		{
			hdc = GetDC(hWnd);
			//LOGBRUSH lb;
			//lb.lbStyle = BS_NULL;
			HBRUSH hb = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, hb);
			RECT rect;
			rect.left = LOWORD(lParam)-5;
			rect.right = LOWORD(lParam) + 5;
			rect.top = HIWORD(lParam) - 5;
			rect.bottom=HIWORD(lParam)+5;
			FillRect(hdc, &rect, hb);
			ReleaseDC(hWnd, hdc);
		}
		break;

	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if (flag){
			SelectObject(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, currentColor);
			if (currentInstrument == 0)
			{
				
				SelectObject(hdc, GetStockObject(DC_PEN));
				SetDCPenColor(hdc, currentColor);
				if ((xpos >= menu) || (LOWORD(lParam) >= menu))
				{

					MoveToEx(hdc, xpos, ypos, NULL);
					if (!pause) LineTo(hdc, menu, HIWORD(lParam));
					xpos = menu - 1;
					pause = true;
				}
				else
				{
					MoveToEx(hdc, xpos, ypos, NULL);
					LineTo(hdc, LOWORD(lParam), HIWORD(lParam));
					xpos = LOWORD(lParam);
					pause = false;
				}
				ypos = HIWORD(lParam);
				
			}
			else
			{
				if (currentInstrument == 2)
				{

					LdBitmap(hdc, hWnd);
					MoveToEx(hdc, xpos, ypos, NULL);
					LineTo(hdc, LOWORD(lParam), HIWORD(lParam));
				}
				else
				{
					if (currentInstrument == 3)
					{
							hdcMem = CreateCompatibleDC(hdc);
							hb = CreateCompatibleBitmap(hdc, menu, bottom);
							han = SelectObject(hdcMem, hb);
							RECT rect;
							GetClientRect(hWnd, &rect);
							FillRect(hdcMem, &rect, WHITE_BRUSH);
							LdBitmap(hdcMem, hWnd);
						
							LOGBRUSH lb;
							lb.lbStyle = BS_NULL;
							//HBRUSH hb = CreateBrushIndirect(&lb);
							HBRUSH hb = CreateSolidBrush(currentColor);
							SelectObject(hdcMem, hb);
							Ellipse(hdcMem, xpos, ypos, LOWORD(lParam), HIWORD(lParam));
							BitBlt(hdc, 0, 0, menu, bottom, hdcMem, 0, 0, SRCCOPY);
							DeleteDC(hdcMem);
					}
					if (currentInstrument == 1)
					{
						LdBitmap(hdc, hWnd);
						LOGBRUSH lb;
						lb.lbStyle = BS_NULL;
						HBRUSH hb = CreateBrushIndirect(&lb);
						SelectObject(hdc, hb);
						SetBkMode(hdc, TRANSPARENT);
						Rectangle(hdc, LOWORD(lParam), HIWORD(lParam), xpos, ypos);
					}
				}
			}
			if (currentInstrument == 5)
			{
				//LOGBRUSH lb;
				//lb.lbStyle = BS_NULL;
				HBRUSH hb = CreateSolidBrush(RGB(255, 255, 255));
				SelectObject(hdc, hb);
				RECT rect;
				rect.left = LOWORD(lParam) - 5;
				rect.right = LOWORD(lParam) + 5;
				rect.top = HIWORD(lParam) - 5;
				rect.bottom = HIWORD(lParam) + 5;
				FillRect(hdc, &rect, hb);
			}

		}
		ReleaseDC(hWnd, hdc);
		break;

	
	case WM_LBUTTONUP:
		hdc = GetDC(hWnd);
		if ((flag)&&(currentInstrument==4))
		{
			if (GetPixel(hdc, xpos, ypos) != currentColor)
			{
				SelectObject(hdc, GetStockObject(DC_BRUSH));
				SetDCBrushColor(hdc, currentColor);
				ExtFloodFill(hdc, xpos, ypos, GetPixel(hdc, xpos, ypos), FLOODFILLSURFACE);
				//FillArea(LOWORD(lParam), HIWORD(lParam), currentColor, GetPixel(hdc, LOWORD(lParam), HIWORD(lParam)), hWnd);
			}
		}
		flag = false;	
		
		
		
		/*if (currentInstrument == 1)
		{	
			

		}*/
		if (currentInstrument == 2)
		{
			
			MoveToEx(hdc, LOWORD(lParam), HIWORD(lParam), NULL);
			LineTo(hdc, xpos, ypos);
		}
		SetCapture(hWnd);
		CrBitmap(hdc);
		ReleaseDC(hWnd, hdc);
		break;
		
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawMenu(hdc, hWnd);
		//if (cz)
		//{
			
			LdBitmap(hdc, hWnd);
			cz = false;
		//}
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
