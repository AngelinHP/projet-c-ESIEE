#pragma once
#include <cmath>	
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "GX_DemoManager.hpp"

using std::map;
using std::string;
using std::vector;
using std::cout;
constexpr double PI = 3.14159265358979323846;

 
#include "GX Color.h"
#include "GX V2.h"
#include "GX V3.h"
#include "GX TexturedFace3D.h"
#include "GX Objects3D.h"

 
 
enum class Key {   Unknown, A, B, C, D, E, F, GX, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	               LEFT, RIGHT, UP, DOWN, ENTER, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12   };

// pour gérer la transparence dans une image de sprite
enum class Transparency { None, BottomLeft, BottomRight, UpperLeft, UpperRight };

namespace GX
{
	///////////////////    GXD  - common functions   ////////////////

	// timing
	double elapsedTimeFromStartSeconds();   // temps écoulé depuis le début du jeu
	bool   isInPause();

	// key management 
	bool isKeyPressed(Key k);   // indique si la touche est enfoncée (retourne true tant que le touche est appuyée)
	bool keyHasBeenHit(Key k);  // retourne true si la touche a été appuyée, retourne false si la touche reste enfoncée
	
	// Mouse event
	void getMousePos(int& x, int& y);
	bool isMouseLeftButtonPressed();   // indique si le bouton gauche de la souris est enfoncé
	bool isMouseRightButtonPressed();
	bool isAnyMouseButtonPressed();
	bool detectLeftClick();            // détecte si le bouton gauche de la souris a été cliqué
	bool detectRightClick();

	// Textures
	int  ExtractTextureFromPNG(const string& filepath, Transparency T);  // 1 pixel gives key-color transparency
	int  ExtractTextureFromPNG(const string& filepath, int R = 0, int GX = 0, int B = 0);
	int  initTextureFromString(V2& Size, const string& Sprite); // 1 lettre = 1 pixel/couleur


	// Main Graphic System Function
	void initWindow(V2 WindowSize, V2 WindowStartPos, string name);
	void initGX();
	void select2DMode(); // demarre en mode 2D orthographique
	 
 
	// Font   -  for 2D, set z=0
	void drawStringFontMono (V3 pos, string text, float fontSize = 20, float thickness = 3, Color c = Color::Black);
	void drawStringFontRoman(V3 pos, string text, float fontSize = 20, float thickness = 3, Color c = Color::Black);

	void clearScreen(Color c);

	///////////////////    G2D  ////////////////


	// Affichage d'un sprite - angleDeg produit une rotation du sprite autours de son centre
	void D2_RectWithTexture(int IDtexture, V2 pos, V2 size, float angleDeg = 0);
	
	// Draw Geometry
	void D2_Pixel(V2 P, Color c);
	void D2_Line(V2 P1, V2 P2, Color c );
	void D2_Polygon(std::vector<V2> & PointList, Color c, bool fill = false);
	void D2_Rectangle(V2 P1, V2 Size, Color c, bool fill = false);
	void D2_Circle(V2 C, float r, Color c, bool fill = false);


	///////////////////    G3D  ////////////////

	struct Camera { V3 pos; float yaw = 0.0f;  float pitch = 0.0f;  };

	void DrawString3D(V3 pos, string text, float fontSize, float thickness, Color c, bool FontMono);
	void select3DMode(Color Background, Camera Cam, int W, int H);
	void ActivateUImode(); // pour dessiner les éléments de l'interface
	void DrawAxes(float size, float thickness);
	void DrawLine(V3 P1, V3 P2, Color c, float thickness);

	void BeginLocalFrame();
	void EndLocalFrame();
	void Rotate(float angleDeg, V3 axis);
	void Translate(V3 v);
	void Scale(V3 zoom);

  
}
