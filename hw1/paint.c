#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#define    SIZEX   2000   //畫面大小 ->開大一點 reshape後load才會正常
#define    SIZEY   2000

//file 相關常數
#define    MY_SAVE  1
#define    MY_LOAD  2      
#define    MY_BLEND 3
#define    MY_CLEAR 4
#define    MY_QUIT  5 

//obj_type 相關常數
#define    POINT   1
#define    LINE    2
#define    POLYGON 3
#define    CIRCLE  4
#define    TRIANGLE   5
#define    RECTANGLE   6
#define    TEXT   7
#define    PAINT 8

//text mode & circle mode 相關常數
#define RANDOM 1
#define FIXED 2
#define CURSOR 3  //for 游標的圓形

//paint mode 相關常數
#define NORMAL 1  
#define DOT 2

//color 相關常數
#define RED 1
#define ORANGE 2
#define YELLOW 3
#define GREEN 4
#define BLUE 5
#define DBLUE 6
#define PURPLE 7
#define BLACK 8

typedef    int   menu_t;
menu_t     top_m, text_mode_m, text_font_m;

int        height = 800, width = 1000;       //螢幕大小
unsigned char  image[SIZEX * SIZEY][4];      //Image data in main memory for user save
unsigned char myImage[SIZEX * SIZEY][4];     //for 一些功能需要的save
unsigned char gridImage[SIZEX * SIZEY][4];   //存純網格的圖案

int        pos_x = -1, pos_y = -1;           //位置
int        obj_type = PAINT;                 //預設是筆筆
int        first = 0;                        //flag of initial points for lines and paint
int        vertex[128][2];                   //coords of vertices 
int        side = 0;                         //num of sides of polygon
float      pnt_size = 10.0;                  //筆刷大小
//int menuShow = 0;

int color = BLACK;                           //初始顏色預設黑
int text_mode = RANDOM;                      //初始打字模式預設隨機
int paint_mode = NORMAL;                     //初始畫筆預設normal
int text_font = GLUT_BITMAP_HELVETICA_18;    //字型:GLUT_BITMAP_HELVETICA_18  GLUT_BITMAP_TIMES_ROMAN_24 GLUT_BITMAP_9_BY_15
char mySyting[1000];                         //打字專用陣列
int stringIndex = 0;                         //打字的index
int fill = 0;                                //是否填滿 0->不填滿  1->填滿

int color_btn[9][5] = {
    //x1,y1,x2,y2,isPress
    {  0,  0,  0, 0,0 },
    { 65, 13,100,25,0 },    //red
    { 105,13,140,25,0 },    //orange
    { 145,13,180,25,0 },    //yellow
    { 185,13,220,25,0 },    //green
    { 225,13,260,25,0 },    //blue
    { 265,13,300,25,0 },    //dblue
    { 305,13,340,25,0 },    //purple
    { 345,13,380,25,1 }     //black
};

int type_btn[9][5] = {
    //x1,y1,x2,y2,isPress
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
    //x1,y1,x2,y2,isPassiveMotion
    {   0,  0,   0,  0, 0},
    {  65, 59, 100, 71, 0},   //save
    { 105, 59, 140, 71, 0},   //load
    { 145, 59, 180, 71, 0},   //blend
    { 185, 59, 220, 71, 0},   //clear
    { 225, 59, 260, 71, 0},   //quit
};

