/* HelloWorld template provided */

//  
// Example code for OpenGL programming
//
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <ctime>
#include <SOIL/SOIL.h>
#include <cmath>

#define PI 3.14159265

int nFPS = 40;
float fRotateAngle = 0.f;
clock_t startClock=0,curClock;
long long int prevF=0,curF=0;
int dipMode=1;

float x[] = {
				-0.6,			0.0,			0.6,
				-0.6,	-0.2,			0.2,	0.6,
				-0.6,	-0.2,			0.2,	0.6,
				-0.6,							0.6
			};
float y[] = {
				1.0,			1.0,			1.0,
				0.6,	0.6,			0.6,	0.6,
				-0.6,	-0.6,			-0.6,	-0.6,
				-1.0,							-1.0
			};
int top = 0;

void init(void)
{
	// init your data, setup OpenGL environment here
	glClearColor(0.9,0.9,0.9,1.0); // clear color is gray		
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment this function if you only want to draw wireframe model
					// GL_POINT / GL_LINE / GL_FILL (default)
	glPointSize(4.0);
}

/* Make all vertices dance */
void dance(float var)
{
	/*--LEFT & RIGHT--*/
	// TOP vertices move according to cos(x) function
	x[0] += cos(var) / 10;
	x[1] += cos(var) / 10;
	x[2] += cos(var) / 10;
	x[3] += cos(var) / 10;
	x[4] += cos(var) / 10;
	x[5] += cos(var) / 10;
	x[6] += cos(var) / 10;

	// BOTTOM vertices move according to cos(PI + x) function
	x[7] += cos(PI + var) / 10;
	x[8] += cos(PI + var) / 10;
	x[9] += cos(PI + var) / 10;
	x[10] += cos(PI + var) / 10;
	x[11] += cos(PI + var) / 10;
	x[12] += cos(PI + var) / 10;
	/*----------------*/

	/*--UP & DOWN--*/
	// RIGHT vertices move according to sin(PI/2 + x) function
	y[2] += sin(PI/2 + var) / 20;
	y[6] += sin(PI/2 + var) / 20;
	y[5] += sin(PI/2 + var) / 20;
	y[9] += sin(PI/2 + var) / 20;
	y[10] += sin(PI/2 + var) / 20;
	y[12] += sin(PI/2 + var) / 20;

	// MIDDLE vertices move according to sin(PI/4 + x) function
	//y[1] += sin(PI/4 + var) / 20;

	// LEFT vertices move according to sin(x) function
	y[0] += sin(var) / 20;
	y[3] += sin(var) / 20;
	y[4] += sin(var) / 20;
	y[8] += sin(var) / 20;
	y[7] += sin(var) / 20;
	y[11] += sin(var) / 20;
	/* ------ */
}

/* Make the top vertices move up and down */
void topdance(int var) {

	var = var % 5;
	y[1] -= var / 100.0;
}
 
void triCombo() {
	/* TOP and MIDDLE */
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x[1], y[1]);	// 0.0, 1.0
	glVertex2f(x[2], y[2]);	// 0.6, 1.0
	glVertex2f(x[6], y[6]);	// 0.6, 0.6
	glVertex2f(x[5], y[5]);	// 0.2, 0.6
	glVertex2f(x[9], y[9]);	// 0.2, -0.6
	glVertex2f(x[8], y[8]);	// -0.2, -0.6
	glVertex2f(x[4], y[4]);	// -0.2, 0.6
	glVertex2f(x[3], y[3]);	// -0.6, 0.6
	glVertex2f(x[0], y[0]);	// -0.6, 1.0
	glEnd();

	/* BOTTOM */
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(x[10], y[10]);	// 0.6, -1.0
	glVertex2f(x[9], y[9]);		// 0.2, -0.6
	glVertex2f(x[12], y[12]);	// 0.6, -1.0
	glVertex2f(x[8], y[8]);		// -0.2, -0.6
	glVertex2f(x[11], y[11]);	// -0.6, -1.0
	glVertex2f(x[7], y[7]);		// -0.6, -0.6
	glEnd();
}

void display(void)
{
	if(dipMode==1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	

	curF++;
	// put your OpenGL display commands here
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// reset OpenGL transformation matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // reset transformation matrix to identity

	// setup look at transformation so that 
	// eye is at : (0,0,3)
	// look at center is at : (0,0,0)
	// up direction is +y axis
	gluLookAt(0.f,0.f,3.f,0.f,0.f,0.f,0.f,1.f,0.f);
	glRotatef(fRotateAngle,1.f,1.f,1.f); // rotate the I letter on x-y-z axis

	// Test drawing a solid teapot

	/* Changing color every second */
	if (curClock % 1000 < 500) {
		glClearColor(0.0, 0.0, 1.0, 1.0); // set current color to Blue
		glColor3f(1.0, 0.5, 0.0); // set current color to Orange
	}
	else {
		glClearColor(1.0, 0.5, 0.0, 1.0); // set current color to Orange
		glColor3f(0.0, 0.0, 1.0); // set current color to Blue
	}
	//glutSolidTeapot(1.f); // call glut utility to draw a solid teapot 

	dance(top); // dance the vertices other than top
	// dance the center-top vertice
	if (curClock % 4000 < 2000)
		topdance(top++);
	else
		topdance(-1 * top++);

	triCombo(); // draw the vertices

	//glFlush();
	glutSwapBuffers();	// swap front/back framebuffer to avoid flickering 

	curClock=clock();
	float elapsed=(curClock-startClock)/(float)CLOCKS_PER_SEC;
	if(elapsed>1.0f){
		float fps=(float)(curF-prevF)/elapsed;
		printf("fps:%f\n",fps);
		prevF=curF;
		startClock=curClock;
	}
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
	if( key == 'h'){
		dipMode = 1-dipMode;
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
	glutCreateWindow ((const char*)"Alexander Hadiwijaya: MP 1");

	init(); // setting up user data & OpenGL environment
	
	// set up the call-back functions 
	glutDisplayFunc(display);  // called when drawing 
	glutReshapeFunc(reshape);  // called when change window size
	glutKeyboardFunc(keyboard); // called when received keyboard interaction
	glutMouseFunc(mouse);	    // called when received mouse interaction
	glutTimerFunc(100,timer,nFPS); // a periodic timer. Usually used for updating animation
	
	startClock=clock();

	glutMainLoop(); // start the main message-callback loop

	return 0;
}
