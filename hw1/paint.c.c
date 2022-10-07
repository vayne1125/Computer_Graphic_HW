/******************************************************************
 * This program illustrates the fundamental instructions for handling
 * mouse and keyboeard events as well as menu buttons.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#define    SIZEX   1000
#define    SIZEY   1000

 //file
#define    MY_QUIT  5
#define    MY_CLEAR 4
#define    MY_SAVE  1
#define    MY_BLEND 3
#define    MY_LOAD  2

//obj_type
#define    POINT   1
#define    LINE    2
#define    POLYGON 3
#define    CIRCLE  4
#define    TRIANGLE   5
#define    RECTANGLE   6
#define    TEXT   7
#define    PAINT 8

//text mode
#define RANDOM 1
#define FIXED 2

//paint mode
#define NORMAL 1
#define DOT 2

//color
#define RED 1
#define ORANGE 2
#define YELLOW 3
#define GREEN 4
#define BLUE 5
#define DBLUE 6
#define PURPLE 7
#define BLACK 8

//motion
#define MOTION 1
#define NOT_MOTION 2

typedef    int   menu_t;
menu_t     top_m, text_mode_m, paint_mode_m, text_font_m;

int        height = 800, width = 1000;
unsigned char  image[SIZEX * SIZEY][4];  /* Image data in main memory */
unsigned char myImage[SIZEX * SIZEY][4];
int        pos_x = -1, pos_y = -1;
int        obj_type = PAINT;
int        first = 0;      /* flag of initial points for lines and curve,..*/
int        vertex[128][2]; /*coords of vertices */
int        side = 0;         /*num of sides of polygon */
float      pnt_size = 1.0;

int isMotion = NOT_MOTION;
int color = BLACK;
int text_mode = RANDOM;
int paint_mode = NORMAL;
int text_font = GLUT_BITMAP_HELVETICA_18;  //GLUT_BITMAP_HELVETICA_18  GLUT_BITMAP_TIMES_ROMAN_24 GLUT_BITMAP_9_BY_15
char mySyting[1000];
int stringIndex = 0;

int color_btn[9][5] = {
    //x1,y1,x2,y2,isMotion
    {  0,  0,  0, 0,0 },
    { 65, 13,100,25,0 },    //red
    { 105,13,140,25,0 },    //orange
    { 145,13,180,25,0 },    //yellow
    { 185,13,220,25,0 },    //green
    { 225,13,260,25,0 },    //blue
    { 265,13,300,25,0 },    //dblue
    { 305,13,340,25,0 },    //purple
    { 345,13,380,25,0 }     //black
};

int type_btn[9][5] = {
    {   0,  0,   0,  0, 0},
    {  65, 36, 100, 48, 0}, //point
    { 105, 36, 140, 48, 0}, //line
    { 145, 36, 180, 48, 0}, //poly
    { 185, 36, 225, 48, 0}, //circle
    { 230, 36, 278, 48, 0}, //triangle
    { 283, 36, 341, 48, 0}, //rectangle
    { 346, 36, 381, 48, 0}, //text
    { 386, 36, 421, 48, 1}  //paint
};

int file_btn[6][5] = {
    {   0,  0,   0,  0, 0},
    {  65, 59, 100, 71, 0},   //save
    { 105, 59, 140, 71, 0},   //load
    { 145, 59, 180, 71, 0},   //blend
    { 185, 59, 220, 71, 0},   //clear
    { 225, 59, 260, 71, 0},   //quit
};

int outSize[4] = { 65,76,169,92 };
int inSize[4] = { 67,78,167,90 };
int valueSize[4] = { 67,78,72,90 }; //pnt_size + 1 => value_size + 5
int pnt_sizeBG[4] = { 175,76,195,92 };

