#include "render.h"

bool Sprite::IsNull()
{
	return colorList == NULL;
}

void Sprite::SetDimension(int width, int height)
{
	this->width = width;
	this->height = height;
}
	
void Sprite::Import(const int* spriteArray)
{	
	colorList = new COLOR*[height];
	
	for(int i = 0; i < height; i++)
	{
		colorList[i] = new COLOR[width];
	}
	
	if(spriteArray == NULL) return;
	
	for(int i = 0; i < height; i++)
	for(int j = 0; j < width; j++)
		colorList[i][j] = (COLOR)spriteArray[i * width + j];
}

void Sprite::Draw(int x, int y)
{	
	for(int i = 0; i < height / 2; i++)
	{
		for(int j = 0; j < width; j++)
		{
			SetForeColor(colorList[i*2][j]);
			if(i*2+1 < height)
				SetBackColor(colorList[i*2+1][j]);
			else
				SetBackColor(BLACK);
			GoToXY(x + j, y + i);
			cout << char(223);
		}
	}
	
	SetForeColor(GRAY);
	SetBackColor(BLACK);
}

bool ASCIISprite::IsNull()
{
	return charList == NULL;
}

void ASCIISprite::Import(const string* spriteArray, const int* colorArray)
{
	charList = new char*[height];
	
	for(int i = 0; i < height; i++)
	{
		charList[i] = new char[width];
	}
	
	colorList = new COLOR*[height];
	
	for(int i = 0; i < height; i++)
	{
		colorList[i] = new COLOR[width];
	}
	
	if(spriteArray == NULL) return;
	
	bool defaultColor = colorArray == NULL;
	
	for(int i = 0; i < height; i++)
	{
		const char* cstr = spriteArray[i].c_str();
		for(int j = 0; j < width; j++)
		{
			charList[i][j] = cstr[j];
			if(!defaultColor)
				colorList[i][j] = (COLOR)colorArray[i * width + j];
			else
				colorList[i][j] = GRAY;
		}
	}
}

void ASCIISprite::Draw(int x, int y)
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			SetForeColor(colorList[i][j]);
			GoToXY(x + j, y + i);
			cout << charList[i][j];
		}
	}
	
	SetForeColor(GRAY);
}

// Spritesheets

const string Spritesheet::GAMETITLE[8]
{
	" _______  _______  _______                 _______  _        _______  _______ ",
	"(  ____ \\(  ___  )(  ____ )      |\\     /|(  ___  )( (    /|(  ___  )(  ____ )",
	"| (    \\/| (   ) || (    )|      | )   ( || (   ) ||  \\  ( || (   ) || (    )|",
	"| (__    | |   | || (____)|      | (___) || |   | ||   \\ | || |   | || (____)|",
	"|  __)   | |   | ||     __)      |  ___  || |   | || (\\ \\) || |   | ||     __)",
	"| (      | |   | || (\\ (         | (   ) || |   | || | \\   || |   | || (\\ (   ",
	"| )      | (___) || ) \\ \\__      | )   ( || (___) || )  \\  || (___) || ) \\ \\__",
	"|/       (_______)|/   \\__/      |/     \\|(_______)|/    )_)(_______)|/   \\__/"
};

const int Spritesheet::MARIO[16][16]
{
	{ 0,  0,  0,  0,  0, 12, 12, 12, 12, 12,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0, 12, 12, 12, 12, 12, 12, 12, 12, 12,  0,  0,  0},
	{ 0,  0,  0,  0,  6,  6,  6, 14, 14,  6, 14,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  6, 14,  6, 14, 14, 14,  6, 14, 14, 14,  0,  0,  0},
	{ 0,  0,  0,  6, 14,  6,  6, 14, 14, 14,  6, 14, 14, 14,  0,  0},
	{ 0,  0,  0,  6,  6, 14, 14, 14, 14,  6,  6,  6,  6,  0,  0,  0},
	{ 0,  0,  0,  0,  0, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  6,  6, 12,  6,  6,  6,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  6,  6,  6, 12,  6,  6, 12,  6,  6,  6,  0,  0,  0},
	{ 0,  0,  6,  6,  6,  6, 12, 12, 12, 12,  6,  6,  6,  6,  0,  0},
	{ 0,  0, 14, 14,  6, 12, 14, 12, 12, 14, 12,  6, 14, 14,  0,  0},
	{ 0,  0, 14, 14, 14, 12, 12, 12, 12, 12, 12, 14, 14, 14,  0,  0},
	{ 0,  0, 14, 14, 12, 12, 12, 12, 12, 12, 12, 12, 14, 14,  0,  0},
	{ 0,  0,  0,  0, 12, 12, 12,  0,  0, 12, 12, 12,  0,  0,  0,  0},
	{ 0,  0,  0,  6,  6,  6,  0,  0,  0,  0,  6,  6,  6,  0,  0,  0},
	{ 0,  0,  6,  6,  6,  6,  0,  0,  0,  0,  6,  6,  6,  6,  0,  0}
};

