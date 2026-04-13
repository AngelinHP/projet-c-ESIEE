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

	int tilesetSize = 64;

	string map1 =
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMMM    MMMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMM      MMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMM  V   MMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMM VSV  MMMMMMMMMMMMMMMMMMMMMMMM   R    MMMMM"
		"MMMMM  K   MMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMM      MMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMM      MMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMM      MMMMMMMMMMMMMMMMMMMMMMMM        MMMMM"
		"MMMMMM    MMMMM         MMMMMMMMMMM        MMMMM"
		"MMMMMMM MMMMMMM    N                       MMMMM"
		"MMMMMMM MMMMMMM         MMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMM MMMMMMM         MMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMM MMMMMMM         MMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMM MMMMMMM         MMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMM                 MMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMM         MMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMM         MMMMMMMMMMMMMMMMMMMMMMMM"
		"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
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

	

	int getTexture(char tile)
	{
		return 1;
	}

	void drawMap(Camera2D& camera)
	{

		cout << "Map size: " << map1.size() << endl;
		for (int x = 0; x < mapWidth; x++) {
			for (int y = 0; y < mapHeight; y++) {


				V2 screenPos = camera.renderWcamera(V2(x * tilesetSize, y * tilesetSize));

				V2 screenSize = V2(tilesetSize, tilesetSize)*camera.zoom;

				G2D::drawRectWithTexture(textures[' '], screenPos, screenSize);

				if (map1[(mapHeight - y - 1) * mapWidth + x] == 'M')
					G2D::drawRectangle(screenPos, screenSize, Color::Blue, true);


				
			}
		}
	}

	bool Mur(int x, int y) { return map1[(mapHeight - y - 1) * mapWidth + x] == 'M'; }

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
	}

};