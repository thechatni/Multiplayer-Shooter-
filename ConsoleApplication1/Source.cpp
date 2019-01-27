/*********
CTIS164 - Template Source Program
----------
STUDENT : Fahad Ahmad
SECTION : 164 -2
HOMEWORK: 3
----------
PROBLEMS: I was not able to display an initial screen asking the user to enter choice of number of players. Similarly there is no screen indicating the winner of the game at th end. To test the program, the value of variable players can be changed to either 1 or 2.
----------
ADDITIONAL FEATURES:
choice of number of players.
in 2 player mode the second cannon is moved by up and down arrow key and spacebar to shoot.
*********/

#include <glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD    15 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

bool start = 0, start2 = 0;

typedef struct {
	int Rx, Ry;
}coordinates;

bool end = true;
int radius[3] = { 150,200,250 };
int radius2[3] = { 100,150,200 };
typedef struct {
	coordinates c;
	int radius;
	int r, g, b;
	float angle;
	int speed;
	int move;
	bool present;
}ball_s;

typedef struct {
	coordinates pos;
	float angle;
}can;

float angle1;
float angle2 = 0;
ball_s balls[3];
ball_s ball1[3];
ball_s ball2[3];
coordinates p = { 0,0 };
coordinates p1 = { -300,0 };
coordinates p2 = { 300,0 };
can c;
can c2;
can bull;
can bullet1;
can bullet2;
int players = 2;
//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}


void vertex(coordinates P, coordinates Tr, double angle)
{
	double xp = (P.Rx * cos(angle) - P.Ry * sin(angle)) + Tr.Rx;
	double yp = (P.Rx * sin(angle) + P.Ry * cos(angle)) + Tr.Ry;
	glVertex2d(xp, yp);
}
void bullet()
{
	glColor3f(1, 1, 1);
	switch (players)
	{
	case 1:
		circle(bull.pos.Rx, bull.pos.Ry, 6);
		break;

	case 2:
		circle(bullet1.pos.Rx, bullet1.pos.Ry, 6);
		circle(bullet2.pos.Rx, bullet2.pos.Ry, 6);
		break;

	}
}
void cannon()
{
	switch (players)
	{
	case 1:
		angle1 = c.angle * D2R;
		glColor3ub(206, 147, 76);
		glBegin(GL_QUADS);
		vertex({ 50, 20 }, { 0,0 }, angle1);
		vertex({ 50, -20 }, { 0,0 }, angle1);
		vertex({ -25, -10 }, { 0,0 }, angle1);
		vertex({ -25, 10 }, { 0,0 }, angle1);
		glEnd();

		glLineWidth(5);
		glColor3ub(122, 120, 117);
		glBegin(GL_LINES);
		vertex({ 50, 25 }, { 0,0 }, angle1);
		vertex({ 50, -25 }, { 0,0 }, angle1);
		glEnd();

		glColor3ub(122, 120, 117);
		circle(c.pos.Rx, c.pos.Ry - 10, 15);
		break;

	case 2:
		angle1 = c.angle * D2R; //cannon for player 1 
		glColor3ub(206, 147, 76);
		glBegin(GL_QUADS);
		vertex({ 50, 20 }, { p1 }, angle1);
		vertex({ 50, -20 }, { p1 }, angle1);
		vertex({ -25, -10 }, { p1 }, angle1);
		vertex({ -25, 10 }, { p1 }, angle1);
		glEnd();

		glLineWidth(5);
		glColor3ub(122, 120, 117);
		glBegin(GL_LINES);
		vertex({ 50, 25 }, { p1 }, angle1);
		vertex({ 50, -25 }, { p1 }, angle1);
		glEnd();

		glColor3ub(122, 120, 117);
		circle(p1.Rx, p1.Ry - 10, 15);

		//////////////////////////////////////////////

		//cannon for player 2
		glColor3ub(69, 173, 148);
		glBegin(GL_QUADS);
		vertex({ 50, 20 }, { p2 }, angle2);
		vertex({ 50, -20 }, { p2 }, angle2);
		vertex({ -25, -10 }, { p2 }, angle2);
		vertex({ -25, 10 }, { p2 }, angle2);
		glEnd();

		glLineWidth(5);
		glColor3ub(43, 42, 42);
		glBegin(GL_LINES);
		vertex({ 50, 25 }, { p2 }, angle2);
		vertex({ 50, -25 }, { p2 }, angle2);
		glEnd();

		circle(p2.Rx, p2.Ry - 10, 15);
		break;

	}
}

