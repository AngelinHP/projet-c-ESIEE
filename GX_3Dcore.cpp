#include "GX.h"
#include "GlutImport.h"


////////////////////////////////////////////////////////
//
//      	graphiX functions of GXD
//
///////////////////////////////////////////////////////

  
#include <string>
#include <map>
#include <vector>
#include <chrono>
#include "GX_GLWrapper.h"

using namespace std;

/////////////////////////////////////////////////////////////
//
//		GL management
//
/////////////////////////////////////////////////////////////

 

/*
void GX::D3_Run3D(float minDist, float maxDist)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  // reset matrix stack


	double aspect = Wwidth;
	aspect /= Wheight;
	double fov = 60;
	gluPerspective(fov, aspect, minDist, maxDist);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	double Zcamera = -200;
	double center = 0;
	double Yup = 1;
	glTranslatef(0, 0, -200);

	glShadeModel(GL_SMOOTH);



	float light_pos[] = { -1.0f, -1.0, -1, 0.0f };


	GLfloat light_ambient[] = { 0.2, 0.2, 0.2 , 1 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8 , 1 };
	GLfloat light_spec[] = { 0.0, 0.0, 0.0 , 1 };


	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// suprimme la light ambiant par défaut
	GLfloat light_amb[] = { 0,0,0,1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_amb);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDisable(GL_LIGHTING);  // XXX
	glDisable(GL_CULL_FACE);
	// glEnable(GL_POLYGON_SMOOTH); à éviter rendu erroné

	glEnable(GL_COLOR_MATERIAL);

	glutMainLoop();
}*/

 
 

/////////////////////////////////////////////////////////////
//
//		Sous repères
//
/////////////////////////////////////////////////////////////


void GX::BeginLocalFrame()                          { glPushMatrix(); }
void GX::EndLocalFrame()                           { glPopMatrix(); }
void GX::Rotate(float angleDeg, V3 axis) { glRotatef(angleDeg, axis.x, axis.y, axis.z); }
void GX::Translate(V3 v)                 { glTranslatef(v.x, v.y, v.z); }
void GX::Scale(V3 zoom)                  { glScalef(zoom.x, zoom.y, zoom.z); }



/////////////////////////////////////////////////////////////
//
//	    RectWithTexture
//
/////////////////////////////////////////////////////////////

/*
const int FLAT = 0;
const int FACETIZED = 1;
const int SHADED = 2;


void GX::D3_Draw(Face& F)
{
	// Illumination
	if (F.RenderMode == FLAT)
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	else
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  // pour multiplier light effect et texture color
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Color or Texture
	if (F.idTexture < 0)
		glDisable(GL_TEXTURE_2D);
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, F.idTexture);
	}



	for (int i = 0; i < 3; i++)
	{
		glBegin(GL_TRIANGLES);

		// couleur
		if (F.idTexture < 0)
		{
			Color Col = F.MyColor;
			glColor4d(Col.R, Col.G, Col.B, Col.A);
		}
		else
			glTexCoord2f(F.u[i].u, F.u[i].v);

		// normal
		if (F.RenderMode != FLAT)
			glNormal3f(F.N[i].x, F.N[i].y, F.N[i].z);

		glVertex3f(F.P[i].x, F.P[i].y, F.P[i].z);

	}
	glEnd();

}

*/

/////////////////////////////////////////////////////////////
//
//	    Border
//
/////////////////////////////////////////////////////////////



void ThickLine(V3 P1, V3 P2, Color c)
{
	glBegin(GL_LINES);
	glColor4d(c.R, c.G, c.B, c.A); glVertex3f(P1.x, P1.y, P1.z);
	glColor4d(c.R, c.G, c.B, c.A);  glVertex3f(P2.x, P2.y, P2.z);
	glEnd();
}

/*
void GX::D3_DrawBorder(V3 A, V3 B, V3 C, V3 D, float thickness, Color EdgeColor)
{

	glDisable(GL_TEXTURE_2D);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glDisable(GL_LIGHTING);
	glLineWidth(thickness);

	Color& c = EdgeColor;

	V3 dec = V3(0, 0, -0.1); // to be closer to camera (depth test)
	ThickLine(A - dec, B - dec, EdgeColor);
	ThickLine(B - dec, C - dec, EdgeColor);
	ThickLine(C - dec, D - dec, EdgeColor);
	ThickLine(D - dec, A - dec, EdgeColor);
	glEnable(GL_LIGHTING);

}

void GX::D3_DrawBorder(V3 A, V3 B, V3 C, float thickness, Color EdgeColor)
{
	glDisable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glDisable(GL_LIGHTING);

	glLineWidth(thickness);
	V3 dec = V3(0, 0, -0.1); // to be closer to camera (depth test)
	ThickLine(A - dec, B - dec, EdgeColor);
	ThickLine(B - dec, C - dec, EdgeColor);
	ThickLine(C - dec, A - dec, EdgeColor);
	glEnable(GL_LIGHTING);
}

*/


/////////////////////////////////////////////////////////////
//
//	    Geometry
//
/////////////////////////////////////////////////////////////



void GX::DrawLine(V3 P1, V3 P2, Color c, float thickness)
{
	GLWrapper::DrawLine(P1, P2, c, thickness);
}

void GX::DrawAxes(float size, float thickness)
{
	GX::DrawLine(V3(0, 0, 0), V3(size, 0, 0), Color::Red, thickness);
	GX::DrawLine(V3(0, 0, 0), V3(0, size, 0), Color::Green, thickness);
	GX::DrawLine(V3(0, 0, 0), V3(0, 0, size), Color::Blue, thickness);
}

/*

void GX::D3_DrawPolygon(vector<V3>& PointList, Color c, bool fill)
{

	if (fill) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_TEXTURE_2D);

	glLineWidth(1);
	glColor4d(c.R, c.G, c.B, c.A);

	glBegin(GL_POLYGON);
	for (V3 P : PointList)
		glVertex3f(P.x, P.y, P.z);
	glEnd();
}
 
 
 */