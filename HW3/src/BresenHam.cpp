#include "BresenHam.h"
#include <algorithm>
const int width = 800;
const int height = 800;
BresenHam::BresenHam(){
	pixelNum = 0;
}
BresenHam::~BresenHam(){
}
void BresenHam::input() {
	cout << "Please enter the coordinates of x0, y0, x1, y1, x2 and y2: " << endl;
	for (int i = 0; i < 3; i++) {
		cin >> x[i] >> y[i];
	}
	cout << "Please enter the center of circle and the radius : " << endl;
	cin >> centerX >> centerY >> radius;
}
void BresenHam::drawTriangle() {
	int pointNum = 0, index = 0;
	for (int i = 0; i < 3; i++) {
		if (x[i % 3] == x[(i + 1) % 3]) {
			//make sure the startPos is smaller than the endPos.
			int y0 = y[i % 3], y1 = y[(i + 1) % 3];
			if (y[i % 3] > y[(i + 1) % 3]) {
				y0 = y[(i + 1) % 3];
				y1 = y[i % 3];
			}
			pointNum = y1 - y0 + 1;
			xPos[0] = x[i % 3];
			yPos[0] = y0;
			for (int j = 1; j < pointNum; j++) {
				xPos[j] = xPos[j - 1];
				yPos[j] = yPos[j - 1] + 1;
			}
		}
		else {
			int deltaX = x[(i + 1) % 3] - x[i % 3], deltaY = y[(i + 1) % 3] - y[i % 3];
			if (abs(deltaX) > abs(deltaY)) {
				pointNum = abs(deltaX) + 1;
			}
			else {
				pointNum = abs(deltaY) + 1;
			}
			bresenHamLine(x[i % 3], y[i % 3], x[(i + 1) % 3], y[(i + 1) % 3], pointNum, abs(deltaX), abs(deltaY));
		}
		for (int j = 0; j < pointNum; j++) {
			Trivertices[index + j * 2] = (float)xPos[j] / width;
			Trivertices[index + j * 2 + 1] = (float)yPos[j] / height;
		}
		index += pointNum * 2;
	}
	this->pixelNum = index;
	unsigned int VAO, VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Trivertices), Trivertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	drawPoint(VAO);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
void BresenHam::bresenHamLine(int x0, int y0, int x1, int y1, int pointNum, int deltaX, int deltaY) {
	int p = 0, stepX, stepY, gradient;
	xPos[0] = x0;
	yPos[0] = y0;
	if (x1 > x0) {
		stepX = 1;
	}
	else {
		stepX = -1;
	}
	if (y1 > y0) {
		stepY = 1;
	}
	else {
		stepY = -1;
	}
	//if the gradient is bigger than 1, gradient = 1
	if (deltaY > deltaX) {
		p = 2 * deltaX - 2 * deltaY;
		gradient = 1;
	}
	else {
		p = 2 * deltaY - 2 * deltaX;
		gradient = 0;
	}
	for (int i = 1; i < pointNum; i++) {
		//x = x+stepX
		if (gradient == 0) {
			if (p <= 0) {
				xPos[i] = xPos[i - 1] + stepX;
				yPos[i] = yPos[i - 1];
				p = p + 2 * deltaY;
			}
			else {
				xPos[i] = xPos[i - 1] + stepX;
				yPos[i] = yPos[i - 1] + stepY;
				p = p + 2 * deltaY - 2 * deltaX;
			}

		}
		//y = y+stepY
		else {
			if (p <= 0) {
				xPos[i] = xPos[i - 1];
				yPos[i] = yPos[i - 1] + stepY;
				p = p + 2 * deltaX;
			}
			else {
				xPos[i] = xPos[i - 1] + stepX;
				yPos[i] = yPos[i - 1] + stepY;
				p = p + 2 * deltaX - 2 * deltaY;
			}
		}
	}
}
void BresenHam::drawCircle() {
	int pixelNum;
	pixelNum = bresenHamCircle(centerX, centerY, radius);
	for (int i = 0; i < pixelNum; i++) {
		Circlevertices[i * 2] = xPos[i];
		Circlevertices[i * 2 + 1] = yPos[i];
	}
	this->pixelNum = pixelNum;
	unsigned int VAO, VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Circlevertices), Circlevertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	drawPoint(VAO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
void BresenHam::drawCircle(int r) {
	int pixelNum;
	pixelNum = bresenHamCircle(centerX, centerY, r);
	for (int i = 0; i < pixelNum; i++) {
		Circlevertices[i * 2] = xPos[i];
		Circlevertices[i * 2 + 1] = yPos[i];
	}
	this->pixelNum = pixelNum;
	unsigned int VAO, VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Circlevertices), Circlevertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	drawPoint(VAO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
int BresenHam::bresenHamCircle(int x, int y, int radius) {
	x = (float)x / 500;
	y = (float)y / 500;
	int d = 1 - radius;
	int x1 = 0, y1 = radius;
	int index = 0;
	while (y1 > x1) {
		float temp1 = (float)x1 / 500, temp2 = (float)y1 / 500;
		xPos[index] = temp1 + x;
		yPos[index++] = temp2 + y;
		xPos[index] = x - temp1;
		yPos[index++] = y - temp2;
		xPos[index] = temp1 + x;
		yPos[index++] = y - temp2;
		xPos[index] = x - temp1;
		yPos[index++] = temp2 + y;

		xPos[index] = x + temp2;
		yPos[index++] = temp1 + y;
		xPos[index] = x - temp2;
		yPos[index++] = y - temp1;
		xPos[index] = temp2 + x;
		yPos[index++] = y - temp1;
		xPos[index] = x - temp2;
		yPos[index++] = temp1 + y;
		if (d < 0)
		{
			d = d + 2 * x1 + 3;
		}
		else
		{
			d = d + 2 * (x1 - y1) + 5;
			y1--;
		}
		x1++;
	}
	return index;
}
void BresenHam::fillTriangle() {
	int A[3], B[3], C[3];
	int top, bottom, left, right;
	this->pixelNum = 1;
	top = (*max_element(y, y + 3));
	bottom = (*min_element(y, y + 3));
	left = (*min_element(x, x + 3));
	right = (*max_element(x, x + 3));
	for (int i = 0; i < 3; i++) {
		A[i] = y[(i + 1) % 3] - y[i % 3];
		B[i] = -(x[(i + 1) % 3] - x[i % 3]);
		C[i] = -(A[i] * x[i] + B[i] * y[i]);
	}
	for (int i = left; i <= right; i++) {
		for (int j = top; j >= bottom; j--) {
			bool in = true;
			for (int k = 0; k < 3; k++) {
				if (A[k] * i + B[k] * j + C[k] < 0) {
					in = false;
					break;
				}
			}
			if (in) {
				float arr[2] = { 0 };
				arr[0] = (float)i / width;
				arr[1] = (float)j / height;
				unsigned int VAO, VBO;
				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(arr), arr, GL_STATIC_DRAW);
				glGenVertexArrays(1, &VAO);
				glBindVertexArray(VAO);

				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);

				drawPoint(VAO);
				glDeleteVertexArrays(1, &VAO);
				glDeleteBuffers(1, &VBO);
			}
		}
	}

}
void BresenHam::drawPoint(unsigned int VAO) {
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, pixelNum);
}
