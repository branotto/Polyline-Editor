//Main.cpp: Defines the entry point for the polyline application.
//This program creates a polyline and allows the polyline to 
//be edited by mouse interaction.

#include <Windows.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define MY_POINTS 1
#define MY_LINES 2
#define MY_LOOP 3
#define MY_POLY 4
#define MY_SIMPLE 5
#define MY_CONVEX 6
#define MY_CW 7

struct myPoint
{
	int xValue;
	int yValue;
};


//-----global variables;
int shape = 3;
int numPoints;
int height;
int selectedPoint;
boolean picked = false;
myPoint pointList[100];


//-----function declarations/interface-----
//-----myInit-----
void myInit(void);

//-----point distance calculation-----
float myDistance(myPoint p1, myPoint p2);

//-----myMouse-----
void myMouse(int button, int state, int mouseX, int mouseY);

//-----myMouseDrag-----
void myMouseDrag(int moveToX, int moveToY);

//-----Draw points only-----
void drawPoints();

//-----Draw lines-----
void drawLines();

//-----Draw line loop-----
void drawLoop();

//-----Draw polygon-----
void drawPolygon();

//-----myDisplay
void renderPolyLine(void);

//-----deletePoint-----
void deletePoint();

//-----Menu Handler-----
void processMenuEvents(int option);

//-----Keyboard Handler-----
void myKeyBoard(unsigned char key, int x, int y);

//-----Draw from a file-----
//Provide the filename as a commandline string
void drawFromFile(char * fileName);

//-----Write the drawing to a text file-----
void writeToFile(char * fileName);

//-----Test if two segments interset-----
bool segIntersect(myPoint A, myPoint B, myPoint C, myPoint D, myPoint &InterPt);

//-----Test for a simple polygon-----
bool isSimple(myPoint my_polyline[]);

//-----Test for a convex polygon-----
bool isConvex(myPoint my_polyline[]);

//-----Test for a clockwise polygon-----
bool isCW(myPoint my_polyline[]);


//---------------------MAIN----------------------
void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Polyline Editor");
	//drawFromFile("polyFile.txt");
	glutDisplayFunc(renderPolyLine);
	myInit();
	glutMouseFunc(myMouse);
	glutMotionFunc(myMouseDrag);
	glutKeyboardFunc(myKeyBoard);
	glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Points", MY_POINTS);
	glutAddMenuEntry("Lines", MY_LINES);
	glutAddMenuEntry("Closed Loop", MY_LOOP);
	glutAddMenuEntry("Polygon", MY_POLY);
	glutAddMenuEntry("Simple?", MY_SIMPLE);
	glutAddMenuEntry("Convex?", MY_CONVEX);
	glutAddMenuEntry("ClockWise?", MY_CW);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	glutMainLoop();
}

//-----function implimentations-----
//-----myInit-----
void myInit(void)
{
	numPoints = 0;
	height = 500;
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 500, 0, 500);
}

//----Point distance function-----
float myDistance(myPoint p1, myPoint p2)
{
	return (p2.xValue - p1.xValue)*(p2.xValue - p1.xValue) + (p2.yValue - p1.yValue)*(p2.yValue - p1.yValue);
}

//-----myMouse-----
void myMouse(int button, int state, int mouseX, int mouseY)
{
	
	if (state == GLUT_DOWN)
	{
		//left moust button adds a point.
		if (button == GLUT_LEFT_BUTTON)
		{
			if (numPoints < 100)
			{
				pointList[numPoints].xValue = mouseX;
				pointList[numPoints].yValue = height - mouseY;
				numPoints++;
			}
		}

		//Right mouse button selects a point.
		else if (button == GLUT_RIGHT_BUTTON)
		{
			for (int i = 0; i < numPoints; i++)
			{
				myPoint mousePositon;
				mousePositon.xValue = mouseX;
				mousePositon.yValue = height - mouseY;
				if (myDistance(mousePositon, pointList[i]) < 50)
				{
					selectedPoint = i;
					picked = true;
				}
			}
		}
	}

	if (state == GLUT_UP)
	{
		picked = false;
		selectedPoint = -1;
	}

	glutPostRedisplay();

}

