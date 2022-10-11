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
#define    STICKER 9

//sticker_type 相關常數
#define    NORMAL_PUPU_LB   1
#define    SHY_PUPU_LB   2
#define    NORMAL_PUPU_DB 3
#define    SHY_PUPU_DB 4

//text mode & circle mode 相關常數
#define RANDOM 1
#define FIXED 2
#define CURSOR 3  //for 游標的圓形

//paint mode 相關常數
#define NORMAL 1  
#define DOT 2

//color 相關常數
#define RED     1
#define ORANGE  2
#define YELLOW  3
#define GREEN   4
#define BLUE    5
#define DBLUE   6
#define PURPLE  7
#define BLACK   8
#define BLUE_PU       9
#define GRAY    100       //UI顏色 不可用
#define DARK_PINK   102
#define WHITE         103

typedef    int   menu_t;
menu_t     top_m, text_mode_m, text_font_m;

int        height = 800, width = 1000;       //螢幕大小
unsigned char  image[SIZEX * SIZEY][4];      //Image data in main memory for user save
unsigned char myImage[SIZEX * SIZEY][4];     //for 一些功能需要的save
unsigned char gridImage[SIZEX * SIZEY][4];   //存純網格的圖案

int        pos_x = -1, pos_y = -1;           //位置
int        text_x = -1, text_y = -1;         //text位置 
int        obj_type = PAINT;                 //預設是筆筆
int        first = 0;                        //flag of initial points for lines and paint
int        vertex[128][2];                   //coords of vertices 
int        side = 0;                         //num of sides of polygon
float      pnt_size = 10.0;                  //筆刷大小

int sticker_type = SHY_PUPU_DB;
int color = BLACK;                           //初始顏色預設黑
int text_mode = RANDOM;                      //初始打字模式預設隨機
int paint_mode = NORMAL;                     //初始畫筆預設normal
int text_font = GLUT_BITMAP_TIMES_ROMAN_24;  //字型:GLUT_BITMAP_HELVETICA_18  GLUT_BITMAP_TIMES_ROMAN_24 
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

int advance_btn[9][5] = {
    //x1,y1,x2,y2,isPress
    {   0, 0,   0, 0, 0},
    { 485, 78,500,90, 1},    //grid
    { 565, 78,580,90, 1},    //fill
    { 540, 13,555,25, 1},    //paint mode - normal
    { 605, 13,620,25, 0},    //paint mode - dot
    { 540, 36,555,48, 1},    //text mode - random
    { 605, 36,620,48, 0},    //text mode - fixed
    { 540, 59,555,71, 1},    //text font - 羅馬
    { 640, 59,655,71, 0},    //text font - Hel
};
int outSize[4] = { 65,76,169,92 };          //size最下面那層
int inSize[4] = { 67,78,167,90 };           //size中間那層
int valueSize[4] = { 67,78,72,90 };         //pnt_size + 1 => value_size + 5   最上面那層 (每次重畫中間和上面，達到效果)
int pnt_sizeBG[4] = { 175,76,195,92 };      //秀出現在大小的格子

int pupu_btn[2][5] = {
    {785, 60,  0, 0, 0},   //center 785,50
    {740,25,830,95, 0}
};
//第一層menu
int sticker_menu_1[3][5] = {
    {0,0,0,0,0},
    {828,45,886,60,0},          //pupu
    {828,60,886,75,0},          //word
};
//第二層menu
int sticker_menu_2[5][5] = {
    {0,0,0,0,0},                // =1  -> 滑到那個menu  滑到normal/shy  {0,1,0,0,0}   ，滑到 hi/hello {0,0,1,0,0}
    {886,45,944,60,0},          //normal
    {886,60,944,75,0},          //shy
    {886,60,944,75,0},          //hi
    {886,75,944,90,0},          //hello
};
void grid_line(void);
void mySave(void);
void myLoad(void);
void file_func(int value);
void draw_circle(int mode, int x);

