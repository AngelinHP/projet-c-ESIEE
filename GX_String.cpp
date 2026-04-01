#include "GX.h"
#include "GlutImport.h"

///////////////////////////////////////////////////////////////
//
//		Draw Text
//
///////////////////////////////////////////////////////////////


void DrawString(V3 pos, string text, float fontSize, float thickness, Color c, bool FontMono)
{
	glColor4f(c.R, c.G, c.B, c.A);

	// EPAISSEUR de la font
	glLineWidth(thickness);

	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	glScalef(1 / 152.38, 1 / 152.38, 1 / 152.38);
	glScalef(fontSize, fontSize, fontSize);
	const char* cc = text.c_str();
	for (char* p = (char*)cc; *p; p++)
	{
		if (FontMono) glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);
		else          glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	}
	glPopMatrix();
	glLineWidth(1);
}

void GX::DrawString3D(V3 pos, string text, float fontSize, float thickness, Color c, bool FontMono)
{
	GLboolean lighting = glIsEnabled(GL_LIGHTING);
	GLboolean textureEnabled = glIsEnabled(GL_TEXTURE_2D);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor4f(c.R, c.G, c.B, c.A);
	glLineWidth(thickness);

	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(fontSize / 152.38f, fontSize / 152.38f, fontSize / 152.38f);

	const char* cc = text.c_str();
	for (const char* p = cc; *p; ++p)
	{
		if (FontMono) glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);
		else          glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	}

	glPopMatrix();

	glLineWidth(1.0f);

	if (lighting) glEnable(GL_LIGHTING);
	if (textureEnabled) glEnable(GL_TEXTURE_2D);
}

void GX::drawStringFontMono(V3 pos, string text, float fontSize, float thickness, Color c)
{
	DrawString(pos, text, fontSize, thickness, c, true);
}

void GX::drawStringFontRoman(V3 pos, string text, float fontSize, float thickness, Color c)
{
	DrawString(pos, text, fontSize, thickness, c, false);
}
