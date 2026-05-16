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
	V2 futurePos;

	int attackDamage = 20;

	AnimationHandler anim;
	MapManager mapMan;

	// Gestion des états
	int speed;
	int currentHealth = 100;

	enum class Movement { Up, Down, Left, Right, None };
	Direction lastDir = Direction::Down;
	Movement lastMove = Movement::None;
	map<Movement, V2> dirVectors;


	bool isMoving = false;
	bool canMove = false;

	bool playerIsInSight = false;
	bool playerIsInRange = false;

	bool isAttacking = false;
	bool dealDamage = false;

	bool isAlive = true;

	bool turnDone = false;

	double currentTime = G2D::elapsedTimeFromStartSeconds();

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
		anim.LoadTexture("Idle", "sprites\\ennemies\\Idle.png", 32, V2(128, 32), 1);
		anim.LoadTexture("Walk", "sprites\\ennemies\\Walk.png", 32, V2(128, 128), 4);
		anim.LoadTexture("Attack", "sprites\\ennemies\\Attack.png", 32, V2(128, 32), 1);
	}

	void takeDamage(int amount) {
		currentHealth -= amount;
		if (currentHealth < 0) currentHealth = 0; // On empêche la vie de passer en négatif
	}

	void die() {
		// Ici on pourrait ajouter une animation de mort ou autre
		isAlive = false;
		turnDone = true; // Le tour du joueur est passé
	}	

	bool LineOfSight(V2 playerPos) {
		if (!(playerPos.x <= pos.x + 2 * mapMan.tilesetSize 
			&& playerPos.x >= pos.x - 2 * mapMan.tilesetSize && playerPos.y <= pos.y + mapMan.tilesetSize && playerPos.y >= pos.y - mapMan.tilesetSize))
		if (!(playerPos.x <= pos.x + mapMan.tilesetSize 
			&& playerPos.x >= pos.x - mapMan.tilesetSize 
			&& playerPos.y <= pos.y + 2 * mapMan.tilesetSize && playerPos.y >= pos.y - 2 * mapMan.tilesetSize))
		if (!(playerPos.x <= pos.x + 3 * mapMan.tilesetSize 
			&& playerPos.x >= pos.x - 3 * mapMan.tilesetSize && playerPos.y == pos.y))
		if (!(playerPos.y <= pos.y + 3 * mapMan.tilesetSize 
			&& playerPos.y >= pos.y - 3 * mapMan.tilesetSize && playerPos.x == pos.x))
						return false;

		return true;
	}


	bool rangeOfAttack(V2 playerPos) {
		return playerPos == pos + V2(mapMan.tilesetSize, 0) 
			|| playerPos == pos - V2(mapMan.tilesetSize, 0) 
			|| playerPos == pos + V2(0, mapMan.tilesetSize) 
			|| playerPos == pos - V2(0, mapMan.tilesetSize);
	}

	void attack() {
		// Attaque en fonction de la direction
		if (isAttacking) {
			anim.isAttacking = true;
			// 15 de vitesse * 1 frame = 15 donc on arrete a 14
			if (anim.timer >= 14) {
				isAttacking = false;
				anim.isAttacking = false;
				dealDamage = true;
				turnDone = true;
			}
		}
	}

	void ChangeToPlayerDirection(V2& playerPos) {
		V2 toPlayer = playerPos - pos;
		if (toPlayer.x > 0) lastDir = Direction::Right;
		else if (toPlayer.x < 0) lastDir = Direction::Left;
		else if (toPlayer.y > 0) lastDir = Direction::Up;
		else if (toPlayer.y < 0) lastDir = Direction::Down;
	}

	void nextAction(V2& playerPos,V2& playerFuturePos) {

		// Si déjà en train d'agir, on ne re-décide pas
		if (isAttacking || isMoving || turnDone) return;

		playerIsInSight = LineOfSight(playerFuturePos);
		playerIsInRange = rangeOfAttack(playerFuturePos);

		if (pos == futurePos && !playerIsInRange && playerIsInSight){
			registerMove(playerFuturePos);
		}
			
		else if (playerIsInRange) {
			if (!isAttacking) {
				isAttacking = true;
				anim.timer = 0;
			}

			if (playerFuturePos == playerPos) {
				ChangeToPlayerDirection(playerFuturePos);
			}
		}
		else
			turnDone = true; // Si on ne voit pas le joueur il passe son tour

	}

	void registerMove(V2& playerFuturePos) {

		// Mise à jour de l'animateur
		V2 toPlayer = playerFuturePos - futurePos;

		V2 tempFPos;

		
		if (toPlayer.x != 0) {

			lastMove = toPlayer.x > 0 ? Movement::Right : Movement::Left;
			lastDir = toPlayer.x > 0 ? Direction::Right : Direction::Left;

			tempFPos = pos + dirVectors[lastMove];

			canMove = !mapMan.Mur(tempFPos.x / mapMan.tilesetSize, tempFPos.y / mapMan.tilesetSize);
			if (!canMove) {
				lastMove = toPlayer.y > 0 ? Movement::Up : Movement::Down;
				lastDir = toPlayer.y > 0 ? Direction::Up : Direction::Down;
				tempFPos = pos + dirVectors[lastMove];
				canMove = true;
			}

		}
		else {
			lastMove = toPlayer.y > 0 ? Movement::Up : Movement::Down;
			lastDir = toPlayer.y > 0 ? Direction::Up : Direction::Down;
			tempFPos = pos + dirVectors[lastMove];
			canMove = true;
		}

		futurePos = tempFPos;
		isMoving = lastMove != Movement::None && !(pos == futurePos);
		

	}

	void move() {
		if (isMoving && canMove && !(futurePos == pos)) {
			pos = pos + dirVectors[lastMove].GetNormalized() * speed;
		}
		if (isMoving && futurePos == pos) {
			isMoving = false;
			lastMove = Movement::None;
			turnDone = true;
		}
		anim.SetDirection(lastDir);
		anim.isMoving = isMoving && canMove;
		// On fait avancer le temps de l'animation
		anim.Update();
	}

	void setcanMove(bool _canMove) {
		canMove = _canMove;
		if (!canMove)
			futurePos = pos;
	}

	bool update(V2& playerPos, V2& playerFuturePos)
	{
		currentTime = G2D::elapsedTimeFromStartSeconds();
		isAlive = currentHealth > 0;


		if (isAlive) {
			nextAction(playerPos, playerFuturePos);
			move();
			if(playerFuturePos == playerPos)
				attack();
		}
		else
			die();

		return turnDone;

	}

	void draw(Camera2D& camera)
	{
		if(isAlive)
			anim.Draw(camera, pos);
	}
};


