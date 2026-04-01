#include "GX.h"

void Demo2DRender(bool isInPause);
void Demo2DLogic(float deltaT);
void Demo2DInit();



int main(int argc, char* argv[])
{
	GX::RegisterGame(Demo2DRender, Demo2DLogic, Demo2DInit, 2);
	


	int HeighPix = 800;   // hauteur de la fenêtre d'application
	int WidthPix = 600;   // largeur de la fenêtre d'application

	// crée la fenêtre de l'application
	GX::initWindow(V2(WidthPix, HeighPix), V2(20, 20), string("GXD DEMO"));
	 

	// lance l'application 
	GX::initGX();


	// aucun code ici
	return 0;
}