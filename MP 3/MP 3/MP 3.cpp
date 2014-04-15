//  
// Example code for OpenGL programming
//
#include <stdio.h>
#include <stdlib.h>
//these libraries might be different for your platform
//Ex. on mac they should be GLEW/glew.h and GLUT/glut.h
#include <GL/glew.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;

int nmyfileS = 60;
float fRotateAngle = 0.f;
float randomVar = 1;
float heyThisGetsSentToAShader = 0;

GLuint program;
char* vertex_shader_code = "\
	uniform float thisCameFromC;\
	vec4 p;\
	void main(void){\
		p = gl_Vertex;\
		p.x = p.x + sin(thisCameFromC)*sin(p.y);\
		p.y = p.y + cos(thisCameFromC)*sin(p.x);\
		p.z = sin(5.0*p.x+thisCameFromC )*0.25;\
		gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * p;\
	}";
char* fragment_shader_code = "\
	void main(void) {\
	    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\
	}";

void printShaderInfoLog(GLuint obj);
GLint uniformLocation;

int vC = 0;
int fC = 0;
float vN[1202][3];
float fN[2256][3];
float v[1202][3];
int f[2256][3];
char b1[100];

char rotation = 0;
float rot = 0.0;
vector<string> flib;
int fcode = 0;
GLfloat lpos[] = { 5.0, 5.0, 5.0, 1.0 };

// load teapot
void teapot() {
	FILE *myfile;
	myfile = fopen("teapot_0.obj", "r");

	char *token;
	while (!feof(myfile)) {
		fgets(b1, 100, myfile);
		token = strtok(b1, " ");
		if (strcmp(token, "v") == 0) {
			for (int i = 0; i < 3; i++) {
				token = strtok(NULL, " ");
				v[vC][i] = atof(token);
			}
			vC++;
		}

		if (strcmp(token, "f") == 0) {
			char *f0 = strtok(NULL, " ");
			char *f1 = strtok(NULL, " ");
			char *f2 = strtok(NULL, " ");

			if (f0 == NULL && f1 == NULL && f2 == NULL)
				break;

			f[fC][0] = atoi(f0);
			f[fC][1] = atoi(f1);
			f[fC][2] = atoi(f2);
			fC++;
		}
	}

	fclose(myfile);
}
// vertices and faces normal
void initializeVF() {
	for (int i = 0; i < vC; i++) {
		for (int j = 0; j < 3; j++) {
			vN[i][j] = 0;
		}
	}

	for (int i = 0; i < fC; i++) {
		fN[i][0] = (((v[f[i][1] - 1][1] - v[f[i][0] - 1][1]) * (v[f[i][2] - 1][2] - v[f[i][0] - 1][2])) - ((v[f[i][1] - 1][2] - v[f[i][0] - 1][2]) * (v[f[i][2] - 1][1] - v[f[i][0] - 1][1])));
		fN[i][1] = (((v[f[i][1] - 1][2] - v[f[i][0] - 1][2]) * (v[f[i][2] - 1][0] - v[f[i][0] - 1][0])) - ((v[f[i][1] - 1][0] - v[f[i][0] - 1][0]) * (v[f[i][2] - 1][2] - v[f[i][0] - 1][2])));
		fN[i][2] = (((v[f[i][1] - 1][0] - v[f[i][0] - 1][0]) * (v[f[i][2] - 1][1] - v[f[i][0] - 1][1])) - ((v[f[i][1] - 1][1] - v[f[i][0] - 1][1]) * (v[f[i][2] - 1][0] - v[f[i][0] - 1][0])));
	}

	for (int i = 0; i < fC; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				vN[f[i][j] - 1][k] += fN[i][k];
			}
		}
	}
}
// update normal
void updateVN() {
	for (int i = 0; i < fC; i++) {
		for (int j = 0; j < 3; j++) {
			glNormal3f(vN[f[i][j] - 1][0], vN[f[i][j] - 1][1], vN[f[i][j] - 1][2]);
			glTexCoord2f(v[f[i][j] - 1][0], v[f[i][j] - 1][1]);
			glVertex3f(v[f[i][j] - 1][0], v[f[i][j] - 1][1], v[f[i][j] - 1][2]);
		}
	}
}

