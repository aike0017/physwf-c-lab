#include <stdlib.h>
#include "draw.h"
#include <time.h>

int rand_by_range(int min,int max)
{
	return rand() % (max-min) + min;
}
float random()
{
	return (float)rand() / RAND_MAX;
}

void drawPoints()
{
	srand(time(NULL));
	glPointSize(2);
	glBegin(GL_POINTS);
		for(int i=0;i<10;i++)
		{
			int x = rand_by_range(0,STAGE_WIDTH);
			int y = rand_by_range(0,STAGE_HEIGHT);
			glColor3f(random(),random(),random());
			glVertex2i(x,y);
		}
	glEnd();
}

void drawLines()
{
	srand(time(NULL));
	
	glBegin(GL_LINES);
		for(int i=0;i<10;i++)
		{
			int x = rand_by_range(0,STAGE_WIDTH);
			int y = rand_by_range(0,STAGE_HEIGHT);
			glColor3f(random(),random(),random());
			glVertex2i(x,y);
		}
	glEnd();
}

void drawLineStrip()
{
	srand(time(NULL));
	glBegin(GL_LINE_STRIP);
		for(int i=0;i<10;i++)
		{
			int x = rand_by_range(0,STAGE_WIDTH);
			int y = rand_by_range(0,STAGE_HEIGHT);
			glColor3f(random(),random(),random());
			glVertex2i(x,y);
		}
	glEnd();
}

void drawLineLoop()
{
	srand(time(NULL));
	glBegin(GL_LINE_LOOP);
		for(int i=0;i<10;i++)
		{
			int x = rand_by_range(0,STAGE_WIDTH);
			int y = rand_by_range(0,STAGE_HEIGHT);
			glColor3f(random(),random(),random());
			glVertex2i(x,y);
		}
	glEnd();
}

void drawTriangles()
{
	srand(time(NULL));
	glBegin(GL_TRIANGLES);
		for(int i=0;i<9;i++)
		{
			int x = rand_by_range(0,STAGE_WIDTH);
			int y = rand_by_range(0,STAGE_HEIGHT);
			glColor3f(random(),random(),random());
			glVertex2i(x,y);
		}
	glEnd();
}

void drawTriangleStrip()
{
	srand(time(NULL));
	glBegin(GL_TRIANGLE_STRIP);
		for(int i=0;i<9;i++)
		{
			int x = rand_by_range(0,STAGE_WIDTH);
			int y = rand_by_range(0,STAGE_HEIGHT);
			glColor3f(random(),random(),random());
			glVertex2i(x,y);
		}
	glEnd();
}

void drawTriangleFan()
{
	srand(time(NULL));
	glBegin(GL_TRIANGLE_FAN);
		for(int i=0;i<9;i++)
		{
			int x = rand_by_range(0,STAGE_WIDTH);
			int y = rand_by_range(0,STAGE_HEIGHT);
			glColor3f(random(),random(),random());
			glVertex2i(x,y);
		}
	glEnd();
}

void drawQuads()
{
	srand(time(NULL));
	glBegin(GL_QUADS);
		for(int i=0;i<8;i++)
		{
			int x = rand_by_range(0,STAGE_WIDTH);
			int y = rand_by_range(0,STAGE_HEIGHT);
			glColor3f(random(),random(),random());
			glVertex2i(x,y);
		}
	glEnd();
}

void drawQuadStrip()
{
	srand(time(NULL));
	int vertex[] = {100,100,200,0,300,100,400,300,500,400};
	glBegin(GL_QUAD_STRIP);
		for(int i=0;i<5;i+=2)
		{
			int x = rand_by_range(0,STAGE_WIDTH);
			int y = rand_by_range(0,STAGE_HEIGHT);
			glColor3f(random(),random(),random());
			glVertex2i(vertex[i],vertex[i+1]);
		}
	glEnd();
}

void drawPolygon()
{
	srand(time(NULL));
	int vertex[] = {100,100,200,0,300,100,400,300,500,400};
	glBegin(GL_POLYGON);
		for(int i=0;i<10;i+=2)
		{
			// int x = rand_by_range(0,STAGE_WIDTH);
			// int y = rand_by_range(0,STAGE_HEIGHT);
			glColor3f(random(),random(),random());
			glVertex2i(vertex[i],vertex[i+1]);
		}
	glEnd();
}