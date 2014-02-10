#include "TetrisMain.h"
#define BUFFER_OFFSET(bytes) ((GLubyte*)NULL + bytes)

enum Buffer_IDs {VERTEX,INDEX,NumBuffers}
GLuint buffers[NumBuffers];


void display()
{
	
}

void init()
{
	glGenBuffers(NumBuffers,buffers);
	glBindBuffer(GL_ARRAY_BUFFER,buffers[VERTEX]);
	glBufferData();
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVetexPointer(2,GL_INT,0,BUFFER_OFFSET(0));
}

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512,512);
	glutInitWindowPosition(0,0);
	glutInitContextVersion(3,0);
	// glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Triangles");
	if(glewInit() != GLEW_OK)
	{
		printf("Glew Init Error!");
		return 1;
	}
	init();
	glutDisplayFunc(&display);
	glutMainLoop();
}