// insert all image files
void textureFiles() {
	flib.push_back("clamp.png"); // 0
	flib.push_back("cubic.png"); // 1
	flib.push_back("flower2.png"); // 2
	flib.push_back("sciencelake.jpg"); // 3
	flib.push_back("dragon.jpg"); // 4
	flib.push_back("sunnyday.jpg"); // 5
	flib.push_back("sunset.jpg"); // 6
}


// reload Texture when called
void changeTexture() {
	/* load an image file directly as a new OpenGL texture */
	GLuint textures;
	glGenTextures(1, &textures);

	int width, height;
	string temp = flib[fcode];
	char *filename = new char[temp.size() + 1];
	strcpy(filename, temp.c_str());

	unsigned char* img = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);

	glBindTexture(GL_TEXTURE_2D, textures);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glEnable(GL_TEXTURE_2D);
}

void init(void)
{
	// init your data, setup OpenGL environment here
	glClearColor(0.3, 0.3, 0.3, 1.0); // clear color is gray
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment this function if you only want to draw wireframe model

	GLfloat amb[] = { 0.2, 0.2, 0.2 };
	GLfloat diff[] = { 1.0, 1.0, 1.0 };
	GLfloat spec[] = { 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

	teapot(); // load teapot
	textureFiles();
	changeTexture(); // texture mapping
	initializeVF(); // normal coordinates

	glEnable(GL_DEPTH_TEST);

}

void display(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	// put your OpenGL display commands here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	glUniform1f(uniformLocation, heyThisGetsSentToAShader);

	// reset OpenGL transformation matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // reset transformation matrix to identity

	// setup look at transformation so that 
	// eye is at : (0,0,3)
	// look at center is at : (0,0,0)
	// up direction is +y axis
	//gluLookAt(0.f,0.f,6.f,0.f,0.f,0.f,0.f,1.f,0.f);
	gluLookAt(6.f * sin(rot), 4.f + cos(rot), 5.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f);

	if (!rotation) {
		rot += 0.005;
	}

	//glScalef(2,1,0);
	//glTranslatef(2,-2,1);
	//
	/* =====================================================================
	glPushMatrix();
	glRotatef(fRotateAngle,0.f,0.f,1.f);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glTranslatef(2.0, 2.0, 0.0);
	glScalef(1.0, 1.5, 1.0);
	glRotatef(fRotateAngle*3,0.f,0.f,1.f);

	// Test drawing a solid teapot
	glColor3f(1.0,0.0,0.0); // set current color to Red
	glutSolidTeapot(1.f); // call glut utility to draw a solid teapot
	glPopMatrix();


	glPushMatrix();
	//glScalef(1,1,randomVar);
	//glScalef(2,2,2);
	glScalef(randomVar,randomVar,randomVar);

	glColor3f(1.0,1.0,0.0); // set current color to Red
	glutSolidTeapot(1.f); // call glut utility to draw a solid teapot

	glPopMatrix();
	======================================================================= */

	glPushMatrix();
	glRotatef(fRotateAngle, 0.f, 1.f, 0.f);

	glColor3f(0.0, 1.0, 0.0);

	glEnable(GL_NORMALIZE);
	glBegin(GL_TRIANGLES);

	updateVN();
	glEnd();

	glPopMatrix();
	//glFlush();
	glutSwapBuffers();	// swap front/back frameb1 to avoid flickering 
}

void reshape(int w, int h)
{
	// reset viewport ( drawing screen ) size
	glViewport(0, 0, w, h);
	float fAspect = ((float)w) / h;
	// reset OpenGL projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.f, fAspect, 0.001f, 30.f);
}



void keyboard(unsigned char key, int x, int y)
{
	printf(" Key has been pressed: %c\n", key);
	if (key == 'w') {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment this function if you only want to draw wireframe model
	}
	else if (key == 'f') {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // uncomment this function if you only want to draw wireframe model
	}
	else if (key == 'p') {
		rotation = !rotation; // Pause / unpause object movement
	}
	else if (key == 'z') { // all light
		lpos[0] = 5.0;
		lpos[1] = 5.0;
		lpos[2] = 5.0;
		lpos[3] = 1.0;
	}
	else if (key == 'x') { // top
		lpos[0] = 0.0;
		lpos[1] = 5.0;
		lpos[2] = 0.0;
		lpos[3] = 1.0;
	}
	else if (key == 'c') { // bottom
		lpos[0] = 0.0;
		lpos[1] = -6.0;
		lpos[2] = 0.0;
		lpos[3] = 1.0;
	}
	else if (key == 'v') { // left
		lpos[0] = -8.0;
		lpos[1] = 0.0;
		lpos[2] = 0.0;
		lpos[3] = 1.0;
	}
	else if (key == 'b') { // right
		lpos[0] = 8.0;
		lpos[1] = 0.0;
		lpos[2] = 0.0;
		lpos[3] = 1.0;
	}
	else if (key == 'n') { // front
		lpos[0] = 0.0;
		lpos[1] = 0.0;
		lpos[2] = 4.0;
		lpos[3] = 1.0;
	}
	else if (key == 'm') { // back
		lpos[0] = 0.0;
		lpos[1] = 0.0;
		lpos[2] = -3.0;
		lpos[3] = 1.0;
	}
	// put your keyboard control here
	if (key == 27)
	{
		// ESC hit, so quit
		printf("demonstration finished.\n");
		exit(0);
	}
}

void mouse(int button, int state, int x, int y)
{
	// process your mouse control here
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		printf("push left mouse button.\n");
}
void motion(int x, int y)
{
	//printf("mouse has been moved at %d,%d\n", x, y);
	randomVar = (float)x / 30.f;

}

void special(int key, int x, int y) {
	// put your movement key here
	switch (key) {
	case GLUT_KEY_LEFT:
		printf("left is pressed\n");
		if (fcode == 0)
			fcode = flib.size()-1;
		else
			fcode--;
		changeTexture();
		break;
	case GLUT_KEY_RIGHT:
		printf("right is pressed\n");
		if (fcode == flib.size()-1)
			fcode = 0;
		else
			fcode++;
		changeTexture();
		break;
	case GLUT_KEY_UP:
		printf("up is pressed\n");
		break;
	case GLUT_KEY_DOWN:
		printf("down is pressed\n");
		break;
	}
}


void timer(int v)
{
	fRotateAngle += 1.f; // change rotation angles
	glutTimerFunc(1000 / nmyfileS, timer, v); // restart timer again
	heyThisGetsSentToAShader += 0.01f;
	//glutTimerFunc(0,timer,v); // restart timer again
	glutPostRedisplay(); // trigger display function by sending redraw into message queue
}

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv);
	// set up for double-buffering & RGB color buffer & depth test
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow((const char*)"Alexander Hadiwijaya: Teapot Contest");

	/*******************************/
	/*DELETE glewInit if you are using xcode
	*/
	/*******************************/
	//glewInit();




	init(); // setting up user data & OpenGL environment

	// set up the call-back functions 
	glutDisplayFunc(display);  // called when drawing 
	glutReshapeFunc(reshape);  // called when change window size
	glutKeyboardFunc(keyboard); // called when received keyboard interaction
	glutMouseFunc(mouse);	    // called when received mouse interaction
	glutMotionFunc(motion);	    // called when received mouse interaction
	glutSpecialFunc(special); // called when received special interaction
	glutTimerFunc(100, timer, nmyfileS); // a periodic timer. Usually used for updating animation

	glutMainLoop(); // start the main message-callback loop

	return 0;
}
