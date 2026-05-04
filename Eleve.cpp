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
#include "Inventory.h"

using namespace std;

struct Enemy
{
	V2 pos;
	AnimationHandler anim;
	MapManager mapMan;
	enum class Movement { Up, Down, Left, Right, None };

	// Gestion des états
	int speed;
	Direction lastDir = Direction::Down;
	Movement lastMove = Movement::None;
	map<Movement, V2> dirVectors;


	bool isMoving = false;
	bool canMove = false;
	bool playerIsInSight = false;

	double currentTime = G2D::elapsedTimeFromStartSeconds();
	V2 futurePos;


	V2 center = V2(pos.x + 32, pos.y + 32);

	Enemy(V2& _pos, int& tilesetSize, MapManager& _map)
	{
		pos = _pos;
		speed = 2;
		dirVectors = {
		{Movement::Up, V2(0, tilesetSize)},
		{Movement::Down, V2(0, -tilesetSize)},
		{Movement::Left, V2(-tilesetSize, 0)},
		{Movement::Right, V2(tilesetSize,0)},
		{Movement::None, V2(0,0)}
		};
		futurePos = _pos;
		mapMan = _map;
	}

	void InitTexture()
	{
		anim.LoadTextures(
			"sprites\\ennemies\\Idle.png",
			"sprites\\ennemies\\Walk.png"
		);

		anim.loadSizes(32, V2(128, 32));
	}

	bool LineOfSight(V2 playerPos, MapManager& map) {
		if (!(playerPos.x <= pos.x + 2 * mapMan.tilesetSize && playerPos.x >= pos.x - 2 * mapMan.tilesetSize && playerPos.y <= pos.y + mapMan.tilesetSize && playerPos.y >= pos.y - mapMan.tilesetSize)) {
			if (!(playerPos.x <= pos.x + mapMan.tilesetSize && playerPos.x >= pos.x - mapMan.tilesetSize && playerPos.y <= pos.y + 2 * mapMan.tilesetSize && playerPos.y >= pos.y - 2 * mapMan.tilesetSize)) {
				if (!(playerPos == pos + 3 * V2(mapMan.tilesetSize, 0) || playerPos == pos - 3 * V2(mapMan.tilesetSize, 0) || playerPos == pos + 3 * V2(0, mapMan.tilesetSize) || playerPos == pos - 3 * V2(0, mapMan.tilesetSize)))
					return false;
			}
		}
		return true;
	}

	void move(V2& playerFuturePos) {


		// Mise à jour de l'animateur
		isMoving = lastMove != Movement::None && !(pos == futurePos);
		canMove = !mapMan.Mur(futurePos.x / mapMan.tilesetSize, futurePos.y / mapMan.tilesetSize);
		playerIsInSight = LineOfSight(playerFuturePos, mapMan);

		if (playerIsInSight) {
			V2 toPlayer = playerFuturePos - pos;

			if(toPlayer.x*toPlayer.x >= toPlayer.y*toPlayer.y){
				lastMove = toPlayer.x > 0 ? Movement::Right : Movement::Left;
		}
		else{
				lastMove = toPlayer.y > 0 ? Movement::Up : Movement::Down;
			}
			futurePos = pos + dirVectors[lastMove];
		}

		if (!canMove)
			futurePos = pos;
		anim.SetDirection(lastDir);

		if (isMoving && canMove)
			pos = pos + dirVectors[lastMove].GetNormalized() * speed;

		anim.isMoving = isMoving && canMove;

		// On fait avancer le temps de l'animation
		anim.Update();

	}

	void setcanMove(bool _canMove) {
		canMove = _canMove;
		if (!canMove)
			futurePos = pos;
	}

	void update(V2& playerPos)
	{
		currentTime = G2D::elapsedTimeFromStartSeconds();

		
		move(playerPos);
		center = V2(pos.x + 32, pos.y + 32);


	}

	void draw(Camera2D& camera)
	{
		anim.Draw(camera, pos);
	}
};


struct Player
{
	V2 pos;
	AnimationHandler anim;
	MapManager mapMan;
	enum class Movement { Up, Down, Left, Right, None };

	// Gestion des états
	int speed;
	Direction lastDir = Direction::Down;
	Movement lastMove = Movement::None;
	map<Movement, V2> dirVectors;


