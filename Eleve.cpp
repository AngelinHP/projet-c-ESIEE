#pragma warning( disable : 4996 ) 

 
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include "G2D.h"
using namespace std;

// touche P   : mets en pause
// touche ESC : ferme la fenêtre et quitte le jeu


#pragma warning( disable : 4996 ) 


#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include "G2D.h"


using namespace std;

struct Camera2D
{
	V2 pos;
	int winWidth;
	int winHeight;

	int offsetX;
	int offsetY;

	Camera2D(V2 cPos, int width, int height)
	{
		pos = cPos;
		winWidth = width;
		winHeight = height;

		offsetX = winWidth / 2 - pos.x;
		offsetY = winHeight / 2 - pos.y;
	}

	V2 renderWcamera(V2 ePos)
	{
		return V2(ePos.x + offsetX, ePos.y + offsetY);
	}

	void update(V2 nPos)
	{
		pos = nPos;
		offsetX = winWidth / 2 - pos.x;
		offsetY = winHeight / 2 - pos.y;
	}
};


struct Player
{
	V2 pos;
	int texture;

	Player()
	{
		pos = V2(200, 200);
	}

	void changePos(V2& newPos)
	{
		pos = newPos;
	}

	void InitTexture()
	{
		texture = G2D::ExtractTextureFromPNG("player.png", Transparency::None);
	}


	void draw(V2 drawPos)
	{
		//G2D::drawRectangle(drawPos, V2(32, 32), Color::Red, true);
		//G2D::drawRectWithTexture(texture, drawPos, V2(64, 112));
		G2D::drawSpriteFrame(texture, V2(300, 300), V2(32, 32), V2(0, 96), V2(16, 16), V2(64, 112));

	}

	void Movement()
	{
		if (G2D::isKeyPressed(Key::Z))
			pos = pos + V2(0, 10);
		if (G2D::isKeyPressed(Key::Q))
			pos = pos + V2(-10, 0);
		if (G2D::isKeyPressed(Key::D))
			pos = pos + V2(10, 0);
		if (G2D::isKeyPressed(Key::S))
			pos = pos + V2(0, -10);
	}


	void update()
	{
		Movement();
	}


};
 
   

///////////////////////////////////////////////////////////////////////////////
//
//    Données du jeu - structure instanciée dans le main

struct GameData
{
	int HeighPix = 800;   // hauteur de la fenêtre d'application
	int WidthPix = 600;   // largeur de la fenêtre d'application

	Player& player = Player();

	Camera2D& camera = Camera2D(player.pos,WidthPix, HeighPix);

	V2 rectPos = V2(0, 400);

	GameData() {}
};

 
///////////////////////////////////////////////////////////////////////////////
//
// 
//     fonction de rendu - reçoit en paramètre les données du jeu par référence



void Render(const GameData& G)
{
	G2D::clearScreen(Color::Black);

	G2D::drawRectangle(G.camera.renderWcamera(G.rectPos), V2(300, 100), Color::Blue, true);

	G.player.draw(G.camera.renderWcamera(G.player.pos));

	G2D::Show();
}




	
///////////////////////////////////////////////////////////////////////////////
//
//
//      Gestion de la logique du jeu - reçoit en paramètre les données du jeu par référence



void Logic(GameData & G) // appelé 20 fois par seconde
{
	G.camera.update(G.player.pos);

	G.player.update();
}
 

///////////////////////////////////////////////////////////////////////////////
//
//
//        Démarrage de l'application



int main(int argc, char* argv[])
{
	GameData G;   // instanciation de l'unique objet GameData qui sera passé aux fonctions render et logic
	

	// crée la fenêtre de l'application
	G2D::initWindow(V2(G.WidthPix, G.HeighPix), V2(20, 20), string("G2D DEMO"));

	// nombre de fois où la fonction Logic est appelée par seconde
	int callToLogicPerSec = 50;  

	// lance l'application en spécifiant les deux fonctions utilisées et l'instance de GameData
	G.player.InitTexture();
	

	G2D::Run(Logic, Render, G, callToLogicPerSec,true);

	// aucun code ici
}





