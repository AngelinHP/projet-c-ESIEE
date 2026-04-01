#pragma once
#include <string> 
#include "GX V3.h"
#include "GX Color.h"


struct TexturedFace  
{
private :
	int   idTexture;
	V3    P[4]; // A B C D
	V3    N;

public :
	
	TexturedFace::TexturedFace(V3 _A, V3 _B, V3 _C, bool reverseN, const std::string & filename);
	
	void Draw(int rot90 = 0);  // 0 1 2 3 : n rot90  // 4 5 6 7 n rot90+sym
};
 