	bool isMoving = false;
	bool canMove = false;
	bool isInInventory = false;

	double currentTime = G2D::elapsedTimeFromStartSeconds();
	V2 futurePos;


	V2 center = V2(pos.x + 32, pos.y + 32);

	Player(V2& _pos, int& tilesetSize, MapManager& _map)
	{
		pos = _pos;
		speed = 2;
		dirVectors = {
		{Movement::Up, V2(0, tilesetSize)},
		{Movement::Down, V2(0, -tilesetSize)},
		{Movement::Left, V2(-tilesetSize, 0)},
		{Movement::Right, V2(tilesetSize,0)},
		{Movement::None, V2(0,0)}
		};
		futurePos = _pos;
		mapMan = _map;
	}

	void InitTexture()
	{
		anim.LoadTextures(
			"sprites\\player\\Idle.png",
			"sprites\\player\\Walk.png"
		);
	}

	void registerMovement()
	{

		if (pos == futurePos) {
			if(!isInInventory)
				if (G2D::isKeyPressed(Key::Z)) { lastDir = Direction::Up; lastMove = Movement::Up; futurePos = pos + dirVectors[Movement::Up]; }
				else if (G2D::isKeyPressed(Key::S)) { lastDir = Direction::Down; lastMove = Movement::Down; futurePos = pos + dirVectors[Movement::Down]; }
				else if (G2D::isKeyPressed(Key::Q)) { lastDir = Direction::Left; lastMove = Movement::Left; futurePos = pos + dirVectors[Movement::Left]; }
				else if (G2D::isKeyPressed(Key::D)) { lastDir = Direction::Right; lastMove = Movement::Right; futurePos = pos + dirVectors[Movement::Right]; }
			if (G2D::keyHasBeenHit(Key::I)) { isInInventory = !isInInventory; }
		}

	}

	void move(Enemy& enemy) {
		// Mise à jour de l'animateur
		isMoving = lastMove != Movement::None && !(pos == futurePos);
		canMove = !mapMan.Mur(futurePos.x / mapMan.tilesetSize, futurePos.y / mapMan.tilesetSize) && !(futurePos == enemy.pos);
		if(!canMove)
			futurePos = pos;
		anim.SetDirection(lastDir);

		if (isMoving && canMove)
			pos = pos + dirVectors[lastMove].GetNormalized() * speed;

		anim.isMoving = isMoving && canMove;

		// On fait avancer le temps de l'animation
		anim.Update();

	}

	void setcanMove(bool _canMove) {
		canMove = _canMove;
		if (!canMove)
			futurePos = pos;
	}

	void update(Enemy& enemy)
	{
		currentTime = G2D::elapsedTimeFromStartSeconds();

		registerMovement();
		move(enemy);
		center = V2(pos.x + 32, pos.y + 32);

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
	int HeighPix = 1000;   // hauteur de la fen�tre d'application
	int WidthPix = 1600;   // largeur de la fen�tre d'application

	MapManager& map = MapManager();

	V2 spawn = map.recupSpawn();
	V2 eSpawn = map.recupESpawn();

	Player& player = Player(spawn, map.tilesetSize, map);
	Enemy& enemy = Enemy(eSpawn, map.tilesetSize, map);

	Inventory& inventory = Inventory();

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

	G.player.draw(G.camera);
	G.enemy.draw(G.camera);

	if (G.player.isInInventory)
		G.inventory.drawInventory(G.camera, 200, 200);

	G2D::Show();
}




	
///////////////////////////////////////////////////////////////////////////////
//
//
//      Gestion de la logique du jeu - re�oit en param�tre les donn�es du jeu par r�f�rence



void Logic(GameData & G) // appel� 20 fois par seconde
{
	G.camera.update(G.player.pos);

	G.player.update(G.enemy);
	G.enemy.update(G.player.futurePos);
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
	int callToLogicPerSec = 60;  

	// lance l'application en sp�cifiant les deux fonctions utilis�es et l'instance de GameData
	G.player.InitTexture();
	G.enemy.InitTexture();
	G.map.InitTilesTexture();
	G.inventory.addItem("Baton");
	G.inventory.addItem("Couteau");
	

	G2D::Run(Logic, Render, G, callToLogicPerSec, true);

	// aucun code ici
}





