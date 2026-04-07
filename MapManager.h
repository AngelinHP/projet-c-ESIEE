#pragma once

#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <map>

#include "G2D.h"

using namespace std;

struct MapManager
{

	int tilesetSize = 16;

	string map1 = "";

	int mapWidth;
	int mapHeight;

	map<char, int> textures;

	

	int getTexture(char tile)
	{
		return 0;
	}

	void drawMap()
	{
		for (int x = 0; x < mapWidth; x++)
			for (int y = 0; y < mapHeight; y++) {

				int texture = getTexture(map1[(15 - y - 1) * 15 + x]);

				G2D::drawRectWithTexture(texture, V2(x * tilesetSize, y * tilesetSize), V2(tilesetSize, tilesetSize));
			}
	}

	bool Mur(int x, int y) { return map1[(15 - y - 1) * 15 + x] == 'M'; }

	bool Eau(int x, int y) { return map1[(15 - y - 1) * 15 + x] == 'E';}

	void InitTilesTexture()
	{

	}

};