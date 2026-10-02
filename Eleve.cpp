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
	bool playerIsInRange = false;
	bool isAttacking = false;

	double currentTime = G2D::elapsedTimeFromStartSeconds();
	V2 futurePos;

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

	bool LineOfSight(V2 playerPos) {
		if (!(playerPos.x <= pos.x + 2 * mapMan.tilesetSize && playerPos.x >= pos.x - 2 * mapMan.tilesetSize && playerPos.y <= pos.y + mapMan.tilesetSize && playerPos.y >= pos.y - mapMan.tilesetSize)) {
			if (!(playerPos.x <= pos.x + mapMan.tilesetSize && playerPos.x >= pos.x - mapMan.tilesetSize && playerPos.y <= pos.y + 2 * mapMan.tilesetSize && playerPos.y >= pos.y - 2 * mapMan.tilesetSize)) {
				if (!(playerPos.x <= pos.x + 3 * mapMan.tilesetSize && playerPos.x >= pos.x - 3 * mapMan.tilesetSize && playerPos.y == pos.y))
					if (!(playerPos.y <= pos.y + 3 * mapMan.tilesetSize && playerPos.y >= pos.y - 3 * mapMan.tilesetSize && playerPos.x == pos.x))
						return false;
			}
		}
		return true;
	}


	bool rangeOfAttack(V2 playerPos) {
		return playerPos == pos + V2(mapMan.tilesetSize, 0) || playerPos == pos - V2(mapMan.tilesetSize, 0) || playerPos == pos + V2(0, mapMan.tilesetSize) || playerPos == pos - V2(0, mapMan.tilesetSize);
	}

	void attack(bool& turn) {
		// Attaque en fonction de la direction
		if (isAttacking) {
			anim.isAttacking = true;
			// 15 de vitesse * 1 frame = 15 donc on arrete a 14
			if (anim.timer >= 14) {
				isAttacking = false;
				anim.isAttacking = false;
				turn = true;
			}
		}
	}

	void move(V2& playerFuturePos, bool& turn) {

		// Mise à jour de l'animateur
		playerIsInSight = LineOfSight(playerFuturePos);
		playerIsInRange = rangeOfAttack(playerFuturePos);
		isMoving = lastMove != Movement::None && !(pos == futurePos);
		V2 toPlayer = playerFuturePos - futurePos;

		if (!playerIsInRange && playerIsInSight && !turn) {
			if (pos == futurePos) {
				if (toPlayer.x * toPlayer.x >= toPlayer.y * toPlayer.y && canMove) {

					lastMove = toPlayer.x > 0 ? Movement::Right : Movement::Left;
					lastDir = toPlayer.x > 0 ? Direction::Right : Direction::Left;

				}
				else {

					lastMove = toPlayer.y > 0 ? Movement::Up : Movement::Down;
					lastDir = toPlayer.y > 0 ? Direction::Up : Direction::Down;

				}
				futurePos = pos + dirVectors[lastMove];
			}
		}
		else if (playerIsInRange && !turn)
		{
			futurePos = pos;

			// On oriente l'ennemi vers le joueur avant l'attaque
			if (toPlayer.x > 0) lastDir = Direction::Right;
			else if (toPlayer.x < 0) lastDir = Direction::Left;
			else if (toPlayer.y > 0) lastDir = Direction::Up;
			else if (toPlayer.y < 0) lastDir = Direction::Down;

			if (!isAttacking) {
				isAttacking = true;
				anim.timer = 0;
			}
			attack(turn);
		}
		else {
			turn = true; // Si on ne voit pas le joueur il passe son tour
		}

		canMove = !mapMan.Mur(futurePos.x / mapMan.tilesetSize, futurePos.y / mapMan.tilesetSize);
		
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

	void update(V2& playerPos, bool& playerTurn)
	{
		currentTime = G2D::elapsedTimeFromStartSeconds();

		move(playerPos, playerTurn);


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
	bool isAttacking = false;

	bool playerTurn = true;

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
		anim.LoadTexture("Idle", "sprites\\player\\Idle.png", 32, V2(128, 128), 4);
		anim.LoadTexture("Walk", "sprites\\player\\Walk.png", 32, V2(128, 128), 4);
		anim.LoadTexture("Attack", "sprites\\player\\Attack.png", 32, V2(128, 128), 4);
	}

	void registerMovement()
	{
		if (pos == futurePos) {
			if(!isInInventory && !isAttacking && playerTurn)
				if (G2D::isKeyPressed(Key::Z)) { lastDir = Direction::Up; lastMove = Movement::Up; futurePos = pos + dirVectors[Movement::Up];}
				else if (G2D::isKeyPressed(Key::S)) { lastDir = Direction::Down; lastMove = Movement::Down; futurePos = pos + dirVectors[Movement::Down]; }
				else if (G2D::isKeyPressed(Key::Q)) { lastDir = Direction::Left; lastMove = Movement::Left; futurePos = pos + dirVectors[Movement::Left];}
				else if (G2D::isKeyPressed(Key::D)) { lastDir = Direction::Right; lastMove = Movement::Right; futurePos = pos + dirVectors[Movement::Right];}
				else if (G2D::keyHasBeenHit(Key::F)) { isAttacking = true; anim.timer = 0; }
			if (G2D::keyHasBeenHit(Key::I)) { isInInventory = !isInInventory; }
		}
		
	}

	void move(Enemy& enemy) {
		// Mise à jour de l'animateur
		isMoving = lastMove != Movement::None && !(pos == futurePos);
		canMove = !mapMan.Mur(futurePos.x / mapMan.tilesetSize, futurePos.y / mapMan.tilesetSize) && !(futurePos == enemy.futurePos);

		if(!canMove){
			futurePos = pos;
		}
		anim.SetDirection(lastDir);

		if (isMoving && canMove) {
			pos = pos + dirVectors[lastMove].GetNormalized() * speed;
			playerTurn = false;
		}
		anim.isMoving = isMoving && canMove;

		// On fait avancer le temps de l'animation
		anim.Update();

	}

	void attack() {
		// Attaque en fonction de la direction
		if (isAttacking) {
			anim.isAttacking = true;
			// Meme logique que pour l'ennemie
			if (anim.timer >= 59) {
				isAttacking = false;
				anim.isAttacking = false;
				playerTurn = false;
			}
		}
	}

	// la vie
	int maxHealth = 100;
	int currentHealth = 100;

	void takeDamage(int amount) {
		currentHealth -= amount;
		if (currentHealth < 0) currentHealth = 0; // On empêche la vie de passer en négatif
	}

	void heal(int amount) {
		currentHealth += amount;
		if (currentHealth > maxHealth) currentHealth = maxHealth; // On empêche de dépasser le max
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
		attack();

	}

	void draw(Camera2D& camera)
	{
		anim.Draw(camera, pos);
	}

	void setPlayerTurn(bool _playerTurn) {
		playerTurn = _playerTurn;
	}

	bool getPlayerTurn() {
		return playerTurn;
	}
};



