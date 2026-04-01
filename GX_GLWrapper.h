#pragma once
#include "GX.h"


struct GLWrapper
{

	static void initGLWindow(V2 WindowSize, V2 WindowStartPos, std::string name);
	static void initGX(void Render(), void Anim(int value) );
	static void GL_2DMode();
	static void GL_3DMode(Color Background, GX::Camera Cam, int W, int H);
	static int  getWindowHeight();
	static int  getWindowWidth();
	static void requestRedraw_Callback();
	static void SwapBuffer();

	static void DrawTriangles(const vector<Triangle> & myTriangles, bool flat = true, bool mesh = true, bool Normals = true);
	static void DrawLine(const V3 & P1, const V3 & P2, const Color & c,  float thickness);
	static void BeginUI();
};







