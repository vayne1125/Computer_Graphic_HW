/***************************************************
 * Example program: Draw a moving cube
 *   Author: S. K. Ueng
 *   National Taiwan Ocean Univ. Comp. Sci. Dept.
 */
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>


#define   PI   3.1415927
 //Define polygon drwaing models, filled or outlined.
#define   FILL   1
#define   LINE  2   

#define ICE 100
#define FLOOR 101
#define WOOD 102


//coordinates of the 8 corners.(0,0,0)-(1,1,1)
float  points[][3] = { {0.0, 0.0, 0.0}, {10.0, 0.0, 0.0},
                      {10.0, 0.0, 10.0}, {0.0, 0.0, 10.0},
                      {0.0, 10.0, 0.0}, {10.0, 10.0, 0.0},
{10.0, 10.0, 10.0}, {0.0, 10.0, 10.0} };
//Define 6 faces using the 8 corners (vertices)
int    face[][4] = { {0, 1, 2, 3}, {7, 6, 5, 4}, {0, 4, 5, 1},
                    {1, 5, 6, 2}, {3, 2, 6, 7}, {0, 3, 7, 4} };
//Define the cube using the 6 faces.
int    cube[6] = { 0, 1, 2, 3, 4, 5 };
//Define colors
float  color[][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 0.0},
{1.0, 0.0, 1.0},{0.0, 1.0, 1.0} };

/* window shape */
int    width = 500, height = 500;
//define a base position in the z-x plane
float  pos[3] = { 0.0, 0.0, 0.0 };
//declare the rotational angle.
float  angle = 0.0;

//Quodri objects for drawing the world coordinate system.
/*-----Define GLU quadric objects, a sphere and a cylinder----*/
GLUquadricObj* sphere = NULL, * cylind = NULL, * mycircle = NULL;
int    polygonMode = FILL;

void draw_magic_field();
void draw_cylinder(void);

void change_color(int value) {  //設定畫筆顏色
    switch (value){
    case ICE:
        glColor3f(235 / 255.0, 1.0, 1.0);
        break;
    case FLOOR:
        glColor3f(50 / 255.0, 50 / 255.0, 50 / 255.0);
        break;
    case WOOD:
        glColor3f(158/255.0, 79/255.0, 0);
    }
}


