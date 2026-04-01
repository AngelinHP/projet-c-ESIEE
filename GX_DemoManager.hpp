#pragma once
#include <vector>
 

namespace GX
{ 

	using RenderFunc = void (*)(bool);
	using LogicFunc  = void (*)(float);
	using InitFunc   = void (*)(void);

	struct GameInfo 
	{ 
		RenderFunc  render;
		LogicFunc   logic;
		InitFunc    init;
		float       demoTime;
		bool        active         = false;
		float       activeTime     = 0;
		bool        isInitialized  = false;

		GameInfo(RenderFunc R, LogicFunc L, InitFunc I, float demoDuration)
		{
			render   = R;
			logic    = L;
			init     = I;
			demoTime = demoDuration;
		}
	};
 
	void RegisterGame(RenderFunc Render, LogicFunc Logic, InitFunc Init, float DemoTime = 2);
	 
	 
	void allGamesInit();
	void currentGameRender(bool isInPause);
	void currentGameLogic(float dt);
}
