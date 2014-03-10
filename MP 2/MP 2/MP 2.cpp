//  
// Example code for OpenGL programming
//
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <ctime>
#include <SOIL/SOIL.h>
#include <gfx/vec4.h>
#include <gfx/mat4.h>
using namespace gfx;
using namespace std;

float sealevel;

int res = 257;

#define ADDR(i,j,k) (3*((j)*res + (i)) + (k))

GLfloat *verts = 0;
GLfloat *norms = 0;
GLuint *faces = 0;

float frand(float x, float y) {
	static int a = 1588635695, b = 1117695901;
	int xi = *(int *)&x;
	int yi = *(int *)&y;
	srand(((xi * a) % b) - ((yi * b) % a));
	return 2.0*((float)rand() / (float)RAND_MAX) - 1.0;
}

void mountain(int i, int j, int s)
{
	if (s > 1) {

		float x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3, x01, y01, z01, x02, y02, z02, x13, y13, z13, x23, y23, z23, x0123, y0123, z0123;

		x0 = verts[ADDR(i, j, 0)];
		y0 = verts[ADDR(i, j, 1)];
		z0 = verts[ADDR(i, j, 2)];

		x1 = verts[ADDR(i + s, j, 0)];
		y1 = verts[ADDR(i + s, j, 1)];
		z1 = verts[ADDR(i + s, j, 2)];

		x2 = verts[ADDR(i, j + s, 0)];
		y2 = verts[ADDR(i, j + s, 1)];
		z2 = verts[ADDR(i, j + s, 2)];

		x3 = verts[ADDR(i + s, j + s, 0)];
		y3 = verts[ADDR(i + s, j + s, 1)];
		z3 = verts[ADDR(i + s, j + s, 2)];

		x01 = 0.5*(x0 + x1);
		y01 = 0.5*(y0 + y1);
		z01 = 0.5*(z0 + z1);

		x02 = 0.5*(x0 + x2);
		y02 = 0.5*(y0 + y2);
		z02 = 0.5*(z0 + z2);

		x13 = 0.5*(x1 + x3);
		y13 = 0.5*(y1 + y3);
		z13 = 0.5*(z1 + z3);

		x23 = 0.5*(x2 + x3);
		y23 = 0.5*(y2 + y3);
		z23 = 0.5*(z2 + z3);

		x0123 = 0.25*(x0 + x1 + x2 + x3);
		y0123 = 0.25*(y0 + y1 + y2 + y3);
		z0123 = 0.25*(z0 + z1 + z2 + z3);

		z01 += 0.5*((float)s / res)*frand(x01, y01);
		z02 += 0.5*((float)s / res)*frand(x02, y02);
		z13 += 0.5*((float)s / res)*frand(x13, y13);
		z23 += 0.5*((float)s / res)*frand(x23, y23);
		z0123 += 0.5*((float)s / res)*frand(x0123, y0123);

		verts[ADDR(i + s / 2, j, 0)] = x01;
		verts[ADDR(i + s / 2, j, 1)] = y01;
		verts[ADDR(i + s / 2, j, 2)] = z01;

		verts[ADDR(i, j + s / 2, 0)] = x02;
		verts[ADDR(i, j + s / 2, 1)] = y02;
		verts[ADDR(i, j + s / 2, 2)] = z02;

		verts[ADDR(i + s, j + s / 2, 0)] = x13;
		verts[ADDR(i + s, j + s / 2, 1)] = y13;
		verts[ADDR(i + s, j + s / 2, 2)] = z13;

		verts[ADDR(i + s / 2, j + s, 0)] = x23;
		verts[ADDR(i + s / 2, j + s, 1)] = y23;
		verts[ADDR(i + s / 2, j + s, 2)] = z23;

		verts[ADDR(i + s / 2, j + s / 2, 0)] = x0123;
		verts[ADDR(i + s / 2, j + s / 2, 1)] = y0123;
		verts[ADDR(i + s / 2, j + s / 2, 2)] = z0123;

		mountain(i, j, s / 2);
		mountain(i + s / 2, j, s / 2);
		mountain(i, j + s / 2, s / 2);
		mountain(i + s / 2, j + s / 2, s / 2);

	}
	else {

		float dx, dy, dz;

		if (i == 0) {
			dx = verts[ADDR(i + 1, j, 2)] - verts[ADDR(i, j, 2)];
		}
		else if (i == res - 1) {
			dx = verts[ADDR(i, j, 2)] - verts[ADDR(i - 1, j, 2)];
		}
		else {
			dx = (verts[ADDR(i + 1, j, 2)] - verts[ADDR(i - 1, j, 2)]) / 2.0;
		}

		if (j == 0) {
			dy = verts[ADDR(i, j + 1, 2)] - verts[ADDR(i, j, 2)];
		}
		else if (j == res - 1) {
			dy = verts[ADDR(i, j, 2)] - verts[ADDR(i, j - 1, 2)];
		}
		else {
			dy = (verts[ADDR(i, j + 1, 2)] - verts[ADDR(i, j - 1, 2)]) / 2.0;
		}

		dx *= res;
		dy *= res;
		dz = 1.0 / sqrt(dx*dx + dy*dy + 1.0);
		dx *= dz;
		dy *= dz;

		norms[ADDR(i, j, 0)] = dx;
		norms[ADDR(i, j, 1)] = dy;
		norms[ADDR(i, j, 2)] = dz;
	}
}

