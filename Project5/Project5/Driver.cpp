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

int screenWidth = 100;
int screenHeight = 100;

void reshape(int w, int h)
{

}//end reshape

void display()
{

}//end display

void makeClockHand()
{

}//end makeClockHand

void makeClockFace()
{

}//end makeClockFace

//Pre:None
//Post:Compiles the drawing of the clock face and hands and stores them to lists for later reference
void init()
{
	makeClockHand();
	makeClockFace();
}//end init

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(10, 30);
	glutCreateWindow("Triple Threat!");

	gluOrtho2D(-100, 100, -100, 100);

	init();//Pre-compile clock face & hands

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();

}//end main