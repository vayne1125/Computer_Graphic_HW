#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#define    SIZEX   2000   //�e���j�p ->�}�j�@�I reshape��load�~�|���`
#define    SIZEY   2000

//file �����`��
#define    MY_SAVE  1
#define    MY_LOAD  2      
#define    MY_BLEND 3
#define    MY_CLEAR 4
#define    MY_QUIT  5 

//obj_type �����`��
#define    POINT   1
#define    LINE    2
#define    POLYGON 3
#define    CIRCLE  4
#define    TRIANGLE   5
#define    RECTANGLE   6
#define    TEXT   7
#define    PAINT 8
#define    STICKER 9

//sticker_type �����`��
#define    NORMAL_PUPU_LB   1
#define    SHY_PUPU_LB   2
#define    NORMAL_PUPU_DB 3
#define    SHY_PUPU_DB 4

//text mode & circle mode �����`��
#define RANDOM 1
#define FIXED 2
#define CURSOR 3  //for ��Ъ����

//paint mode �����`��
#define NORMAL 1  
#define DOT 2

//color �����`��
#define RED     1
#define ORANGE  2
#define YELLOW  3
#define GREEN   4
#define BLUE    5
#define DBLUE   6
#define PURPLE  7
#define BLACK   8
#define BLUE_PU       9
#define GRAY    100       //UI�C�� ���i��
#define DARK_PINK   102
#define WHITE         103

typedef    int   menu_t;
menu_t     top_m, text_mode_m, text_font_m;

int        height = 800, width = 1000;       //�ù��j�p
unsigned char  image[SIZEX * SIZEY][4];      //Image data in main memory for user save
unsigned char myImage[SIZEX * SIZEY][4];     //for �@�ǥ\��ݭn��save
unsigned char gridImage[SIZEX * SIZEY][4];   //�s�º��檺�Ϯ�

int        pos_x = -1, pos_y = -1;           //��m
int        text_x = -1, text_y = -1;         //text��m 
int        obj_type = PAINT;                 //�w�]�O����
int        first = 0;                        //flag of initial points for lines and paint
int        vertex[128][2];                   //coords of vertices 
int        side = 0;                         //num of sides of polygon
float      pnt_size = 10.0;                  //����j�p

int sticker_type = SHY_PUPU_DB;
int color = BLACK;                           //��l�C��w�]��
int text_mode = RANDOM;                      //��l���r�Ҧ��w�]�H��
int paint_mode = NORMAL;                     //��l�e���w�]normal
int text_font = GLUT_BITMAP_TIMES_ROMAN_24;  //�r��:GLUT_BITMAP_HELVETICA_18  GLUT_BITMAP_TIMES_ROMAN_24 
char mySyting[1000];                         //���r�M�ΰ}�C
int stringIndex = 0;                         //���r��index
int fill = 0;                                //�O�_�� 0->����  1->��

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
    { 540, 59,555,71, 1},    //text font - ù��
    { 640, 59,655,71, 0},    //text font - Hel
};
int outSize[4] = { 65,76,169,92 };          //size�̤U�����h
int inSize[4] = { 67,78,167,90 };           //size�������h
int valueSize[4] = { 67,78,72,90 };         //pnt_size + 1 => value_size + 5   �̤W�����h (�C�����e�����M�W���A�F��ĪG)
int pnt_sizeBG[4] = { 175,76,195,92 };      //�q�X�{�b�j�p����l

