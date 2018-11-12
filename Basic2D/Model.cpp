///////////////////////////////////////////////////////////////////
//  A basic skeleton for 2D like game developpers.
//
//  model data
///////////////////////////////////////////////////////////////////

#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include "Model.h"
#include "SOIL.h"

// All Setup For OpenGL Goes Here
bool MyModel::InitGL(void)
{
	if (!this->LoadGLTextures())				// Jump To Texture Loading Routine
  {	return false; }							// If Texture Didn't Load Return FALSE


	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
  this->BuildFont();

  // eye    (0,0,0)
  // center (0,0,-1)
  // up     (0,1,0)
  gluLookAt(0.0,0.0,0.0, 0.0,0.0,-1.0, 0.0,1.0,0.0);

  return true;										// Initialization Went OK
}


void MyModel::ReSizeGLScene(int width, int height)
{
	if (height==0) height=1;					// Prevent A Divide By Zero By
	if (width==0) width=1;					// Prevent A Divide By Zero By

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// orthographic projection
  // background in [-1, 1] on both x and y
  // fill th background according to the window aspect ratio
  // void WINAPI glOrtho( GLdouble left,   GLdouble right,
  //                      GLdouble bottom, GLdouble top,
  //                      GLdouble zNear,  GLdouble zFar );
  if( width >= height ) {
    this->plx = 1.0;
    this->ply = (double) height /(double) width;
  } else {
    this->plx = (double) width /(double) height;
    this->ply = 1.0;
  }
  glOrtho(-this->plx, this->plx, -this->ply, this->ply, 1, 5.1);
  
  //  saving the window width and height in pixels
  this->Wheight = height;
  this->Wwidth = width;

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}



// Load Bitmaps And Convert To Textures
bool MyModel::LoadGLTextures(void)
{
	/* load an image file directly as a new OpenGL texture */
	texture[0] = SOIL_load_OGL_texture
		( "../Data/background.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y );

	if(texture[0] == 0) return false;

	/* load an image file directly as a new OpenGL texture */
	texture[1] = SOIL_load_OGL_texture
	("../Data/ship.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	if (texture[1] == 0) return false;

   // Load animation
  //char ll[200];
  //for(int i=0; i < 27; i++) {
  //  sprintf(ll,"../Data/FIRE%02d.TGA",i);
  //  this->texture[i+1] = SOIL_load_OGL_texture (
  //    ll,	SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,	SOIL_FLAG_INVERT_Y);
	 // if(texture[i+1] == 0) return false;
  //}

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	return true;										// Return Success
}



bool MyModel::DrawGLScene(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
  glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();									// Reset The View

  //  TIMING - start
  clock_t t = clock();
  // elapsed time in seconds from the last draw
  double elapsed = double (t - Tstamp) /  (double) CLOCKS_PER_SEC;
  // elapsed time in milliseconds from the last draw
  int ms_elapsed = (int) (t - Tstamp);
  // elapsed time in seconds from the beginning of the program
  this->Full_elapsed = double (t - Tstart) /  (double) CLOCKS_PER_SEC;
  this->frameTime += double (t - Tstamp) /  (double) CLOCKS_PER_SEC;

  this->Tstamp = t;
  //  TIMING - end

  glBindTexture(GL_TEXTURE_2D, texture[0]);

  //  Background
  glBegin(GL_QUADS);
  for(int i = 0; i < 4; i++) {
		glTexCoord2f(Background[i].u, Background[i].v);
    glVertex3f(Background[i].x, Background[i].y,  Background[i].z);
  }
  glEnd();
  
  //  Texture for the ship, change every 1/19 sec.
  //int texF = 1 + ((int( (Full_elapsed*19) )) %27 );
  glBindTexture(GL_TEXTURE_2D, texture[1]);

  //  ship geometrical trasformations
  glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();									// Reset The View

  glScalef(0.20f,0.20f,0.20f);    // 1- scale the ship



  //  ship
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0);

  glBegin(GL_QUADS);
  for(int i = 0; i < 4; i++) {
		glTexCoord2f(ship[i].u, ship[i].v); //cordinate texture da lasciare così
    glVertex3f(ship[i].x, ship[i].y,  ship[i].z); //disegno la ship
  }
  glEnd();

  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);

  //  Floating cursor - start
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0);
  glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
  glLoadIdentity();									// Reset The View

//  Disable texture to see the rectangle size
//  The cursor "hot spot" is the center of the rectangle
// glDisable(GL_TEXTURE_2D);

  glTranslatef(ClientX2World(cx), ClientY2World(cy), 0);				//AGGIORNA MATRICE POSIZIONE
  // proportional scaling (fixed percentual of window dimension)
  // if(1) proportional, if(0) fixed
  if (1) glScalef(0.20f, 0.20f, 0.20f);    // 1- scale the ship
   //  Fixed scaling, alwais 100 pixels width/height
  else {
	  float dx = PixToCoord_X(100);
	  float dy = PixToCoord_Y(100);
	  glScalef(dx / 2, dy / 2, 1);
  }

  glBegin(GL_QUADS);
  for (int i = 0; i < 4; i++) {
	  glTexCoord2f(curs[i].u, curs[i].v);
	  glVertex3f(curs[i].x, ship[i].y, curs[i].z);
  }
  glEnd();

  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  //  Floating cursor - end

  //  Some text
 // glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
 	glLoadIdentity();									// Reset The Current Modelview Matrix
	glDisable(GL_TEXTURE_2D);

 	// Color
	glColor3f(1.0f,1.0f,1.0f);

	// Position The Text On The Screen
  glRasterPos3f(- (float) plx + PixToCoord_X(10), (float) ply-PixToCoord_Y(21),
    -4);

  // compute fps and write text
  this->frames++;
  if( this->frames > 18 ) {
    this->fps = frames / frameTime;
    this->frames = 0; this->frameTime = 0;
  }
  this->glPrint("Elapsed time: %6.2f sec.  -  Fps %6.2f",
    Full_elapsed, fps);

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
  return true;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//  bitmap fonts
void MyModel::BuildFont(void)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	-20,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

void MyModel::KillFont(void)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

void MyModel::glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}