///////////////////////////////////////////////////////////////////////////////
//
//    Donn�es du jeu - structure instanci�e dans le main

struct GameData
{
	int HeighPix = 800;   // hauteur de la fen�tre d'application
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

	int uiX = 20;
	int uiY = 700;
	int barMaxWidth = 300;
	int barHeight = 20;

	// Fond de la barre
	G2D::drawRectangle(V2(uiX, uiY), V2(barMaxWidth, barHeight), Color::Red, true);

	// Calcul de la largeur de la barre verte en fonction du pourcentage de vie
	int currentBarWidth = (G.player.currentHealth * barMaxWidth) / G.player.maxHealth;

	// Dessin de la jauge verte (seulement s'il reste de la vie)
	if (currentBarWidth > 0) {
		G2D::drawRectangle(V2(uiX, uiY), V2(currentBarWidth, barHeight), Color::Green, true);
	}

	// Contour blanc
	G2D::drawLine(V2(uiX, uiY), V2(uiX + barMaxWidth, uiY), Color::White);
	G2D::drawLine(V2(uiX, uiY), V2(uiX, uiY + barHeight), Color::White);
	G2D::drawLine(V2(uiX + barMaxWidth, uiY), V2(uiX + barMaxWidth, uiY + barHeight), Color::White);
	G2D::drawLine(V2(uiX, uiY + barHeight), V2(uiX + barMaxWidth, uiY + barHeight), Color::White);

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
	bool turn = G.player.getPlayerTurn();
	G.enemy.update(G.player.futurePos, turn);
	G.player.setPlayerTurn(turn);// on mets a jour le joueur en focntion de la décision de l'ennemi

	// On vérifie que c'est bien à nous de jouer, qu'on ne bouge pas et qu'on n'attaque pas si c'est le cas on peut prendre une potion
	if (G.player.getPlayerTurn() && G.player.pos == G.player.futurePos && !G.player.isAttacking) {
		if (G2D::keyHasBeenHit(Key::A)) {
			// On vérifie qu'on a au moins 1 potion ET qu'on n'est pas déjà full life
			if (G.inventory.items["Potion de vie"] > 0 && G.player.currentHealth < G.player.maxHealth) {
				G.inventory.removeItem("Potion de vie"); // On consomme l'objet
				G.player.heal(30);                       // On rend 30 HP

<<<<<<< Updated upstream
				// On passe le tour a l'ennemi
				G.player.setPlayerTurn(false);
=======
	//Gestion de la boule de feu
	bool fireballFinished = false;
	if (G.player.fireball.active) {
		G.player.fireball.update(G.enemy, G.boss);
		// Si elle était active mais vient de s'éteindre après l'update
		if (!G.player.fireball.active) {
			fireballFinished = true;
		}
	}

	if(G.currentTurn == GameData::TurnState::Player && G.player.isAlive) {

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
				G.player.haveKatana = true; // Le joueur a maintenant le katana, ce qui augmente ses dégâts d'attaque
			}

			G.playerHasActed = G.player.handleInput(G.enemy, G.boss);

			if (G.player.dealDamage) {
				V2 attackPos = G.player.getAttackPos(); // On récupère la case visée

				// Dégâts sur l'ennemi de base
				if (attackPos == G.enemy.pos && G.enemy.isAlive) {
					G.enemy.takeDamage(G.player.attackDamage);
				}
				// Dégâts sur le Boss (HitBox permet de gérer sa taille de 2x2)
				if (G.boss.isAlive && G.boss.HitBox(G.boss.pos, attackPos)) {
					G.boss.takeDamage(G.player.attackDamage);
				}

				G.player.setDealDamage(false);
>>>>>>> Stashed changes
			}
		}
	}

	// Gestion du faire de ramasser des objets et les mettre dans l'inventaire
	if (G.player.pos == G.player.futurePos) {

		// On convertit la position du joueur en pixels vers une position sur la grille (x, y)
		int tileX = G.player.pos.x / G.map.tilesetSize;
		int tileY = G.player.pos.y / G.map.tilesetSize;

		// Calcul pour trouver l'index dans le string avec TA FORMULE FREROT G PAS COMPRIS CE QUE CA FAIS g juste compris que ca récup l'indice d'ou on est mais jsp comment je te laisse avec ton caca
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





