#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <vector>
#include "GlutImport.h"
#include "GX.h"
#include "GX_Internal.hpp"

 


/////////////////////////////////////////////////////////////
//
//	    Mouse
//
/////////////////////////////////////////////////////////////


bool mousePressed[16];
bool mouseClicked[16];
int  MouseX = 0;
int  MouseY = 0;


void GX::getMousePos(int& x, int& y)
{
	x = MouseX;
	y = MouseY;
}


bool GX::detectLeftClick()
{
	bool r = mouseClicked[GLUT_LEFT_BUTTON];
	mouseClicked[GLUT_LEFT_BUTTON] = false; // on efface l'ev click
	return r;
}


bool GX::detectRightClick()
{
	bool r = mouseClicked[GLUT_RIGHT_BUTTON];
	mouseClicked[GLUT_RIGHT_BUTTON] = false; // on efface l'ev click
	return r;
}


bool GX::isAnyMouseButtonPressed()
{
	return mousePressed[0] | mousePressed[1] | mousePressed[2];
}

bool GX::isMouseLeftButtonPressed()
{
	return mousePressed[GLUT_LEFT_BUTTON];
}

bool GX::isMouseRightButtonPressed()
{
	return mousePressed[GLUT_RIGHT_BUTTON];
}

void GXinternal_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) { mousePressed[button] = true;   mouseClicked[button] = true; }
	if (state == GLUT_UP) { mousePressed[button] = false;  mouseClicked[button] = false; }
}




void GXinternal_mouseMove(int x, int y)
{
	MouseX = x;
	MouseY = GXinternal_getWindowHeight() - y;
}

 



