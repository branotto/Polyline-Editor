//The GLintPointArray class holds a list of vertices in an array.

#include <GL/freeglut.h>

//The GLintPoint holds a 2d point with an x-coordinate and y-coordinate
struct GLintPoint
{
	GLint x;
	GLint y;
};

class GLintPointArray
{
	const static int MAX_NUM = 100;
public:
	int num;
	GLintPoint vertices[MAX_NUM];
};