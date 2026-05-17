#pragma once

#include <map>

#include "G2D.h"
#include "Camera2D.h"

enum class Direction { Down, Up, Left, Right };

class AnimationHandler {
public:
    int spriteSize = 32;
    V2 textureTotalSize = V2(128, 128);

    Direction currentDir = Direction::Down;
    bool isMoving = false;
	bool isAttacking = false;
	bool Hit = false;

    int animSpeed = 8;
    int timer = 0;
    int currentFrame = 0;

    map<string, int> textures;
    map<string, V2> textureSizes;
    map<string, int> framecounts;
	map<string, int> animSpeeds;
    map<string, bool> horizontalSpriteSheets;

    void LoadTexture(const std::string& name, const std::string& path, int _spriteSize, V2 _textureTotalSize, int _frameCount, bool _horizontalSpriteSheet = false) {
       int texture = G2D::ExtractTextureFromPNG(path, Transparency::None);
	   if (texture == 0) {
		   return;
	   }
	   textures[name] = texture;
	   textureSizes[name] = _textureTotalSize;

       spriteSize = _spriteSize;
	   framecounts[name] = _frameCount;
	   horizontalSpriteSheets[name] = _horizontalSpriteSheet;
    }

    void Update() {
        timer++;

        int currentSpeed = (GetCurrentTextureName() == "Idle") ? animSpeed : (isMoving && !isAttacking ? animSpeed : 15);
        currentSpeed = isAttacking ? 15 : currentSpeed;

        //Protection contre la division par 0 pour la vitesse
        if (currentSpeed <= 0) {
            currentSpeed = 1;
        }

        string texName = GetCurrentTextureName();
        int maxFrames = framecounts[texName];

        //Protection contre le modulo par 0 (texture non chargée ou 0 frames)
        if (maxFrames <= 0) {
            currentFrame = 0;
            cout << "Invalid animation frame count for texture: " << texName << endl;
            return; // On annule la mise à jour si l'animation est invalide
        }

        currentFrame = (timer / currentSpeed) % maxFrames;

        if (timer >= currentSpeed * maxFrames) {
            timer = 0;
        }
    }

	std::string GetCurrentTextureName() {
        if (isAttacking) return "Attack";
		if (Hit) return "Hit";
		return isMoving ? "Walk" : "Idle";
	}

    void SetDirection(Direction dir) {
        currentDir = dir;
    }

    int GetCurrentTexture() {
        string texName = GetCurrentTextureName();

        // Si la texture demandée existe dans la map, on la renvoie
        if (textures.find(texName) != textures.end()) {
            return textures[texName];
        }

        // Sécurité/Fallback au cas où (pour le joueur et les ennemis)
        int texture = isMoving && !isAttacking ? textures["Walk"] : textures["Idle"];
        texture = isAttacking ? textures["Attack"] : texture;
        texture = Hit ? textures["Hit"] : texture;
        return texture;
    }

    int GetSrcX(const string& texName) {

        if (horizontalSpriteSheets[texName])
            return currentFrame * spriteSize;

        switch (currentDir) {
        case Direction::Down:  return 0;
        case Direction::Up:    return 32;
        case Direction::Left:  return 64;
        case Direction::Right: return 96;
        default:               return 0;
        }
    }

    int GetSrcY(const string& texName) {

        if (horizontalSpriteSheets[texName])
            return 0;
        return currentFrame * spriteSize;
    }

    void Draw(Camera2D& camera, V2 drawPos) {
		string texName = GetCurrentTextureName();
        int srcX = GetSrcX(texName);
        int srcY = GetSrcY(texName);
        int currentTex = GetCurrentTexture();

		//cout << "srcY: " << srcY << endl;

        if (currentTex == 0) return;


        G2D::drawSpriteFrame(
            currentTex,
            camera.renderWcamera(drawPos),
            V2(spriteSize, spriteSize)*camera.zoom,
            V2(srcX, srcY),
            V2(spriteSize, spriteSize),
            textureSizes[GetCurrentTextureName()]
        );
    }
};