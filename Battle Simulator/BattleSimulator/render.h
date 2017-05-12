#ifndef	_RENDER_
#define	_RENDER_

#include <iostream>
#include "conio_nt.h"

using namespace std;

class Sprite
{
protected:
	int width;
	int height;
	
	COLOR** colorList;
	
public:
	Sprite()
	{
		colorList = NULL;
	}
	
	Sprite(const int* spriteArray, int width, int height)
	{
		SetDimension(width, height);
		Import(spriteArray);
	}
	
	virtual bool IsNull();
	
	void SetDimension(int width, int height);
	
	void Import(const int* spriteArray);
	
	virtual void Draw(int x, int y);
};

class ASCIISprite : public Sprite
{
protected:
	char** charList;
	
public:
	ASCIISprite() : Sprite()
	{
		charList = NULL;
	}
	
	ASCIISprite(const string* spriteArray, const int* colorArray, int width, int height)
	{
		SetDimension(width, height);
		Import(spriteArray, colorArray);
	}
	
	bool IsNull();
	
	void Import(const string* spriteArray, const int* colorArray);
	
	void Draw(int x, int y);
};

class Spritesheet
{
public:
	const static string GAMETITLE[8];
	const static int MARIO[16][16];
	const static int FOOTMAN[16][16];
	const static int ARCHER[16][16];
	const static int ROGUE[16][16];
	const static int KNIGHT[16][16];
};

#endif
