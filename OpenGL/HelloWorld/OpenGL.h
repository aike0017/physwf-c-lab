#if defined(WIN)
#include <GL/glew.h>
#include <GL/glut.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glew32s.lib")
#endif

#if defined(MAC)
#include <GLUT/glut.h>
#endif

