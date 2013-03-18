/*
 * Project 5, CECS 449
 * Author: Bret Erickson
 * Description: 
 *      Generates a 2D analog clock that displays system time.
 */


#include <windows.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
#include <gl/Glut.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265

int screenWidth = 500;
int screenHeight = 500;
int lineWidth = 2;
int radius = 220;
int L = 250;

float hour, minutes, seconds;

GLuint clockFaceIndex,clockHandIndex;

void drawCirclePoint(int x, int y)
{
	glPointSize(lineWidth);
	glColor3f(1.0,1.0,1.0);

	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINES);
		glVertex2i(x,y);
		glVertex2i(-x,y);
		glVertex2i(x,-y);
		glVertex2i(-x,-y);
		glVertex2i(y,+x);
		glVertex2i(-y,x);
		glVertex2i(y,-x);
		glVertex2i(-y,-x);
	glEnd();
	glDisable(GL_LINE_SMOOTH);
}//end drawCirclePoint

void circleMidpoint()
{
	int x = 0;
	int y = radius;

	float pK = (5/4) - radius;//p0

	//draw the first point
	drawCirclePoint(x, y);

	while(x < y)
	{
		if(pK < 0)
		{
			x++;
			pK += 2*x+1;
		}//end if
		else
		{
			x++;
			y--;
			pK += 2*(x-y)+1;
		}
		
		drawCirclePoint(x, y);//Draw additional points
	}//end while

}//end circleMidpoint

void clockNumbers()
{
	glColor3i(0,0,0);//Black text

	float angle = 60;
	
	char text[12];
	text[0] = '1';
	text[1] = '2';
	text[2] = '3';
	text[3] = '4';
	text[4] = '5';
	text[5] = '6';
	text[6] = '7';
	text[7] = '8';
	text[8]= '9';

	for(int i=0; i<12; i++)
	{
		//Draw the number
		if(i<9)
		{
			glRasterPos2i((cos(angle*PI/180)*(radius-10))-4.5, sin(angle*PI/180)*(radius-10));
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
		}
		else//Compensate for drawing two characters instead of one, we want the number to appear centered still
			glRasterPos2i((cos(angle*PI/180)*(radius-10))-9, sin(angle*PI/180)*(radius-10));

		//Special cases where we need to draw two characters in the same area
		if(i==9)
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[0]);
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '0');
		}
		else if(i==10)
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[0]);
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[0]);
		}
		else if(i==11)
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[0]);
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[1]);
		}

		//update angle for next iteration
		angle = angle - 30;
	}

}//end clockNumbers

void makeClockHand()
{
	clockHandIndex = glGenLists(1);

	glNewList(clockHandIndex, GL_COMPILE);

		glEnable(GL_LINE_SMOOTH);
		glBegin(GL_POLYGON);
			glVertex2i(0,0);
			glVertex2i(-15, 20);
			glVertex2i(0, radius/2);
			glVertex2i(15, 20);
		glEnd();
		glDisable(GL_LINE_SMOOTH);
	glEndList();
}//end makeClockHand

void makeClockFace()
{
	clockFaceIndex = glGenLists(1);

	glNewList(clockFaceIndex, GL_COMPILE);
		circleMidpoint();
		clockNumbers();
	glEndList();

}//end makeClockFace

void reshape(int w, int h)
{
	//update screen dim
	screenWidth = w;
	screenHeight =h;

	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat aspect = (GLfloat) screenWidth / (GLfloat)screenHeight;

	////setup worldwindow
	glMatrixMode(GL_PROJECTION);				
	glLoadIdentity();							

	glViewport(0,0,screenWidth, screenHeight);

	if(screenWidth <= screenHeight)
		gluOrtho2D(-L, L, -L/aspect, L/aspect);
	else
		gluOrtho2D(-L*aspect, L*aspect, -L, L);
	
	glutPostRedisplay();
}//end reshape

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);	//Paint black background

	//Draw the clock face first
	glCallList(clockFaceIndex);  

	//Draw hands
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//minute
	glColor3f(0,0,1);
	glPushMatrix();	
		glRotatef(360 - (6*minutes), 0.0, 0.0, 1.0);
		glScalef(.8,2,1);
		glCallList(clockHandIndex);	
	glPopMatrix();

	//second
	glColor3f(0,1,0);
	glPushMatrix();	
		glRotatef(360 - (6* seconds), 0.0, 0.0, 1.0);
		glScalef(.3, 2, 1);
		glCallList(clockHandIndex);	
	glPopMatrix();

	//hour
	glColor3f(1,0,0);
	glPushMatrix();
		glRotatef(360 - .5*(60*hour+minutes), 0.0, 0.0, 1.0);
		glCallList(clockHandIndex);	
	glPopMatrix();

	glutSwapBuffers();

}//end display



//Pre:None
//Post:Compiles the drawing of the clock face and hands and stores them to lists for later reference
void initDrawing()
{
	makeClockHand();
	makeClockFace();
}//end init

void initializeWindow()
{
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Triple Threat!");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-L,L,-L,L);
	
}//end initialize

void timeUpdate()
{

	seconds += .5;

	if(seconds > 60)
	{
		minutes ++;
		seconds =0;

		if(minutes > 60)
		{
			minutes =0;
			hour++;

			if(hour > 12)
			{
				hour = 0;
			}
		}
	}

	glutPostRedisplay();
}//end timeUpdate

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	hour = 11;
	minutes = 50;
	seconds = 0;

	initializeWindow();//Initial setup of clipping & screen windows

	initDrawing();//compile clock face & hands

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(timeUpdate);//Check for a new time and redraw

	glutMainLoop();

}//end main