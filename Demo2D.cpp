#pragma warning( disable : 4996 ) 

#include <cmath>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include "GX.h"
using namespace std;

// touche P   : mets en pause
// touche ESC : ferme la fenêtre et quitte le jeu

struct Camera2D
{
	V2 pos;
	int winWidth;
	int winHeight;

	Camera2D(V2 cPos, int width, int height)
	{
		pos = cPos;
		winWidth = width;
		winHeight = height;
	}

	V2 renderWcamera(V2 ePos)
	{
		return V2(ePos.x + winWidth / 2 - pos.x, ePos.y + winHeight / 2 - pos.y);
	}
};


struct Player
{
	V2 pos;
	int texture;

	Player()
	{
		pos = V2(200, 200);
		texture = GX::ExtractTextureFromPNG(".//(centré).png", Transparency::None);
	}


	void draw()
	{
		GX::D2_Rectangle(pos, V2(32, 32), Color::Red, true);
		GX::D2_RectWithTexture(texture, pos, V2(128, 128));

	}

	void Movement()
	{
		if (GX::isKeyPressed(Key::Z))
			pos = pos + V2(0, 10);
		if (GX::isKeyPressed(Key::Q))
			pos = pos + V2(-10, 0);
		if (GX::isKeyPressed(Key::D))
			pos = pos + V2(10, 0);
		if (GX::isKeyPressed(Key::S))
			pos = pos + V2(0, -10);
	}


	void update()
	{
		Movement();
	}


};

struct GameData
{
	int HeighPix = 800;   // hauteur de la fenêtre d'application
	int WidthPix = 600;   // largeur de la fenêtre d'application

	int idFrame = 0;

	Player& player = Player();

	Camera2D& camera = Camera2D(player.pos, WidthPix, HeighPix);

	float timer = 0;

	V2 rectPos = V2(0, 400);

	GameData() {}

};


GameData G;   // instanciation de l'unique objet GameData qui sera passé aux fonctions render et logic


void renderWcamera(V2& ePos)
{
	ePos = G.camera.renderWcamera(ePos);
}



 
///////////////////////////////////////////////////////////////////////////////
//
// 
//     fonction de rendu - reçoit en paramètre les données du jeu par référence



void Demo2DRender(bool isInPause)
{
	GX::select2DMode();

	// fond noir	 
	GX::clearScreen(Color::Black);

	// affiche du texte
	if (isInPause)
	   GX::drawStringFontMono(V3(50, 400,0), "Pause", 60, 3, Color::Green);

	GX::D2_Rectangle(G.rectPos, V2(300, 100), Color::Blue, true);
	
	G.player.draw();
}


	
///////////////////////////////////////////////////////////////////////////////
//
//
//      Gestion de la logique du jeu - reçoit en paramètre les données du jeu par référence


// appelé seulement si en pause
void Demo2DLogic(float deltaT)
{
	G.timer  += deltaT;

	G.camera.renderWcamera(G.player.pos);
	G.camera.renderWcamera(G.rectPos);

	G.player.update();
}


void Demo2DInit()
{
	
}
 





