#pragma once

#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <map>

#include "G2D.h"
#include "Camera2D.h"

using namespace std;

struct MapManager
{

	int tilesetSize = 32;

	string map1 =
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMCMMMMMMMMCMMMM"
		"MMMMMCMMMMCMMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMCM    MCMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMM      MMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMM  V   MMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMM VSV  MMMMMMMMMMMMMMMMMMMMMMMM   R    MMMMM"
		"MMMMM  K   MMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMM      MMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMM      MMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMM      MMMCMMMMMMMMMCMMMMMMMMMM        MMMMM"
		"MMMMCM    MCMMM         MMMMMMMMMMM        MMMMM"
		"MMMMMCM MMCMMMM    N                       MMMMM"
		"MMMMMMM MMMMMMM         MMMMMMMMMMMMMMMMMMMCMMMM"
		"MMMMMMM MMMMMMM         MMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMM MMMMMMM         MMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMM MMMMMMM         MMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMM                 MMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMCMMMMMMMM         MMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMM         MMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMCMMMMMMMMMCMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM";

	
	int mapHeight = 30;
	int mapWidth = 48;

	map<char, int> textures;

	void drawTextureCorner(int x, int y, Camera2D& camera)
	{

		int horizontal = 2 - Mur(x+1,y+1) - Mur(x+1,y-1), vertical = 1 - Mur(x - 1, y - 1) - Mur(x + 1, y - 1);


		G2D::drawSpriteFrame(
			textures['C'],
			camera.renderWcamera(V2(x * tilesetSize, y * tilesetSize)),
			V2(tilesetSize, tilesetSize) * camera.zoom,
			V2(16 * horizontal, 16 * vertical),
			V2(16, 16),
			V2(32,32)
		);
	}

	void drawTextureWall(int x, int y, Camera2D& camera)
	{

		int horizontal = 0, vertical = 0;

		if (x >= mapWidth-1 || x == 0 || y >= mapHeight-1 || y == 0) {
			vertical = 1;
			horizontal = 1;
		}
		else {
			vertical = 1 - Mur(x, y + 1) + Mur(x, y - 1);
			horizontal = 1 + Mur(x - 1, y) - Mur(x + 1, y);	
		}

		G2D::drawSpriteFrame(
			textures['M'],
			camera.renderWcamera(V2(x * tilesetSize, y * tilesetSize)),
			V2(tilesetSize, tilesetSize) * camera.zoom,
			V2(16*horizontal, 16*vertical),
			V2(16, 16),
			V2(48,48)
		);
	}

	void drawMap(Camera2D& camera)
	{
		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
			


				V2 screenPos = camera.renderWcamera(V2(x * tilesetSize, y * tilesetSize));

				V2 screenSize = V2(tilesetSize, tilesetSize)*camera.zoom;

				G2D::drawRectWithTexture(textures[' '], screenPos, screenSize);

				if (map1[(mapHeight - y - 1) * mapWidth + x] == 'M')
					drawTextureWall(x, y, camera);
				if (map1[(mapHeight - y - 1) * mapWidth + x] == 'C')
					drawTextureCorner(x, y, camera);


				
			}
		}
	}

	bool Mur(int x, int y) { return map1[(mapHeight - y - 1) * mapWidth + x] == 'M' || map1[(mapHeight - y - 1) * mapWidth + x] == 'C'; }

	V2 recupSpawn() {
		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
				if (map1[(mapHeight - y - 1) * mapWidth + x] == 'S')
					return V2(x * tilesetSize, y * tilesetSize);
			}
		}
	}

	void InitTilesTexture()
	{
		textures[' '] = G2D::ExtractTextureFromPNG("sprites/tileset/floor.png", Transparency::None);
		textures['M'] = G2D::ExtractTextureFromPNG("sprites/tileset/walls.png", Transparency::None);
		textures['C'] = G2D::ExtractTextureFromPNG("sprites/tileset/corners.png", Transparency::None);
	}

};