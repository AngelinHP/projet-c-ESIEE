#pragma warning( disable : 4996 ) 

 
#pragma warning( disable : 4996 )

#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include "G2D.h"
#include "AnimationHandler.h"
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
	float speed = 8.0f;
	AnimationHandler anim;

	// Gestion des états
	Direction lastDir = Direction::Down;
	double lastMoveTime = 0.0;
	double idleDelay = 0.3;  // 300ms avant de revenir en idle

	Player()
	{
		pos = V2(200, 200);
	}

	void InitTexture()
	{
		anim.LoadTextures(
			"C:\\Users\\Admin\\Documents\\GitHub\\projet-c-ESIEE\\sprites\\Idle.png",
			"C:\\Users\\Admin\\Documents\\GitHub\\projet-c-ESIEE\\sprites\\Walk.png"
		);
	}

	void Movement()
	{
		bool isMoving = false;
		double currentTime = G2D::elapsedTimeFromStartSeconds();

		if (G2D::isKeyPressed(Key::Z)) { pos = pos + V2(0, speed);  isMoving = true; lastDir = Direction::Up; }
		if (G2D::isKeyPressed(Key::S)) { pos = pos + V2(0, -speed); isMoving = true; lastDir = Direction::Down; }
		if (G2D::isKeyPressed(Key::Q)) { pos = pos + V2(-speed, 0); isMoving = true; lastDir = Direction::Left; }
		if (G2D::isKeyPressed(Key::D)) { pos = pos + V2(speed, 0);  isMoving = true; lastDir = Direction::Right; }

		// Mise à jour de l'animateur
		if (isMoving)
		{
			lastMoveTime = currentTime;
			anim.isMoving = true;
			anim.SetDirection(lastDir);
		}
		else if (currentTime - lastMoveTime > idleDelay)
		{
			// Le délai est écoulé, le joueur repasse au repos
			anim.isMoving = false;
		}

		// On fait avancer le temps de l'animation
		anim.Update();
	}

	void update()
	{
		Movement();
	}

	void draw(V2 drawPos)
	{
		anim.Draw(drawPos);
	}
};
 
   

///////////////////////////////////////////////////////////////////////////////
//
//    Donn�es du jeu - structure instanci�e dans le main

struct GameData
{
	int HeighPix = 800;   // hauteur de la fen�tre d'application
	int WidthPix = 600;   // largeur de la fen�tre d'application

	Player& player = Player();

	Camera2D& camera = Camera2D(player.pos,WidthPix, HeighPix);

	V2 rectPos = V2(0, 400);

	GameData() {}
};

 
///////////////////////////////////////////////////////////////////////////////
//
// 
//     fonction de rendu - re�oit en param�tre les donn�es du jeu par r�f�rence



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
//      Gestion de la logique du jeu - re�oit en param�tre les donn�es du jeu par r�f�rence



void Logic(GameData & G) // appel� 20 fois par seconde
{
	G.camera.update(G.player.pos);

	G.player.update();
}
 

///////////////////////////////////////////////////////////////////////////////
//
//
//        D�marrage de l'application



int main(int argc, char* argv[])
{
	GameData G;   // instanciation de l'unique objet GameData qui sera pass� aux fonctions render et logic
	

	// cr�e la fen�tre de l'application
	G2D::initWindow(V2(G.WidthPix, G.HeighPix), V2(20, 20), string("G2D DEMO"));

	// nombre de fois o� la fonction Logic est appel�e par seconde
	int callToLogicPerSec = 50;  

	// lance l'application en sp�cifiant les deux fonctions utilis�es et l'instance de GameData
	G.player.InitTexture();
	

	G2D::Run(Logic, Render, G, callToLogicPerSec, true);

	// aucun code ici
}





