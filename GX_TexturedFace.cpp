#include <string>
#include "GX TexturedFace3D.h"
#include  "GlutImport.h"
#include <string>
#include <algorithm>


int GetTextureIdFromPNG(std::string PNGFileName);
int GetTextureIdFromJPG(std::string JPGFileName);

std::string GetExtension(std::string filename)
{
	return filename.substr(filename.size() - 4);
}

#include <cctype>
static std::string ToLower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return (char)std::tolower(c); });
	return s;
}
static std::string GetExtSafe(const std::string& f) {
	auto s = ToLower(f);
	if (s.size() >= 5 && s.rfind(".jpeg") == s.size() - 5) return ".jpeg";
	if (s.size() >= 4) return s.substr(s.size() - 4); // .jpg / .png
	return "";
}



TexturedFace::TexturedFace(V3 A, V3 B, V3 C, bool reverseN, const std::string& JPGPNGFileName)
{
	P[0] = A;
	P[1] = B;
	P[3] = C;

	// A B C D
	V3 AB = B - A;
	V3 AC = C - A;
	P[2] = A + AB + AC;
	V3 N = prodVect(AB, AC);
	N.normalize();
	if (reverseN) N = -N;

	// --- choix texture
	 
	auto ext = GetExtSafe(JPGPNGFileName);

	if (ext == ".jpg" || ext == ".jpeg")      idTexture = GetTextureIdFromJPG(JPGPNGFileName);
	else if (ext == ".png")                   idTexture = GetTextureIdFromPNG(JPGPNGFileName);

	if (idTexture < 0) // error detected
	        idTexture = GetTextureIdFromPNG(".\\assets\\error.png");
 
}



 
void Drawold()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	const int  GL_CLAMP_TO_EDGE = 0x812F;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, (GLuint) 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Facultatif mais utile contre les franges :
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(255, 255, 255, 255);          // ne pas assombrir la texture
	 
	 

	glBegin(GL_QUADS);
	//glTexCoord2f(0.0f, 0.0f); glVertex3f(A.x, A.y, A.z);
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(B.x, B.y, B.z);
	//glTexCoord2f(1.0f, 1.0f); glVertex3f(D.x, D.y, D.z);
	//glTexCoord2f(1.0f, 0.0f); glVertex3f(C.x, C.y, C.z);
	glEnd();

	glPopMatrix();

	// Nettoyage léger (optionnel)
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glColor4ub(255, 255, 255, 255);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void TexturedFace::Draw(int rot90)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, (GLuint)idTexture);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(255, 255, 255, 255);

	float uv[4][2] = { {0,0}, {0,1}, {1,1}, {1,0} };
	float uv2[4][2] = { {0,0}, {1,0}, {1,1}, {0,1} };


	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++)
	{
		V3 A = P[(i+rot90)%4];
		if (rot90 <4) 		glTexCoord2f(uv[i][0],uv[i][1]);
		else                glTexCoord2f(uv2[i][0], uv2[i][1]);
		glVertex3f(A.x, A.y, A.z);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glColor4ub(255, 255, 255, 255);
}