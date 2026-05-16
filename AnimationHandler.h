#pragma once

#include <map>
#include <iostream>

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
		   std::cout << "Failed to load texture: " << path << std::endl;
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

        int currentSpeed = isMoving && !isAttacking ? animSpeed : 15;
        currentSpeed = isAttacking ? 15 : currentSpeed;

        currentFrame = (timer / currentSpeed) % framecounts[GetCurrentTextureName()];

        if(timer >= currentSpeed * framecounts[GetCurrentTextureName()]) {
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