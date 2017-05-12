#ifndef	_GAME_OBJECT_H_
#define	_GAME_OBJECT_H_

#include "AllHeaderFiles.h"

using namespace std;

class GameObject
{
public :    
	int x, y, x0, y0, row, column, speedX, speedY, colour;
	char** charArray;
	
	GameObject()
	{
		speedX = 1;
		speedY = 1;          
		x0 = -1;
		y0 = -1;
	}
	
	void DrawTrail()
	{
		for(int i=0; i<row; i++)
		{
			for(int j=0; j<column; j++)
			{
				gotoxy(x0 + j, y0 + i);
				
				if(y0 + i>BOUNDARY_TOP &&
					y0 + i<BOUNDARY_BOTTOM &&
					x0 + j>BOUNDARY_LEFT &&
					x0 + j<BOUNDARY_RIGHT)
				{
					cout<<" ";
				}
			}
		}
	}
	
	void Draw()
	{
		//! erase old draw
		if(x0 != -1 &&
			y0 != -1 &&
			y0 != BOUNDARY_TOP &&
			(x0 != x || y0 != y))
		{
			DrawTrail();  
		}  
		textcolor(colour);   
		
		for(int i=0; i<row; i++)
		{
			for(int j=0; j<column; j++)
			{
				if(y + i>BOUNDARY_TOP &&
					y + i<BOUNDARY_BOTTOM &&
					x + j>BOUNDARY_LEFT &&
					x + j<BOUNDARY_RIGHT)
				{
					gotoxy(x + j,y + i);
					cout<<charArray[i][j];
				}
			}
		}
		textcolor(WHITE);
		
		//! update the old position
		x0 = x;
		y0 = y;
	}
	
	void MoveLeft()
	{
		x -= speedX;                 
	}
	
	void MoveRight()
	{
		x += speedX;
	}
	
	void MoveUp()
	{
		y -= speedY;
	}
	
	void MoveDown()
	{
		y += speedY;
	}
	
	bool CheckLeftBoundary()
	{
	    if(x <= BOUNDARY_LEFT + speedX)
	    {
	        return true;
	    }
	    
	    return false;  
	}
	
	bool CheckRightBoundary()
	{
	    if(x > BOUNDARY_RIGHT - speedX - column)
	    {
	        return true;
	    }
	    
	    return false;  
	}
	
	bool CheckTopBoundary()
	{
	    if(y <= BOUNDARY_TOP + speedY)
	    {
	        return true;
	    }
	    
	    return false;  
	}
	
	bool CheckBottomBoundary()
	{
	    if(y > BOUNDARY_BOTTOM - speedY - row)
	    {
	        return true;
	    }
	    
	    return false;  
	}
	             
     virtual void Update() = 0;
};

#endif