const int Spritesheet::FOOTMAN[16][16]
{
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0, 15,  0,  0,  0,  8,  8,  8,  8,  8,  0,  0,  0,  0},
	{ 0,  0, 15, 15,  0,  0,  0,  8,  7,  7,  7,  8,  0,  0,  0,  0},
	{ 0,  0, 15, 15,  0,  8,  0,  8,  8,  7,  8,  8,  0,  8,  0,  0},
	{ 0,  0, 15, 15, 0 ,  8,  0,  8,  8,  7, 15, 15, 15, 15, 15,  0},
	{ 0,  0, 15, 15,  8,  8,  0,  8,  8, 15, 15, 15, 15, 15, 15, 15},
	{ 0,  0, 15, 15,  8,  8,  0,  0,  0, 15, 15, 15, 15, 15, 15, 15},
	{ 0,  0, 15, 15,  0,  8,  8,  8,  8, 15, 15, 15, 15, 15, 15, 15},
	{ 0,  0, 15, 15,  0,  8,  8,  8,  8, 15, 15, 15, 15, 15, 15, 15},
	{ 0,  7,  7,  7,  7,  0,  8,  8,  8, 15, 15, 15, 15, 15, 15, 15},
	{ 0,  0,  8,  8,  8,  8,  0,  8,  8, 15, 15, 15, 15, 15, 15, 15},
	{ 0,  0,  7,  7,  8,  8,  0,  0,  0, 15, 15, 15, 15, 15, 15, 15},
	{ 0,  0,  0,  7,  0,  0,  0,  8,  8,  8, 15, 15, 15, 15, 15,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  8,  8,  8,  8, 15, 15, 15,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  8,  8,  0,  8,  8, 15,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  8,  8,  0,  8,  8,  0,  0,  0,  0}
};

const int Spritesheet::ARCHER[16][16]
{
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0,  8,  8,  8,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0, 15,  0,  8,  7,  7,  7,  8,  0,  0,  0,  0},
	{ 0,  0,  0,  0, 15,  7,  0,  7,  7,  7,  7,  7,  0,  8,  0,  0},
	{ 0,  0,  0, 15, 15,  7,  0,  7,  7,  7,  7,  7,  0,  8,  0,  0},
	{ 0,  0,  0, 15,  0,  7,  8,  0,  7,  7,  7,  0,  8,  8,  0,  0},
	{ 0,  0, 15,  0,  0,  7,  8,  0,  0,  0,  0,  0,  8,  0,  0,  0},
	{ 0,  0, 15,  0,  0,  7,  8,  8,  8,  8,  8,  8,  8,  0,  8,  0},
	{ 0,  0, 15,  8,  0,  0,  7,  8,  8,  8,  8,  8,  0,  0,  8,  0},
	{ 0,  0, 15,  8,  8,  8,  7,  0,  8,  8,  8,  0,  0,  8,  8,  0},
	{ 0,  0,  8, 15,  8,  8,  7,  0,  0,  0,  0,  0,  0,  8,  8,  0},
	{ 0,  0,  0, 15, 15,  0,  7,  0,  8,  8,  8,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0, 15, 15,  7,  8,  8,  8,  8,  8,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0, 15, 15,  8,  8,  0,  8,  8,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  8,  8,  0,  8,  8,  0,  0,  0,  0}
};

const int Spritesheet::ROGUE[16][16]
{
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0, 15, 15, 15,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0, 15, 15, 15, 15, 15,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0, 15,  7,  7,  7, 15,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0, 15,  7,  7,  7, 15,  0,  0,  0,  0},
	{ 7,  0,  0,  0,  0,  0,  0, 15,  7,  7,  7, 15,  0,  0,  0,  0},
	{15,  7,  0,  0,  0,  0, 15, 15, 15,  7, 15, 15, 15,  0,  0,  0},
	{ 0, 15,  7,  0,  0,  0,  8,  8, 15, 15, 15,  8,  8,  0,  0,  0},
	{ 0,  0, 15,  7,  0,  0,  8,  8,  8,  8,  8,  8,  8,  0,  8,  0},
	{ 0,  0,  0, 15,  7,  0,  0,  8,  8,  8,  8,  8,  0,  0,  8,  0},
	{ 0,  0,  0, 15,  8,  8,  8,  0,  8,  8,  8,  0,  8,  7,  8,  0},
	{ 0,  0,  0,  0,  8, 15,  0,  0,  0,  0,  0,  0, 15,  8,  8,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0,  8,  8,  8,  0,  0, 15,  7,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  8,  8,  8,  8,  8,  0,  0, 15,  7},
	{ 0,  0,  0,  0,  0,  0,  0,  8,  8,  0,  8,  8,  0,  0,  0, 15},
	{ 0,  0,  0,  0,  0,  0,  0,  8,  8,  0,  8,  8,  0,  0,  0,  0}
};

const int Spritesheet::KNIGHT[16][16]
{
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0,  8,  8,  8,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  8,  7,  7,  7,  8,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  8,  7,  7,  7,  8,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  7,  7,  0,  8,  7,  7,  7,  8,  0,  8,  0,  0},
	{ 0,  0,  0, 15, 15,  7,  8,  0,  7,  7,  7,  0,  8,  8,  0,  0},
	{ 0,  0, 15, 15, 15, 15,  8,  8,  0,  0,  0,  8,  8,  0,  0,  0},
	{15, 15,  8, 15, 15, 15,  8,  8,  8,  8,  8,  8,  8,  0,  8,  0},
	{15, 15, 15,  8, 15, 15,  0,  8,  8,  8,  8,  8,  0,  8,  8, 15},
	{ 0,  0,  0, 15,  8,  8,  8,  0,  0,  0,  0,  0,  0,  8,  8, 15},
	{ 0,  0,  0,  0, 15, 15,  7,  7,  8,  8,  8,  8,  7,  7, 15, 15},
	{ 0,  0,  0,  0, 15, 15, 15,  7,  7,  7,  8,  8,  8, 15,  8, 15},
	{ 0,  0,  0, 15, 15, 15,  8, 15, 15, 15, 15,  8,  8, 15, 15,  8},
	{ 0,  0, 15, 15, 15,  8, 15, 15,  8,  0,  0,  8,  8, 15, 15,  0},
	{ 0,  0, 15, 15, 15,  8,  0,  0,  0,  0,  0,  8,  8, 15, 15,  0},
	{ 0,  0, 15, 15, 15,  0,  0,  0,  0,  0,  0,  0, 15, 15, 15,  0}
};