void myinit()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */
                     /*Clear the Depth & Color Buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, width, height);

    /*-----Set a parallel projection mode-----*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-8.0, 8.0, -8.0, 8.0, 0.0, 20.0);

    glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */

    glFlush();/*Enforce window system display the results*/
}
void draw_cube(){
    int    i;
    glPolygonMode(GL_FRONT, GL_FILL);
    for (i = 0; i < 6; i++) {     /* draw the six faces one by one */
        //glColor3fv(color[i]);  /* Set the current color */
        glBegin(GL_POLYGON);  /* Draw the face */
        glVertex3fv(points[face[i][0]]);
        glVertex3fv(points[face[i][1]]);
        glVertex3fv(points[face[i][2]]);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
}
void draw_circle(double size, int wid) {    // Procedure to draw a circle
    glLineWidth(wid);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 1440; ++i)
        glVertex3f(size * cos(i * 0.01745), 0, size * sin(i * 0.01745));
    glEnd();
}
void draw_square(int hei,int wid){     //躺在地上的正方形 定義: x軸向為寬，z向為高
    glScaled(wid,0,hei);
    glBegin(GL_POLYGON);
    glVertex3f(0, 0.0, 1);           
    glVertex3f(1, 0.0, 1);
    glVertex3f(1, 0.0, 0);
    glVertex3f(0, 0.0, 0);
    glEnd();
}
void draw_magic_wand(){                 //x右 y上 z前
    change_color(WOOD);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPushMatrix();
    glTranslatef(0, 0, -4);
    draw_cylinder(1, 1, 8);
    glPopMatrix();

    glColor3f(102/255.0, 34/255.0, 0);
    glPushMatrix();
    glTranslatef(0, 0, -4);
    glRotatef(270, 1, 0, 0);        //圓形站立
    draw_circle(1, 1);        
    glPopMatrix();

    glColor3f(102 / 255.0, 34 / 255.0, 0);
    glPushMatrix();
    glTranslatef(0, 0, 4);
    glRotatef(270, 1, 0, 0);        //圓形站立
    draw_circle(1, 1);
    glPopMatrix();
}
void draw_floor() {           //畫牆壁和地板
    change_color(ICE);  
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glPushMatrix();                   //保存0,0
    glRotatef(270,1,0,0);             //延x軸逆時針轉270度
    draw_square(60,60);      
    glPopMatrix();                    //回到0,0
  
    glPushMatrix();                   //保存0,0
    glRotatef(90, 0, 0, 1);           //延z軸逆時針轉270度
    draw_square(60, 60);
    glPopMatrix();                    //回到0,0

    glPushMatrix();
    draw_square(60, 60);              //畫地板
    glPopMatrix();

    /*
    glTranslatef(1,0,0);    //向右移動(1,0,0)
    glPushMatrix();         //保存當前位置
    glTranslatef(0,1,0);    //現在是(1,1,0)了
    glPopMatrix();          //這樣，現在又回到(1,0,0)了
    */
}
void draw_magic_field() {   
    //魔法陣
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    change_color(ICE);
    glRotatef(90, 0, 1, 0);                          //旋轉90度(對y軸) -> 改變月亮開口
    draw_circle(30, 1);
    draw_circle(29, 1);
    draw_circle(27.8, 1); //1.2
    draw_circle(27.3, 1); //0.5

    glLineWidth(1);                        //直線
    for (int i = 0; i < 360; i += 15) {
        glBegin(GL_LINES);
        glVertex3f(25.3 * cos(i * 0.01745), 0, 25.3 * sin(i * 0.01745));
        glVertex3f(27.3 * cos(i * 0.01745), 0, 27.3 * sin(i * 0.01745));
        glEnd();
    }

    draw_circle(25.3, 1);   //2
    draw_circle(24.8, 1);   //0.5

    glLineWidth(2);
    for (int t = 0; t < 3; t++) {            //星星陣
        glBegin(GL_POLYGON);
        for (int i = (t * 30); i < 360; i += 90) {
            glVertex3f(24.8 * cos(i * 0.01745), 0, 24.8 * sin(i * 0.01745));
        }
        glEnd();
    }

    draw_circle(17, 1);       //6

    draw_circle(14, 4);
    for (float t = 0; t < 12; t += 0.5) {     //月亮陣
        glLineWidth(4);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 1440; ++i)
            glVertex3f((14 - t * 0.2) * cos(i * 0.01745), 0, (t * 0.23) + (14 - t * 0.2) * sin(i * 0.01745));
        glEnd();
    }

    for (int i = 0, j = 0; i < 360; i += 15, j++) { //太陽陣
        if (i % 30 == 0) {
            glLineWidth(1);
            glBegin(GL_LINES);
            glVertex3f(15.8 * cos((i + 5) * 0.01745), 0, 15.8 * sin((i + 5) * 0.01745));
            glVertex3f(8.2 * cos((i + 5) * 0.01745), 0, 8.2 * sin((i + 5) * 0.01745));
            glVertex3f(15.8 * cos((i - 5) * 0.01745), 0, 15.8 * sin((i - 5) * 0.01745));
            glVertex3f(8.2 * cos((i - 5) * 0.01745), 0, 8.2 * sin((i - 5) * 0.01745));
            glEnd();
        }
        if (i % 60 == 0) continue;
        if (j % 4 == 2) glLineWidth(1);
        else glLineWidth(2);
        glBegin(GL_LINES);
        glVertex3f(17 * cos(i * 0.01745), 0, 17 * sin(i * 0.01745));
        glVertex3f(8.2 * cos(i * 0.01745), 0, 8.2 * sin(i * 0.01745));
        glEnd();
    }
    draw_circle(8.2, 2);         //9.5
    draw_circle(7.7, 2);          //0.5
}
void draw_scene1() {


    draw_floor();

    glPushMatrix();
    glTranslatef(30, 0, 30);       //法陣的 lcs
    draw_magic_field();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(30, 10, 30);     //法仗的 lcs  飄在空中
    draw_magic_wand();
    glPopMatrix();
}
void draw_cylinder(int up,int down,int height){
    if (cylind == NULL) {
        cylind = gluNewQuadric();
    }
    /*--- Draw a cylinder ---*/
    gluCylinder(cylind, up, down, /* radius of top and bottom circle */
        height,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        4);               /* Divide it into 3 sections */
}

void draw_arrow(void)
{
    if (cylind == NULL) {
        cylind = gluNewQuadric();
    }
    /*--- Draw a cylinder ---*/

    gluCylinder(cylind, 0.6, 0.0, /* radius of top and bottom circle */
        1.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        4);               /* Divide it into 3 sections */

}

