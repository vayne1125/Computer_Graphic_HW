/*---------------------------------------------------------------------------------------------------------
 * This file contains a procedure to initialize light0 and the callback functions.
 */
#include <stdio.h>
#include <stdlib.h>

#include "glad.h"
#include <GL/glut.h>

#include "GLInterface.h"
#include "GLSLSetup.h"

#define DEFAULT_SHADER 0
#define MY_SHADER           1

//Directional Light
float light_amb[4] = {1.0, 1.0, 1.0, 1};
float light_dif[4] = {1.0, 1.0, 1.0, 1};
float light_spe[4] = {1.0, 1.0, 1.0, 1};
float light_pos[4] = {0.0, 40.0, 0.0, 1}; //light position
//Polygon Material
float material_amb[4] = {0.3,0.1,0.1,1};
float material_dif[4] = {0.6,0.1,0.1,1};
float material_spe[4] = {0.4,0.4,0.4,1};
float shininess = 256.0f;
int shader_effect = DEFAULT_SHADER;
//View parameter control
float rollAng=0.0;
//Yaw angle of the rectangle 
float yawAng=0.0;

/*------------------------------------------------------------------------
 *Enhancing specular effect by increasing the shininess of the material
 * AT the same time, decrease the specular intensity of the material.
 */
void  enhance_specular(void)
{
	if(shininess < 256.0f){
		shininess *= 2.0f;
        material_spe[0] = 1.1*material_spe[0];
        material_spe[1] = 1.1*material_spe[1];
        material_spe[2] = 1.1*material_spe[2];
	}
}

/*------------------------------------------------------------------------
 * Weaken specular effect by decreasing the shininess of the material
 * AT the same time, increase the specular intensity of the material.
 */
void weaken_specular(void)
{
	if(shininess > 1.0f){
		shininess /= 2.0f;
        material_spe[0] = 0.9*material_spe[0];
        material_spe[1] = 0.9*material_spe[1];
        material_spe[2] = 0.9*material_spe[2];
	}
}

/*----------------------------------------------------------------------------------------------
 * Switch between the default and our program object.
 */
void switch_shaders(void)
{
	if(shader_effect == DEFAULT_SHADER) shader_effect = MY_SHADER;
	else shader_effect = DEFAULT_SHADER;
}

/*-------------------------------------------------------------------------------------------------
 * Set up light0 and enable lighting effect.
 */
void init_light(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, light_dif);
	glLightfv(GL_LIGHT0,GL_SPECULAR, light_spe);
	glLightfv(GL_LIGHT0,GL_POSITION, light_pos);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
}

/*---------------------------------------------------------------------------------------------------
 * Initializing light0
 */
void myInit()
{
	init_light();
}

/*---------------------------------------------------------------------------------------------
 * Draw 3 axes to show the world coordinate.
 */
void DrawAxis(void)
{
	glBegin(GL_LINES);
	//draw the x axis, RED
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(100, 0, 0);

	//Draw the y axis, GREEN
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 100, 0);
	
	//Draw the z axis, BLUE
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 100);
	glEnd();
}


/*----------------------------------------------------------------------------------
 * The display callback function.
 */
void myDisplay(void)
{
	glEnable(GL_DEPTH_TEST);
    glClearColor(0.10, 0.10, 0.10, 1.0);  //Dark grey background
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	//Projection transformation matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 2000);

	//View transformation matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Roll the eye space
	glRotatef(rollAng, 0, 0, 1.0);
	gluLookAt(100, 100, 100, 0, 0, 0, 0, 1, 0);
	//Yaw the world space
	glRotatef(yawAng, 0, 1, 0);
    //Model transformation matrix
    //Using default opengl program object to draw the world coordinate system
    glDisable(GL_LIGHTING);
	glUseProgram(0); 
	DrawAxis();
	
    //Using our program object to render the scene
	if(shader_effect == MY_SHADER)
    glUseProgram(ReturnProgramID(0)); //Using our shaders
     //Turn on lighting and light0
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Set the polygon material properties
	//glColor3f(1,1,1); //If rendered by the default program object
	glMaterialfv(GL_FRONT,GL_AMBIENT , material_amb);
	glMaterialfv(GL_FRONT,GL_DIFFUSE , material_dif);
	glMaterialfv(GL_FRONT,GL_SPECULAR, material_spe);
	glMaterialf(GL_FRONT,GL_SHININESS, shininess);

	glNormal3f(0, 1, 0);
	glBegin(GL_POLYGON);
		glVertex3f( 50, 0, 50);
		glVertex3f(-50, 0, 50);
		glVertex3f(-50, 0, -50);
		glVertex3f( 50, 0, -50);
	glEnd();

	glDisable(GL_LIGHT0);//close light0
	glDisable(GL_LIGHTING);//close lighting

	glutSwapBuffers();
}


/*------------------------------------------------------------------------
 * The keyboard call back function.
 */
void myKeyboardFunc(unsigned char c,int x,int y)
{
       //Switch between the default and our program objects
       if(c == 's') switch_shaders();
       //Increase the shininess
       if(c == 'a') enhance_specular();
       //Decrease the shininess
       if(c == 'd') weaken_specular();
       //Quick this program
	   if(c=='q') exit(0); 
	   if(c=='r'){
		   rollAng += 3.0;
		   if(rollAng>360.0) rollAng -= 360.0;
	   }else if(c=='y'){
		   yawAng += 3.0;
		   if(yawAng>360.0) yawAng -= 360.0;
	   }
	   
      //Redraw the window
       glutPostRedisplay();
}

