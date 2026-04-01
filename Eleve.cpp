#pragma warning( disable : 4996 ) 

 
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include "G2D.h"
using namespace std;

// touche P   : mets en pause
// touche ESC : ferme la fenêtre et quitte le jeu


///////////////////////////////////////////////////////////////////////////////
//
//    Données du jeu - structure instanciée dans le main

struct GameData
{
	int HeighPix = 800;   // hauteur de la fenêtre d'application
	int WidthPix = 600;   // largeur de la fenêtre d'application

	int idFrame = 0;

	GameData() {}

};

 
///////////////////////////////////////////////////////////////////////////////
//
// 
//     fonction de rendu - reçoit en paramètre les données du jeu par référence



void Render(const GameData& G)
{
	// fond noir	 
	G2D::clearScreen(Color::Black);

	// affiche du texte
	if ( G2D::isOnPause() )
	   G2D::drawStringFontMono(V2(50, 400), "Pause", 60, 3, Color::Green);


	// affiche du texte
	G2D::drawStringFontMono(V2(50, 700), "Hello World !", 20, 3, Color::Red);

	// recupère les coordonnées de la souris
	int x, y;
	G2D::getMousePos(x, y);

	// dessinne un viseur à la position actuelle de la souris
	G2D::drawLine(V2(x - 50,y), V2(x + 50,y), Color::White );
	G2D::drawLine(V2(x,y - 50), V2(x,y + 50), Color::White);

	// si le bouton gauche de la souris est appuyé, affiche un rond rouge
	if (G2D::isMouseLeftButtonPressed())
		G2D::drawCircle(V2(x, y), 20, Color::Red, true);

	// affiche une bille qui roule
	G2D::drawCircle(V2(G.idFrame, 20), 20, ColorFromHex(0xf28f93), true);

 
	
	float angle = G.idFrame  /  100.0 * 3.141519;
	V2 dir = 100 * V2(cos(angle), sin(angle));
	V2 centre = V2(300, 300);
	G2D::drawLine(centre-dir, centre+dir, Color::White);
	G2D::drawCircle(centre, 5, Color::White, true);


	G2D::Show();
}




	
///////////////////////////////////////////////////////////////////////////////
//
//
//      Gestion de la logique du jeu - reçoit en paramètre les données du jeu par référence



void Logic(GameData & G) // appelé 20 fois par seconde
{
	if (G2D::isOnPause())
		return;

	G.idFrame += 1;
 
	 
}
 

///////////////////////////////////////////////////////////////////////////////
//
//
//        Démarrage de l'application



int main(int argc, char* argv[])
{
	GameData G;   // instanciation de l'unique objet GameData qui sera passé aux fonctions render et logic

	// crée la fenêtre de l'application
	G2D::initWindow(V2(G.WidthPix, G.HeighPix), V2(20, 20), string("G2D DEMO"));

	// nombre de fois où la fonction Logic est appelée par seconde
	int callToLogicPerSec = 50;  

	// lance l'application en spécifiant les deux fonctions utilisées et l'instance de GameData
	G2D::Run(Logic, Render, G, callToLogicPerSec,true);

	// aucun code ici
}





