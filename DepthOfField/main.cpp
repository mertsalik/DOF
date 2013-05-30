#include <windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <iostream>

#define WINDOW_TITLE_PREFIX "Depth Of Field"
#define SKYBOX_COOR 0
#define SKYBOX_LENGTH 12
#define NUM_SHADERS 3
#define SHADERS_DIR "Shaders/"

struct Shaders {
	GLuint vert[NUM_SHADERS], frag[NUM_SHADERS];
    GLuint p[NUM_SHADERS];
};

Shaders shaders;


void IdleFunction(void);
void ResizeFunction(int Width, int Height);

//Light specifications
GLfloat ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat position[] = {0.0f, 0.0f, 12.0f, 1.0f};

GLfloat coor[5][3] = {{-4,-4,4},{-2,-2,2},{0,0,0},{2,2,-2},{4,4,-4}};

unsigned FrameCount = 0;

GLfloat zoom_factor = 45.0;
GLfloat zNear = 0.1;
GLfloat zFar = 40;
int CurrentWidth = 600,
	CurrentHeight = 600;
GLfloat currentCamX = 0;

GLuint boxList;
GLint textureWall, textureBox;
char *textureWallPath = "Textures/wall.jpg", *textureBoxPath = "Textures/box.png";

void generateBoxList()
{
	boxList = glGenLists(1);
	glNewList(boxList, GL_COMPILE);
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1,-1,1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1,1,1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1,1,1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1,-1,1);
	glNormal3f(0,0,-1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1,-1,-1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1,1,-1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1,1,-1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1,-1,-1);

	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1,1,1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1,1,-1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1,1,-1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1,1,1);
	glNormal3f(0,-1,0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1,-1,1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1,-1,-1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1,-1,-1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1,-1,1);

	glNormal3f(1,0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1,1,-1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1,-1,-1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1,-1,1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1,1,1);
	glNormal3f(-1,0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1,1,-1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1,-1,-1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1,-1,1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1,1,1);
	glEnd();
	glEndList();
}

void drawSkybox(float x, float y, float z, float width, float height, float length) {
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;
	glBindTexture(GL_TEXTURE_2D, textureWall);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, y, z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textureWall);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y, z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textureWall);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + width, y + height, z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textureWall);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y + height, z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textureWall);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + width, y, z);
	glEnd();
}

GLint LoadGLTexture(const char *filename)
{
	GLuint _texture;

	_texture = SOIL_load_OGL_texture // load an image file directly as a new OpenGL texture 
		(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_MIPMAPS
		);

	// check for an error during the load process 
	if(_texture == 0)
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}


	glBindTexture(GL_TEXTURE_2D, _texture); // select our current texture
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // texture should tile
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return _texture;
}

static void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(sh.p[0]);
	loc = glGetUniformLocation(sh.p[0], "focalLen");
	glUniform1f(loc, focalLen);
	loc = glGetUniformLocation(sh.p[0], "Zfocus");
	glUniform1f(loc, Zfocus);
	loc = glGetUniformLocation(sh.p[0], "maxCoC");
	glUniform1f(loc, maxCoC);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom_factor, (float) CurrentWidth / (float) CurrentHeight, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	glEnable(GL_TEXTURE_2D);
	glRotatef(currentCamX, 0, 1, 0);

	drawSkybox(SKYBOX_COOR, SKYBOX_COOR, SKYBOX_COOR, SKYBOX_LENGTH, SKYBOX_LENGTH, SKYBOX_LENGTH);
	for (int i = 0; i < 5; i++)
	{
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureBox);
		glTranslatef(coor[i][0], coor[i][1], coor[i][2]);
		glCallList(boxList);
		glPopMatrix();
	}

	glutSwapBuffers();
	FrameCount++;
}

static void Key(unsigned char key, int x, int y)
{
	printf("Key: %d\n", key);
	switch (key) {
	case 27: // ESC
		exit(0);
		break;
	case 'a':
		currentCamX -= 1.0f;
		break;
	case 'd':
		currentCamX += 1.0f;
		break;
	}
}

