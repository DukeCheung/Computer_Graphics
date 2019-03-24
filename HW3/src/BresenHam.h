#ifndef BRESENHAM
#define BRESENHAM
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
using namespace std;
class BresenHam {
private:
	float xPos[10000];
	float yPos[10000];
	float x[3], y[3];
	int centerX, centerY, radius;
	int pixelNum;
	float Circlevertices[20000];
	float Trivertices[20000];
public:
	BresenHam();
	~BresenHam();
	void input();
	void drawTriangle();
	void drawCircle();
	void drawCircle(int);
	void fillTriangle();
	void drawPoint(unsigned int);
	void bresenHamLine(int x0, int y0, int x1, int y1, int pointNum, int deltaX, int deltaY);
	int bresenHamCircle(int, int, int);
};

#endif // !BRESENHAM
