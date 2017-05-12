/* 
	Console Input/Output written by Nicholas Tan.
	Revamped by FattyMieo
	Last update : 22/03/2017
	conio_nt : version 1.1
*/

#ifndef	_CONIO_NT_

#include <windows.h>
#include <cwchar>

enum COLOR
{
	BLACK,
	DARKBLUE,
	DARKGREEN,
	DARKCYAN,
	DARKRED,
	DARKMAGENTA,
	DARKYELLOW,
	GRAY,
	DARKGRAY,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	YELLOW,
	WHITE
};

static COLOR foregroundColor = GRAY;
static COLOR backgroundColor = BLACK;

void SetForeColor(COLOR color);						// Changes the color of the text.
void SetBackColor(COLOR color);						// Changes the color behind the text.
void GoToXY(int x, int y);							// Sets the cursor to the position you want.
int GetCursorX();									// Get the cursors current X position.
int GetCursorY();									// Get the cursors current Y position.
void SetCursorType(int type);						// Sets the size and visibility of the cursor
void HideCursor();									// Hides the cursor.
void ShowCursor();									// Shows the cursor.
void ClrScreen();									// Clear the entire screen.
void ClrLine();										// Clear the current line the cursor is on.
void SetDisplayMode(int mode);						// Full screen - 1 / Window mode - 2
void SetWindowSize(short width, short height);		// Sets the size of the console.
void SetFontSize(int size);							// Sets the size of the font.
void SetQuickEditMode(bool b);						// Quick Edit Mode: Disable to prevent mouse clicks from interrupting the program

#define	_CONIO_NT_
#endif