void initializeAll()
{

	switch (players)
	{
	case 1:
		for (int i = 0; i < 3; i++)
		{
			balls[i].r = rand() % 256; //if number of players is 1 
			balls[i].g = rand() % 256;
			balls[i].b = rand() % 256;
			balls[i].radius = rand() % 20 + 10;
			balls[i].move = rand() % 2;
			balls[i].angle = rand() % 361;
			balls[i].speed = rand() % 2 + 1;
			balls[i].present = 1;
		}
		bull.pos.Rx = 0;
		bull.pos.Ry = 0;
		break;
		///////////////////////////
	case 2:
		for (int i = 0; i < 3; i++)
		{
			ball1[i].r = rand() % 256; //for player 1 
			ball1[i].g = rand() % 256;
			ball1[i].b = rand() % 256;
			ball1[i].radius = rand() % 20 + 10;
			ball1[i].move = rand() % 2;
			ball1[i].angle = rand() % 361;
			ball1[i].speed = rand() % 2 + 1;
			ball1[i].present = 1;

			ball2[i].r = rand() % 256; //for player 2 
			ball2[i].g = rand() % 256;
			ball2[i].b = rand() % 256;
			ball2[i].radius = rand() % 20 + 10;
			ball2[i].move = rand() % 2;
			ball2[i].angle = rand() % 361;
			ball2[i].speed = rand() % 2 + 1;
			ball2[i].present = 1;
		}
		bullet2.pos.Rx = p2.Rx;
		bullet2.pos.Ry = p2.Ry;
		bullet1.pos.Rx = p1.Rx;
		bullet1.pos.Ry = p1.Ry;

		break;
	}

}
void targets()
{


	switch (players)
	{
	case 1:
		for (int k = 0; k < 3; k++)
		{
			if (balls[k].present == 1)
			{
				balls[k].c.Rx = radius[k] * cos(balls[k].angle*D2R);

				balls[k].c.Ry = radius[k] * sin(balls[k].angle*D2R);

				glColor3ub(balls[k].r, balls[k].g, balls[k].b);
				circle(balls[k].c.Rx, balls[k].c.Ry, balls[k].radius);

				if (balls[k].angle < 0)
					balls[k].angle += 360;
			}
		}
		break;
	case 2:
		for (int j = 0; j < 3; j++)
		{
			if (ball1[j].present == 1)
			{
				ball1[j].c.Rx = radius2[j] * cos(ball1[j].angle*D2R);
				ball1[j].c.Rx += p1.Rx;
				ball1[j].c.Ry = radius2[j] * sin(ball1[j].angle*D2R);
				glColor3ub(ball1[j].r, ball1[j].g, ball1[j].b);
				circle(ball1[j].c.Rx, ball1[j].c.Ry, ball1[j].radius);
				if (ball1[j].angle < 0)
					ball1[j].angle += 360;
			}

			//////////////////////////////////////

			if (ball2[j].present == 1)
			{
				ball2[j].c.Rx = radius2[j] * cos(ball2[j].angle*D2R);
				ball2[j].c.Rx += p2.Rx;
				ball2[j].c.Ry = radius2[j] * sin(ball2[j].angle*D2R);
				//ball2[j].c.Ry += p2.Ry;
				glColor3ub(ball2[j].r, ball2[j].g, ball2[j].b);
				circle(ball2[j].c.Rx, ball2[j].c.Ry, ball2[j].radius);
				glColor3f(1, 1, 1);
				if (ball2[j].angle < 0)
					ball2[j].angle += 360;
			}
		}
		break;

	}
}
void displayBackground()
{
	switch (players)
	{
	case 1:
		glColor3f(1, 0.3, 0.8);
		glLineWidth(2);
		glBegin(GL_LINES);	//drawing x and y axes
		glVertex2f(0, 300);
		glVertex2f(0, -300);
		glVertex2f(300, 0);
		glVertex2f(-300, 0);
		glEnd();

		circle_wire(0, 0, 150); //drawing rings
		circle_wire(0, 0, 200);
		circle_wire(0, 0, 250);
		break;

	case 2:
		glLineWidth(2);
		glColor3ub(69, 123, 173);
		glBegin(GL_LINES); //drawing x and y axes (left)
		glVertex2f(-550, 0);
		glVertex2f(-50, 0);
		glVertex2f(-300, 250);
		glVertex2f(-300, -250);
		glEnd();

		circle_wire(-300, 0, 100); //drawing circles (left)
		circle_wire(-300, 0, 150);
		circle_wire(-300, 0, 200);
		vprint(-330, 270, GLUT_BITMAP_9_BY_15, "PLAYER 1");
		////////////////////////////
		glColor3ub(219, 89, 132);//drawing x and y axes (right)
		glBegin(GL_LINES);
		glVertex2f(50, 0);
		glVertex2f(550, 0);
		glVertex2f(300, 250);
		glVertex2f(300, -250);
		glEnd();

		circle_wire(300, 0, 100);//drawing circles (right)
		circle_wire(300, 0, 150);
		circle_wire(300, 0, 200);

		vprint(270, 270, GLUT_BITMAP_9_BY_15, "PLAYER 2");
	}
}
//
// To display onto window using OpenGL commands
//
bool testCollision(can fr, ball_s t)
{
	float dx = t.c.Rx - fr.pos.Rx;
	float dy = t.c.Ry - fr.pos.Ry;
	float d = sqrt(dx*dx + dy*dy);
	return d <= t.radius;
}
void displayAngle()
{
	float disp;
	glColor3f(1, 1, 1);
	switch (players)
	{
	case 1:
		for (int h = 0; h < 3; h++)
			vprint(balls[h].c.Rx - 8, balls[h].c.Ry, GLUT_BITMAP_8_BY_13, "%0.0f", balls[h].angle);
		vprint(285 * cos(c.angle*D2R), 285 * sin(c.angle*D2R), GLUT_BITMAP_8_BY_13, "%0.0f", c.angle);
		break;
	case 2:
		for (int h = 0; h < 3; h++)
		{
			vprint(ball1[h].c.Rx - 8, ball1[h].c.Ry, GLUT_BITMAP_8_BY_13, "%0.0f", ball1[h].angle);
			vprint(ball2[h].c.Rx - 8, ball2[h].c.Ry, GLUT_BITMAP_8_BY_13, "%0.0f", ball2[h].angle);
		}
		vprint(p1.Rx + 220 * cos(c.angle*D2R), p1.Ry + 220 * sin(c.angle*D2R), GLUT_BITMAP_8_BY_13, "%0.0f", c.angle);
		disp = c2.angle * 180 / PI;
		if (disp > 360)
			disp -= 360;
		else if (disp < -360)
			disp += 360;
		vprint(p2.Rx + 220 * cos(c2.angle), p2.Ry + 220 * sin(c2.angle), GLUT_BITMAP_8_BY_13, "%0.0f", disp);
		break;
	}
}
void start1() // screen meant to be shown at entrance
{
	glColor3ub(232, 229, 76);
	glRectf(-200, 0, 200, 100);
	glRectf(-200, -50, 200, -150);
	vprint2(-270, 200, 0.4, "THE ULTIMATE RACE");

	glColor3f(0, 0, 0);
	vprint(-30, 50, GLUT_BITMAP_9_BY_15, "1 PLAYER");
	vprint(-30, -100, GLUT_BITMAP_9_BY_15, "2 PLAYERS");


}
void display() {
	//
	// clear window to black
	//

	glClearColor(0.3, 0.3, 0.3, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	if (end == false)
	{

		displayBackground();
		bullet();
		targets();
		cannon();
		displayAngle();
	}
	else vprint(-100, 0, GLUT_BITMAP_9_BY_15, "CLICK ANYWHERE TO START");

	glutSwapBuffers();

}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//

void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	if (key == 32)
	{
		if (start2 == 0)
		{
			bullet2.angle = c2.angle;
		}

		if (end == true)
		{
			start2 = 0;
			end = false;
		}
		else start2 = 1;
	}
	//////////////////////




	// to refresh the window it calls display() function
	glutPostRedisplay();
}
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	// exit when ESC is pressed.

	if (left == true)
	{
		angle2 += 0.1;
		c2.angle = angle2;

	}
	else if (right == true)
	{
		angle2 -= 0.1;
		c2.angle = angle2;
	}

	//c2.angle = angle2;
	// to refresh the window it calls display() function
	glutPostRedisplay();
}