void draw_coord_sys(void)
{

    //draw coord. sys. in line or fill mode.
    if (polygonMode == LINE)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Draw a sphere to show the origin
    if (sphere == NULL) {
        sphere = gluNewQuadric();
    }
    glColor3f(1.0, 1.0, 1.0);
    gluSphere(sphere, 0.5,   /* radius=0.5 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 12 stacks */
//Draw the x-axis in RED.
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glRotatef(90.0, 0, 1, 0);   // The x-axis
    //draw_cylinder();
    glTranslatef(0, 0, 3.5);
    // glColor3f(1.0, 1.0, 0.0);
     //draw_arrow();
    glPopMatrix();
    //Draw the Y-axis in GREEN.
    glColor3f(0.0, 1.0, 0.0);
    glPushMatrix();  //The y-axis
    glRotatef(-90.0, 1, 0, 0);
    //draw_cylinder();
    glTranslatef(0, 0, 3.5);
    draw_arrow();
    glPopMatrix();
    //Draw the z-axis in BLUE.
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    //draw_cylinder();   //The z-axis
    glTranslatef(0, 0, 3.5);
    draw_arrow();
    glPopMatrix();
}

void display()
{
    printf("display");
    /*Clear previous frame and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*----Define the current eye position and the eye-coordinate system---*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //動作
    //相機位置    相機對準的位置   相機向上的角度
    
    //gluLookAt(45.0, 30.0, 60.0,       25.0, 10.0, 0.0,         0.0, 1.0, 0.0);      //動作
    gluLookAt(40.0, 70.0, 55.0, 25.0, 0.0, 25.0, 0.0, 1.0, 0.0);                  //場景


    /*-------Draw the floor & coordinate system----*/
    //draw_coord_sys();
    //draw_magic_field();
    
    draw_scene1();

    /*-------Draw the 1st cube ----*/
    //move to the base position
    glTranslatef(pos[0], pos[1], pos[2]);

    //glPushMatrix();
    //glScalef(1.0, 4.0, 1.0);
    //draw_cube();
    //glPopMatrix();

    glutSwapBuffers();
    return;

    /*-------Draw another cube---*/
    glPushMatrix();
    glTranslatef(0.0, 0.0, 3.0);
    glTranslatef(0.5, 0.50, 2.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glTranslatef(-0.5, -0.50, -2.0);
    glScalef(1.0, 1.0, 4.0);
    draw_cube();
    glPopMatrix();

    /*-------Draw the third cube ---*/
    glPushMatrix();
    glTranslatef(3.0, 0.0, 0.0);
    glScalef(4.0, 1.0, 1.0);
    glTranslatef(0.5, 0.5, 0.5);
    glRotatef(angle, 1.0, 0.0, 0.0);
    glTranslatef(-0.5, -0.5, -0.5);
    draw_cube();
    glPopMatrix();

    /*-------Swap the back buffer to the front --------*/
    glutSwapBuffers();
}

void my_reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /*
     if (w>h)
       glOrtho(-10.0, 10.0, -10.0*(float)h/(float)w, 10.0*(float)h/(float)w,
                0.50, 30.0);
     else
       glOrtho(-10.0*(float)w/(float)h, 10.0*(float)w/(float)h, -10.0, 10.0,
               0.50, 30.0);

   */
    //glOrtho(-40.0, 40.0,   -30.0, 20.0,    0.0, 120);  //動作
    glOrtho(-50.0 , 50.0 , -40.0, 40.0, 0.0, 120);       //場景
    width = w; height = h;
}


void my_order(unsigned char key, int x, int y)
{
    if (key == 'Q' || key == 'q') exit(0);
    if (key == 'Z') {
        if (pos[2] < 9.0) pos[2] += 1.0;
    }
    else if (key == 'z') {
        if (pos[2] > 0.0) pos[2] -= 1.0;
    }
    if (key == 'x') {
        if (pos[0] > 0.0) pos[0] -= 1.0;
    }
    else if (key == 'X' && pos[0] < 50.0) pos[0] += 1.0;
    else if (key == 'R' || key == 'r') angle += 3.0;
    else if (key == 'L' || key == 'l') angle -= 3.0;
    if (key == '1') polygonMode = FILL;
    else if (key == '2') polygonMode = LINE;

    if (angle >= 360.0) angle -= 360.0;
    else if (angle < 0.0) angle += 360.0;

    display();
}
void motion_func(int  x, int y) {};
void passive_motion_func(int x, int y) {};
void mouse_func(int button, int state, int x, int y) {};
void idle_func(void) {};


/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char** argv)
{
    /*-----Initialize the GLUT environment-------*/
    glutInit(&argc, argv);

    /*-----Depth buffer is used, be careful !!!----*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(500, 500);
    glutCreateWindow("Robot");

    myinit();      /*---Initialize other state varibales----*/

    /*----Associate callback func's whith events------*/
    glutDisplayFunc(display);

    glutReshapeFunc(my_reshape);
    glutKeyboardFunc(my_order);
    glutIdleFunc(idle_func);
    glutMouseFunc(mouse_func);  /* Mouse Button Callback func */
    glutMotionFunc(motion_func);/* Mouse motion event callback func */
    glutPassiveMotionFunc(passive_motion_func);
    glutMainLoop();
}