struct Player
{
	V2 pos;
	AnimationHandler anim;
	MapManager mapMan;
	enum class Movement { Up, Down, Left, Right, None };

	int attackDamage = 35;

	// Gestion des états
	int speed;
	Direction lastDir = Direction::Down;
	Movement lastMove = Movement::None;
	map<Movement, V2> dirVectors;
	
	
	// la vie
	int maxHealth = 100;
	int currentHealth = 100;


	bool isMoving = false;
	bool canMove = false;
	bool isInInventory = false;
	bool isAttacking = false;
	bool dealDamage = false;

	double currentTime = G2D::elapsedTimeFromStartSeconds();
	V2 futurePos;

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
		anim.LoadTexture("Idle", "sprites\\player\\Idle.png", 32, V2(128, 128), 4);
		anim.LoadTexture("Walk", "sprites\\player\\Walk.png", 32, V2(128, 128), 4);
		anim.LoadTexture("Attack", "sprites\\player\\Attack.png", 32, V2(128, 128), 4);
		anim.LoadTexture("Hit", "sprites\\player\\Hit.png", 32, V2(128, 64), 2);
		anim.LoadTexture("Dead", "sprites\\player\\Dead.png", 32, V2(128, 64), 2);
	}

	bool hasActed() {
		return !(pos == futurePos) || dealDamage;
	}

	void registerMovement(Enemy& enemy)
	{
		if (pos == futurePos) {
			if (!isInInventory && !isAttacking) {

				V2        newPos = pos;
				Movement  newMove = Movement::None;
				Direction newDir = lastDir;

				if (G2D::isKeyPressed(Key::Z)) { newDir = Direction::Up; newMove = Movement::Up; newPos = pos + dirVectors[Movement::Up];}
				else if (G2D::isKeyPressed(Key::S)) { newDir = Direction::Down; newMove = Movement::Down; newPos = pos + dirVectors[Movement::Down];}
				else if (G2D::isKeyPressed(Key::Q)) { newDir = Direction::Left; newMove = Movement::Left; newPos = pos + dirVectors[Movement::Left];}
				else if (G2D::isKeyPressed(Key::D)) { newDir = Direction::Right; newMove = Movement::Right; newPos = pos + dirVectors[Movement::Right];}
				else if (G2D::keyHasBeenHit(Key::F)) { isAttacking = true; anim.timer = 0; }


				if (newMove != Movement::None) {
					lastDir = newDir; // la direction visuelle est mise à jour même si bloqué
					bool wallBlocked = mapMan.Mur(newPos.x / mapMan.tilesetSize, newPos.y / mapMan.tilesetSize) || (newPos == enemy.futurePos && enemy.isAlive);
					if (!wallBlocked) {
						futurePos = newPos; 
						lastMove = newMove;
					}
					// Si mur : futurePos reste == pos → hasActed() = false → tour conservé
				}
			}
			if (G2D::keyHasBeenHit(Key::I)) { isInInventory = !isInInventory; }
		}
		
	}

	void move(Enemy& enemy) {
		// Mise à jour de l'animateur
		isMoving = lastMove != Movement::None && !(pos == futurePos);
		canMove = !mapMan.Mur(futurePos.x / mapMan.tilesetSize, futurePos.y / mapMan.tilesetSize) && !(futurePos == enemy.futurePos && enemy.isAlive);

		if(!canMove) futurePos = pos;

		anim.SetDirection(lastDir);

		if (isMoving && canMove)
			pos = pos + dirVectors[lastMove].GetNormalized() * speed;

		anim.isMoving = isMoving && canMove;

	}

	void attack() {
		// Attaque en fonction de la direction
		if (isAttacking) {
			anim.isAttacking = true;
			// Meme logique que pour l'ennemie
			if (anim.timer >= 59) {
				isAttacking = false;
				anim.isAttacking = false;
				dealDamage = true;
			}
		}
	}
	bool rangeOfAttack(V2& ennemyPos) {
		return ennemyPos == pos + V2(mapMan.tilesetSize, 0) 
			|| ennemyPos == pos - V2(mapMan.tilesetSize, 0) 
			|| ennemyPos == pos + V2(0, mapMan.tilesetSize) 
			|| ennemyPos == pos - V2(0, mapMan.tilesetSize);
	}

	void die() {
		
	}

	void getHit() {
		anim.Hit = true;
		anim.timer = 0;
	}
	void takeDamage(int amount) {
		currentHealth -= amount;
		if (currentHealth < 0) currentHealth = 0; // On empêche la vie de passer en négatif
	}

	void heal(int amount) {
		currentHealth += amount;
		if (currentHealth > maxHealth) currentHealth = maxHealth; // On empêche de dépasser le max
	}

	void animation(Enemy& enemy)
	{

		move(enemy);
		attack();
		

		// On fait avancer le temps de l'animation
		anim.Update();

	}

	bool handleInput(Enemy& enemy) {
		registerMovement(enemy);
		return hasActed();
	}

	void draw(Camera2D& camera)
	{
		anim.Draw(camera, pos);
	}

	void setDealDamage(bool _dealDamage) {
		dealDamage = _dealDamage;
	}
};



