#pragma once


#include <map>
#include <string>

#include "Camera2D.h"
#include "G2D.h"


using namespace std;

struct Inventory
{
	map<string, int> items;


	int offsetTextX = 10;
	int offsetTextY = 35;

	int width = 600;
	int height = 200;


	void addItem(string item)
	{
		items[item]++;
	}
	void removeItem(string item)
	{
		if (items[item] > 0) {
			items[item]--;
		}
	}

	void drawInventory(Camera2D& camera, int x, int y)
	{

		G2D::drawRectangle(V2(x, y), V2(width, height), Color::Black, true);
		G2D::drawLine(V2(x, y), V2(x + width, y), Color::White);
		G2D::drawLine(V2(x, y), V2(x, y + height), Color::White);
		G2D::drawLine(V2(x + width, y), V2(x + width, y + height), Color::White);
		G2D::drawLine(V2(x, y + height), V2(x + width, y + height), Color::White);


		int row = 0;

		for (auto it = items.begin(); it != items.end(); it++) {
			string itemText = it->first + ": x" + to_string(it->second);
			G2D::drawStringFontMono(V2(x + offsetTextX, y + height - offsetTextY - (row * offsetTextY)), itemText.c_str(), 20, 2, Color::White);
			row++;
		}
	}
};