static void timerCallback (int value)
{
	if (0 != value) {
		char* TempString = (char*)
			malloc(512 + strlen(WINDOW_TITLE_PREFIX));

		sprintf(
			TempString,
			"%s: %d Frames Per Second @ %d x %d",
			WINDOW_TITLE_PREFIX,
			FrameCount * 4,
			CurrentWidth,
			CurrentHeight
			);

		glutSetWindowTitle(TempString);
		free(TempString);
	}
	FrameCount = 0;
	glutTimerFunc(250, timerCallback, 1);
}

void IdleFunction(void)
{
	glutPostRedisplay();
}

void InitShaders(struct Shaders *sh)
{
	int i;
	char *vs, *fs, *filename;
	const char *vv, *ff;
	const char *vfiles[NUM_SHADERS] = {"first.vert", "second.vert", "second.vert"};
	const char *ffiles[NUM_SHADERS] = {"first.frag", "second.frag", "renderblur.frag"};

	for (i = 0; i < NUM_SHADERS; i++) {

		sh->p[i] = glCreateProgram();

		filename = (char *)malloc(sizeof(char) * (strlen(SHADERS_DIR) + strlen(vfiles[i])));
		strcpy(filename, SHADERS_DIR);
		strcat(filename, vfiles[i]);

		sh->vert[i] = glCreateShader(GL_VERTEX_SHADER);
		vs = textFileRead(filename);
		printf("Vert[%d]: %s\n", i, filename);
		free(filename);
		vv = vs;
		glShaderSource(sh->vert[i], 1, &vv, NULL);
		free(vs);
		glCompileShader(sh->vert[i]);
		glAttachShader(sh->p[i], sh->vert[i]);

		filename = (char *)malloc(sizeof(char) * (strlen(SHADERS_DIR) + strlen(ffiles[i])));
		strcpy(filename, SHADERS_DIR);
		strcat(filename, ffiles[i]);

		sh->frag[i] = glCreateShader(GL_FRAGMENT_SHADER);
		fs = textFileRead(filename);
		printf("Frag[%d]: %s\n", i, filename);
		free(filename);
		ff = fs;
		glShaderSource(sh->frag[i], 1, &ff, NULL);
		free(fs);
		glCompileShader(sh->frag[i]);
		glAttachShader(sh->p[i], sh->frag[i]);

		glLinkProgram(sh->p[i]);
		printObjectInfoLog(sh->p[i]);
	}

	printf("\n");
}


void DeleteShaders(struct Shaders *sh)
{
	int i;

	for (i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(sh->p[i], sh->frag[i]);
		glDeleteShader(sh->frag[i]);
		glDetachShader(sh->p[i], sh->vert[i]);
		glDeleteShader(sh->vert[i]);
		glDeleteProgram(sh->p[i]);
	}
}

int main (int argc, char ** argv)
{
	GLenum type;

	glutInit(&argc, argv);
	glutInitWindowSize(CurrentWidth,CurrentHeight);
	type = GLUT_RGB;
	type |= GLUT_DOUBLE;
	type |= GLUT_DEPTH;
	type |= GLUT_MULTISAMPLE;
	glutInitDisplayMode(type);
	glutCreateWindow("");
#ifdef _WIN32
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	printf("GLSL Version: %s\n",glslVersion);

	const GLubyte* glVersion = glGetString(GL_VERSION);

	printf("GL Version: %s\n",glVersion);

	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	InitShaders(&shaders);
	glUseProgram(shaders.p[1]);
	loc = glGetUniformLocation(shaders.p[1], "Width");
	glUniform1i(loc, width);
	loc = glGetUniformLocation(shaders.p[1], "Height");
	glUniform1i(loc, height);
	loc = glGetUniformLocation(shaders.p[1], "Tex0");
	glUniform1i(loc, 0);
	loc = glGetUniformLocation(shaders.p[1], "Tex1");
	glUniform1i(loc, 1);
	glUseProgram(shaders.p[2]);
	loc = glGetUniformLocation(shaders.p[2], "Tex1");
	glUniform1i(loc, 1);
	
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	textureBox = LoadGLTexture(textureBoxPath);
	textureWall = LoadGLTexture(textureWallPath);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	generateBoxList();

	timerCallback(0);
	glutDisplayFunc(Draw);
	glutKeyboardFunc(Key);
	glutIdleFunc(IdleFunction);
	glutMainLoop();

	return 0;
}


