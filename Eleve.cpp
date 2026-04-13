#pragma warning( disable : 4996 ) 

 
#pragma warning( disable : 4996 )

#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include "G2D.h"
#include "AnimationHandler.h"
#include "MapManager.h"
#include "Camera2D.h"

using namespace std;




struct Player
{
	V2 pos;
	AnimationHandler anim;
	enum class Movement {Up, Down, Left, Right, None};

	// Gestion des états
	double lastMoveTime = -200.0;
	double idleDelay = 0.0; // 300ms avant de revenir en idle
	float moveTime = 0.5f;
	float speed;
	Direction lastDir = Direction::Down;
	Movement lastMove = Movement::None;
	map<Movement, V2> speedVectors;


	bool isMoving = false;
	double currentTime = G2D::elapsedTimeFromStartSeconds();
	double deltaTime = currentTime - lastMoveTime;
	

	V2 center = V2(pos.x + 32, pos.y + 32);

	Player(V2& _pos)
	{
		pos = _pos;
		speed = 1/moveTime;
		speedVectors = {
		{Movement::Up, V2(0, speed)},
		{Movement::Down, V2(0, -speed)},
		{Movement::Left, V2(-speed, 0)},
		{Movement::Right, V2(speed,0)},
		{Movement::None, V2(0,0)}
		};
	}

	void InitTexture()
	{
		anim.LoadTextures(
			"sprites\\Idle.png",
			"sprites\\Walk.png"
		);
	}

	void registerMovement()
	{

		if (deltaTime > moveTime) {
			if (G2D::isKeyPressed(Key::Z)) { lastDir = Direction::Up; lastMove = Movement::Up; lastMoveTime = currentTime; }
			else if (G2D::isKeyPressed(Key::S)) { lastDir = Direction::Down; lastMove = Movement::Down; lastMoveTime = currentTime; }
			else if (G2D::isKeyPressed(Key::Q)) { lastDir = Direction::Left; lastMove = Movement::Left;lastMoveTime = currentTime; }
			else if (G2D::isKeyPressed(Key::D)) { lastDir = Direction::Right; lastMove = Movement::Right;lastMoveTime = currentTime; }
		}

	}

	void move() {
		// Mise à jour de l'animateur
		isMoving = lastMove != Movement::None && deltaTime < moveTime;

		if (isMoving) {
			anim.SetDirection(lastDir);
			pos = pos + speedVectors[lastMove];
		}



		anim.isMoving = isMoving;

		// On fait avancer le temps de l'animation
		anim.Update();

	}

	void update()
	{
		currentTime = G2D::elapsedTimeFromStartSeconds();
		deltaTime = currentTime - lastMoveTime;

		registerMovement();
		move();
		center = V2(pos.x + 32, pos.y + 32);

		cout << "Player pos: (" << pos.x << ", " << pos.y << ") " << endl;
	}

	void draw(Camera2D& camera)
	{
		anim.Draw(camera, pos);
	}
};
 
   

///////////////////////////////////////////////////////////////////////////////
//
//    Donn�es du jeu - structure instanci�e dans le main

struct GameData
{
	int HeighPix = 800;   // hauteur de la fen�tre d'application
	int WidthPix = 600;   // largeur de la fen�tre d'application

	V2 rectPos = V2(0, 400);

	MapManager& map = MapManager();

	V2 spawn = map.recupSpawn();

	Player& player = Player(spawn);

	Camera2D& camera = Camera2D(player.pos, WidthPix, HeighPix);

	GameData() {}
};

 
///////////////////////////////////////////////////////////////////////////////
//
// 
//     fonction de rendu - re�oit en param�tre les donn�es du jeu par r�f�rence



void Render(const GameData& G)
{
	G2D::clearScreen(Color::Black);

	G.map.drawMap(G.camera);

	//G2D::drawRectangle(G.camera.renderWcamera(G.rectPos), V2(300, 100), Color::Blue, true);

	G.player.draw(G.camera);

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
	int callToLogicPerSec = 64;  

	// lance l'application en sp�cifiant les deux fonctions utilis�es et l'instance de GameData
	G.player.InitTexture();
	

	G2D::Run(Logic, Render, G, callToLogicPerSec, true);

	// aucun code ici
}