//-----myMouseDrag-----
void myMouseDrag(int moveToX, int moveToY)
{
	if (picked)
	{
		pointList[selectedPoint].xValue = moveToX;
		pointList[selectedPoint].yValue = height - moveToY;
	}

	glutPostRedisplay();
}

//-----Draw points only-----
void drawPoints()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POINTS);
	for (int i = 0; i < numPoints; i++)
	{
		glVertex2i(pointList[i].xValue, pointList[i].yValue);
	}
	glEnd();

	glFlush();
}

//-----Draw lines-----
void drawLines()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < numPoints; i++)
	{
		glVertex2i(pointList[i].xValue, pointList[i].yValue);
	}
	glEnd();

	glFlush();
}

//-----Draw a line loop-----
void drawLoop()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < numPoints; i++)
	{
		glVertex2i(pointList[i].xValue, pointList[i].yValue);
	}
	glEnd();

	glFlush();
}

//-----Draw a polygon-----
void drawPolygon()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
	for (int i = 0; i < numPoints; i++)
	{
		glVertex2i(pointList[i].xValue, pointList[i].yValue);
	}
	glEnd();

	glFlush();
}

//-----myDisplay
void renderPolyLine(void)
{

	switch (shape)
	{
	case 1: drawPoints();
		break;
	case 2: drawLines();
		break;
	case 3: drawLoop();
		break;
	case 4: drawPolygon();
		break;
	default: drawLoop();
		break;
	}
}

//-----deletePoint-----
//Deletes the current point if one is picked.
//Else delete the last point.
void deletePoint()
{
	if (picked)
	{
		myPoint temp;
		temp.xValue = 0;
		temp.yValue = 0;

		for (int i = selectedPoint; i < numPoints; i++)
		{
			temp.xValue = pointList[i + 1].xValue;
			temp.yValue = pointList[i + 1].yValue;
			pointList[i].xValue = temp.xValue;
			pointList[i].yValue = temp.yValue;
		}
	}

	numPoints--;

	glutPostRedisplay();
}

//-----Menu Handler-----
//Middle mouse button displays the GLUT menu
//GLUT menu selects the draw method.
void processMenuEvents(int option)
{
	switch (option)
	{
	case MY_POINTS:	shape = 1;
		break;
	case MY_LINES:	shape = 2;
		break;
	case MY_LOOP:	shape = 3;
		break;
	case MY_POLY:	shape = 4;
		break;
	case MY_SIMPLE:	isSimple(pointList);
		break;
	case MY_CONVEX:	isConvex(pointList);
		break;
	case MY_CW:	isCW(pointList);
		break;
	default:
		break;
	}
}

//-----Keyboard Handler-----
//Use the "d" key to delete a point
void myKeyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'd':	deletePoint();
		break;
	default:
		break;
	}
}

//-----Draw from a file-----
//Provide the filename as a commandline string
void drawFromFile(char * fileName)
{
	printf("Opening file.");
	fstream inStream("polyFile.txt", fstream::in | fstream::out);
	//inStream.open("D:\Development\Visual Studio Apps\Polyline Editor\polyFile.txt", ios::in);
	if (inStream.fail())
	{
		printf("file failed.");
		return;
	}
	
	int numPolys, x, y;

	inStream >> numPolys;

	for (int j = 0; j < numPolys; j++)
	{
		inStream >> x >> y;
		pointList[numPoints].xValue = x;
		pointList[numPoints].yValue = height - y;
		numPoints++;
	}

	printf("closing file.");
	inStream.close();

	glutPostRedisplay();
}

// writing to a text file
void writeToFile(char * fileName)
{
	ofstream myfile(fileName);
	if (myfile.is_open())
	{
		myfile << numPoints << "\n";

		for (int i = 0; i < numPoints; i++)
		{
			myfile << pointList[i].xValue << " " << pointList[i].yValue << "\n";
		}
		
		myfile.close();
	}
	else cout << "Unable to open file";
}

