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
		if (currentHealth <= 0) currentHealth = 0;
	} // On empêche la vie de passer en négatif

	void die() {
		isAlive = false;
		turnDone = true; // Le tour du joueur est passé
	}	

	virtual bool LineOfSight(V2 playerPos) {
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


	virtual bool rangeOfAttack(V2 playerPos) {
		return playerPos == pos + V2(mapMan.tilesetSize, 0) 
			|| playerPos == pos - V2(mapMan.tilesetSize, 0) 
			|| playerPos == pos + V2(0, mapMan.tilesetSize) 
			|| playerPos == pos - V2(0, mapMan.tilesetSize);
	}

	virtual void attack() {
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

	virtual void ChangeToPlayerDirection(V2& playerPos) {
		V2 toPlayer = playerPos - pos;
		if (toPlayer.x > 0) lastDir = Direction::Right;
		else if (toPlayer.x < 0) lastDir = Direction::Left;
		else if (toPlayer.y > 0) lastDir = Direction::Up;
		else if (toPlayer.y < 0) lastDir = Direction::Down;
	}

	virtual void nextAction(V2& playerPos,V2& playerFuturePos) {

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

	virtual void registerMove(V2& playerFuturePos) {

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

	virtual void move() {
		if (isMoving && canMove && !(futurePos == pos)) {

			V2 toFuture = V2(futurePos.x - pos.x, futurePos.y - pos.y);
			float distSq = toFuture.x * toFuture.x + toFuture.y * toFuture.y;

			// Si on est à moins d'un pas, on arrive directement
			if (distSq <= speed * speed)
				pos = futurePos;
			else
				pos = pos + dirVectors[lastMove].GetNormalized() * speed;
		}

		if (isMoving && futurePos == pos) {
			isMoving = false;
			lastMove = Movement::None;
			turnDone = true;
		}

		anim.SetDirection(lastDir);
		anim.isMoving = isMoving && canMove;
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


struct Boss : public Enemy {

	int maxHealth = 130;
	int phase = 1;

	AnimationHandler chargeEffectAnim;

	int bossSize = 2; // taille en cases

	bool isJumping = false;
	bool isCharging = false;

	bool isAggressive = false; //le boss dort tant qu'on ne l'agresse pas

	bool isChargingUp = false; // tour 1 : vent de charge
	bool isChargeAttacking = false;
	int  chargeAttackDamage = 60; // double des dégâts normaux

	bool  isTransitioning = false;
	float transitionProgress = 0.0f;
	int   transitionTimer = 0;
	const int TRANSITION_DURATION = 180;

	int moveCooldown = 0; // 0 = peut bouger, 1 = attend ce tour

	Boss(V2& _pos, int& tilesetSize, MapManager& _map)
		: Enemy(_pos, tilesetSize, _map)
	{
		currentHealth = maxHealth;
		attackDamage = 25;
	}

	void InitTexture() {
		// Même sprites que l'ennemi pour l'instant
		anim.LoadTexture("Idle", "sprites\\boss\\phase1\\Idle.png", 60, V2(360, 60), 6, true);
		anim.LoadTexture("Walk", "sprites\\boss\\phase1\\Idle.png", 60, V2(360, 60), 6, true);
		anim.LoadTexture("Attack", "sprites\\boss\\phase1\\Attack.png", 60, V2(240, 60), 4, true);
		anim.LoadTexture("Charge", "sprites\\boss\\phase1\\Charge.png", 60, V2(120, 60), 2, true);
		anim.LoadTexture("Jump", "sprites\\boss\\phase1\\Jump.png", 60, V2(300, 60), 5, true);
		anim.LoadTexture("Idle_red", "sprites\\boss\\phase2\\Idle.png", 60, V2(360, 60), 6, true);
		anim.LoadTexture("Walk_red", "sprites\\boss\\phase2\\Walk.png", 60, V2(360, 60), 6, true);
		anim.LoadTexture("Attack_red", "sprites\\boss\\phase2\\Attack.png", 60, V2(240, 60), 4, true);
		anim.LoadTexture("Charge_red", "sprites\\boss\\phase2\\Charge.png", 60, V2(120, 60), 2, true);
		anim.LoadTexture("Jump_red", "sprites\\boss\\phase2\\Jump.png", 60, V2(300, 60), 5, true);

		//effet de claw
		chargeEffectAnim.LoadTexture("Idle", "sprites\\boss\\phase2\\Claw.png", 32, V2(128, 32), 4, true);
		chargeEffectAnim.animSpeed = 10; // Vitesse rapide pour l'effet d'attaque
		chargeEffectAnim.isMoving = false; // Force la lecture avec 'animSpeed'
	}

	// Vérifie si une position monde est occupée par le boss
	bool HitBox(V2 anchor, V2 worldPos) {
		for (int i = 0; i < bossSize; i++)
			for (int j = 0; j < bossSize; j++)
				if (worldPos == V2(anchor.x + i * mapMan.tilesetSize, anchor.y + j * mapMan.tilesetSize))
					return true;
		return false;
	}
	
	// Vérifie si toutes les cases d'une zone 4x4 sont libres
	bool canLand(V2& anchor, V2& playerFuturepos) {
		for (int i = 0; i < bossSize; i++)
			for (int j = 0; j < bossSize; j++) {
				V2 tile = V2(anchor.x + i * mapMan.tilesetSize, anchor.y + j * mapMan.tilesetSize);
				if (mapMan.Mur(tile.x / mapMan.tilesetSize, tile.y / mapMan.tilesetSize) || HitBox(playerFuturepos, tile))
					return false;
			}
		return true;
	}

	void takeDamage(int amount) {
		// Appelle la fonction de base pour réduire la vie
		Enemy::takeDamage(amount);

		// Le boss se réveille dès qu'il prend des dégâts
		if (!isAggressive) {
			isAggressive = true;
		}
	}

	// Choisit la bonne texture selon l'état du boss
	string getBossTextureName() {
		if (isTransitioning || isChargingUp || isChargeAttacking) return "Charge"; // ← priorité absolue
		if (isJumping)       return "Jump";
		if (isAttacking)     return "Attack";
		return "Idle";
	}

	bool LineOfSight(V2 playerPos) override {
		int ts = mapMan.tilesetSize;
		int range = 7 * ts;

		for (int j = 0; j < bossSize; j++) {
			int rowY = pos.y + j * ts;
			if (playerPos.y == rowY) {
				if (playerPos.x < pos.x && pos.x - playerPos.x <= range)                          return true;
				if (playerPos.x >= pos.x + bossSize * ts && playerPos.x - pos.x <= range + bossSize * ts) return true;
			}
		}

		for (int i = 0; i < bossSize; i++) {
			int colX = pos.x + i * ts;
			if (playerPos.x == colX) {
				if (playerPos.y < pos.y && pos.y - playerPos.y <= range)                          return true;
				if (playerPos.y >= pos.y + bossSize * ts && playerPos.y - pos.y <= range + bossSize * ts) return true;
			}
		}

		// Proximité immédiate (joueur dans ou très proche du boss)
		V2 center = V2(pos.x + (bossSize / 2) * ts, pos.y + (bossSize / 2) * ts);
		return abs(playerPos.x - center.x) <= bossSize * ts
			&& abs(playerPos.y - center.y) <= bossSize * ts;
	}

	bool rangeOfAttack(V2 playerPos) override {
		int ts = mapMan.tilesetSize;
		for (int i = 0; i < bossSize; i++) {
			if (playerPos == V2(pos.x + i * ts, pos.y - ts))              return true; // bas
			if (playerPos == V2(pos.x + i * ts, pos.y + bossSize * ts))     return true; // haut
			if (playerPos == V2(pos.x - ts, pos.y + i * ts))            return true; // gauche
			if (playerPos == V2(pos.x + bossSize * ts, pos.y + i * ts))     return true; // droite
		}
		return false;
	}

	void ChangeToPlayerDirection(V2& playerPos) override {
		V2 center = V2(pos.x + (bossSize / 2) * mapMan.tilesetSize,
			pos.y + (bossSize / 2) * mapMan.tilesetSize);
		V2 toPlayer = playerPos - center;
		if (abs(toPlayer.x) >= abs(toPlayer.y))
			lastDir = toPlayer.x > 0 ? Direction::Right : Direction::Left;
		else
			lastDir = toPlayer.y > 0 ? Direction::Up : Direction::Down;
	}

	void registerMove(V2& playerFuturePos) override {
		int ts = mapMan.tilesetSize;
		int jumpDist = rand() % 3 + 1; // 1, 2 ou 3 cases

		// Toutes les cases de la zone 3x3 (8 directions, diagonales incluses)
		vector<V2> candidates;
		for (int dx = -1; dx <= 1; dx++)
			for (int dy = -1; dy <= 1; dy++) {
				if (dx == 0 && dy == 0) continue;
				V2 dest = V2(pos.x + dx * jumpDist * ts, pos.y + dy * jumpDist * ts);
				if (canLand(dest, playerFuturePos))
					candidates.push_back(dest);
			}

		if (candidates.empty()) { turnDone = true; return; }

		V2 toPlayer = playerFuturePos - pos;
		V2 chosen;

		if (rand() % 100 < 70) {
			// 70% : choisir la case la plus proche du joueur
			float bestDot = -999999; //valeur initiale très basse pour être sûr de la remplacer
			for (V2& c : candidates) {
				float dot = (c.x - pos.x) * toPlayer.x + (c.y - pos.y) * toPlayer.y;
				if (dot > bestDot) { bestDot = dot; chosen = c; }
			}
		}
		else {
			// 30% : choisir une case aléatoire
			chosen = candidates[rand() % candidates.size()];
		}

		futurePos = chosen;

		// Direction visuelle (axe dominant)
		V2 dir = V2(futurePos.x - pos.x, futurePos.y - pos.y);
		if (abs(dir.x) >= abs(dir.y))
			lastDir = dir.x > 0 ? Direction::Right : Direction::Left;
		else
			lastDir = dir.y > 0 ? Direction::Up : Direction::Down;

		lastMove = Movement::None; // plus utilisé, mouvement géré dans move()
		isMoving = true;
		isJumping = true;
		canMove = true;
		speed = 4;
	}

	void move() override {
		if (isMoving && canMove && !(futurePos == pos)) {
			V2 toFuture = V2(futurePos.x - pos.x, futurePos.y - pos.y);
			float distSq = toFuture.x * toFuture.x + toFuture.y * toFuture.y;

			if (distSq <= speed * speed)
				pos = futurePos;
			else {
				float dist = sqrt(distSq);
				pos = V2(pos.x + (toFuture.x / dist) * speed,
					pos.y + (toFuture.y / dist) * speed);
			}
		}

		if (isMoving && futurePos == pos) {
			isMoving = false;
			turnDone = true;
		}

		anim.SetDirection(lastDir);
		anim.isMoving = isMoving && canMove;
		anim.Update();
	}

	void nextAction(V2& playerPos, V2& playerFuturePos) override {

		// Si le joueur n'a pas encore frappé le boss, le boss passe son tour immédiatement
		if (!isAggressive) {
			turnDone = true;
			return;
		}

		if (isAttacking || isMoving || turnDone) return;


		if (isChargingUp) {
			isChargingUp = false;
			isChargeAttacking = true;
			isAttacking = true;
			anim.timer = 0;
			return;
		}

		playerIsInSight = LineOfSight(playerFuturePos);
		playerIsInRange = rangeOfAttack(playerFuturePos);

		if (!playerIsInRange && playerIsInSight) {
			if (moveCooldown == 0) { registerMove(playerFuturePos); moveCooldown = 1; //passe son tour
			}
			else { moveCooldown = 0; turnDone = true; }
		}// saut vers le joueur
		else if (playerIsInRange) {
			int roll = rand() % 100;
			if (phase == 2 && roll < 30) { //30% d'attaque chargée
				// Tour 1 : début du chargement
				isChargingUp = true;
				turnDone = true;
				ChangeToPlayerDirection(playerPos);
			}
			else if (roll < 30) {
				flee(playerFuturePos); // se deplace 30% du temps en range d'attaque pour tenter de se repositionner
			}
			else {
				isChargeAttacking = false;
				isAttacking = true;
				anim.timer = 0;
				ChangeToPlayerDirection(playerPos);
			} // 40% d'attaque normale
		}
		else
			turnDone = true;
	}

	void flee(V2& playerFuturePos) {
		// Construire une cible dans la direction OPPOSÉE au joueur
		V2 toPlayer = playerFuturePos - pos;
		V2 fleeTarget = V2(pos.x - toPlayer.x, pos.y - toPlayer.y);
		registerMove(fleeTarget); // réutilise le saut, mais en sens inverse
	}

	void updatePhase() {
		if (phase == 1 && currentHealth <= maxHealth / 2 && !isTransitioning) {
			isTransitioning = true;
			isCharging = true;
			anim.timer = 0;
			transitionTimer = 0;
			transitionProgress = 0.0f;
		}
	}

	// Override : updatePhase en plus à chaque tour
	bool update(V2& playerPos, V2& playerFuturePos) {
		updatePhase();

		if (isTransitioning) {
			transitionTimer++;
			transitionProgress = (float)transitionTimer / TRANSITION_DURATION;

			anim.isAttacking = false;
			anim.isMoving = false;
			anim.Update();

			if (transitionTimer >= TRANSITION_DURATION) {
				phase = 2;
				attackDamage = 40;
				isTransitioning = false;
				isCharging = false;
				transitionProgress = 1.0f;
				turnDone = true;
			}
			return turnDone; // le boss est figé pendant la transition
		}

		currentTime = G2D::elapsedTimeFromStartSeconds();
		isAlive = currentHealth > 0;

		bool wasMoving = isMoving;

		if (isAlive) {
			nextAction(playerPos, playerFuturePos);
			move();
			if (playerFuturePos == playerPos && !isChargingUp)
				attack();
			else if (isChargingUp)
				attack();
		}
		else {
			Enemy::die();
		}

		if (wasMoving && !isMoving) { isJumping = false; speed = 2; }

		// Mise à jour de l'effet si le boss est en train de faire son attaque chargée
		if (isChargeAttacking) {
			chargeEffectAnim.Update();
		}

		return turnDone;
	}

	void draw(Camera2D& camera) {
		if (!isAlive) return;

		string texName = getBossTextureName();
		if (anim.textures.find(texName) == anim.textures.end()) return;

		int srcX = anim.currentFrame * anim.spriteSize; // horizontal strip
		int srcY = 0;

		bool drawRed = false;
		if (isTransitioning) {
			int flashRate = max(1, (int)(20 * (1.0f - transitionProgress)));
			drawRed = (transitionTimer % flashRate) < (flashRate / 2);
		}
		else if (phase == 2) {
			drawRed = true; //swap vers sprites rouges ici quand disponibles
		}

		if (!drawRed) {
			G2D::drawSpriteFrame(
				anim.textures[texName], camera.renderWcamera(pos), V2(anim.spriteSize, anim.spriteSize) * camera.zoom,
				V2(srcX, 0), V2(anim.spriteSize, anim.spriteSize),
				anim.textureSizes[texName]
			);
		}
		else {
			G2D::drawSpriteFrame(
				anim.textures[texName + "_red"], camera.renderWcamera(pos), V2(anim.spriteSize, anim.spriteSize) * camera.zoom,
				V2(srcX, 0), V2(anim.spriteSize, anim.spriteSize),
				anim.textureSizes[texName + "_red"]
			);
		}

		// Dessin de l'effet tout autour si l'attaque chargée est en cours
		if (isChargeAttacking) {
			int ts = mapMan.tilesetSize;

			// Coordonnées des 8 cases entourant le Boss
			vector<V2> effectPositions = {
				// Ligne du Haut
				V2(pos.x, pos.y - ts), V2(pos.x + ts, pos.y - ts),
				// Ligne du Bas
				V2(pos.x, pos.y + 2 * ts), V2(pos.x + ts, pos.y + 2 * ts),
				// Colonne de Gauche
				V2(pos.x - ts, pos.y), V2(pos.x - ts, pos.y + ts),
				// Colonne de Droite
				V2(pos.x + 2 * ts, pos.y), V2(pos.x + 2 * ts, pos.y + ts)
			};

			for (V2 effectPos : effectPositions) {
				chargeEffectAnim.Draw(camera, effectPos);
			}
		}
	}
};

struct Projectile {
	V2 pos;
	V2 dirVector;
	bool active = false;
	int damage = 40;
	int speed = 4; // Vitesse de déplacement fluide entre les cases
	int maxRange = 2; // en cases
	V2 targetPos;
	V2 maxPos; // position maximale atteignable selon la direction et la portée

	AnimationHandler anim;
	MapManager mapMan;

	Projectile() {}

	void Init(V2 startPos, Direction dir, int tilesetSize, MapManager& _map) {
		pos = startPos;
		targetPos = startPos;
		maxPos = startPos;
		mapMan = _map;
		active = true;

		// Détermine la direction du vecteur selon l'orientation du joueur
		if (dir == Direction::Up)    dirVector = V2(0, tilesetSize);
		else if (dir == Direction::Down)  dirVector = V2(0, -tilesetSize);
		else if (dir == Direction::Left)  dirVector = V2(-tilesetSize, 0);
		else if (dir == Direction::Right) dirVector = V2(tilesetSize, 0);


		// Calcule la position maximale atteignable
		maxPos = pos + dirVector * maxRange;
	}

	void loadTexture() {
		anim.LoadTexture("Idle", "sprites\\player\\Fireball.png", 32, V2(128, 32), 4, true);
		anim.animSpeed = 6;
		anim.isMoving = false;
	}

	// Retourne true si le projectile a fini son déplacement pour ce tour
	void update(Enemy& enemy, Boss& boss) {
		if (!active) return;

		anim.Update();

		// Déplacement fluide vers la case cible
		V2 toTarget = targetPos - pos;
		float distSq = toTarget.x * toTarget.x + toTarget.y * toTarget.y;

		if (distSq <= speed * speed) {
			pos = targetPos; // On se cale précisément sur la case

			//Collision avec l'ennemi normal
			if (enemy.currentHealth > 0 && pos == enemy.pos) {
				enemy.takeDamage(damage);
				active = false;
				return;
			}

			//Collision avec le Boss
			if (boss.currentHealth > 0 && boss.HitBox(boss.pos, pos)) {
				boss.takeDamage(damage);
				active = false;
				return;
			}

			//Collision avec un mur
			if (mapMan.Mur(pos.x / mapMan.tilesetSize, pos.y / mapMan.tilesetSize)) {
				active = false;
				return;
			}

			if(pos == maxPos)
			{
				active = false;
				return;
			}

			// Si aucune collision, on planifie la case suivante
			targetPos = pos + dirVector;
		}
		else {
			pos = pos + dirVector.GetNormalized() * speed;
		}
	}

	void draw(Camera2D& camera) {
		if (active) {
			anim.Draw(camera, pos);
		}
	}
};


struct Player
{
	V2 pos;

	Projectile fireball; //le joueur a un projectile
	
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
	bool alreadyLaunchedFireball = false;
	bool haveKatana = false;

	bool isAlive = true;

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
		fireball.loadTexture();
	}

	bool hasActed() {
		return !(pos == futurePos) || dealDamage;
	}

	void registerMovement(Enemy& enemy, Boss& boss)
	{
		if (pos == futurePos) {
			if (!isInInventory && !isAttacking) {

				V2        newPos = pos;
				Movement  newMove = Movement::None;
				Direction newDir = lastDir;

				if (G2D::isKeyPressed(Key::Z)) { newDir = Direction::Up; newMove = Movement::Up; newPos = pos + dirVectors[Movement::Up]; alreadyLaunchedFireball = false;}
				else if (G2D::isKeyPressed(Key::S)) { newDir = Direction::Down; newMove = Movement::Down; newPos = pos + dirVectors[Movement::Down]; alreadyLaunchedFireball = false;}
				else if (G2D::isKeyPressed(Key::Q)) { newDir = Direction::Left; newMove = Movement::Left; newPos = pos + dirVectors[Movement::Left]; alreadyLaunchedFireball = false;}
				else if (G2D::isKeyPressed(Key::D)) { newDir = Direction::Right; newMove = Movement::Right; newPos = pos + dirVectors[Movement::Right]; alreadyLaunchedFireball = false;}
				
				
				else if (G2D::keyHasBeenHit(Key::F)) { isAttacking = true; anim.timer = 0; }
				else if (G2D::keyHasBeenHit(Key::E) && !fireball.active && !alreadyLaunchedFireball) {fireball.Init(pos, lastDir, mapMan.tilesetSize, mapMan); alreadyLaunchedFireball = true;}


				if (newMove != Movement::None) {
					lastDir = newDir; // la direction visuelle est mise à jour même si bloqué
					bool wallBlocked = mapMan.Mur(newPos.x / mapMan.tilesetSize, newPos.y / mapMan.tilesetSize)
						|| (newPos == enemy.futurePos && enemy.isAlive)
						|| (boss.isAlive && boss.HitBox(boss.pos, newPos))      // cases actuelles
						|| (boss.isAlive && boss.HitBox(boss.futurePos, newPos)); // cases futures					
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

	void move(Enemy& enemy, Boss& boss) {
		// Mise à jour de l'animateur
		isMoving = lastMove != Movement::None && !(pos == futurePos);
		canMove = !mapMan.Mur(futurePos.x / mapMan.tilesetSize, futurePos.y / mapMan.tilesetSize)
			&& !(futurePos == enemy.futurePos && enemy.isAlive)
			&& !(boss.isAlive && boss.HitBox(boss.pos, futurePos));
		if(!canMove) futurePos = pos;

		anim.SetDirection(lastDir);

		if (isMoving && canMove)
			pos = pos + dirVectors[lastMove].GetNormalized() * speed;

		anim.isMoving = isMoving && canMove;

	}

	void attack() {
		// Attaque en fonction de la direction
		if (isAttacking) {
			attackDamage = haveKatana ? 35 : 5; // dégâts plus élevés si le joueur a le katana
			anim.isAttacking = true;
			// Meme logique que pour l'ennemie
			if (anim.timer >= 59) {
				isAttacking = false;
				anim.isAttacking = false;
				dealDamage = true;
			}
		}
	}
	

	// Retourne la position exacte de la case attaquée en fonction de la direction du joueur
	V2 getAttackPos() {
		if (lastDir == Direction::Up)    return pos + V2(0, mapMan.tilesetSize);
		if (lastDir == Direction::Down)  return pos + V2(0, -mapMan.tilesetSize);
		if (lastDir == Direction::Left)  return pos + V2(-mapMan.tilesetSize, 0);
		if (lastDir == Direction::Right) return pos + V2(mapMan.tilesetSize, 0);
		return pos;
	}

	void die() {
		
	}

	void getHit() {
		anim.Hit = true;
		anim.timer = 0;
	}
	void takeDamage(int amount) {
		currentHealth -= amount;
		if (currentHealth <= 0) { currentHealth = 0; isAlive = false; } // On empêche la vie de passer en négatif
	}

	void heal(int amount) {
		currentHealth += amount;
		if (currentHealth > maxHealth) currentHealth = maxHealth; // On empêche de dépasser le max
	}

	void animation(Enemy& enemy, Boss& boss)
	{

		move(enemy, boss);
		attack();
		

		// On fait avancer le temps de l'animation
		anim.Update();

	}

	bool handleInput(Enemy& enemy, Boss& boss) {
		registerMovement(enemy, boss);
		return hasActed() || fireball.active;
	}

	void draw(Camera2D& camera)
	{
		anim.Draw(camera, pos);
		if (fireball.active)
			fireball.draw(camera);
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

	V2 centerMap = V2(WidthPix / 2, HeighPix / 2);

	bool playerHasActed = false;

	enum class TurnState { Player, Enemy, Boss };
	enum class GameState { Playing, GameOver, Victory };
	TurnState currentTurn = TurnState::Player;
	GameState currentGameState = GameState::Playing;

	MapManager& map = MapManager();

	V2 spawn = map.recupSpawn();
	V2 eSpawn = map.recupESpawn();
	V2 bossSpawn = map.recupBossSpawn();
	
	
	Player& player = Player(spawn, map.tilesetSize, map);
	Enemy& enemy = Enemy(eSpawn, map.tilesetSize, map);
	Boss& boss = Boss(bossSpawn, map.tilesetSize, map);

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
	if (G.currentGameState == GameData::GameState::GameOver) {
		// On dessine un fond noir par dessus le jeu
		G2D::drawRectangle(V2(0, 0), V2(G.WidthPix, G.HeighPix), Color::Black, true);

		// On écrit le texte au centre
		G2D::drawStringFontMono(V2(G.WidthPix / 2 - 150, G.HeighPix / 2), "GAME OVER", 50.0F, 4.0F, Color::Red);
	}
	else if (G.currentGameState == GameData::GameState::Victory) {
		// On dessine un fond noir par dessus le jeu
		G2D::drawRectangle(V2(0, 0), V2(G.WidthPix, G.HeighPix), Color::Black, true);
		// On écrit le texte au centre
		G2D::drawStringFontMono(V2(G.WidthPix / 2 - 150, G.HeighPix / 2), "VICTORY", 50.0F, 4.0F, Color::Green);
	}
	else if (G.currentGameState == GameData::GameState::Playing) {
		// Dessine la carte


		G.map.drawMap(G.camera);

		G.player.draw(G.camera);
		G.enemy.draw(G.camera);
		G.boss.draw(G.camera);

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

	}
	G2D::Show();
}




	
///////////////////////////////////////////////////////////////////////////////
//
//
//      Gestion de la logique du jeu - re�oit en param�tre les donn�es du jeu par r�f�rence



void Logic(GameData & G) // appel� 20 fois par seconde
{
	G.camera.update(G.player.pos);

	G.player.animation(G.enemy, G.boss);

	G.boss.anim.Update();

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
			}
		}
		else
		{
			G.playerHasActed = false;
		}


		// On vérifie que c'est bien à nous de jouer, qu'on ne bouge pas et qu'on n'attaque pas si c'est le cas on peut prendre une potion
	
			if (G2D::keyHasBeenHit(Key::A)) {
				// On vérifie qu'on a au moins 1 potion ET qu'on n'est pas déjà full life
				if (G.inventory.items["Potion de vie"] > 0 && G.player.currentHealth < G.player.maxHealth) {
					G.inventory.removeItem("Potion de vie"); // On consomme l'objet
					G.player.heal(30);                       // On rend 30 HP
					G.playerHasActed = true;                    // On considère que le joueur a agi pour passer son tour
				}
			}
		
			if ((G.playerHasActed && !G.player.fireball.active) || fireballFinished) {
				G.enemy.turnDone = false; // On reset le tour de l'ennemi pour qu'il puisse agir à son tour
				G.boss.turnDone = false;
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
			G.currentTurn = GameData::TurnState::Boss;
	}


	else if (G.currentTurn == GameData::TurnState::Boss) {
		bool bossDone = G.boss.update(G.player.pos, G.player.futurePos);
		if (G.boss.dealDamage) {
			if (G.boss.rangeOfAttack(G.player.pos)) {
				int dmg = G.boss.isChargeAttacking
					? G.boss.chargeAttackDamage  // attaque chargée 
					: G.boss.attackDamage;       // attaque normale
				G.player.takeDamage(dmg);
			}
			G.boss.dealDamage = false;
			G.boss.isChargeAttacking = false; // reset
		}

		if (bossDone) G.currentTurn = GameData::TurnState::Player;
	}

	// ÉCRAN DE GAME OVER
	if (!G.player.isAlive) {
		G.currentGameState = GameData::GameState::GameOver;
	}

	if (!G.boss.isAlive)
	{
		G.currentGameState = GameData::GameState::Victory;
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
	G.boss.InitTexture();
	G.map.InitTilesTexture();
	

	G2D::Run(Logic, Render, G, callToLogicPerSec, true);

	// aucun code ici
}