void change_color(int value) {
    //color = value;
    switch (value)
    {
    case RED:
        glColor3f(1.0, 0.0, 0.0);
        break;
    case ORANGE:
        glColor3f(1.0, 129 / 255.0, 66 / 255.0);
        break;
    case YELLOW:
        glColor3f(1.0, 231 / 255.0, 112 / 255.0);
        break;
    case GREEN:
        glColor3f(0.0, 1.0, 0.0);
        break;
    case BLUE:
        glColor3f(77 / 255.0, 1.0, 1.0);
        break;
    case DBLUE:
        glColor3f(41 / 255.0, 41 / 255.0, 1.0);
        break;
    case PURPLE:
        glColor3f(156 / 255.0, 51 / 255.0, 1.0);
        break;
    case BLACK:
        glColor3f(0, 0, 0.0);
        break;
    default:
        break;
    }
}
void change_size(int value) {
    char c[5];
    //上面那條
    glColor3f(1.0, 240 / 255.0, 250 / 255.0);
    glBegin(GL_QUADS);
    glVertex2i(inSize[0], height - inSize[1]);
    glVertex2i(inSize[2], height - inSize[1]);
    glVertex2i(inSize[2], height - inSize[3]);
    glVertex2i(inSize[0], height - inSize[3]);
    glEnd();
    if (value < 5) value = 5;
    valueSize[2] = valueSize[0] + value;
    //調值
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    glBegin(GL_QUADS);
    glVertex2i(valueSize[0], height - valueSize[1]);
    glVertex2i(valueSize[2], height - valueSize[1]);
    glVertex2i(valueSize[2], height - valueSize[3]);
    glVertex2i(valueSize[0], height - valueSize[3]);
    glEnd();

    //數字底色
    glColor3f(1.0, 240 / 255.0, 250 / 255.0);
    //glColor3f(1, 0, 1);
    glBegin(GL_QUADS);
    glVertex2i(pnt_sizeBG[0], height - pnt_sizeBG[1]);
    glVertex2i(pnt_sizeBG[2], height - pnt_sizeBG[1]);
    glVertex2i(pnt_sizeBG[2], height - pnt_sizeBG[3]);
    glVertex2i(pnt_sizeBG[0], height - pnt_sizeBG[3]);
    glEnd();

    //數字
    int tp = value / 5;
    _itoa(tp, c, 10);
    glColor3f(0, 0, 0);
    glRasterPos2i(pnt_sizeBG[0], height - 91);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }
}
void color_bar(void) {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "Color\0";
    glRasterPos2i(15, height - 25);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }
    glColor3f(1.0, 240 / 255.0, 250 / 255.0);
    for (int i = 1; i <= 8; i++) {
        glBegin(GL_QUADS);
        glVertex2i(color_btn[i][0], height - color_btn[i][1]);
        glVertex2i(color_btn[i][2], height - color_btn[i][1]);
        glVertex2i(color_btn[i][2], height - color_btn[i][3]);
        glVertex2i(color_btn[i][0], height - color_btn[i][3]);
        glEnd();
    }
    for (int i = 1; i <= 8; i++) {
        change_color(i);
        if (color_btn[i][4]) {
            glBegin(GL_QUADS);
            glVertex2i(color_btn[i][0] + 2, height - color_btn[i][1] - 2);
            glVertex2i(color_btn[i][2] - 2, height - color_btn[i][1] - 2);
            glVertex2i(color_btn[i][2] - 2, height - color_btn[i][3] + 2);
            glVertex2i(color_btn[i][0] + 2, height - color_btn[i][3] + 2);
            glEnd();
        }
        else {
            glBegin(GL_QUADS);
            glVertex2i(color_btn[i][0], height - color_btn[i][1]);
            glVertex2i(color_btn[i][2], height - color_btn[i][1]);
            glVertex2i(color_btn[i][2], height - color_btn[i][3]);
            glVertex2i(color_btn[i][0], height - color_btn[i][3]);
            glEnd();
        }
    }
    change_color(color);
}
void type_bar(void) {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "Type\0";
    glRasterPos2i(15, height - 47);
    for (char* i = c; *i != '\0'; i++) {
        //printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }

    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    for (int i = 1; i <= 8; i++) {
        if (type_btn[i][4] == 1) glColor3f(208 / 255.0, 159 / 255.0, 159 / 255.0);
        else glColor3f(1.0, 194 / 255.0, 222 / 255.0);
        glBegin(GL_QUADS);
        glVertex2i(type_btn[i][0], height - type_btn[i][1]);
        glVertex2i(type_btn[i][2], height - type_btn[i][1]);
        glVertex2i(type_btn[i][2], height - type_btn[i][3]);
        glVertex2i(type_btn[i][0], height - type_btn[i][3]);
        glEnd();
    }

    glColor3f(0.0, 0.0, 0.0);
    c = " point\0";
    glRasterPos2i(type_btn[1][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = "  line\0";
    glRasterPos2i(type_btn[2][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = " poly\0";
    glRasterPos2i(type_btn[3][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = " circle\0";
    glRasterPos2i(type_btn[4][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = " triangle\0";
    glRasterPos2i(type_btn[5][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = " rectangle\0";
    glRasterPos2i(type_btn[6][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = "  text\0";
    glRasterPos2i(type_btn[7][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = " paint\0";
    glRasterPos2i(type_btn[8][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }
}
void file_bar(void) {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "File\0";
    glRasterPos2i(17, height - 69);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }
    for (int i = 1; i <= 5; i++) {
        if (file_btn[i][4] == 1) glColor3f(208 / 255.0, 159 / 255.0, 159 / 255.0);
        else glColor3f(1.0, 194 / 255.0, 222 / 255.0);
        glBegin(GL_QUADS);
        glVertex2i(file_btn[i][0], height - file_btn[i][1]);
        glVertex2i(file_btn[i][2], height - file_btn[i][1]);
        glVertex2i(file_btn[i][2], height - file_btn[i][3]);
        glVertex2i(file_btn[i][0], height - file_btn[i][3]);
        glEnd();
    }
    //save

    glColor3f(0.0, 0.0, 0.0);
    c = " save\0";
    glRasterPos2i(file_btn[1][0], height - 69);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = "load\0";
    glRasterPos2i(file_btn[2][0] + 5, height - 69);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = "blend\0";
    glRasterPos2i(file_btn[3][0] + 2, height - 69);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = " clear\0";
    glRasterPos2i(file_btn[4][0], height - 69);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    glColor3f(0.0, 0.0, 0.0);
    c = " quit\0";
    glRasterPos2i(file_btn[5][0] + 2, height - 69);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    change_color(color);
}
void size_bar(void) {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "Size\0";
    glRasterPos2i(15, height - 91);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }
    //下面那條
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    glBegin(GL_QUADS);
    glVertex2i(outSize[0], height - outSize[1]);
    glVertex2i(outSize[2], height - outSize[1]);
    glVertex2i(outSize[2], height - outSize[3]);
    glVertex2i(outSize[0], height - outSize[3]);
    glEnd();

    change_size(pnt_size * 5);
}
void myMenu(void) {
    //glPolygonMode(GL_FRONT, GL_FILL);
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    glBegin(GL_POLYGON);
    glVertex2i(0, height);
    glVertex2i(width, height);
    glVertex2i(width, height - 100);
    glVertex2i(0, height - 100);
    glEnd();

    //rgb(255, 235, 244)
    glColor3f(1.0, 240 / 255.0, 250 / 255.0);
    glBegin(GL_POLYGON);
    glVertex2i(5, height - 5);
    glVertex2i(width - 5, height - 5);
    glVertex2i(width - 5, height - 95);
    glVertex2i(5, height - 95);
    glEnd();

    color_bar();
    type_bar();
    file_bar();
    size_bar();
    change_color(color);
}
void file_func(int value);
void mySave() {
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
        image);
    for (int i = 0; i < width; i++)   /* Assign 0 opacity to black pixels */
        for (int j = 0; j < height; j++)
            if (myImage[i * width + j][0] == 0 &&
                myImage[i * width + j][1] == 0 &&
                myImage[i * width + j][2] == 0) myImage[i * width + j][3] = 0;
            else myImage[i * width + j][3] = 127; /* Other pixels have A=127*/
}
void myLoad() {
    glRasterPos2i(0, 0);
    glDrawPixels(width, height,
        GL_RGBA, GL_UNSIGNED_BYTE,
        myImage);
}
/*------------------------------------------------------------
 * Callback function for display, redisplay, expose events
 * Just clear the window again
 */
void display_func(void)
{
    /* define window background color */
    //glClear(GL_COLOR_BUFFER_BIT);
    myMenu();
    glFlush();
}
/*-------------------------------------------------------------
 * reshape callback function for window.
 */
void my_reshape(int new_w, int new_h)
{
    height = new_h;
    width = new_w;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)width, 0.0, (double)height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();   /*---Trigger Display event for redisplay window*/

    glClearColor(1.0, 1.0, 1.0, 1.0); //定義背景顏色
    glClear(GL_COLOR_BUFFER_BIT);//清除畫面

    myMenu();
}
/*--------------------------------------------------------------
 * Callback function for keyboard event.
 * key = the key pressed,
 * (x,y)= position in the window, where the key is pressed.
 */
void keyboard(unsigned char key, int x, int y)
{
    if (obj_type == TEXT) {
        if (text_mode == RANDOM) {
            glRasterPos2i(x, height - y);
            glutBitmapCharacter(text_font, key);
        }
        else if (text_mode == FIXED) {
            glRasterPos2i(pos_x, height - pos_y);  //按下滑鼠的地方
            mySyting[stringIndex++] = key;
            for (int i = 0; i < stringIndex; i++) {
                glutBitmapCharacter(text_font, mySyting[i]);
            }
        }

        glFlush();
    }
    else {
        if (key == 'Q' || key == 'q') exit(0);
    }
}

/*---------------------------------------------------------
 * Procedure to draw a polygon
 */
void draw_polygon()
{
    int  i;
    glBegin(GL_POLYGON);
    for (i = 0; i < side; i++)
        glVertex2f(vertex[i][0], height - vertex[i][1]);
    glEnd();
    glFinish();
    side = 0;    /* set side=0 for next polygon */
}

/*------------------------------------------------------------
 * Procedure to draw a circle
 */
void draw_circle()
{
    static GLUquadricObj* mycircle = NULL;

    if (mycircle == NULL) {
        mycircle = gluNewQuadric();
        gluQuadricDrawStyle(mycircle, GLU_FILL);
    }
    glPushMatrix();
    glTranslatef(pos_x, height - pos_y, 0.0);
    gluDisk(mycircle,
        0.0,           /* inner radius=0.0 */
        pnt_size,          /* outer radius=10.0 */
        16,            /* 16-side polygon */
        3);
    glPopMatrix();
}

/*------------------------------------------------------------
 * Callback function handling mouse-press events
 */
void mouse_func(int button, int state, int x, int y) {
    //printf("%d %d:::\n", button, state);
    if (button == 0 && state == 1 && obj_type == PAINT) { //右鍵放開 (代碼是錯的...)
        first = 0;
    }
    if (button == 0 && state == 0 && obj_type == TEXT) {
        stringIndex = 0;
    }
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;
    //printf("%d %d\n", x, y);
    //按鈕區 -> 不能畫圖
    //color按鈕
    if (x >= color_btn[1][0] && x <= color_btn[8][2] && y >= color_btn[1][1] && y <= color_btn[1][3]) {
        //color
        for (int i = 1; i <= 8; i++) {
            if (x >= color_btn[i][0] && x <= color_btn[i][2] && y >= color_btn[i][1] && y <= color_btn[i][3]) {
                color = i;
                color_btn[i][4] = 1;
                change_color(i);
            }
            else color_btn[i][4] = 0;
        }
    }
    //type 按鈕
    if (x >= type_btn[1][0] && x <= type_btn[8][2] && y >= type_btn[1][1] && y <= type_btn[1][3]) {
        for (int i = 1; i <= 8; i++) {
            if (x >= type_btn[i][0] && x <= type_btn[i][2] && y >= type_btn[i][1] && y <= type_btn[i][3]) {
                obj_type = i;
                if (i == LINE || i == PAINT) first = 0;
                if (i == POLYGON) side = 0;
                type_btn[i][4] = 1;
            }
            else type_btn[i][4] = 0;
        }
    }
    //file
    if (x >= file_btn[1][0] && x <= file_btn[5][2] && y >= file_btn[1][1] && y <= file_btn[1][3]) {
        for (int i = 1; i <= 5; i++) {
            if (x >= file_btn[i][0] && x <= file_btn[i][2] && y >= file_btn[i][1] && y <= file_btn[i][3]) {
                file_func(i);
            }
        }
    }
    //size
    if (x >= inSize[0] && x <= inSize[2] && y >= inSize[1] && y <= inSize[3]) {
        printf("size\n");
        pnt_size = (x - inSize[0]) / 5.0;
        change_size(x - inSize[0]);
        change_color(color);
    }
    // return;
    if (y >= 100) {
        switch (obj_type) {
        case POINT:
            glPointSize(pnt_size);     /*  Define point size */
            glBegin(GL_POINTS);     /*  Draw a point */
            glVertex2f(x, height - y);
            glEnd();
            break;
        case LINE:
            if (first == 0) {
                first = 1;
                pos_x = x; pos_y = y;
                glPointSize(0);
                glBegin(GL_POINTS);   /*  Draw the 1st point */
                glVertex3f(x, height - y, 0);
                glEnd();
            }
            else {
                first = 0;
                glLineWidth(pnt_size);     /* Define line width */
                glBegin(GL_LINES);    /* Draw the line */
                glVertex2f(pos_x, height - pos_y);
                glVertex2f(x, height - y);
                glEnd();
            }
            break;
        case POLYGON:  /* Define vertices of poly */
            if (side == 0) {
                vertex[side][0] = x; vertex[side][1] = y;
                side++;
            }
            else {
                if (fabs(vertex[0][0] - x) + fabs(vertex[0][1] - y) < 5)
                    draw_polygon();
                else {
                    glLineWidth(pnt_size);
                    glBegin(GL_LINES);
                    glVertex2f(vertex[side - 1][0], height - vertex[side - 1][1]);
                    glVertex2f(x, height - y);
                    glEnd();
                    vertex[side][0] = x;
                    vertex[side][1] = y;
                    side++;
                }
            }
            break;
        case CIRCLE:
            pos_x = x; pos_y = y;
            draw_circle();
            break;
        case TRIANGLE:
            break;
        case RECTANGLE:
            pos_x = x; pos_y = y;
            mySave();
            break;
        case TEXT:
            pos_x = x; pos_y = y;
            break;
        default:
            break;
        }
        glFinish();
    }
}
void passive_motion_func(int x, int y) {
    //file
    if (y <= 100) {
        for (int i = 1; i <= 5; i++) {
            if (x >= file_btn[i][0] && x <= file_btn[i][2] && y >= file_btn[i][1] && y <= file_btn[i][3]) {
                file_btn[i][4] = 1;
            }
            else file_btn[i][4] = 0;
        }
        myMenu();
        glFlush();
    }
}

/*-------------------------------------------------------------
 * motion callback function. The mouse is pressed and moved.
 */
void motion_func(int  x, int y) {
    // if (obj_type != PAINT) return;
    if (y >= 100) {
        if (obj_type == PAINT) {
            if (first == 0) {
                first = 1;
                pos_x = x; pos_y = y;
            }
            else {
                if (paint_mode == NORMAL) {
                    glLineWidth(pnt_size);
                    glBegin(GL_LINES);
                    glVertex3f(pos_x, height - pos_y, 0.0);
                    glVertex3f(x, height - y, 0.0);
                    glEnd();
                    pos_x = x; pos_y = y;
                }
                else if (paint_mode == DOT) {
                    draw_circle();
                    pos_x = x; pos_y = y;
                }
            }
        }
        else if (obj_type == RECTANGLE) {


        }
    }
    glFinish();
}

/*--------------------------------------------------------
 * procedure to clear window
 */
void init_window(void)
{
    /*Do nothing else but clear window to white*/

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)width, 0.0, (double)height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(1.0, 1.0, 1.0, 1.0); //背景
    glClear(GL_COLOR_BUFFER_BIT);//清除畫面

    my_reshape(width, height);
    glFlush();
}


/*------------------------------------------------------
 * Procedure to initialize data alighment and other stuff
 */
void init_func()
{
    glReadBuffer(GL_FRONT);
    glDrawBuffer(GL_FRONT);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

/*------------------------------------------------------------
 * Callback function for top menu.
 */
void file_func(int value)
{
    int i, j;

    if (value == MY_QUIT) exit(0);
    else if (value == MY_CLEAR) init_window();
    else if (value == MY_SAVE) { /* Save current window */
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
            image);
        for (i = 0; i < width; i++)   /* Assign 0 opacity to black pixels */
            for (j = 0; j < height; j++)
                if (image[i * width + j][0] == 0 &&
                    image[i * width + j][1] == 0 &&
                    image[i * width + j][2] == 0) image[i * width + j][3] = 0;
                else image[i * width + j][3] = 127; /* Other pixels have A=127*/
    }
    else if (value == MY_LOAD) { /* Restore the saved image */
        glRasterPos2i(0, 0);
        glDrawPixels(width, height,
            GL_RGBA, GL_UNSIGNED_BYTE,
            image);
    }
    else if (value == MY_BLEND) { /* Blending current image with the saved image */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(0, 0);
        glDrawPixels(width, height,
            GL_RGBA, GL_UNSIGNED_BYTE,
            image);
        glDisable(GL_BLEND);
    }

    glFlush();
}

void size_func(int value)
{
    if (value == 1) {
        if (pnt_size < 10.0) pnt_size += 1.0;
    }
    else {
        if (pnt_size > 1.0) pnt_size = pnt_size - 1.0;
    }
}

void text_mode_func(int value) {
    text_mode = value;
}

void paint_mode_func(int value) {
    paint_mode = value;
}

void text_font_func(int value) {
    text_font = value;
    stringIndex = 0;
}

/*---------------------------------------------------------------
 * Callback function for top menu. Do nothing.
 */
void top_menu_func(int value)
{
}

/*---------------------------------------------------------------
 * Main procedure sets up the window environment.
 */
void main(int argc, char** argv)
{
    int  size_menu;

    glutInit(&argc, argv);    /*---Make connection with server---*/

    glutInitWindowPosition(0, 0);  /*---Specify window position ---*/
    glutInitWindowSize(width, height); /*--Define window's height and width--*/

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); /*---set display mode---*/
    init_func();
    /* Create parent window */
    glutCreateWindow("paint"); ;

    glutDisplayFunc(display_func); /* Associate display event callback func */
    glutReshapeFunc(my_reshape);  /* Associate reshape event callback func */

    glutKeyboardFunc(keyboard); /* Callback func for keyboard event */

    glutMouseFunc(mouse_func);  /* Mouse Button Callback func */
    glutMotionFunc(motion_func);/* Mouse motion event callback func */
    glutPassiveMotionFunc(passive_motion_func);
    paint_mode_m = glutCreateMenu(paint_mode_func);
    glutAddMenuEntry("normal", NORMAL);
    glutAddMenuEntry("dot", DOT);

    text_mode_m = glutCreateMenu(text_mode_func);
    glutAddMenuEntry("random", RANDOM);
    glutAddMenuEntry("fixed", FIXED);
ITM
    text_font_m = glutCreateMenu(text_font_func);   //GLUT_BITMAP_HELVETICA_18  GLUT_BAP_TIMES_ROMAN_24 GLUT_BITMAP_9_BY_15
    glutAddMenuEntry("default", GLUT_BITMAP_9_BY_15);
    glutAddMenuEntry("Times Roman", GLUT_BITMAP_TIMES_ROMAN_24);
    glutAddMenuEntry("Helvetica", GLUT_BITMAP_HELVETICA_18);

    top_m = glutCreateMenu(top_menu_func);/* Create top menu */
    glutAddSubMenu("Paint mode", paint_mode_m);
    glutAddSubMenu("Text mode", text_mode_m);
    glutAddSubMenu("Text font", text_font_m);

    glutAttachMenu(GLUT_RIGHT_BUTTON);    /* associate top-menu with right but*/

    /*---Test whether overlay support is available --*/
    if (glutLayerGet(GLUT_OVERLAY_POSSIBLE)) {
        fprintf(stderr, "Overlay is available\n");
    }
    else {
        fprintf(stderr, "Overlay is NOT available, May encounter problems for menu\n");
    }
    /*---Enter the event loop ----*/
    glutMainLoop();
}

