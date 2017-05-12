/*
	Utility Library
	by FattyMieo
	Last update : 22/03/2017
	Version 1.0
*/

#ifndef	_UTIL_
#define	_UTIL_

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

static class Utilities
{
public:
	static int HexToInt(char hex);					// Change hex to int
	static char IntToHex(int i);					// Change int to hex
	static int Rand_Int(int min, int max);			// Generate a random integer between two numbers
	static int cin_int();									// Input an integer with iterations
	static char cin_char();								// Input a character with iterations
	static string cin_string();							// Input a string with iterations
	static int cin_int(int min, int max);					// Input an integer with iterations	(With limits)
	static char cin_char(char* charList, int charAmount);	// Input a character with iterations	(With limits)
	static string cin_string(int min, int max);			// Input a string with iterations	(With limits)
}util;

#endif