///////////////////////////////////////////////////////////////////////////////
//
//    Donn�es du jeu - structure instanci�e dans le main

struct GameData
{
	int HeighPix = 800;   // hauteur de la fen�tre d'application
	int WidthPix = 1600;   // largeur de la fen�tre d'application

	bool playerHasActed = false;

	enum class TurnState { Player, Enemy };
	TurnState currentTurn = TurnState::Player;

	MapManager& map = MapManager();

	V2 spawn = map.recupSpawn();
	V2 eSpawn = map.recupESpawn();

	Player& player = Player(spawn, map.tilesetSize, map);
	Enemy& enemy = Enemy(eSpawn, map.tilesetSize, map);

	Inventory& inventory = Inventory();

	Camera2D& camera = Camera2D(player.pos, WidthPix, HeighPix);


	int HealthBarUiX = 20;
	int HealthBarUiY = 700;
	int HealthbarMaxWidth = 300;
	int HealthbarHeight = 20;

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

	// Fond de la barre
	G2D::drawRectangle(V2(G.HealthBarUiX, G.HealthBarUiY), V2(G.HealthbarMaxWidth, G.HealthbarHeight), Color::Red, true);

	// Calcul de la largeur de la barre verte en fonction du pourcentage de vie
	int currentBarWidth = (G.player.currentHealth * G.HealthbarMaxWidth) / G.player.maxHealth;

