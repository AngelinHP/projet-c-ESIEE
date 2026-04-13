#pragma once
#include "G2D.h"
#include "Camera2D.h"

enum class Direction { Down, Up, Left, Right };

class AnimationHandler {
public:
    int spriteSize = 32;
    V2 textureTotalSize = V2(128, 128);

    int idleTexture = 0;
    int walkTexture = 0;

    Direction currentDir = Direction::Down;
    bool isMoving = false;

    int animSpeed = 8;
    int frameCount = 4;
    int timer = 0;
    int currentFrame = 0;

    void LoadTextures(const std::string& idlePath, const std::string& walkPath) {
        idleTexture = G2D::ExtractTextureFromPNG(idlePath, Transparency::None);
        walkTexture = G2D::ExtractTextureFromPNG(walkPath, Transparency::None);
    }

    void Update() {
        timer++;

        int currentSpeed = isMoving ? animSpeed : 15;

        currentFrame = (timer / currentSpeed) % frameCount;
    }

    void SetDirection(Direction dir) {
        currentDir = dir;
    }

    int GetCurrentTexture() {
        return isMoving ? walkTexture : idleTexture;
    }

    int GetSrcX() {
        switch (currentDir) {
        case Direction::Down:  return 0;
        case Direction::Up:    return 32;
        case Direction::Left:  return 64;
        case Direction::Right: return 96;
        default:               return 0;
        }
    }

    int GetSrcY() {
        return currentFrame * spriteSize;
    }

    void Draw(Camera2D& camera, V2 drawPos) {
        int srcX = GetSrcX();
        int srcY = GetSrcY();
        int currentTex = GetCurrentTexture();

        if (currentTex == 0) return;


        G2D::drawSpriteFrame(
            currentTex,
            camera.renderWcamera(drawPos),
            V2(spriteSize * 2, spriteSize * 2)*camera.zoom,
            V2(srcX, srcY),
            V2(spriteSize, spriteSize),
            textureTotalSize
        );
    }
};