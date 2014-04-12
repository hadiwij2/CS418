#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

using namespace std;

float nFPS = 40;
float fRotateAngle = 0.f;

vector<vector<float>> v;
vector<vector<float>> f;

vector<float> makeset(ifstream &myfile) {
	string input;
	vector<float> set;
	for (int i = 0; i < 3; i++) {
		myfile >> input;
		set.push_back(atof(input.c_str()));
		cout << input << endl;
	}

	return set;
}

void v_set(vector<float> set) {
	v.push_back(set);
}

void f_set(vector<float> set) {
	f.push_back(set);
}

void teapot() {
	string mark;
	ifstream myfile ("teapot_0.obj");
	
	if (myfile.is_open()) {
		while (!myfile.eof()) {

			myfile >> mark;
			if (mark == "v")
				v_set(makeset(myfile));
			else if (mark == "f")
				f_set(makeset(myfile));
			else
				;
		}
		myfile.close();
	}
}

void init(void)
{
	// init your data, setup OpenGL environment here
	glClearColor(0.9,0.9,0.9,1.0); // clear color is gray		
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment this function if you only want to draw wireframe model
					// GL_POINT / GL_LINE / GL_FILL (default)
	glPointSize(4.0);
	teapot();
}

void display() {
	glEnable(GL_TEXTURE_2D);

	GLfloat lpos[] = {5.0,5.0,5.0,1.0};
	GLfloat La[] = {1.0,1.0,1.0,1.0};
	GLfloat Lid[] = {1.0,1.0,1.0,1.0};
	GLfloat Lis[] = {1.0,1.0,1.0,1.0};

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, La);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lid);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Lis);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// reset OpenGL transformation matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // reset transformation matrix to identity

	// setup look at transformation so that 
	// eye is at : (0,0,3)
	// look at center is at : (0,0,0)
	// up direction is +y axis
	gluLookAt(1.f,4.f,6.f,0.f,1.f,0.f,0.f,1.f,0.f);

	glPushMatrix();
	
	glRotatef(fRotateAngle,0.f,1.f,0.f);

	// Test drawing a solid teapot
	glColor3f(1.0,0.0,0.0); // set current color to Red
	//glBindTexture(GL_TEXTURE_2D, textures);

	glEnable(GL_NORMALIZE);
	glBegin(GL_TRIANGLES);

	for(int i=0; i < f.size(); i++)
	{	
		//glNormal3f(vnorm[f[i][0]-1][0],vnorm[f[i][0]-1][1],vnorm[f[i][0]-1][2]);
		glTexCoord2f(v[f[i][0]-1][0],v[f[i][0]-1][1]);
		glVertex3f(v[f[i][0]-1][0],v[f[i][0]-1][1],v[f[i][0]-1][2]);

		//glNormal3f(vnorm[f[i][1]-1][0],vnorm[f[i][1]-1][1],vnorm[f[i][1]-1][2]);
		glTexCoord2f(v[f[i][1]-1][0],v[f[i][1]-1][1]);
		glVertex3f(v[f[i][1]-1][0],v[f[i][1]-1][1],v[f[i][1]-1][2]);
		
		//glNormal3f(vnorm[f[i][2]-1][0],vnorm[f[i][2]-1][1],vnorm[f[i][2]-1][2]);
		glTexCoord2f(v[f[i][2]-1][0],v[f[i][2]-1][1]);
		glVertex3f(v[f[i][2]-1][0],v[f[i][2]-1][1],v[f[i][2]-1][2]);
	}
	glEnd();
 
	glPopMatrix();
	//glFlush();
	glutSwapBuffers();	// swap front/back frameb1 to avoid flickering
}

void reshape (int w, int h)
{
	// reset viewport ( drawing screen ) size
	glViewport(0, 0, w, h);
	float fAspect = ((float)w)/h; 
	// reset OpenGL projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.f,fAspect,0.001f,30.f); 
}

void keyboard(unsigned char key, int x, int y)
{
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


void timer(int v)
{
	fRotateAngle += 1.f; // change rotation angles
	glutPostRedisplay(); // trigger display function by sending redraw into message queue
	glutTimerFunc(1000/nFPS,timer,v); // restart timer again
}

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv);
	// set up for double-buffering & RGB color buffer & depth test
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ((const char*)"Alexander Hadiwijaya: Teapot Contest");

	init(); // setting up user data & OpenGL environment
	
	// set up the call-back functions 
	glutDisplayFunc(display);  // called when drawing 
	glutReshapeFunc(reshape);  // called when change window size
	glutKeyboardFunc(keyboard); // called when received keyboard interaction
	glutMouseFunc(mouse);	    // called when received mouse interaction
	glutTimerFunc(100,timer,nFPS); // a periodic timer. Usually used for updating animation
	
	//startClock=clock();

	glutMainLoop(); // start the main message-callback loop

	return 0;
}
	