void makemountain()
{
	int i, j;

	if (verts) free(verts);
	if (norms) free(norms);
	if (faces) free(faces);

	verts = (GLfloat *)malloc(res*res * 3 * sizeof(GLfloat));
	norms = (GLfloat *)malloc(res*res * 3 * sizeof(GLfloat));
	faces = (GLuint *)malloc((res - 1)*(res - 1) * 6 * sizeof(GLuint));

	verts[ADDR(0, 0, 0)] = -5.0;
	verts[ADDR(0, 0, 1)] = -5.0;
	verts[ADDR(0, 0, 2)] = 0.0;

	verts[ADDR(res - 1, 0, 0)] = 5.0;
	verts[ADDR(res - 1, 0, 1)] = -5.0;
	verts[ADDR(res - 1, 0, 2)] = 0.0;

	verts[ADDR(0, res - 1, 0)] = -5.0;
	verts[ADDR(0, res - 1, 1)] = 5.0;
	verts[ADDR(0, res - 1, 2)] = 0.0;

	verts[ADDR(res - 1, res - 1, 0)] = 5.0;
	verts[ADDR(res - 1, res - 1, 1)] = 5.0;
	verts[ADDR(res - 1, res - 1, 2)] = 0.0;

	mountain(0, 0, res - 1);

	GLuint *f = faces;
	for (j = 0; j < res - 1; j++) {
		for (i = 0; i < res - 1; i++) {
			*f++ = j*res + i;
			*f++ = j*res + i + 1;
			*f++ = (j + 1)*res + i + 1;
			*f++ = j*res + i;
			*f++ = (j + 1)*res + i + 1;
			*f++ = (j + 1)*res + i;
		}
	}

}

void init(void)
{
	GLfloat amb[] = { 0.2, 0.2, 0.2 };
	GLfloat diff[] = { 1.0, 1.0, 1.0 };
	GLfloat spec[] = { 1.0, 1.0, 1.0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

	glClearColor(0.5, 0.5, 1.0, 0.0);	// sky
	glEnable(GL_DEPTH_TEST);

	sealevel = 0.0;

	makemountain();
}

float eye[] = {0.0, 0.0, 0.25};
float center[] = {0.5, 0.5, 0.0};
float upV[] = {0.0, 0.0, 1.0};

float speed = 0.0005;

void move() {
	eye[0] += speed * center[0];
	eye[1] += speed * center[1];
	eye[2] += speed * center[2];
}

void display(void)
{
	GLfloat tanamb[] = { 0.2, 0.15, 0.1, 1.0 };
	GLfloat tandiff[] = { 0.4, 0.3, 0.2, 1.0 };
	GLfloat tanspec[] = { 0.0, 0.0, 0.0, 1.0 };	// dirt doesn't glisten

	GLfloat seaamb[] = { 0.0, 0.0, 0.2, 1.0 };
	GLfloat seadiff[] = { 0.0, 0.0, 0.8, 1.0 };
	GLfloat seaspec[] = { 0.5, 0.5, 1.0, 1.0 };	// Single polygon, will only have highlight if light hits a vertex just right

	GLfloat lpos[] = { 0.0, 0.0, 10.0, 0.0 };	// sun, high noon


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();             /* clear the matrix */
	/* viewing transformation, look at the origin  */
	gluLookAt (eye[0], eye[1], eye[2], center[0]+eye[0], center[1]+eye[1], center[2]+eye[2], upV[0], upV[1], upV[2]);
	static GLfloat angle = 0.0;
	//glRotatef(angle, 0.0, 0.0, 1.0);
	angle += 0.1;
	//move();
	// send the light position down as if it was a vertex in world coordinates
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	// load terrain material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tanamb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tandiff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tanspec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);

	// Send terrain mesh through pipeline
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, verts);
	glNormalPointer(GL_FLOAT, 0, norms);
	glDrawElements(GL_TRIANGLES, 6 * (res - 1)*(res - 1), GL_UNSIGNED_INT, faces);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	// load water material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, seaamb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, seadiff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, seaspec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

	// Send water as a single quad
	glNormal3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(-5, -5, sealevel);
	glVertex3f(5, -5, sealevel);
	glVertex3f(5, 5, sealevel);
	glVertex3f(-5, 5, sealevel);
	glEnd();

	glutSwapBuffers();
	glFlush();

	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (float)w / h, 0.01, 10.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '-':
		sealevel -= 0.01;
		break;
	case '+':
	case '=':
		sealevel += 0.01;
		break;
	case 'f':
		res = (res - 1) * 2 + 1;
		makemountain();
		break;
	case 'c':
		res = (res - 1) / 2 + 1;
		makemountain();
		break;
	case 27:
		exit(0);
		break;
	case 'w': // increase speed
		if (speed < 0.005)
			speed += 0.0001;
		break;
	case 's': // lower speed
		if (speed > 0.0005)
			speed -= 0.0001;
		break;
	}
}

