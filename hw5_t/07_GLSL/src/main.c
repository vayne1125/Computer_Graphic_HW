/*--------------------------------------------------------------------------------------------
 * Sample code for GLSL shaders (vertex and fragment shader) using glew libraries.
 * This is the main procedure.
 */
#include <stdio.h>
#include <stdlib.h>
//We need this file for glad
#include "glad.h"
#include <GL/glut.h>

#include "GLInterface.h"
#include "GLSLSetup.h"

#ifdef GLAD_DEBUG
void pre_gl_call(const char *name, void *funcptr, int len_args, ...) {
    printf("Calling: %s (%d arguments)\n", name, len_args);
}
#endif


/*-----------------------------------------------------------------------------------
 * The main procedure of the sample program.
 * 1. Initialize glad and glut.
 * 2. Build the user interface, registering callbacks.
 * 3. Create the program object, vertex shader, and fragment shader.
 * 4. Enter the main loop.
 */
void main(int argc, char** argv)
{

  //Initialize glut
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(800,800);
	glutCreateWindow("Shader Sample Program"); //Window title bar
	glutDisplayFunc(myDisplay); //Display callback function
	glutKeyboardFunc(myKeyboardFunc); //Keyboard callback

	//Initiate glad by calling gladLoadGL(void)
    //We have to initialize OpenGL before starting glad. Error checking
   if(!gladLoadGL()) {
        printf("Something went wrong!\n");
        exit(-1);
    }

   //print out OpenGL and GLSL versions
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
                                                         glGetString(GL_SHADING_LANGUAGE_VERSION));
    //Setup the shaders and create my program object
    //Program objects and shaders must be created after glad has been initiated!!!
	setGLSLshaders("Phong.vert","Phong.frag");
	myInit();
            //The main loop.
	glutMainLoop();
}