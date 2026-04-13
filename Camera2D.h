#pragma once

#include <iostream>

#include "G2D.h"

using namespace std;


struct Camera2D
{
    V2 pos;
    int winWidth;
    int winHeight;

    // Utiliser un float permet des zooms plus précis (ex: 1.5f, 0.5f pour dézoomer)
    float zoom = 2.0f;

    Camera2D(V2 cPos, int width, int height)
    {
        pos = cPos;
        winWidth = width;
        winHeight = height;
    }

    V2 renderWcamera(V2 ePos)
    {
        //Distance relative
        float relX = ePos.x - pos.x;
        float relY = ePos.y - pos.y;

        // Application du zoom
        relX *= zoom;
        relY *= zoom;

        // Centrage sur l'écran
        float screenX = relX + (winWidth / 2.0f);
        float screenY = relY + (winHeight / 2.0f);

        return V2(screenX, screenY);
    }

    void update(V2 nPos)
    {
        pos = nPos;
    }

    void setZoom(float newZoom)
    {
        if (newZoom > 0.01f) {
            zoom = newZoom;
        }
    }
};