void roll_after_pitch(float angle) {
	
	float cross[] = { center[1] * upV[2] - center[2] * upV[1],
		center[2] * upV[0] - center[0] * upV[2],
		center[0] * upV[1] - center[1] * upV[0] };

	float x = upV[0]; // This simplifies the below calcuation (visually at least)
    float y = upV[1];
    float z = upV[2];
    float i = cross[0];
    float j = cross[1];
    float k = cross[2];
    float L = pow(i, 2) + pow(j, 2) + pow(k, 2); // This is the square of the length of the vector
    float sine = sin(angle); // Calculate sine and cosine each only once (more efficient)
    float cosine = cos(angle);

    // Use the formulas from the above web pages to calculate the parameters of the vector we will return
    upV[0] = ((i*((i*x)+(j*y)+(k*z)))*(1-cosine) + (L*x*cosine) + (sqrt(L)*(-k*y+j*z)*sine))/L;
    upV[1] = ((j*((i*x)+(j*y)+(k*z)))*(1-cosine) + (L*y*cosine) + (sqrt(L)*(k*x-i*z)*sine))/L;
    upV[2] = ((k*((i*x)+(j*y)+(k*z)))*(1-cosine) + (L*z*cosine) + (sqrt(L)*(-j*x+i*y)*sine))/L;
}

void pitch(float angle) {

	float cross[] = { center[1] * upV[2] - center[2] * upV[1],
		center[2] * upV[0] - center[0] * upV[2],
		center[0] * upV[1] - center[1] * upV[0] };

	float x = center[0]; // This simplifies the below calcuation (visually at least)
    float y = center[1];
    float z = center[2];
    float i = cross[0];
    float j = cross[1];
    float k = cross[2];
    float L = pow(i, 2) + pow(j, 2) + pow(k, 2); // This is the square of the length of the vector
    float sine = sin(angle); // Calculate sine and cosine each only once (more efficient)
    float cosine = cos(angle);

    // Use the formulas from the above web pages to calculate the parameters of the vector we will return
    center[0] = ((i*((i*x)+(j*y)+(k*z)))*(1-cosine) + (L*x*cosine) + (sqrt(L)*(-k*y+j*z)*sine))/L;
    center[1] = ((j*((i*x)+(j*y)+(k*z)))*(1-cosine) + (L*y*cosine) + (sqrt(L)*(k*x-i*z)*sine))/L;
    center[2] = ((k*((i*x)+(j*y)+(k*z)))*(1-cosine) + (L*z*cosine) + (sqrt(L)*(-j*x+i*y)*sine))/L; 

	roll_after_pitch(angle);
}

void roll(float angle) {

	float x = upV[0]; // This simplifies the below calcuation (visually at least)
    float y = upV[1];
    float z = upV[2];
    float i = center[0];
    float j = center[1];
    float k = center[2];
    float L = pow(i, 2) + pow(j,2) + pow(k, 2); // This is the square of the length of the vector
    float sine = sin(angle); // Calculate sine and cosine each only once (more efficient)
    float cosine = cos(angle);

    // Use the formulas from the above web pages to calculate the parameters of the vector we will return
    upV[0] = ((i*((i*x)+(j*y)+(k*z)))*(1-cosine) + (L*x*cosine) + (sqrt(L)*(-k*y+j*z)*sine))/L;
    upV[1] = ((j*((i*x)+(j*y)+(k*z)))*(1-cosine) + (L*y*cosine) + (sqrt(L)*(k*x-i*z)*sine))/L;
    upV[2] = ((k*((i*x)+(j*y)+(k*z)))*(1-cosine) + (L*z*cosine) + (sqrt(L)*(-j*x+i*y)*sine))/L; 
}

void special(int key, int x, int y) {
	// put your movement key here
	switch (key) {
		case GLUT_KEY_LEFT:
			printf("left is pressed\n");
			roll(-0.01); // LEFT hit, roll left
			break;
		case GLUT_KEY_RIGHT:
			printf("right is pressed\n");
			roll(0.01); // RIGHT hit, roll right
			break;
		case GLUT_KEY_UP:
			printf("up is pressed\n");
			pitch(0.01); // UP hit, pitch up
			break;
		case GLUT_KEY_DOWN:
			printf("down is pressed\n");
			pitch(-0.01); // DOWN hit, pitch down
			break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	//glutCreateWindow (argv[0]);
	glutCreateWindow ((const char*)"Alexander Hadiwijaya: Flight Simulator");
	init();
	glutDisplayFunc(display);  // called when drawing 
	glutReshapeFunc(reshape);  // called when change window size
	glutKeyboardFunc(keyboard); // called when received keyboard interaction
	glutSpecialFunc(special); // called when received special interaction
	glutMainLoop();
	return 0;
}