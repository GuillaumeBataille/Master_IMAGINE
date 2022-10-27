/* essai.cpp indique la version de opengl
compilation: g++ test1.cpp -o test1 -lglut -lGLU -lGL
exécution: ./test1
*/
#include <GL/freeglut.h>
#include <cstdio>
int main(int argc, char **argv)
{
glutInit(&argc, argv);
glutCreateWindow("OpenGL-Test1");
printf("Version de OpenGL supportée (%s): \n", glGetString(GL_VERSION));
} 