//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.

	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
	{
		if (start == 0)
		{
			bull.angle = c.angle;
			bullet1.angle = c.angle;

		}

		if (end == true)
		{
			start = 0;
			end = false;

		}
		else start = 1;
	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMove(int x, int y) {
	// Write your codes here.

	int x2 = x - winWidth / 2;
	int y2 = winHeight / 2 - y;
	switch (players)
	{
	case 1:

		angle1 = (atan2f(y2, x2)) * (180 / PI);
		if (angle1 < 0)
			angle1 += 360;
		c.angle = angle1;
		break;

	case 2:

		x2 -= p1.Rx;
		angle1 = (atan2f(y2, x2)) * (180 / PI);
		if (angle1 < 0)
			angle1 += 360;
		c.angle = angle1;
		break;
	}
	// to refresh the window it calls display() function
	glutPostRedisplay();

}
void onMoveDown(int x, int y) {
	// Write your codes here.


	// to refresh the window it calls display() function   
	glutPostRedisplay();
}


#if TIMER_ON == 1
void onTimer(int v) {


	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	switch (players)
	{
	case 1:
		for (int i = 0; i < 3; i++)
		{
			if (balls[i].present == 1)
			{
				if (balls[i].move == 1)
					balls[i].angle += balls[i].speed;
				else
					balls[i].angle -= balls[i].speed;

				if (balls[i].angle > 360)
					balls[i].angle -= 360;
				else if (balls[i].angle < -360)
					balls[i].angle += 360;
			}
			if (testCollision(bull, balls[i]) == 1 && start == 1 && end == false)
			{
				balls[i].present = 0;
				if (balls[0].present == 0 && balls[1].present == 0 && balls[2].present == 0)
				{
					end = true;
					initializeAll();
					glColor3f(1, 1, 1);
					circle_wire(0, 0, 20);
				}
			}
		}

		if (start == 1 && end == false)
		{
			bull.pos.Rx += 8 * cos(bull.angle*D2R);
			bull.pos.Ry += 8 * sin(bull.angle*D2R);
			if (bull.pos.Rx > 250 || bull.pos.Rx < -250 || bull.pos.Ry < -250 || bull.pos.Ry>250)
			{
				bull.pos.Rx = 0;
				bull.pos.Ry = 0;
				start = 0;
			}

		}
		break;
		///////////////////////////////////////////////////////////
	case 2:

		for (int i = 0; i < 3; i++)
		{
			if (ball1[i].present == 1)
			{
				if (ball1[i].move == 1)
					ball1[i].angle += ball1[i].speed;
				else
					ball1[i].angle -= ball1[i].speed;

				if (ball1[i].angle > 360)
					ball1[i].angle -= 360;
				else if (ball1[i].angle < -360)
					ball1[i].angle += 360;

				if (testCollision(bullet1, ball1[i]) == 1 && start == 1 && end == false)
				{
					ball1[i].present = 0;
					if (ball1[0].present == 0 && ball1[1].present == 0 && ball1[2].present == 0)
					{
						end = true;
						initializeAll();
						glColor3f(1, 1, 1);
						circle_wire(0, 0, 20);
					}
				}
			}
		}

		if (start == 1 && end == false)
		{
			bullet1.pos.Rx += 5 * cos(bullet1.angle*D2R);
			bullet1.pos.Ry += 5 * sin(bullet1.angle*D2R);
			if (bullet1.pos.Rx > p1.Rx + 300 || bullet1.pos.Rx < p1.Rx - 300 || bullet1.pos.Ry < p1.Ry - 300 || bullet1.pos.Ry>p1.Ry + 300)
			{
				bullet1.pos.Rx = p1.Rx;
				bullet1.pos.Ry = p1.Ry;
				start = 0;
			}
		}
		///////////////////////////////

		for (int i = 0; i < 3; i++)
		{
			if (ball2[i].present == 1)
			{
				if (ball2[i].move == 1)
					ball2[i].angle += ball2[i].speed;
				else
					ball2[i].angle -= ball2[i].speed;

				if (ball2[i].angle > 360)
					ball2[i].angle -= 360;
				else if (ball2[i].angle < -360)
					ball2[i].angle += 360;

				if (testCollision(bullet2, ball2[i]) == 1 && start2 == 1 && end == false)
				{
					ball2[i].present = 0;
					if (ball2[0].present == 0 && ball2[1].present == 0 && ball2[2].present == 0)
					{
						start2 = false;
						end = true;
						initializeAll();
						glColor3f(1, 1, 1);
						circle_wire(0, 0, 20);
					}
				}
			}
		}
		if (start2 == 1 && end == false)
		{
			bullet2.pos.Rx += 5 * cos(bullet2.angle);
			bullet2.pos.Ry += 5 * sin(bullet2.angle);
			if (bullet2.pos.Rx > p2.Rx + 300 || bullet2.pos.Rx < p2.Rx - 300 || bullet2.pos.Ry < p2.Ry - 300 || bullet2.pos.Ry>p2.Ry + 300)
			{
				bullet2.pos.Rx = p2.Rx;
				bullet2.pos.Ry = p2.Ry;
				start2 = 0;
			}
		}
		break;
	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initializeAll();

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("THE RACE");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	srand(time(NULL));

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}