//-----Test if 2 segments intersect-----
bool segIntersect(myPoint A, myPoint B, myPoint C, myPoint D, myPoint &InterPt)
{
	myPoint b, c, d;

	b.xValue = B.xValue - A.xValue;
	b.yValue = B.yValue - A.yValue;

	c.xValue = C.xValue - A.xValue;
	c.yValue = C.yValue - A.yValue;

	d.xValue = D.xValue - C.xValue;
	d.yValue = D.yValue - C.yValue;

	for (float t = 0.0; t <= 1.0; t += 0.001)
	{
		for (float u = 0.0; u <= 1.0; u += 0.001)
		{
			if (abs(b.xValue*t - (c.xValue + (d.xValue*u))) < 0.001 && abs(b.yValue*t -(c.yValue + (d.yValue*u))) < 0.001)
			{
				InterPt.xValue = A.xValue + b.xValue*t;
				InterPt.yValue = A.xValue + b.yValue*t;
				return true;
			}
		}
	}

	return false;
}

//-----Test if the polygon is simple-----
bool isSimple(myPoint my_polyline[])
{
	myPoint intersectionPoint;

	for (int i = 0; i < numPoints; i++)
	{
		for (int j = i + 1; j < numPoints; j++)
		{
			if (segIntersect(my_polyline[i % numPoints], my_polyline[(i + 1) % numPoints], my_polyline[j % numPoints], my_polyline[(j + 1) % numPoints], intersectionPoint))
			{
				printf("The polygon is not simple.\n");
				printf("Intersection found at %d, %d.\n", intersectionPoint.xValue, intersectionPoint.yValue);
				return false;
			}
		}
	}
	printf("The polygon is simple.\n");
	return true;
}

//-----Test if the polygon is convex-----
bool isConvex(myPoint my_polyline[])
{
	//Create an array to hold the crossproducts.
	int crossProduct[100];

	//for each pair of edges, find the z value of the crossproduct
	for (int i = 0; i < numPoints; i++)
	{
		//Find the next 3 points that create edges. 
		myPoint A = my_polyline[i % numPoints];
		myPoint B = my_polyline[(i + 1) % numPoints];
		myPoint C = my_polyline[(i + 2) % numPoints];

		//find the edges between the 3 points
		myPoint a, b;

		a.xValue = B.xValue - A.xValue;
		a.yValue = B.yValue - A.yValue;

		b.xValue = C.xValue - B.xValue;
		b.yValue = C.yValue - B.yValue;

		//calculate the z-value of the crossproduct and store it in an array.
		crossProduct[i] = a.xValue * b.yValue - a.yValue * b.xValue;
	}

	//Iterate through the cross products.
	for (int i = 0; i < numPoints; i++)
	{
		//If the signs are the same, the product will be positive.
		if ((crossProduct[i % numPoints] * crossProduct[(i + 1) % numPoints]) < 0)
		{
			//If any product is negative, the polygon is concave.
			printf("Concave.\n");
			return false;
		}
	}

	//the polygon is convex.
	printf("Convex.\n");
	return true;
}

//-----Test if the polygon is clockwise-----
bool isCW(myPoint my_polyline[])
{
	//Create an array to hold the crossproducts.
	int crossProduct[100];

	//for each pair of edges, find the z value of the crossproduct
	for (int i = 0; i < numPoints; i++)
	{
		//Find the next 3 points that create edges. 
		myPoint A = my_polyline[i % numPoints];
		myPoint B = my_polyline[(i + 1) % numPoints];
		myPoint C = my_polyline[(i + 2) % numPoints];

		//find the edges between the 3 points
		myPoint a, b;

		a.xValue = B.xValue - A.xValue;
		a.yValue = B.yValue - A.yValue;

		b.xValue = C.xValue - B.xValue;
		b.yValue = C.yValue - B.yValue;

		//calculate the z-value of the crossproduct and store it in an array.
		crossProduct[i] = a.xValue * b.yValue - a.yValue * b.xValue;
	}

	//Variables to count the sign of the crossproducts
	int negative = 0;
	int positive = 0;

	for (int i = 0; i < numPoints; i++)
	{
		//if the crossproducts is positive it is a counterclockwise turn
		if (crossProduct[i] > 0)
		{
			positive++;
		}
		else
		{
			//if it is negative it is a clockwise turn.
			negative++;
		}

		//if more of the turns are positive, it is a counterclockwise polygon.
		if (positive > negative)
		{
			printf("Counter Clockwise.\n");
			return false;
		} 
	}

	//else it is a clockwise polygon.
	printf("Clockwise.\n");
	return true;
}

