#include "GX.h"
#include "GlutImport.h"


/////////////////////////////////////////////////////////////
//
// 
////////////////    TEXTURE  /////////////////////////



//	    Create Texture From String



typedef unsigned char u8;

u8 LetterToColor[256][4];

void AssignColor(int code, u8 R, u8 GX, u8 B, u8 A = 255)
{

	LetterToColor[code][0] = R;
	LetterToColor[code][1] = GX;
	LetterToColor[code][2] = B;
	LetterToColor[code][3] = A;
}

void InitColors()
{
	// https://www.rapidtables.org/fr/web/color/RGB_Color.html

	AssignColor('R', 255, 0, 0);
	AssignColor('G', 0, 255, 0);
	AssignColor('B', 0, 0, 255);
	AssignColor('K', 0, 0, 0); // noir
	AssignColor('W', 255, 255, 255); // white
	AssignColor('M', 255, 0, 255); // magenta
	AssignColor('C', 0, 255, 255); // cyan
	AssignColor('Y', 255, 255, 0); // yellow
	AssignColor('S', 192, 192, 192); // silver
	AssignColor('G', 128, 128, 128); // gray
	AssignColor('O', 255, 215, 0); // or
	AssignColor(' ', 255, 255, 255, 0); // transparent
	AssignColor('[', 255, 255, 255, 0); // transparent
	AssignColor(']', 255, 255, 255, 0); // transparent
}

int CORECreateTextureFromRGBA(unsigned char* data, int width, int height)
{
	GLuint t = 0;
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	return t;
}



int GX::initTextureFromString(V2& Size, const std::string& Sprite)
{
	int LargPix = Sprite.find(']') + 1;
	Size.x = LargPix;
	int nb = Sprite.length();
	if (nb % LargPix != 0)
		cout << "Erreur taille texture/string incorrecte";

	int HautPix = nb / LargPix;
	Size.y = HautPix;


	int pos = 0;
	unsigned char data[128 * 128];

	for (int y = HautPix - 1; y >= 0; y--)
		for (int x = 0; x < LargPix; x++)
		{
			int  p = x + y * LargPix;
			char c = Sprite[p];
			for (int t = 0; t < 4; t++)
				data[pos++] = LetterToColor[c][t];
		}

	int IdTexture = CORECreateTextureFromRGBA(data, LargPix, HautPix);
	return IdTexture;
}