int advance_btn[5][5] = {
    //x1,y1,x2,y2,isPress
    {   0, 0,   0, 0, 0},
    { 485, 13,500,25, 1},    //grid
    { 485, 36,500,48, 1},    //fill
    { 440, 78,455,90, 1},    //paint mode - normal
    { 505, 78,520,90, 0},    //paint mode - dot
};
int outSize[4] = { 65,76,169,92 };          //size最下面那層
int inSize[4] = { 67,78,167,90 };           //size中間那層
int valueSize[4] = { 67,78,72,90 };         //pnt_size + 1 => value_size + 5   最上面那層 (每次重畫中間和上面，達到效果)
int pnt_sizeBG[4] = { 175,76,195,92 };      //秀出現在大小的格子
void grid_line(void);
void mySave(void);
void myLoad(void);
void grid_show_func(int value) {            //是否顯示網格 
    mySave();                               //將現在畫面存在myImage裡
    if (value == 0) {                       //不顯示 -> 將myImage陣列中有格子線的地方塗白
        for (int i = 0; i < SIZEX; i++) {
            for (int j = 0; j < SIZEY; j++) {
                if (myImage[i * width + j][0] == 255 && myImage[i * width + j][1] == 194 && myImage[i * width + j][2] == 222) {
                    myImage[i * width + j][0] = 255;
                    myImage[i * width + j][1] = 255;
                    myImage[i * width + j][2] = 255;
                    //printf("%d %d\n",i,j);
                }
            }
        }
    }
    else {
        for (int i = 0; i < SIZEX; i++) {  //顯示 -> 將myImage的白色的地方照著網格塗一次
            for (int j = 0; j < SIZEY; j++) {
                if (myImage[i * width + j][0] == 255 && myImage[i * width + j][1] == 255 && myImage[i * width + j][2] == 255) {
                    myImage[i * width + j][0] = gridImage[i * width + j][0];
                    myImage[i * width + j][1] = gridImage[i * width + j][1];
                    myImage[i * width + j][2] = gridImage[i * width + j][2];
                }
            }
        }
    }
    myLoad();                            //把myImage load在畫面上
    glFlush();                           //刷新畫面
}
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
        //printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }
}
void color_bar(void) {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "Color\0";
    glRasterPos2i(15, height - 25);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }
    glColor3f(1.0, 240 / 255.0, 250 / 255.0); //框框
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
        //printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = "  line\0";
    glRasterPos2i(type_btn[2][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        //printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = " poly\0";
    glRasterPos2i(type_btn[3][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = " circle\0";
    glRasterPos2i(type_btn[4][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = " triangle\0";
    glRasterPos2i(type_btn[5][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = " rectangle\0";
    glRasterPos2i(type_btn[6][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        //printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = "  text\0";
    glRasterPos2i(type_btn[7][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        //  printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = " paint\0";
    glRasterPos2i(type_btn[8][0], height - 47);
    for (char* i = c; *i != '\0'; i++) {
        //   printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }
}
void file_bar(void) {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "F i le\0";
    glRasterPos2i(17, height - 69);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
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
        //printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = "load\0";
    glRasterPos2i(file_btn[2][0] + 5, height - 69);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = "blend\0";
    glRasterPos2i(file_btn[3][0] + 2, height - 69);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    c = " clear\0";
    glRasterPos2i(file_btn[4][0], height - 69);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    glColor3f(0.0, 0.0, 0.0);
    c = " quit\0";
    glRasterPos2i(file_btn[5][0] + 2, height - 69);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    change_color(color);
}
void size_bar(void) {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "Size\0";
    glRasterPos2i(15, height - 91);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
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
void advance_setting_bar(void) {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "Grid\0";
    glRasterPos2i(440, height - 25);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }
    c = "Fi l l\0";
    glRasterPos2i(440, height - 47);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }
    c = "Paint Mode\0";
    glRasterPos2i(440, height - 69);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }
    c = "normal\0";
    glRasterPos2i(460, height - 88);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }
    c = "dot\0";
    glRasterPos2i(525, height - 88);
    for (char* i = c; *i != '\0'; i++) {
        // printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    for (int i = 1; i <= 4; i++) {
        glBegin(GL_QUADS);
        glVertex2i(advance_btn[i][0], height - advance_btn[i][1]);
        glVertex2i(advance_btn[i][2], height - advance_btn[i][1]);
        glVertex2i(advance_btn[i][2], height - advance_btn[i][3]);
        glVertex2i(advance_btn[i][0], height - advance_btn[i][3]);
        glEnd();
    }
    glColor3f(0.0, 0.0, 0.0);
    for (int i = 1; i <= 4; i++) {
        if (advance_btn[i][4]) {
            glLineWidth(2);
            glBegin(GL_LINES);
            glVertex2i(advance_btn[i][0] + 3, height - advance_btn[i][1] - 5);
            glVertex2i(advance_btn[i][0] + 6, height - advance_btn[i][3]);
            glVertex2i(advance_btn[i][0] + 6, height - advance_btn[i][3]);
            glVertex2i(advance_btn[i][2] - 1, height - advance_btn[i][1]);
            glEnd();
        }
    }
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
    advance_setting_bar();
    change_color(color);
}
void file_func(int value);
void draw_circle(int mode, int x);
void mySave() {
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
        myImage);
    /* 255 194 222
    for (int i = 0; i <= 10; i++) {
        for (int j = height; j >= height - 10; j--) {
            printf("%d %d %d\n",myImage[i][j][0], myImage[i][j][1], myImage[i][j][2]);
        }
    }*/
}
void myLoad() {
    glRasterPos2i(0, 0);
    glDrawPixels(width, height - 100,
        GL_RGBA, GL_UNSIGNED_BYTE,
        myImage);
}
void grid_line() {
    int gridSize = 15;
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    for (int i = 0; i <= height; i += gridSize) {
        int tp = 1;
        if (i % (gridSize * 5) == 0) tp = 2;
        glLineWidth(tp);     /* Define line width */
        glBegin(GL_LINES);    /* Draw the line */
        glVertex2f(0, height - i - 100);
        glVertex2f(width, height - i - 100);
        glEnd();
    }
    for (int i = 0; i <= width; i += gridSize) {
        int tp = 1;
        if (i % (gridSize * 5) == 0) tp = 2;
        glLineWidth(tp);     /* Define line width */
        glBegin(GL_LINES);    /* Draw the line */
        glVertex2f(i, height - 100);
        glVertex2f(i, 0);
        glEnd();
    }
    //將網格存起來
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
        gridImage);
    advance_btn[1][4] = 1;//將網格設為開啟
}
int firstIn = 1;  //重新存一次grid
int displayOccur = 0;
void display_func(void)
{
    /*------------------------------------------------------------
     * Callback function for display, redisplay, expose events
     * Just clear the window again
     */
    printf("myDisplay\n");
    //myLoad();
    //cnt = 0;
    //printf("display  %d\n", cnt++);
    /* define window background color */
    //glClear(GL_COLOR_BUFFER_BIT);
    myMenu();
    if (firstIn) {
        firstIn = 0;
        grid_line();
        mySave();
    }
    glFlush();
    displayOccur = 1;
    //myLoad();
}
void idle_func(void) {
    if (obj_type == PAINT && menuShow == 0) {
        if (pos_y >= 110) {
            //glutSetCursor(GLUT_CURSOR_NONE);
            mySave();
            draw_circle(CURSOR, 0);
            glFlush();
            myLoad();
        }
        else {
            glutSetCursor(GLUT_CURSOR_INHERIT);
        }
    }
    /*if (displayOccur) {
        printf("idle\n");
        mySave();
        displayOccur = 0;
    }*/
    //if(firstIn)printf("idle\n");
    //mySave();
}
/*-------------------------------------------------------------
 * reshape callback function for window.
 */
void my_reshape(int new_w, int new_h)
{
    printf("myreshape\n");
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
    firstIn = 1;
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
    glPolygonMode(GL_FRONT, fill);
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
void draw_circle(int mode, int x)
{
    int tp = pnt_size;
    int tp2 = 0;
    if (mode == CURSOR) {
        tp = 5;
    }
    else if (mode == RANDOM) {
        tp = abs(pos_x - x);
        if (fill == GL_LINE) {
            tp2 = tp - pnt_size;
        }
    }
    static GLUquadricObj* mycircle = NULL;

    if (mycircle == NULL) {
        mycircle = gluNewQuadric();
        gluQuadricDrawStyle(mycircle, GLU_FILL);
    }
    glPushMatrix();
    glTranslatef(pos_x, height - pos_y, 0.0);
    gluDisk(mycircle,
        tp2,           /* inner radius=0.0 */
        tp,          /* outer radius=10.0 */
        16,            /* 16-side polygon */
        3);
    glPopMatrix();
}

/*------------------------------------------------------------
 * Callback function handling mouse-press events
 */
void mouse_func(int button, int state, int x, int y) {
    //printf("%d %d:::\n", button, state);
    //printf("%d,%d\n",x,y);
    pos_x = x;
    pos_y = y;
    if (button == 1 && state == 0) {                      //按下左鍵
        menuShow = 1;
        printf("PRESS LEFT");
    }
    if (button == 0 && state == 1 && obj_type == PAINT) { //左鍵放開 (代碼是錯的...)
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
        //printf("size\n");
        pnt_size = (x - inSize[0]) / 5.0;
        if (pnt_size < 1) pnt_size = 1;
        change_size(x - inSize[0]);
        change_color(color);
    }
    //advance setting
    for (int i = 1; i <= 4; i++) {
        if (x >= advance_btn[i][0] && x <= advance_btn[i][2] && y >= advance_btn[i][1] && y <= advance_btn[i][3]) {
            if (advance_btn[i][4] == 0) {
                advance_btn[i][4] = 1;
                if (i == 1)grid_show_func(1);
                else if (i == 2) fill = GL_FILL;
                else if (i == 3) {
                    paint_mode = NORMAL;
                    advance_btn[4][4] = 0;
                }
                else if (i == 4) {
                    paint_mode = DOT;
                    advance_btn[3][4] = 0;
                }
            }
            else {
                if (i == 3 || i == 4) continue;      //paint mode不能取消 一定要勾
                advance_btn[i][4] = 0;
                if (i == 1)grid_show_func(0);
                else if (i == 2) fill = GL_LINE;
            }
        }
    }
    if (y >= 100) {
        switch (obj_type) {
        case POINT:
            glPointSize(pnt_size);     /*  Define point size */
            glBegin(GL_POINTS);     /*  Draw a point */
            glVertex2f(x, height - y);
            glEnd();
            break;
        case LINE:
            mySave();
            pos_x = x; pos_y = y;
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
            mySave();
            //draw_circle();
            break;
        case TRIANGLE:
            pos_x = x; pos_y = y;
            mySave();
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
    pos_x = x;
    pos_y = y;
    //printf("%d %d\n", pos_x, pos_y);
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
                    draw_circle(FIXED, 0);
                    pos_x = x; pos_y = y;
                }
            }
        }
        else if (obj_type == LINE) {
            myLoad();
            glLineWidth(pnt_size);
            glBegin(GL_LINES);
            glVertex2f(pos_x, height - pos_y);
            glVertex2f(x, height - y);
            glEnd();
        }
        else if (obj_type == RECTANGLE) {
            myLoad();
            glLineWidth(pnt_size);
            glPolygonMode(GL_FRONT, fill);
            glBegin(GL_POLYGON);
            if ((pos_x <= x && pos_y <= y) || (x <= pos_x && y <= pos_y)) {  //逆時針 到右下 或 左上
                glVertex2i(pos_x, height - pos_y);
                glVertex2i(pos_x, height - y);
                glVertex2i(x, height - y);
                glVertex2i(x, height - pos_y);
            }
            else {  //到右上 或 左上
                glVertex2i(pos_x, height - pos_y);
                glVertex2i(x, height - pos_y);
                glVertex2i(x, height - y);
                glVertex2i(pos_x, height - y);
            }
            glEnd();
            glFinish();
        }
        else if (obj_type == TRIANGLE) {
            myLoad();
            glLineWidth(pnt_size);
            glPolygonMode(GL_FRONT, fill);
            glBegin(GL_POLYGON);
            if ((pos_x <= x && pos_y <= y) || (x <= pos_x && y <= pos_y)) {  //逆時針 到右下 或 左上
                glVertex2i(pos_x, height - pos_y);
                glVertex2i(2 * pos_x - x, height - y);
                glVertex2i(x, height - y);
            }
            else {  //到右上 或 左上
                glVertex2i(pos_x, height - pos_y);
                glVertex2i(x, height - y);
                glVertex2i(2 * pos_x - x, height - y);
            }
            glEnd();
        }
        else if (obj_type == CIRCLE) {
            myLoad();
            draw_circle(RANDOM, x);
        }
    }
    glFinish();
}

/*--------------------------------------------------------
 * procedure to clear window
 */
void init_window(void)
{
    printf("init window\n");
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
    printf("init fun\n");
    glReadBuffer(GL_FRONT);
    glDrawBuffer(GL_FRONT);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    printf("end init fun\n");
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
        glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE,
            image);
        /*
        for (i = 0; i < SIZEX; i++)   // Assign 0 opacity to black pixels
            for (j = 0; j < SIZEY; j++)
                if (image[i * width + j][0] == 0 &&
                    image[i * width + j][1] == 0 &&
                    image[i * width + j][2] == 0) {
                    image[i * width + j][3] = 0;
                    //printf("%d %d\n",i,j);
                }
                else image[i * width + j][3] = 127; // Other pixels have A=127
            */
    }
    else if (value == MY_LOAD) { /* Restore the saved image */
        glRasterPos2i(0, 0);
        glDrawPixels(SIZEX, SIZEY,
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
void fill_show_func(int value) {
    printf("0\n");
    //menuShow = 0;
    fill = value;
}
void text_mode_func(int value) {
    printf("0\n");
    // menuShow = 0;
    text_mode = value;
}

void paint_mode_func(int value) {
    printf("0\n");
    // menuShow = 0;
    paint_mode = value;
}

void text_font_func(int value) {
    printf("0\n");
    // menuShow = 0;
    text_font = value;
    stringIndex = 0;
}

/*---------------------------------------------------------------
 * Callback function for top menu. Do nothing.
 */
void top_menu_func(int value)
{
    printf("1\n");
    //menuShow = 1;
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
    glutIdleFunc(idle_func);
    glutKeyboardFunc(keyboard); /* Callback func for keyboard event */

    glutMouseFunc(mouse_func);  /* Mouse Button Callback func */
    glutMotionFunc(motion_func);/* Mouse motion event callback func */
    glutPassiveMotionFunc(passive_motion_func);

    text_mode_m = glutCreateMenu(text_mode_func);
    glutAddMenuEntry("random", RANDOM);
    glutAddMenuEntry("fixed", FIXED);

    text_font_m = glutCreateMenu(text_font_func);   //GLUT_BITMAP_HELVETICA_18  GLUT_BITMAP_TIMES_ROMAN_24 GLUT_BITMAP_9_BY_15
    glutAddMenuEntry("default", GLUT_BITMAP_9_BY_15);
    glutAddMenuEntry("Times Roman", GLUT_BITMAP_TIMES_ROMAN_24);
    glutAddMenuEntry("Helvetica", GLUT_BITMAP_HELVETICA_18);

    top_m = glutCreateMenu(top_menu_func);/* Create top menu */
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
