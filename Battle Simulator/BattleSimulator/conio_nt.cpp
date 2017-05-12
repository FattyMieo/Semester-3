/*
	Console Input/Output written by Nicholas Tan.
	Revamped by FattyMieo
	Last update : 22/03/2017
	conio_nt : version 1.1
*/

#include "conio_nt.h"

void SetForeColor(COLOR color)
{
	foregroundColor = color;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color + (backgroundColor << 4));
}

void SetBackColor(COLOR color)
{
	backgroundColor = color;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), foregroundColor + (color << 4));
}

void GoToXY(int x, int y)
{
	COORD coordinate;

	coordinate.X = x;
	coordinate.Y = y ;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}

int GetCursorX()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	return info.dwCursorPosition.X;
}

int GetCursorY()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	return info.dwCursorPosition.Y;
}

void SetCursorType(int type)
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = type;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void HideCursor()
{
	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void ShowCursor()
{
	CONSOLE_CURSOR_INFO info;
	info.bVisible = true;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void ClrScreen()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	COORD coordinate;
	DWORD consoleSize;
	DWORD written;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	coordinate.X = 0;
	coordinate.Y = 0;
	consoleSize = info.dwSize.X * info.dwSize.Y;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', consoleSize, coordinate, &written);
	FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), info.wAttributes, consoleSize, coordinate, &written);
	GoToXY(0, 0);
}

void ClrLine(int y)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	COORD coordinate;
	DWORD written;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	coordinate.X = info.dwCursorPosition.X;
	coordinate.Y = y;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', info.dwSize.X * info.dwCursorPosition.Y, coordinate, &written);
	GoToXY(info.dwCursorPosition.X, info.dwCursorPosition.Y);
}

void SetDisplayMode(int mode)
{
	COORD coordinate;
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), mode, &coordinate);
}

void SetWindowSize(short width, short height)
{
	SMALL_RECT windowSize;
	COORD largestSize;
	COORD coordinate;

	largestSize = GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));

	if (largestSize.X < width) width = largestSize.X - 2;
	if (largestSize.Y < height) height = largestSize.Y - 2;

	windowSize.Left = 0;
	windowSize.Top = 0;
	windowSize.Right = width - 1;
	windowSize.Bottom = height - 1;
	
	coordinate.X = width;
	coordinate.Y = height;

	if (!SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coordinate)) OutputDebugString("Failed to set screen buffer size");
	else OutputDebugString("Buffer Size Success");

	if(!SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize)) OutputDebugString("Failed to set window size");
	else OutputDebugString("Window Size Success");
}

void SetFontSize(int size)
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   	// Width of each character in the font
	cfi.dwFontSize.Y = size;                	// Height of each character in the font
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	std::wcscpy(cfi.FaceName, L"Consolas");		// Font type to Consolas
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void SetQuickEditMode(bool b)
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hIn, &dwMode);
	if(!b)
		dwMode &= ~ENABLE_QUICK_EDIT_MODE;
	else
		dwMode |= ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hIn, dwMode | ENABLE_EXTENDED_FLAGS);
}