void grid_show_func(int value) {            //是否顯示網格 
    mySave();                               //將現在畫面存在myImage裡
    if (value == 0) {                       //不顯示 -> 將myImage陣列中有格子線的地方塗白
        for (int i = 0; i < SIZEX; i++) {
            for (int j = 0; j < SIZEY; j++) {
                //if (i <= 50 && j <= 50)printf("%d %d %d\n", myImage[i * width + j][0], myImage[i * width + j][1], myImage[i * width + j][2] );
                if (myImage[i * width + j][0] == 208 && myImage[i * width + j][1] == 159 && myImage[i * width + j][2] == 159) {
                    myImage[i * width + j][0] = 255;
                    myImage[i * width + j][1] = 255;
                    myImage[i * width + j][2] = 255;
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
void change_color(int value) {  //設定畫筆顏色
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
    case GRAY:
        glColor3f(248 / 255.0, 241 / 255.0, 241 / 255.0);
        break;
    case DARK_PINK:
        glColor3f(208 / 255.0, 159 / 255.0, 159 / 255.0);
        break;
    case WHITE:
        glColor3f(1, 1, 1);
        break;
    case BLUE_PU:
        glColor3f(153 / 255.0, 224 / 255.0, 1);
        break;
    default:
        break;
    }
}
void change_size(int value) {           //調整畫筆大小
    char c[5];
    //上面那條
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    change_color(GRAY);
    glBegin(GL_QUADS);
    glVertex2i(inSize[0], height - inSize[1]);
    glVertex2i(inSize[2], height - inSize[1]);
    glVertex2i(inSize[2], height - inSize[3]);
    glVertex2i(inSize[0], height - inSize[3]);
    glEnd();
    if (value < 5) value = 5;
    valueSize[2] = valueSize[0] + value;
    //調值
    change_color(DARK_PINK);
    glBegin(GL_QUADS);
    glVertex2i(valueSize[0], height - valueSize[1]);
    glVertex2i(valueSize[2], height - valueSize[1]);
    glVertex2i(valueSize[2], height - valueSize[3]);
    glVertex2i(valueSize[0], height - valueSize[3]);
    glEnd();

    //數字底色
    change_color(GRAY);
    glBegin(GL_QUADS);
    glVertex2i(pnt_sizeBG[0], height - pnt_sizeBG[1]);
    glVertex2i(pnt_sizeBG[2], height - pnt_sizeBG[1]);
    glVertex2i(pnt_sizeBG[2], height - pnt_sizeBG[3]);
    glVertex2i(pnt_sizeBG[0], height - pnt_sizeBG[3]);
    glEnd();

    //數字
    int tp = value / 5;
    _itoa(tp, c, 10);
    change_color(BLACK);
    glRasterPos2i(pnt_sizeBG[0], height - 91);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

}
void color_bar(void) {                   //顏色選擇bar
    change_color(BLACK);
    char* c = "Color\0";                 //顯示"Color"字樣
    glRasterPos2i(15, height - 25);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

    change_color(GRAY);           //框框(選擇時會變小，所以要先用底色重新畫)
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
        if (color_btn[i][4]) {                //被選到要變小
            glBegin(GL_QUADS);
            glVertex2i(color_btn[i][0] + 2, height - color_btn[i][1] - 2);
            glVertex2i(color_btn[i][2] - 2, height - color_btn[i][1] - 2);
            glVertex2i(color_btn[i][2] - 2, height - color_btn[i][3] + 2);
            glVertex2i(color_btn[i][0] + 2, height - color_btn[i][3] + 2);
            glEnd();
        }
        else {
            glBegin(GL_QUADS);                //不然就正常
            glVertex2i(color_btn[i][0], height - color_btn[i][1]);
            glVertex2i(color_btn[i][2], height - color_btn[i][1]);
            glVertex2i(color_btn[i][2], height - color_btn[i][3]);
            glVertex2i(color_btn[i][0], height - color_btn[i][3]);
            glEnd();
        }
    }
}
void type_bar(void) {                //type選擇bar
    change_color(BLACK);
    char* c = "Type\0";              //顯示type字樣
    glRasterPos2i(15, height - 47);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

    for (int i = 1; i <= 8; i++) {
        if (type_btn[i][4]) change_color(DARK_PINK);  //被選到就換色
        else change_color(WHITE);                        //沒被選到就原本顏色
        glBegin(GL_QUADS);
        glVertex2i(type_btn[i][0], height - type_btn[i][1]);
        glVertex2i(type_btn[i][2], height - type_btn[i][1]);
        glVertex2i(type_btn[i][2], height - type_btn[i][3]);
        glVertex2i(type_btn[i][0], height - type_btn[i][3]);
        glEnd();
    }
    //印出選項的名字
    if (type_btn[1][4]) change_color(WHITE);
    else  change_color(BLACK);
    c = " point\0";
    glRasterPos2i(type_btn[1][0], height - 47);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    if (type_btn[2][4]) change_color(WHITE);
    else  change_color(BLACK);
    c = "  line\0";
    glRasterPos2i(type_btn[2][0], height - 47);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    if (type_btn[3][4]) change_color(WHITE);
    else  change_color(BLACK);
    c = " poly\0";
    glRasterPos2i(type_btn[3][0], height - 47);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    if (type_btn[4][4]) change_color(WHITE);
    else  change_color(BLACK);
    c = " circle\0";
    glRasterPos2i(type_btn[4][0], height - 47);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    if (type_btn[5][4]) change_color(WHITE);
    else  change_color(BLACK);
    c = " triangle\0";
    glRasterPos2i(type_btn[5][0], height - 47);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    if (type_btn[6][4]) change_color(WHITE);
    else  change_color(BLACK);
    c = " rectangle\0";
    glRasterPos2i(type_btn[6][0], height - 47);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    if (type_btn[7][4]) change_color(WHITE);
    else  change_color(BLACK);
    c = "  text\0";
    glRasterPos2i(type_btn[7][0], height - 47);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    if (type_btn[8][4]) change_color(WHITE);
    else  change_color(BLACK);
    c = " paint\0";
    glRasterPos2i(type_btn[8][0], height - 47);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
}
void file_bar(void) {            //file選擇bar   
    change_color(BLACK);
    char* c = "F i le\0";
    glRasterPos2i(17, height - 69);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

    for (int i = 1; i <= 5; i++) {
        if (file_btn[i][4] == 1) change_color(DARK_PINK);      //如果passive motion就變色(滑鼠移到上面)
        else change_color(WHITE);
        glBegin(GL_QUADS);
        glVertex2i(file_btn[i][0], height - file_btn[i][1]);
        glVertex2i(file_btn[i][2], height - file_btn[i][1]);
        glVertex2i(file_btn[i][2], height - file_btn[i][3]);
        glVertex2i(file_btn[i][0], height - file_btn[i][3]);
        glEnd();
    }
    //選項名字
    if (file_btn[1][4]) change_color(WHITE);
    else  change_color(BLACK);
    c = " save\0";
    glRasterPos2i(file_btn[1][0], height - 69);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    if (file_btn[2][4]) change_color(WHITE);
    else  change_color(BLACK);
    c = "load\0";
    glRasterPos2i(file_btn[2][0] + 5, height - 69);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    if (file_btn[3][4]) change_color(WHITE);
    else  change_color(BLACK);
    c = "blend\0";
    glRasterPos2i(file_btn[3][0] + 2, height - 69);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    if (file_btn[4][4]) change_color(WHITE);
    else  change_color(BLACK);
    c = " clear\0";
    glRasterPos2i(file_btn[4][0], height - 69);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    if (file_btn[5][4]) change_color(WHITE);
    else  change_color(BLACK);
    c = " quit\0";
    glRasterPos2i(file_btn[5][0] + 2, height - 69);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

}
void size_bar(void) {            //調整size的地方
    change_color(BLACK);
    char* c = "Size\0";
    glRasterPos2i(15, height - 91);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

    //下面那條
    change_color(DARK_PINK);
    glBegin(GL_QUADS);
    glVertex2i(outSize[0], height - outSize[1]);
    glVertex2i(outSize[2], height - outSize[1]);
    glVertex2i(outSize[2], height - outSize[3]);
    glVertex2i(outSize[0], height - outSize[3]);
    glEnd();
    change_size(pnt_size * 5);
}
void advance_setting_bar(void) {         //進階設定的bar

    change_color(BLACK);
    char* c = "Grid\0";
    glRasterPos2i(440, height - 91);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

    c = "Fi l l\0";
    glRasterPos2i(520, height - 91);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

    //paint mode
    c = "Paint Mode\0";
    glRasterPos2i(440, height - 25);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

    c = "normal\0";
    glRasterPos2i(560, height - 22);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    c = "dot\0";
    glRasterPos2i(625, height - 22);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    //text mode
    c = "Text Mode\0";
    glRasterPos2i(440, height - 47);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

    c = "random\0";
    glRasterPos2i(560, height - 47);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    c = "fixed\0";
    glRasterPos2i(625, height - 47);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    //text font
    c = "Text Font\0";
    glRasterPos2i(440, height - 69);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

    c = "Times Roman\0";
    glRasterPos2i(560, height - 69);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    c = "Helvetica\0";
    glRasterPos2i(660, height - 69);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

    change_color(WHITE);
    for (int i = 1; i <= 8; i++) {       //畫勾選方格
        glBegin(GL_QUADS);
        glVertex2i(advance_btn[i][0], height - advance_btn[i][1]);
        glVertex2i(advance_btn[i][2], height - advance_btn[i][1]);
        glVertex2i(advance_btn[i][2], height - advance_btn[i][3]);
        glVertex2i(advance_btn[i][0], height - advance_btn[i][3]);
        glEnd();
    }

    change_color(DARK_PINK);
    for (int i = 1; i <= 8; i++) {
        if (advance_btn[i][4]) {         //被選到就打勾
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
void draw_pupu(int x, int y, int type) { //畫史萊姆
    if (type == NORMAL_PUPU_LB || type == SHY_PUPU_LB)change_color(BLUE);
    else change_color(BLUE_PU);
    static GLUquadricObj* mycircle = NULL;
    if (mycircle == NULL) {
        mycircle = gluNewQuadric();
        gluQuadricDrawStyle(mycircle, GLU_FILL);
    }
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glPushMatrix();
    glTranslatef(x - 10, height - y, 0.0);
    gluDisk(mycircle, 0, 35, 16, 3);          // inner radius ,outer radius ,16-side polygon
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x + 10, height - y, 0.0);
    gluDisk(mycircle, 0, 35, 16, 3);          // inner radius ,outer radius ,16-side polygon
    glPopMatrix();

    //眼睛
    change_color(BLACK);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2i(x - 30, height - y + 10);
    glVertex2i(x - 13, height - y + 7);
    glVertex2i(x - 13, height - y + 7);
    glVertex2i(x - 6, height - y + 3);
    glVertex2i(x + 30, height - y + 10);
    glVertex2i(x + 13, height - y + 7);
    glVertex2i(x + 13, height - y + 7);
    glVertex2i(x + 6, height - y + 3);
    glEnd();

    //腮紅
    if (type == SHY_PUPU_DB || type == SHY_PUPU_LB) {
        change_color(RED);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2i(x - 35, height - y);
        glVertex2i(x - 39, height - y - 7);

        glVertex2i(x - 31, height - y);
        glVertex2i(x - 34, height - y - 10);

        glVertex2i(x - 29, height - y);
        glVertex2i(x - 27, height - y - 7);

        glVertex2i(x + 35, height - y);
        glVertex2i(x + 39, height - y - 7);

        glVertex2i(x + 31, height - y);
        glVertex2i(x + 34, height - y - 10);

        glVertex2i(x + 29, height - y);
        glVertex2i(x + 27, height - y - 7);
        glEnd();
    }
}
void pupu_bar(void) {
    //史萊姆的bar
    change_color(BLACK);
    char* c = "Sticker\0";
    glRasterPos2i(pupu_btn[1][0] + 15, height - 25);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    //點擊範圍
    change_color(GRAY);
    for (int i = 1; i <= 1; i++) {
        glBegin(GL_QUADS);
        glVertex2i(pupu_btn[i][0], height - pupu_btn[i][1]);
        glVertex2i(pupu_btn[i][2], height - pupu_btn[i][1]);
        glVertex2i(pupu_btn[i][2], height - pupu_btn[i][3]);
        glVertex2i(pupu_btn[i][0], height - pupu_btn[i][3]);
        glEnd();
    }

    draw_pupu(pupu_btn[0][0], pupu_btn[0][1], sticker_type);
}
void sticker_menu(void) {

    if (pupu_btn[1][4]) {                   //滑到史萊姆，顯示第一層menu
        for (int i = 1; i <= 2; i++) {
            if (sticker_menu_1[i][4]) change_color(DARK_PINK);
            else change_color(WHITE);
            glBegin(GL_QUADS);
            glVertex2i(sticker_menu_1[i][0], height - sticker_menu_1[i][1]);
            glVertex2i(sticker_menu_1[i][2], height - sticker_menu_1[i][1]);
            glVertex2i(sticker_menu_1[i][2], height - sticker_menu_1[i][3]);
            glVertex2i(sticker_menu_1[i][0], height - sticker_menu_1[i][3]);
            glEnd();

            if (sticker_menu_1[i][4]) change_color(WHITE);                   //箭頭
            else  change_color(DARK_PINK);
            glPolygonMode(GL_FRONT, GL_FILL);
            glPolygonMode(GL_BACK, GL_FILL);
            glBegin(GL_POLYGON);
            glVertex2i(sticker_menu_1[i][2] - 2, height - sticker_menu_1[i][1] - 7.5);
            glVertex2i(sticker_menu_1[i][2] - 10, height - sticker_menu_1[i][1] - 2);
            glVertex2i(sticker_menu_1[i][2] - 10, height - sticker_menu_1[i][3] + 2);
            glEnd();
        }
        if (sticker_menu_1[1][4]) change_color(WHITE);
        else  change_color(BLACK);
        char* c = "light\0";
        glRasterPos2i(sticker_menu_1[1][0] + 5, height - sticker_menu_1[1][1] - 10);
        for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

        if (sticker_menu_1[2][4]) change_color(WHITE);
        else  change_color(BLACK);
        c = "dark\0";
        glRasterPos2i(sticker_menu_1[2][0] + 5, height - sticker_menu_1[2][1] - 10);
        for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    if (sticker_menu_1[1][4]) {                     ////滑到pupu，顯示第二層menu
        for (int i = 1; i <= 2; i++) {
            if (sticker_menu_2[i][4]) change_color(DARK_PINK);
            else change_color(WHITE);
            glBegin(GL_QUADS);
            glVertex2i(sticker_menu_2[i][0], height - sticker_menu_2[i][1]);
            glVertex2i(sticker_menu_2[i][2], height - sticker_menu_2[i][1]);
            glVertex2i(sticker_menu_2[i][2], height - sticker_menu_2[i][3]);
            glVertex2i(sticker_menu_2[i][0], height - sticker_menu_2[i][3]);
            glEnd();
            if (sticker_menu_2[i][4]) change_color(WHITE);                   //箭頭
            else  change_color(DARK_PINK);
            glPolygonMode(GL_FRONT, GL_FILL);
            glPolygonMode(GL_BACK, GL_FILL);
            glBegin(GL_POLYGON);
            glVertex2i(sticker_menu_2[i][2] - 2, height - sticker_menu_2[i][1] - 7.5);
            glVertex2i(sticker_menu_2[i][2] - 10, height - sticker_menu_2[i][1] - 2);
            glVertex2i(sticker_menu_2[i][2] - 10, height - sticker_menu_2[i][3] + 2);
            glEnd();
        }
        if (sticker_menu_2[1][4]) change_color(WHITE);
        else  change_color(BLACK);
        char* c = "normal\0";
        glRasterPos2i(sticker_menu_2[1][0] + 5, height - sticker_menu_2[1][1] - 10);
        for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

        if (sticker_menu_2[2][4]) change_color(WHITE);
        else  change_color(BLACK);
        c = "shy\0";
        glRasterPos2i(sticker_menu_2[2][0] + 5, height - sticker_menu_2[2][1] - 10);
        for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    if (sticker_menu_1[2][4]) {                     //滑到word，顯示第二層menu
        for (int i = 3; i <= 4; i++) {
            if (sticker_menu_2[i][4]) change_color(DARK_PINK);
            else change_color(WHITE);
            glBegin(GL_QUADS);
            glVertex2i(sticker_menu_2[i][0], height - sticker_menu_2[i][1]);
            glVertex2i(sticker_menu_2[i][2], height - sticker_menu_2[i][1]);
            glVertex2i(sticker_menu_2[i][2], height - sticker_menu_2[i][3]);
            glVertex2i(sticker_menu_2[i][0], height - sticker_menu_2[i][3]);
            glEnd();
            if (sticker_menu_2[i][4]) change_color(WHITE);                   //箭頭
            else  change_color(DARK_PINK);
            glPolygonMode(GL_FRONT, GL_FILL);
            glPolygonMode(GL_BACK, GL_FILL);
            glBegin(GL_POLYGON);
            glVertex2i(sticker_menu_2[i][2] - 2, height - sticker_menu_2[i][1] - 7.5);
            glVertex2i(sticker_menu_2[i][2] - 10, height - sticker_menu_2[i][1] - 2);
            glVertex2i(sticker_menu_2[i][2] - 10, height - sticker_menu_2[i][3] + 2);
            glEnd();
        }
        if (sticker_menu_2[3][4]) change_color(WHITE);
        else  change_color(BLACK);
        char* c = "normal\0";
        glRasterPos2i(sticker_menu_2[3][0] + 5, height - sticker_menu_2[3][1] - 10);
        for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);

        if (sticker_menu_2[4][4]) change_color(WHITE);
        else  change_color(BLACK);
        c = "shy\0";
        glRasterPos2i(sticker_menu_2[4][0] + 5, height - sticker_menu_2[4][1] - 10);
        for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }
    glFlush();
}
void myMenu(void) {                    //自定義BAR
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    change_color(DARK_PINK);           //最外框
    glBegin(GL_POLYGON);
    glVertex2i(0, height);
    glVertex2i(width, height);
    glVertex2i(width, height - 100);
    glVertex2i(0, height - 100);
    glEnd();

    change_color(GRAY);          //內框
    glBegin(GL_POLYGON);
    glVertex2i(3, height - 3);
    glVertex2i(width - 3, height - 3);
    glVertex2i(width - 3, height - 97);
    glVertex2i(3, height - 97);
    glEnd();

    color_bar();
    type_bar();
    file_bar();
    size_bar();
    advance_setting_bar();
    pupu_bar();
    sticker_menu();
    change_color(color);                      //因為前面重畫，要將顏色更新回畫筆顏色
}
void mySave() {
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, myImage);  //將畫面存到myImage
}
void myLoad() {
    glRasterPos2i(0, 0);
    glDrawPixels(width, height - 100, GL_RGBA, GL_UNSIGNED_BYTE, myImage);    //將myImage顯示出來
}
void grid_line() {
    int gridSize = 15;
    change_color(DARK_PINK);
    for (int i = 0; i <= height; i += gridSize) {
        int tp = 1;
        if (i % (gridSize * 5) == 0) tp = 2;
        glLineWidth(tp);                            // Define line width
        glBegin(GL_LINES);                          //Draw the line
        glVertex2f(0, height - i - 100);
        glVertex2f(width, height - i - 100);
        glEnd();
    }
    for (int i = 0; i <= width; i += gridSize) {
        int tp = 1;
        if (i % (gridSize * 5) == 0) tp = 2;
        glLineWidth(tp);
        glBegin(GL_LINES);
        glVertex2f(i, height - 100);
        glVertex2f(i, 0);
        glEnd();
    }
    //將網格存起來
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, gridImage);
    advance_btn[1][4] = 1;//將網格設為開啟
}
int firstIn = 1;                         //重新存一次grid
void display_func(void) {         //Callback function for display, redisplay
    //printf("myDisplay\n");
    /* define window background color */
    //glClear(GL_COLOR_BUFFER_BIT);
    if (firstIn) {                     //當畫面大小改變時 要重新繪製grid
        firstIn = 0;
        grid_line();
        mySave();
    }
    myMenu();
    glFlush();
}
void idle_func(void) {                 //閒置時的call back func
    if (obj_type == PAINT) {           //如果當前模式是paint
        if (pos_y >= 100) {            //且在畫筆區，那就使用球球游標
            glutSetCursor(GLUT_CURSOR_NONE); //消失滑鼠
            mySave();
            draw_circle(CURSOR, 0);
            glFlush();
            myLoad();
        }
        else {
            glutSetCursor(GLUT_CURSOR_INHERIT);
        }
    }
    else if (obj_type == STICKER) {         //如果是貼紙模式，那就用史萊姆游標
        if (pos_y >= 100) {
            glutSetCursor(GLUT_CURSOR_NONE); //消失滑鼠
            mySave();
            draw_pupu(pos_x, pos_y, sticker_type);
            glFlush();
            myLoad();
        }
        else {
            glutSetCursor(GLUT_CURSOR_INHERIT);
        }
    }
    else {
        glutSetCursor(GLUT_CURSOR_INHERIT);
    }
}
void my_reshape(int new_w, int new_h) {  //reshape callback function for window.
    printf("myreshape\n");
    height = new_h;                      //將畫面大小更新
    width = new_w;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)width, 0.0, (double)height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();   /*---Trigger Display event for redisplay window*/

    glClearColor(1.0, 1.0, 1.0, 1.0);         //定義背景顏色
    glClear(GL_COLOR_BUFFER_BIT);             //清除畫面
    firstIn = 1;                              //設定display重畫表格
}
void keyboard(unsigned char key, int x, int y) {

    //Callback function for keyboard event.
    //key = the key pressed,
    //(x, y) = position in the window, where the key is pressed

    if (obj_type == TEXT) {                  //如果模式是打字       
        if (text_mode == RANDOM) {           //隨機打字
            glRasterPos2i(x, height - y);
            glutBitmapCharacter(text_font, key);
        }
        else if (text_mode == FIXED) {       //打一行字
            glRasterPos2i(text_x, height - text_y);  //按下滑鼠的地方
            mySyting[stringIndex++] = key;
            for (int i = 0; i < stringIndex; i++) {
                glutBitmapCharacter(text_font, mySyting[i]);
            }
        }
        glFlush();
    }
}
void draw_polygon() {            // Procedure to draw a polygon
    int  i;
    glPolygonMode(GL_FRONT, fill);
    glPolygonMode(GL_BACK, fill);
    glBegin(GL_POLYGON);
    for (i = 0; i < side; i++)
        glVertex2f(vertex[i][0], height - vertex[i][1]);
    glEnd();
    glFinish();
    side = 0;    /* set side=0 for next polygon */
}
void draw_circle(int mode, int x) {    // Procedure to draw a circle
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    int tp = pnt_size;
    int tp2 = 0;
    if (mode == CURSOR) {             //游標 -> 大小固定在5
        tp = 5;
    }
    else if (mode == RANDOM) {       //拉出圓型
        tp = abs(pos_x - x);          //大小是使用者拉的
        if (fill == GL_LINE) {        //不填滿(line)  調整內圓大小
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
        tp2,           // inner radius 
        tp,            // outer radius 
        16,            // 16-side polygon 
        3);
    glPopMatrix();
}
void draw_sticker(void) {
    //畫不同的史萊姆
    switch (sticker_type) {
    case NORMAL_PUPU_LB:
        draw_pupu(pos_x, pos_y, NORMAL_PUPU_LB);
        break;

    case SHY_PUPU_LB:
        draw_pupu(pos_x, pos_y, SHY_PUPU_LB);
        break;

    case NORMAL_PUPU_DB:
        draw_pupu(pos_x, pos_y, NORMAL_PUPU_DB);
        break;

    case SHY_PUPU_DB:
        draw_pupu(pos_x, pos_y, SHY_PUPU_DB);
        break;


    default:
        break;
    }

}
void mouse_func(int button, int state, int x, int y) {          //Callback function handling mouse - press events
    //printf("%d %d:::\n", button, state);
    //printf("%d,%d\n",x,y);
    printf("%d %d\n", x, y);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && obj_type == PAINT) {  //畫完一筆畫 
        first = 0;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && obj_type == TEXT) {   //找新的地方打字
        stringIndex = 0;
    }
    if (button != GLUT_LEFT_BUTTON) return;
    //按鈕區 -> 不能畫圖
    //color按鈕，判斷點擊
    if (x >= color_btn[1][0] && x <= color_btn[8][2] && y >= color_btn[1][1] && y <= color_btn[1][3]) {
        for (int i = 1; i <= 8; i++) {
            if (x >= color_btn[i][0] && x <= color_btn[i][2] && y >= color_btn[i][1] && y <= color_btn[i][3]) {
                color = i;
                color_btn[i][4] = 1;      //被選到
                change_color(i);          //選到就換色
            }
            else color_btn[i][4] = 0;     //沒選到
        }
    }
    //type按鈕，判斷點擊
    if (x >= type_btn[1][0] && x <= type_btn[8][2] && y >= type_btn[1][1] && y <= type_btn[1][3]) {
        for (int i = 1; i <= 8; i++) {
            if (x >= type_btn[i][0] && x <= type_btn[i][2] && y >= type_btn[i][1] && y <= type_btn[i][3]) {
                obj_type = i;                             //設定obj_type
                if (i == LINE || i == PAINT) first = 0;
                if (i == POLYGON) side = 0;
                type_btn[i][4] = 1;                       //被選到
            }
            else type_btn[i][4] = 0;                      //沒選到
        }
    }
    //file按鈕，判斷點擊
    if (x >= file_btn[1][0] && x <= file_btn[5][2] && y >= file_btn[1][1] && y <= file_btn[1][3]) {
        for (int i = 1; i <= 5; i++) {
            if (x >= file_btn[i][0] && x <= file_btn[i][2] && y >= file_btn[i][1] && y <= file_btn[i][3]) {
                file_func(i);
            }
        }
    }
    //size 判斷點擊位置
    if (x >= inSize[0] && x <= inSize[2] && y >= inSize[1] && y <= inSize[3]) {    //總共100pixel 點的位置/5就是筆刷大小
        pnt_size = (x - inSize[0]) / 5.0;
        if (pnt_size < 1) pnt_size = 1;
        change_size(x - inSize[0]);
    }
    //advance setting 判斷點擊位置
    for (int i = 1; i <= 8; i++) {
        if (x >= advance_btn[i][0] && x <= advance_btn[i][2] && y >= advance_btn[i][1] && y <= advance_btn[i][3] && button == GLUT_LEFT_BUTTON && state == GLUT_UP) { //點擊一次會觸發2次事件 所以要規定是up 或 down
            if (advance_btn[i][4] == 0) {          //原本未選
                advance_btn[i][4] = 1;             //設為選過
                if (i == 1) grid_show_func(1);     //grid
                else if (i == 2) fill = GL_FILL;   //fill
                else if (i == 3) {                 //paint mode
                    paint_mode = NORMAL;
                    advance_btn[4][4] = 0;         //只能2選一
                }
                else if (i == 4) {
                    paint_mode = DOT;
                    advance_btn[3][4] = 0;
                }
                else if (i == 5) {
                    text_mode = RANDOM;
                    advance_btn[6][4] = 0;         //只能2選一
                }
                else if (i == 6) {
                    text_mode = FIXED;
                    advance_btn[5][4] = 0;
                }
                else if (i == 7) {
                    stringIndex = 0;
                    text_font = GLUT_BITMAP_TIMES_ROMAN_24;
                    advance_btn[8][4] = 0;
                }
                else if (i == 8) {
                    stringIndex = 0;
                    text_font = GLUT_BITMAP_HELVETICA_18;
                    advance_btn[7][4] = 0;
                }
            }
            else {
                if (i >= 3) continue;      //除了grid fill以外都不能取消 一定要勾
                advance_btn[i][4] = 0;
                if (i == 1) grid_show_func(0);
                else if (i == 2) fill = GL_LINE;
            }
        }
    }
    for (int i = 1; i <= 4; i++) {          //sticker 判斷點擊
        if (sticker_menu_2[i][4] && x >= sticker_menu_2[i][0] && x <= sticker_menu_2[i][2] && y >= sticker_menu_2[i][1] && y <= sticker_menu_2[i][3] && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            type_btn[obj_type][4] = 0;
            obj_type = STICKER;
            sticker_type = i;
        }
    }
    if (y >= 100) {                                //畫畫區
        mySave();
        pos_x = x; pos_y = y;
        switch (obj_type) {
        case POINT:
            glPointSize(pnt_size);      //  Define point size
            glBegin(GL_POINTS);         //  Draw a point
            glVertex2f(x, height - y);
            glEnd();
            break;
        case POLYGON:                  // Define vertices of poly
            if (side == 0) {
                vertex[side][0] = x; vertex[side][1] = y;
                side++;
            }
            else {
                if (state == GLUT_UP) {          //當滑鼠放開 -> 確定位置 存點
                    if (fabs(vertex[0][0] - x) + fabs(vertex[0][1] - y) < 12)  //連到頭就塗色
                        draw_polygon();
                    else {
                        vertex[side][0] = x;
                        vertex[side][1] = y;
                        side++;
                    }
                }
            }
            break;
        case TEXT:
            stringIndex = 0;
            text_x = x;
            text_y = y;
            break;
        case STICKER:
            pos_x = x;
            pos_y = y;
            draw_sticker();
            break;
        default:
            break;
        }
        glFinish();
    }
}
void passive_motion_func(int x, int y) {
    pos_x = x;   //游標位置
    pos_y = y;
    if (y <= 100) {
        for (int i = 1; i <= 5; i++) {         //在file上移動
            if (x >= file_btn[i][0] && x <= file_btn[i][2] && y >= file_btn[i][1] && y <= file_btn[i][3]) {
                file_btn[i][4] = 1;
            }
            else file_btn[i][4] = 0;
        }

        if (sticker_menu_1[1][4]) {            //滑過pupu
            int ok = 0;
            for (int i = 1; i <= 2; i++) {
                if (x >= sticker_menu_2[i][0] && x <= sticker_menu_2[i][2] && y >= sticker_menu_2[i][1] && y <= sticker_menu_2[i][3]) {
                    sticker_menu_2[i][4] = 1;
                    ok = 1;
                }
                else sticker_menu_2[i][4] = 0;
            }
            sticker_menu_2[0][1] = ok;
        }

        if (sticker_menu_1[2][4]) {            //滑過word
            int ok = 0;
            for (int i = 3; i <= 4; i++) {
                if (x >= sticker_menu_2[i][0] && x <= sticker_menu_2[i][2] && y >= sticker_menu_2[i][1] && y <= sticker_menu_2[i][3]) {
                    sticker_menu_2[i][4] = 1;
                    ok = 1;
                }
                else sticker_menu_2[i][4] = 0;
            }
            sticker_menu_2[0][2] = ok;
        }

        if (pupu_btn[1][4]) {            //如果他的子選項 = 1，那他也要顯示
            int ok = 0;
            for (int i = 1; i <= 2; i++) {
                if (sticker_menu_2[0][i] || (x >= sticker_menu_1[i][0] && x <= sticker_menu_1[i][2] && y >= sticker_menu_1[i][1] && y <= sticker_menu_1[i][3])) {
                    sticker_menu_1[i][4] = 1;
                    ok = 1;
                }
                else sticker_menu_1[i][4] = 0;
            }
            sticker_menu_1[0][0] = ok;
        }
        if (sticker_menu_1[0][0] == 1 || (x >= pupu_btn[1][0] && x <= pupu_btn[1][2] && y >= pupu_btn[1][1] && y <= pupu_btn[1][3])) {
            pupu_btn[1][4] = 1;
        }
        else pupu_btn[1][4] = 0;
        myMenu();
        glFlush();
    }
}
void motion_func(int  x, int y) {             //motion callback function. The mouse is pressed and moved.
    if (y >= 100) {
        if (obj_type == PAINT) {              //畫筆
            if (first == 0) {
                first = 1;
                pos_x = x; pos_y = y;
            }
            else {
                if (paint_mode == NORMAL) {  //一般模式
                    glLineWidth(pnt_size);
                    glBegin(GL_LINES);
                    glVertex3f(pos_x, height - pos_y, 0.0);
                    glVertex3f(x, height - y, 0.0);
                    glEnd();
                    pos_x = x; pos_y = y;
                }
                else if (paint_mode == DOT) { //畫
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
        else if (obj_type == POLYGON) {
            myLoad();
            glLineWidth(pnt_size);
            glBegin(GL_LINES);
            glVertex2f(vertex[side - 1][0], height - vertex[side - 1][1]);
            glVertex2f(x, height - y);
            glEnd();
        }
        else if (obj_type == RECTANGLE) {
            myLoad();
            glLineWidth(pnt_size);
            glPolygonMode(GL_FRONT, fill);
            glPolygonMode(GL_BACK, fill);
            glBegin(GL_POLYGON);
                glVertex2i(pos_x, height - pos_y);
                glVertex2i(pos_x, height - y);
                glVertex2i(x, height - y);
                glVertex2i(x, height - pos_y);
            glEnd();
            glFinish();
        }
        else if (obj_type == TRIANGLE) {
            myLoad();
            glLineWidth(pnt_size);
            glPolygonMode(GL_FRONT, fill);
            glPolygonMode(GL_BACK, fill);
            glBegin(GL_POLYGON);
                glVertex2i(pos_x, height - pos_y);
                glVertex2i(2 * pos_x - x, height - y);
                glVertex2i(x, height - y);
           
            glEnd();
        }
        else if (obj_type == CIRCLE) {
            myLoad();
            draw_circle(RANDOM, x);
        }
    }
    glFinish();
}
void init_window(void) {                   //procedure to clear window
    printf("init window\n");

    glMatrixMode(GL_PROJECTION);          // Do nothing else but clear window to white
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
void init_func() {        //Procedure to initialize data alighment and other stuff
    printf("init fun\n");
    glReadBuffer(GL_FRONT);
    glDrawBuffer(GL_FRONT);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    printf("end init fun\n");
}
void file_func(int value)
{
    if (value == MY_QUIT) exit(0);
    else if (value == MY_CLEAR) init_window();
    else if (value == MY_SAVE) { /* Save current window */
        glReadPixels(0, 0, width, height - 100, GL_RGBA, GL_UNSIGNED_BYTE, image);
        /*
        for (int i = 0; i < SIZEX; i++)   // Assign 0 opacity to black pixels
            for (int j = 0; j < SIZEY; j++)
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
        glDrawPixels(width, height - 100,
            GL_RGBA, GL_UNSIGNED_BYTE, image);
    }
    else if (value == MY_BLEND) { /* Blending current image with the saved image */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(0, 0);
        glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glDisable(GL_BLEND);
    }
    glFlush();
}
void top_menu_func(int value) {
    // Callback function for top menu. Do nothing.
}
void main(int argc, char** argv) {      //Main procedure sets up the window environment.

    int  size_menu;

    glutInit(&argc, argv);         /*---Make connection with server---*/

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
