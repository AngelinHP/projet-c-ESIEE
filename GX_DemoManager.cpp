#include <iostream>
#include "GX_DemoManager.hpp"

int _idActiveGame = 0;
std::vector<GX::GameInfo> _GameList;


int getActiveGame(float dt = 0)
{
	_GameList[_idActiveGame].activeTime += dt;

	// next game
	if ( _GameList[_idActiveGame].activeTime > _GameList[_idActiveGame].demoTime)
	{

		_idActiveGame = (_idActiveGame + 1) % _GameList.size();
		_GameList[_idActiveGame].activeTime = 0;
	}

	//std::cout << _idActiveGame;
	return _idActiveGame;
}


void GX::RegisterGame(RenderFunc Render, LogicFunc Logic, InitFunc Init, float DemoTime)
{
	GameInfo Game(Render, Logic, Init, DemoTime);
	_GameList.push_back(Game);
}


void GX::allGamesInit()                      
{   
	for(GX::GameInfo & G : _GameList)
	   G.init();  
}

void GX::currentGameRender(bool isInPause)      { _GameList[getActiveGame()].render(isInPause); }
void GX::currentGameLogic(float dt)             { _GameList[getActiveGame(dt)].logic(dt); }