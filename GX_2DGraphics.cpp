////////////////////////////////////////////////////////
//
//      	graphiX functions of G2D
//
///////////////////////////////////////////////////////

#include "GX.h"
#include "GlutImport.h"

 

#include <string>
#include <map>
#include <vector>
#include <chrono>




/////////////////////////////////////////////////////////////
//
//	    Window Management
//
/////////////////////////////////////////////////////////////
 

void GX::clearScreen(Color c)
{
	glClearColor(c.R, c.G, c.B, c.A);
	glClear(GL_COLOR_BUFFER_BIT);

}





/////////////////////////////////////////////////////////////
//
//	    RectWithTexture
//
/////////////////////////////////////////////////////////////



void GX::D2_RectWithTexture(int texture, V2 pos, V2 size, float angleDeg)
{
	glLineWidth(0);

	float w = size.x;
	float h = size.y;
	const double MPI = 3.14159265358979323846;
	glColor4ub(0, 0, 0, 128);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);


	//angleDeg = 45;
	float angleRad = angleDeg / 180 * MPI;

	float rX = cos(angleRad) * w - sin(angleRad) * h;
	float rY = sin(angleRad) * w + cos(angleRad) * h;

	float cx = pos.x + w / 2 - rX / 2;
	float cY = pos.y + h / 2 - rY / 2;

	glPushMatrix();
	glTranslatef(cx, cY, 0.0);
	glRotatef(angleDeg, 0.0, 0.0, 1.0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0, 0, 0.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(0, h, 0.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(w, h, 0.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(w, 0, 0.0f);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}



/////////////////////////////////////////////////////////////
//
//	    Geometry
//
/////////////////////////////////////////////////////////////


void GX::D2_Pixel(V2 P,Color c)
{
	glColor4d(c.R, c.G, c.B, c.A);  
	glBegin(GL_POINTS);
	glVertex2i(P.x, P.y); //Set pixel coordinates 
	glEnd();
	glFlush(); //Render pixel
}


void GX::D2_Rectangle(V2 P1, V2 Size, Color c, bool fill)
{
	glLineWidth(1);
	glColor4d(c.R, c.G, c.B, c.A);
	glDisable(GL_TEXTURE_2D);

	if (fill)   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else    	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRecti((int)P1.x, (int)P1.y, (int)(P1.x + Size.x), (int)(P1.y + Size.y));

}

void GX::D2_Circle(V2 C, float r, Color c, bool fill)
{
	vector<V2> LPoints;

	int lineAmount = r / 4; //  nb of triangles used to draw circle
	if (lineAmount < 20) lineAmount = 20;

	const double PI = 3.14159265358;

	//GLfloat radius = 0.8f; //radius
	double step = 2 * PI / lineAmount;

	for (int i = 0; i <= lineAmount; i++)
		LPoints.push_back(V2(C.x + r * cos(i *  step), C.y + r * sin(i * step)));

	GX::D2_Polygon(LPoints, c, fill);
}

void GX::D2_Line(V2 P1, V2 P2, Color c)
{
	glLineWidth(1);
	glColor4d(c.R, c.G, c.B, c.A);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);
	glVertex2f(P1.x, P1.y);
	glVertex2f(P2.x, P2.y);
	glEnd();
}

void GX::D2_Polygon(vector<V2> & PointList, Color c, bool fill)
{

	if (fill) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_TEXTURE_2D);

	glLineWidth(1);
	glColor4d(c.R, c.G, c.B, c.A);

	glBegin(GL_POLYGON);
	for (V2 P : PointList)
		glVertex2f(P.x, P.y);
	glEnd();
}
 