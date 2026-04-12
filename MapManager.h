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

	string map1 = "MMMMMMMMMMMMMMM"
                  "M M  S        M"
                  "M M M MMM MMM M"
                  "M   M       M M"
                  "MMM M M MMM M M"
                  "M   M M     M M"
                  "M MMM MMM MMMMM"
                  "M   M  M      M"
                  "M M M  M M MM M"
                  "M M M  M M M  M"
                  "M M M MM M MMMM"
                  "M M M    M    M"
                  "M M M MMMMMMM M"
                  "M M      M    M"
				  "MMMMMMMMMMMMMMM";

	int mapWidth = 15;
	int mapHeight = 15;

	map<char, int> textures;

	

	int getTexture(char tile)
	{
		return 1;
	}

	void drawMap(Camera2D& camera)
	{
		for (int x = 0; x < mapWidth; x++) {
			for (int y = 0; y < mapHeight; y++) {

				if (map1[(15 - y - 1) * 15 + x] == 'M')
					G2D::drawRectangle(camera.renderWcamera(V2(x * tilesetSize, y * tilesetSize)), V2(tilesetSize, tilesetSize), Color::Blue, true);

				//int texture = getTexture(map1[(15 - y - 1) * 15 + x]);


				//G2D::drawRectWithTexture(texture, V2(x * tilesetSize, y * tilesetSize), V2(tilesetSize, tilesetSize));
			}
		}
	}

	bool Mur(int x, int y) { return map1[(15 - y - 1) * 15 + x] == 'M'; }

	bool Eau(int x, int y) { return map1[(15 - y - 1) * 15 + x] == 'E';}

	V2 recupSpawn() {
		for (int x = 0; x < mapWidth; x++) {
			for (int y = 0; y < mapHeight; y++) {
				if (map1[(15 - y - 1) * 15 + x] == 'S')
					return V2(x * tilesetSize, y * tilesetSize);
			}
		}
	}

	void InitTilesTexture()
	{

	}

};