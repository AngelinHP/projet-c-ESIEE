#include "GX.h"
#include "GlutImport.h"
#include "GX_GLWRAPPER.h"
#include <string>
#include <map>
#include <vector>
#include <chrono>
 


/////////////////// hidden include //////////////////////////////////
 
# include "GX_Internal.hpp"

/////////////////////////////////////////////////////////////
//
//	    Init   Window
//
/////////////////////////////////////////////////////////////


 
int GXinternal_getWindowWidth()  { return GLWrapper::getWindowWidth(); }
int GXinternal_getWindowHeight() { return GLWrapper::getWindowHeight(); }




void GX::initWindow(V2 WindowSize, V2 WindowStartPos, std::string name)
{
	GLWrapper::initGLWindow(WindowSize, WindowStartPos, name);
}



/////////////////////////////////////////////////////////////
//
//	   Gestion de l'animation et des FPS
//
/////////////////////////////////////////////////////////////

class Timer
{
private :

	std::chrono::steady_clock::time_point T0;

public :

	void Init()
	{
		T0 = std::chrono::steady_clock::now();
	}

	double GetTime()
	{
		typedef std::chrono::milliseconds ms;
		typedef std::chrono::duration<double> duration;

		std::chrono::steady_clock::time_point Tnow = std::chrono::steady_clock::now();

		duration fs = Tnow - T0;
		ms d = std::chrono::duration_cast<ms>(fs);
		double Ts = d.count() * 0.001;

		return Ts;
	}
};
Timer GXTimer;


 
 
 
bool firstCall   = true;
 
void GXRender()
{
	GX::currentGameRender(GX::isInPause());
	GLWrapper::SwapBuffer();
}



double LastT;
void GXLogic(int value)
{
	if (firstCall)
	{
		firstCall = false;
		GXTimer.Init();
		LastT = GXTimer.GetTime();
	}

	double Tnow = GXTimer.GetTime();

	int logicFPS = 60;
	double dt = 1.0 / logicFPS;
	
	// rattrape le retard
	if ( !GX::isInPause() )
	{
		while (LastT + dt < Tnow)
		{
			LastT += dt;
			GX::currentGameLogic(dt);
		}
	}
	else
	{
		LastT = Tnow;
	}
	
	GLWrapper::requestRedraw_Callback();
}
 


/////////////////////////////////////////////////////////////
//
//	    Run Opengl Rendering
//
/////////////////////////////////////////////////////////////
 

void InitColors();

void GX::initGX()
{
	// init des routines internes
	InitColors();

	GX::allGamesInit();
 
	GLWrapper::initGX(GXRender, GXLogic); 
}



void GX::select2DMode() {  GLWrapper::GL_2DMode();  }
void GX::select3DMode(Color Background, Camera Cam, int W,int H) {  GLWrapper::GL_3DMode(Background,Cam,W, H); }



void GX::ActivateUImode()
{
	GLWrapper::BeginUI();
}