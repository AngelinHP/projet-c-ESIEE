// where all opengl call are performed

# include "GX_Internal.hpp"
#include  "GX_GLWRAPPER.h"
#include  "GlutImport.h"
#include  "GX Objects3D.h"

int Wwidth;
int Wheight;
void (* MyAnim)(int);

// détecte un changement de taille de la fenêtre et remet la taille initiale
void reshape(int w, int h)        
{ 
	Wwidth  = w;
	Wheight = h;
	glutReshapeWindow(Wwidth, Wheight);    
	glViewport(0, 0, Wwidth, Wheight);

}
void reshapeEvent(int w, int h)    { glutReshapeWindow(Wwidth, Wheight);    }

int GLWrapper::getWindowHeight() { return Wheight;  }
int GLWrapper::getWindowWidth()  { return Wwidth;   }

void GLWrapper::initGX(  void Render(), void Anim(int value) )
{
	// init des routines internes
	MyAnim = Anim;

	GXinternal_initKeys();

	// callbacks
	glutReshapeFunc(reshapeEvent);
	glutKeyboardFunc(GXinternal_keyboard);
	glutKeyboardUpFunc(GXinternal_keyUp);
	glutSpecialFunc(GXinternal_keySpecial);
	glutSpecialUpFunc(GXinternal_keySpecialUp);
	glutMotionFunc(GXinternal_mouseMove);
	glutPassiveMotionFunc(GXinternal_mouseMove);
	glutMouseFunc(GXinternal_mouse);

	// logic
		 
	glutDisplayFunc(Render);         // fonction appelée lors d'un repaint
	glutTimerFunc(300, Anim, 0);     // callback de lancement
	glutMainLoop();
}

void GLWrapper::initGLWindow(V2 WindowSize, V2 WindowStartPos, std::string name)
{
	int argcc = 3;

	char  arg0[] = "programName";
	char  arg1[] = "arg";
	char  arg2[] = "another arg";
	char* argc[] = { arg0, arg1, arg2, NULL };

	glutInit(&argcc, argc);

	Wwidth  = WindowSize.x;
	Wheight = WindowSize.y;

	glutInitWindowPosition(WindowStartPos.x, WindowStartPos.y);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(Wwidth, Wheight);
	glutCreateWindow(name.c_str());
}

void GLWrapper::DrawLine(const V3 & P1, const  V3 &P2, const  Color & c, float thickness)
{
	GLboolean textureEnabled = glIsEnabled(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_2D);
	glLineWidth(thickness);
	glColor4d(c.R, c.G, c.B, c.A);
	glBegin(GL_LINES);
	glVertex3f(P1.x, P1.y, P1.z);
	glVertex3f(P2.x, P2.y, P2.z);
	glEnd();
	if (textureEnabled) glEnable(GL_TEXTURE_2D);
}


/*
void GLWrapper::GL_2DMode()
{ 
	glLoadIdentity();
	glOrtho(0, GLWrapper::getWindowWidth(), 0, GLWrapper::getWindowHeight(), -1, 10);    // donne l'echelle x et y
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	// glEnable(GL_POLYGON_SMOOTH); à éviter rendu erroné

	
}*/

void GLWrapper::GL_2DMode()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, GLWrapper::getWindowWidth(), 0, GLWrapper::getWindowHeight(), -1, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLWrapper::BeginUI()
{
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_LINE_BIT | GL_CURRENT_BIT);

	// Projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// Origine en bas à gauche (comme tu veux)
	glOrtho(0, Wwidth, 0, Wheight, -1, 1);

	// ModelView
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Etats UI
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(1.0f);
}
/*
void GLWrapper::EndUI()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();
}*/

void GLWrapper::GL_3DMode(Color Background, GX::Camera Cam, int W, int H)
{
	if ((Wwidth != W) || (Wheight != H))
		reshape(W, H);


	glClearColor(Background.R, Background.G, Background.B, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ---------------- PROJECTION ----------------

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  // reset matrix stack
	double aspect = Wwidth;
	aspect /= Wheight;

	double fov = 60;
	float minDist = 1;
	float maxDist = 100000;
	gluPerspective(fov, aspect, minDist, maxDist);

 

	// ---------------- ETATS OPENGL ----------------
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//  caméra (inversées !)
	glRotatef(-Cam.pitch, 1, 0, 0);
	glRotatef(Cam.yaw, 0, 1, 0);
	glTranslatef(-Cam.pos.x, -Cam.pos.y, -Cam.pos.z);
 
	// LIGHTS
	float light_pos[] = { 1.0f, 1.0, 1, 0.0f };
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5 , 1 };
	GLfloat light_diffuse[] = { 0.5, 0.5, 0.5 , 1 };
	GLfloat light_spec[] = { 0.0, 0.0, 0.0 , 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_NORMALIZE);
 
	// LINE
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1);

	// transparence sur alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 3D
	glEnable(GL_DEPTH_TEST);

	// désactive rejet des faces arrière
	glDisable(GL_CULL_FACE);  
 

	// rendu flat
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_FLAT);
	 
	// éclaire les 2 cotés des faces
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

}

  
void GLWrapper::requestRedraw_Callback()
{
	glutPostRedisplay();
	glutTimerFunc(1, MyAnim, 0);
}

void GLWrapper::SwapBuffer()
{
	glFlush(); // single buffer
	glutSwapBuffers();  // double buffer
}


void GLWrapper::DrawTriangles(const vector<Triangle>& myTriangles,bool flat, bool mesh, bool Normals)
{
	
	// 1) Faces pleines
	if (flat)
	{ 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
		glBegin(GL_TRIANGLES);
		for (const Triangle& T : myTriangles)
		{
			glColor3f(T.c.R, T.c.G, T.c.B);

			glNormal3f(T.N.x, T.N.y, T.N.z);
			glVertex3f(T.A.x, T.A.y, T.A.z);

			glNormal3f(T.N.x, T.N.y, T.N.z);
			glVertex3f(T.B.x, T.B.y, T.B.z);

			glNormal3f(T.N.x, T.N.y, T.N.z);
			glVertex3f(T.C.x, T.C.y, T.C.z);
			 
		}
		glEnd();
	}

	// 2) Contours
	if (mesh)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_LIGHTING);
		glColor3f(0.0f, 0.0f, 0.0f);
		glLineWidth(2.0f);

		glBegin(GL_TRIANGLES);
		for (const Triangle& T : myTriangles)
		{
			glVertex3f(T.A.x, T.A.y, T.A.z);
			glVertex3f(T.B.x, T.B.y, T.B.z);
			glVertex3f(T.C.x, T.C.y, T.C.z);
		}
		glEnd();

		// remettre propre
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
	}

	// normales
	if (Normals)
	{
		int size = 5;
		glDisable(GL_LIGHTING);
		glColor3f(1.0f, 1.0f, 0.0f); // jaune bien visible
		glLineWidth(2.0f);

		glBegin(GL_LINES);


		for (const Triangle& T : myTriangles)
		{
			V3 M = (T.A + T.B + T.C) / 3.0f;   // centre du triangle
			V3 P = M + T.N * size;            // normale (échelle 20)

			glVertex3f(M.x, M.y, M.z);
			glVertex3f(P.x, P.y, P.z);
		}

		glEnd();

		glEnable(GL_LIGHTING);
	}

}

 