	// Dessin de la jauge verte (seulement s'il reste de la vie)
	if (currentBarWidth > 0) {
		G2D::drawRectangle(V2(G.HealthBarUiX, G.HealthBarUiY), V2(currentBarWidth, G.HealthbarHeight), Color::Green, true);
	}

	// Contour blanc
	G2D::drawLine(V2(G.HealthBarUiX, G.HealthBarUiY), V2(G.HealthBarUiX + G.HealthbarMaxWidth, G.HealthBarUiY), Color::White);
	G2D::drawLine(V2(G.HealthBarUiX, G.HealthBarUiY), V2(G.HealthBarUiX, G.HealthBarUiY + G.HealthbarHeight), Color::White);
	G2D::drawLine(V2(G.HealthBarUiX + G.HealthbarMaxWidth, G.HealthBarUiY), V2(G.HealthBarUiX + G.HealthbarMaxWidth, G.HealthBarUiY + G.HealthbarHeight), Color::White);
	G2D::drawLine(V2(G.HealthBarUiX, G.HealthBarUiY + G.HealthbarHeight), V2(G.HealthBarUiX + G.HealthbarMaxWidth, G.HealthBarUiY + G.HealthbarHeight), Color::White);

	G2D::Show();
}




	
///////////////////////////////////////////////////////////////////////////////
//
//
//      Gestion de la logique du jeu - re�oit en param�tre les donn�es du jeu par r�f�rence



void Logic(GameData & G) // appel� 20 fois par seconde
{
	G.camera.update(G.player.pos);

	G.player.animation(G.enemy);

	if(G.currentTurn == GameData::TurnState::Player) {

		// Gestion du faire de ramasser des objets et les mettre dans l'inventaire
		if (G.player.pos == G.player.futurePos) {

			// On convertit la position du joueur en pixels vers une position sur la grille (x, y)
			int tileX = G.player.pos.x / G.map.tilesetSize;
			int tileY = G.player.pos.y / G.map.tilesetSize;

			// Calcul pour trouver l'index dans le string
			int charIndex = (G.map.mapHeight - tileY - 1) * G.map.mapWidth + tileX;
			char currentChar = G.map.map1[charIndex];

			// On regarde s'il y a un objet sous ses pieds
			if (currentChar == 'V') {
				G.inventory.addItem("Potion de vie"); // Ajoute à l'inventaire
				G.map.map1[charIndex] = ' '; // Efface l'objet de la carte
			}
			else if (currentChar == 'K') {
				G.inventory.addItem("Katana");
				G.map.map1[charIndex] = ' ';
			}

			G.playerHasActed = G.player.handleInput(G.enemy);

			if(G.player.dealDamage && G.player.rangeOfAttack(G.enemy.pos))
				G.enemy.takeDamage(G.player.attackDamage);
		
			G.player.setDealDamage(false);
		}


		// On vérifie que c'est bien à nous de jouer, qu'on ne bouge pas et qu'on n'attaque pas si c'est le cas on peut prendre une potion
	
			if (G2D::keyHasBeenHit(Key::A)) {
				cout << "pressed A" << endl;
				// On vérifie qu'on a au moins 1 potion ET qu'on n'est pas déjà full life
				if (G.inventory.items["Potion de vie"] > 0 && G.player.currentHealth < G.player.maxHealth) {
					G.inventory.removeItem("Potion de vie"); // On consomme l'objet
					G.player.heal(30);                       // On rend 30 HP
				}
			}
		
			if (G.playerHasActed) {
				G.enemy.turnDone = false; // On reset le tour de l'ennemi pour qu'il puisse agir à son tour
				G.currentTurn = GameData::TurnState::Enemy;
			}
	}

	else if (G.currentTurn == GameData::TurnState::Enemy) {

		bool enemyDone = G.enemy.update(G.player.pos, G.player.futurePos);

		if (G.enemy.dealDamage) {
			G.player.takeDamage(G.enemy.attackDamage);
			G.enemy.dealDamage = false;
		}

		if (enemyDone)
			G.currentTurn = GameData::TurnState::Player;
	}

	
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
	

	G2D::Run(Logic, Render, G, callToLogicPerSec, true);

	// aucun code ici
}