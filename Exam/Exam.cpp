#include<iostream>
#include<GL/freeglut.h>
#include<sstream>

#define VK_W 0x57
#define VK_S 0x53

int width = 1000;
int height = 500;
float interval = 1000 / 60;

int scoreLeft = 0;
int scoreRight = 0;

int paddleWidth = 10;
int paddleHeight = 100;
int paddleSpeed = 5;

float paddleLeftX = 10;
float paddleLeftY = height / 2 - paddleHeight / 2;
float paddleRightX = width - paddleWidth - 10;
float paddleRightY = height / 2 - paddleHeight / 2;

float ballX = width / 2;
float ballY = height / 2;
float ballDirectionX = -1;
float ballDirectionY = 0;
float ballSize = 10;
float ballSpeed = 5;

void DrawScore(int x, int y, std::string text)
{
	glRasterPos2f(x, y);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}

std::string ToString(int x)
{
	std::stringstream ss;
	ss << x;
	return ss.str();
}

void DrawRect(float x, float y, int width, int height)
{
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}

void Draw(void)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawRect(paddleLeftX, paddleLeftY, paddleWidth, paddleHeight);
	DrawRect(paddleRightX, paddleRightY, paddleWidth, paddleHeight);
	DrawRect(ballX, ballY, ballSize, ballSize);
	DrawScore(width / 2 - 10, height - 50, ToString(scoreLeft) + " : " + ToString(scoreRight));

	glutSwapBuffers();
}

void Keyboard(void)
{
	if (GetAsyncKeyState(VK_UP))
		paddleRightY += paddleSpeed;
	else if (GetAsyncKeyState(VK_DOWN))
		paddleRightY -= paddleSpeed;
	if (GetAsyncKeyState(VK_W))
		paddleLeftY += paddleSpeed;
	else if (GetAsyncKeyState(VK_S))
		paddleLeftY -= paddleSpeed;
}

void RandomDirection()
{
	if((int)ballY % 2 == 0)
		ballDirectionX = -1;
	else
		ballDirectionX = 1;
}

void UpdateBall()
{
	ballX += ballDirectionX * ballSpeed;
	ballY += ballDirectionY * ballSpeed;

	if (ballX < 0)
	{
		scoreRight++;
		RandomDirection();
		ballX = width/2;
		ballY = height/2;
		ballDirectionY = 0;
	}

	if (ballX > width)
	{
		scoreLeft++;
		RandomDirection();
		ballX = width / 2;
		ballY = height / 2;
		ballDirectionY = 0;
	}

	if (ballX < paddleLeftX + paddleWidth && ballX > paddleLeftX - ballSize && ballY < paddleLeftY + paddleHeight
		&& ballY > paddleLeftY - ballSize)
	{
		ballDirectionX *= -1;
		float offset = (ballY - paddleLeftY) / paddleHeight - 0.5;
		ballDirectionY = offset;
	}

	if (ballX > paddleRightX - ballSize && ballX < paddleRightX + paddleWidth && ballY < paddleRightY + paddleHeight
		&& ballY > paddleRightY - ballSize)
	{
		ballDirectionX *= -1;
		float offset = (ballY - paddleRightY) / paddleHeight - 0.5;
		ballDirectionY = offset;
	}

	if (ballY < 0)
	{
		ballDirectionY *= -1;
	}

	if (ballY > height - ballSize)
	{
		ballDirectionY *= -1;
	}
}

void Update(int value)
{
	Keyboard();
	UpdateBall();
	glutTimerFunc(interval, Update, 0);

	glutPostRedisplay();
}

void Init()
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("EXAM");

	glutDisplayFunc(Draw);
	glutTimerFunc(interval, Update, 0);
	Init();

	glutMainLoop();

	return 0;
}