int pupu_btn[2][5] = {
    {785, 60,  0, 0, 0},   //center 785,50
    {740,25,830,95, 0}
};
//�Ĥ@�hmenu
int sticker_menu_1[3][5] = {
    {0,0,0,0,0},
    {828,45,886,60,0},          //pupu
    {828,60,886,75,0},          //word
};
//�ĤG�hmenu
int sticker_menu_2[5][5] = {
    {0,0,0,0,0},                // =1  -> �ƨ쨺��menu  �ƨ�normal/shy  {0,1,0,0,0}   �A�ƨ� hi/hello {0,0,1,0,0}
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

void grid_show_func(int value) {            //�O�_��ܺ��� 
    mySave();                               //�N�{�b�e���s�bmyImage��
    if (value == 0) {                       //����� -> �NmyImage�}�C������l�u���a����
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
        for (int i = 0; i < SIZEX; i++) {  //��� -> �NmyImage���զ⪺�a��ӵۺ����@��
            for (int j = 0; j < SIZEY; j++) {
                if (myImage[i * width + j][0] == 255 && myImage[i * width + j][1] == 255 && myImage[i * width + j][2] == 255) {
                    myImage[i * width + j][0] = gridImage[i * width + j][0];
                    myImage[i * width + j][1] = gridImage[i * width + j][1];
                    myImage[i * width + j][2] = gridImage[i * width + j][2];
                }
            }
        }
    }
    myLoad();                            //��myImage load�b�e���W
    glFlush();                           //��s�e��
}
void change_color(int value) {  //�]�w�e���C��
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
void change_size(int value) {           //�վ�e���j�p
    char c[5];
    //�W������
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
    //�խ�
    change_color(DARK_PINK);
    glBegin(GL_QUADS);
    glVertex2i(valueSize[0], height - valueSize[1]);
    glVertex2i(valueSize[2], height - valueSize[1]);
    glVertex2i(valueSize[2], height - valueSize[3]);
    glVertex2i(valueSize[0], height - valueSize[3]);
    glEnd();

    //�Ʀr����
    change_color(GRAY);
    glBegin(GL_QUADS);
    glVertex2i(pnt_sizeBG[0], height - pnt_sizeBG[1]);
    glVertex2i(pnt_sizeBG[2], height - pnt_sizeBG[1]);
    glVertex2i(pnt_sizeBG[2], height - pnt_sizeBG[3]);
    glVertex2i(pnt_sizeBG[0], height - pnt_sizeBG[3]);
    glEnd();

    //�Ʀr
    int tp = value / 5;
    _itoa(tp, c, 10);
    change_color(BLACK);
    glRasterPos2i(pnt_sizeBG[0], height - 91);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

}
void color_bar(void) {                   //�C����bar
    change_color(BLACK);
    char* c = "Color\0";                 //���"Color"�r��
    glRasterPos2i(15, height - 25);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

    change_color(GRAY);           //�خ�(��ܮɷ|�ܤp�A�ҥH�n���Ω��⭫�s�e)
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
        if (color_btn[i][4]) {                //�Q���n�ܤp
            glBegin(GL_QUADS);
            glVertex2i(color_btn[i][0] + 2, height - color_btn[i][1] - 2);
            glVertex2i(color_btn[i][2] - 2, height - color_btn[i][1] - 2);
            glVertex2i(color_btn[i][2] - 2, height - color_btn[i][3] + 2);
            glVertex2i(color_btn[i][0] + 2, height - color_btn[i][3] + 2);
            glEnd();
        }
        else {
            glBegin(GL_QUADS);                //���M�N���`
            glVertex2i(color_btn[i][0], height - color_btn[i][1]);
            glVertex2i(color_btn[i][2], height - color_btn[i][1]);
            glVertex2i(color_btn[i][2], height - color_btn[i][3]);
            glVertex2i(color_btn[i][0], height - color_btn[i][3]);
            glEnd();
        }
    }
}
void type_bar(void) {                //type���bar
    change_color(BLACK);
    char* c = "Type\0";              //���type�r��
    glRasterPos2i(15, height - 47);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

    for (int i = 1; i <= 8; i++) {
        if (type_btn[i][4]) change_color(DARK_PINK);  //�Q���N����
        else change_color(WHITE);                        //�S�Q���N�쥻�C��
        glBegin(GL_QUADS);
        glVertex2i(type_btn[i][0], height - type_btn[i][1]);
        glVertex2i(type_btn[i][2], height - type_btn[i][1]);
        glVertex2i(type_btn[i][2], height - type_btn[i][3]);
        glVertex2i(type_btn[i][0], height - type_btn[i][3]);
        glEnd();
    }
    //�L�X�ﶵ���W�r
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
void file_bar(void) {            //file���bar   
    change_color(BLACK);
    char* c = "F i le\0";
    glRasterPos2i(17, height - 69);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

    for (int i = 1; i <= 5; i++) {
        if (file_btn[i][4] == 1) change_color(DARK_PINK);      //�p�Gpassive motion�N�ܦ�(�ƹ�����W��)
        else change_color(WHITE);
        glBegin(GL_QUADS);
        glVertex2i(file_btn[i][0], height - file_btn[i][1]);
        glVertex2i(file_btn[i][2], height - file_btn[i][1]);
        glVertex2i(file_btn[i][2], height - file_btn[i][3]);
        glVertex2i(file_btn[i][0], height - file_btn[i][3]);
        glEnd();
    }
    //�ﶵ�W�r
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
void size_bar(void) {            //�վ�size���a��
    change_color(BLACK);
    char* c = "Size\0";
    glRasterPos2i(15, height - 91);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);

    //�U������
    change_color(DARK_PINK);
    glBegin(GL_QUADS);
    glVertex2i(outSize[0], height - outSize[1]);
    glVertex2i(outSize[2], height - outSize[1]);
    glVertex2i(outSize[2], height - outSize[3]);
    glVertex2i(outSize[0], height - outSize[3]);
    glEnd();
    change_size(pnt_size * 5);
}
void advance_setting_bar(void) {         //�i���]�w��bar

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
    for (int i = 1; i <= 8; i++) {       //�e�Ŀ���
        glBegin(GL_QUADS);
        glVertex2i(advance_btn[i][0], height - advance_btn[i][1]);
        glVertex2i(advance_btn[i][2], height - advance_btn[i][1]);
        glVertex2i(advance_btn[i][2], height - advance_btn[i][3]);
        glVertex2i(advance_btn[i][0], height - advance_btn[i][3]);
        glEnd();
    }

    change_color(DARK_PINK);
    for (int i = 1; i <= 8; i++) {
        if (advance_btn[i][4]) {         //�Q���N����
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
void draw_pupu(int x, int y, int type) { //�e�v�ܩi
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

    //����
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

    //�|��
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
    //�v�ܩi��bar
    change_color(BLACK);
    char* c = "Sticker\0";
    glRasterPos2i(pupu_btn[1][0] + 15, height - 25);
    for (char* i = c; *i != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    //�I���d��
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

    if (pupu_btn[1][4]) {                   //�ƨ�v�ܩi�A��ܲĤ@�hmenu
        for (int i = 1; i <= 2; i++) {
            if (sticker_menu_1[i][4]) change_color(DARK_PINK);
            else change_color(WHITE);
            glBegin(GL_QUADS);
            glVertex2i(sticker_menu_1[i][0], height - sticker_menu_1[i][1]);
            glVertex2i(sticker_menu_1[i][2], height - sticker_menu_1[i][1]);
            glVertex2i(sticker_menu_1[i][2], height - sticker_menu_1[i][3]);
            glVertex2i(sticker_menu_1[i][0], height - sticker_menu_1[i][3]);
            glEnd();

            if (sticker_menu_1[i][4]) change_color(WHITE);                   //�b�Y
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

    if (sticker_menu_1[1][4]) {                     ////�ƨ�pupu�A��ܲĤG�hmenu
        for (int i = 1; i <= 2; i++) {
            if (sticker_menu_2[i][4]) change_color(DARK_PINK);
            else change_color(WHITE);
            glBegin(GL_QUADS);
            glVertex2i(sticker_menu_2[i][0], height - sticker_menu_2[i][1]);
            glVertex2i(sticker_menu_2[i][2], height - sticker_menu_2[i][1]);
            glVertex2i(sticker_menu_2[i][2], height - sticker_menu_2[i][3]);
            glVertex2i(sticker_menu_2[i][0], height - sticker_menu_2[i][3]);
            glEnd();
            if (sticker_menu_2[i][4]) change_color(WHITE);                   //�b�Y
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

    if (sticker_menu_1[2][4]) {                     //�ƨ�word�A��ܲĤG�hmenu
        for (int i = 3; i <= 4; i++) {
            if (sticker_menu_2[i][4]) change_color(DARK_PINK);
            else change_color(WHITE);
            glBegin(GL_QUADS);
            glVertex2i(sticker_menu_2[i][0], height - sticker_menu_2[i][1]);
            glVertex2i(sticker_menu_2[i][2], height - sticker_menu_2[i][1]);
            glVertex2i(sticker_menu_2[i][2], height - sticker_menu_2[i][3]);
            glVertex2i(sticker_menu_2[i][0], height - sticker_menu_2[i][3]);
            glEnd();
            if (sticker_menu_2[i][4]) change_color(WHITE);                   //�b�Y
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
void myMenu(void) {                    //�۩w�qBAR
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    change_color(DARK_PINK);           //�̥~��
    glBegin(GL_POLYGON);
    glVertex2i(0, height);
    glVertex2i(width, height);
    glVertex2i(width, height - 100);
    glVertex2i(0, height - 100);
    glEnd();

    change_color(GRAY);          //����
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
    change_color(color);                      //�]���e�����e�A�n�N�C���s�^�e���C��
}
void mySave() {
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, myImage);  //�N�e���s��myImage
}
void myLoad() {
    glRasterPos2i(0, 0);
    glDrawPixels(width, height - 100, GL_RGBA, GL_UNSIGNED_BYTE, myImage);    //�NmyImage��ܥX��
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
    //�N����s�_��
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, gridImage);
    advance_btn[1][4] = 1;//�N����]���}��
}
int firstIn = 1;                         //���s�s�@��grid
void display_func(void) {         //Callback function for display, redisplay
    //printf("myDisplay\n");
    /* define window background color */
    //glClear(GL_COLOR_BUFFER_BIT);
    if (firstIn) {                     //��e���j�p���ܮ� �n���sø�sgrid
        firstIn = 0;
        grid_line();
        mySave();
    }
    myMenu();
    glFlush();
}
void idle_func(void) {                 //���m�ɪ�call back func
    if (obj_type == PAINT) {           //�p�G��e�Ҧ��Opaint
        if (pos_y >= 100) {            //�B�b�e���ϡA���N�ϥβy�y���
            glutSetCursor(GLUT_CURSOR_NONE); //�����ƹ�
            mySave();
            draw_circle(CURSOR, 0);
            glFlush();
            myLoad();
        }
        else {
            glutSetCursor(GLUT_CURSOR_INHERIT);
        }
    }
    else if (obj_type == STICKER) {         //�p�G�O�K�ȼҦ��A���N�Υv�ܩi���
        if (pos_y >= 100) {
            glutSetCursor(GLUT_CURSOR_NONE); //�����ƹ�
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
    height = new_h;                      //�N�e���j�p��s
    width = new_w;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)width, 0.0, (double)height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();   /*---Trigger Display event for redisplay window*/

    glClearColor(1.0, 1.0, 1.0, 1.0);         //�w�q�I���C��
    glClear(GL_COLOR_BUFFER_BIT);             //�M���e��
    firstIn = 1;                              //�]�wdisplay���e���
}
void keyboard(unsigned char key, int x, int y) {

    //Callback function for keyboard event.
    //key = the key pressed,
    //(x, y) = position in the window, where the key is pressed

    if (obj_type == TEXT) {                  //�p�G�Ҧ��O���r       
        if (text_mode == RANDOM) {           //�H�����r
            glRasterPos2i(x, height - y);
            glutBitmapCharacter(text_font, key);
        }
        else if (text_mode == FIXED) {       //���@��r
            glRasterPos2i(text_x, height - text_y);  //���U�ƹ����a��
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
    if (mode == CURSOR) {             //��� -> �j�p�T�w�b5
        tp = 5;
    }
    else if (mode == RANDOM) {       //�ԥX�ꫬ
        tp = abs(pos_x - x);          //�j�p�O�ϥΪ̩Ԫ�
        if (fill == GL_LINE) {        //����(line)  �վ㤺��j�p
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
    //�e���P���v�ܩi
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
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && obj_type == PAINT) {  //�e���@���e 
        first = 0;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && obj_type == TEXT) {   //��s���a�襴�r
        stringIndex = 0;
    }
    if (button != GLUT_LEFT_BUTTON) return;
    //���s�� -> ����e��
    //color���s�A�P�_�I��
    if (x >= color_btn[1][0] && x <= color_btn[8][2] && y >= color_btn[1][1] && y <= color_btn[1][3]) {
        for (int i = 1; i <= 8; i++) {
            if (x >= color_btn[i][0] && x <= color_btn[i][2] && y >= color_btn[i][1] && y <= color_btn[i][3]) {
                color = i;
                color_btn[i][4] = 1;      //�Q���
                change_color(i);          //���N����
            }
            else color_btn[i][4] = 0;     //�S���
        }
    }
    //type���s�A�P�_�I��
    if (x >= type_btn[1][0] && x <= type_btn[8][2] && y >= type_btn[1][1] && y <= type_btn[1][3]) {
        for (int i = 1; i <= 8; i++) {
            if (x >= type_btn[i][0] && x <= type_btn[i][2] && y >= type_btn[i][1] && y <= type_btn[i][3]) {
                obj_type = i;                             //�]�wobj_type
                if (i == LINE || i == PAINT) first = 0;
                if (i == POLYGON) side = 0;
                type_btn[i][4] = 1;                       //�Q���
            }
            else type_btn[i][4] = 0;                      //�S���
        }
    }
    //file���s�A�P�_�I��
    if (x >= file_btn[1][0] && x <= file_btn[5][2] && y >= file_btn[1][1] && y <= file_btn[1][3]) {
        for (int i = 1; i <= 5; i++) {
            if (x >= file_btn[i][0] && x <= file_btn[i][2] && y >= file_btn[i][1] && y <= file_btn[i][3]) {
                file_func(i);
            }
        }
    }
    //size �P�_�I����m
    if (x >= inSize[0] && x <= inSize[2] && y >= inSize[1] && y <= inSize[3]) {    //�`�@100pixel �I����m/5�N�O����j�p
        pnt_size = (x - inSize[0]) / 5.0;
        if (pnt_size < 1) pnt_size = 1;
        change_size(x - inSize[0]);
    }
    //advance setting �P�_�I����m
    for (int i = 1; i <= 8; i++) {
        if (x >= advance_btn[i][0] && x <= advance_btn[i][2] && y >= advance_btn[i][1] && y <= advance_btn[i][3] && button == GLUT_LEFT_BUTTON && state == GLUT_UP) { //�I���@���|Ĳ�o2���ƥ� �ҥH�n�W�w�Oup �� down
            if (advance_btn[i][4] == 0) {          //�쥻����
                advance_btn[i][4] = 1;             //�]����L
                if (i == 1) grid_show_func(1);     //grid
                else if (i == 2) fill = GL_FILL;   //fill
                else if (i == 3) {                 //paint mode
                    paint_mode = NORMAL;
                    advance_btn[4][4] = 0;         //�u��2��@
                }
                else if (i == 4) {
                    paint_mode = DOT;
                    advance_btn[3][4] = 0;
                }
                else if (i == 5) {
                    text_mode = RANDOM;
                    advance_btn[6][4] = 0;         //�u��2��@
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
                if (i >= 3) continue;      //���Fgrid fill�H�~��������� �@�w�n��
                advance_btn[i][4] = 0;
                if (i == 1) grid_show_func(0);
                else if (i == 2) fill = GL_LINE;
            }
        }
    }
    for (int i = 1; i <= 4; i++) {          //sticker �P�_�I��
        if (sticker_menu_2[i][4] && x >= sticker_menu_2[i][0] && x <= sticker_menu_2[i][2] && y >= sticker_menu_2[i][1] && y <= sticker_menu_2[i][3] && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            type_btn[obj_type][4] = 0;
            obj_type = STICKER;
            sticker_type = i;
        }
    }
    if (y >= 100) {                                //�e�e��
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
                if (state == GLUT_UP) {          //��ƹ���} -> �T�w��m �s�I
                    if (fabs(vertex[0][0] - x) + fabs(vertex[0][1] - y) < 12)  //�s���Y�N���
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
    pos_x = x;   //��Ц�m
    pos_y = y;
    if (y <= 100) {
        for (int i = 1; i <= 5; i++) {         //�bfile�W����
            if (x >= file_btn[i][0] && x <= file_btn[i][2] && y >= file_btn[i][1] && y <= file_btn[i][3]) {
                file_btn[i][4] = 1;
            }
            else file_btn[i][4] = 0;
        }

        if (sticker_menu_1[1][4]) {            //�ƹLpupu
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

        if (sticker_menu_1[2][4]) {            //�ƹLword
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

        if (pupu_btn[1][4]) {            //�p�G�L���l�ﶵ = 1�A���L�]�n���
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
        if (obj_type == PAINT) {              //�e��
            if (first == 0) {
                first = 1;
                pos_x = x; pos_y = y;
            }
            else {
                if (paint_mode == NORMAL) {  //�@��Ҧ�
                    glLineWidth(pnt_size);
                    glBegin(GL_LINES);
                    glVertex3f(pos_x, height - pos_y, 0.0);
                    glVertex3f(x, height - y, 0.0);
                    glEnd();
                    pos_x = x; pos_y = y;
                }
                else if (paint_mode == DOT) { //�e
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

    glClearColor(1.0, 1.0, 1.0, 1.0); //�I��
    glClear(GL_COLOR_BUFFER_BIT);//�M���e��
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
