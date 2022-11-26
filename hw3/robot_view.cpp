#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glut.h>
#include <vector>
#define   PI   3.1415927
//�w�q�C��
#define ICE_COLOR       100
#define WAND_WOOD_COLOR 102
#define ROBOT_BLUE_MAIN 103
#define ROBOT_BLUE_SUB  104
#define ROBOT_PINK_MAIN 105
#define ROBOT_PINK_SUB  106
#define HOME_COLOR      107

//���ʤ覡
#define WALK 0
#define RUN  1
#define TURN 2   //��
#define FLY  3   //����

//�ɶ��Ҧ�
#define RUNTIMER 50             //�P�_�O�_�]�]�]
#define JUMPTIMER 51            //�@���
#define JUMPONWANDTIMER 52      //���W�k��
#define JUMPTOFLOORTIMER 53     //���^�a�O
#define CHAIR_MOVE 54           //�Ȥl�\��
#define DEBUG_MODE 55           //debug�}�Ұʵe
#define OUT_LINE_FRONT   56           //debug mode �I����� �e
#define OUT_LINE_BACK    57           //debug mode �I����� ��
#define OUT_LINE_LEFT    58           //debug mode �I����� ��
#define OUT_LINE_RIGHT   59           //debug mode �I����� �k


//����� todo:�٦��pbug q
#define LOCK true
#define UNLOCK false

//�����Ҧ���� todo:���y��(���Ӯi�櫢��...)
#define MAGICFIELD 0
#define GRASSLAND 1

using namespace std;
//�w�qcube
float  points[][3] = { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},       //coordinates of the 8 corners.(0,0,0)-(1,1,1)
                      {1.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
                      {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
{1.0, 1, 1.0}, {0.0, 1.0, 1.0} };

int    face[][4] = { {0, 1, 2, 3}, {7, 6, 5, 4}, {0, 4, 5, 1},  //Define 6 faces using the 8 corners (vertices)
                    {1, 5, 6, 2}, {3, 2, 6, 7}, {0, 3, 7, 4} };
int    cube[6] = { 0, 1, 2, 3, 4, 5 };                          //Define the cube using the 6 faces.
int    width = 700, height = 700;                               //window shape 
float  pos[3] = { 0.0, 0.0, 0.0 };                              //��m
float  anglex = 0.0, angley = 0.0;

/*-----Translation and rotations of eye coordinate system---*/
float   eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
double  Eye[3] = { 30.0, 10.0, 80.0 }, Focus[3] = { 0.0, 0.0, 0.0 },
Vup[3] = { 0.0, 1.0, 0.0 };

float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
float   eye[3];
float   cv, sv; /* cos(5.0) and sin(5.0) */
int viewStyle = 0;    //viw change: 
                      //magic - 0x/1y/2z/3perspective/4all
                      //grass - 0x/1y/2z/3perspective
//�� ���� w/h �����i�}������(40~70)
double zNear = 0, zFar = 0, aspect = 0, fovy = 0; 

/*----------------------------------------------------------*/


//Define GLU quadric objects, a sphere and a cylinder
GLUquadricObj* sphere = NULL, * cylind = NULL, * mycircle = NULL;
int see = 0;             //��������(�}�o�a�ϥ� ��ڵL���\��)                                     
int preKey = 0;          //�W�@�ӫ���׽�
int scene = MAGICFIELD;  //��l�I�����]�k�}
bool isLock = 0;         //����O�_��F
bool sitOnChair = 0;
bool debugMode = 0;
int debugModeCmd = 0;
void draw_magic_field();
void draw_cube();
void draw_cylinder(double up, double down, double height);
void change_color(int value);
void draw_circle(double size, int wid);
void draw_square(int hei, int wid,int sz);
float getDis(float x1, float y1, float x2, float y2) {           //��Z��
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
struct node {   //�w�q���y�Ъ��I
    double x = 0, y = 0, z = 0;
};
struct pp {   //�U�ئ�m���c
    float x1 = 0, x2 = 0, z1 = 0, z2 = 0;
    float x = 0, z = 0, r = 0;
    float a1 = 0, a2 = 0;
    pp(float x1_, float z1_, float x2_, float z2_) {      //�x��
        x1 = x1_;
        x2 = x2_;
        z1 = z1_;
        z2 = z2_;
    }
    pp(float x_, float z_, float r_, float a1_, float a2_) {  //�y
        x = x_;
        z = z_;
        r = r_;
        a1 = a1_;
        a2 = a2_;
    }
};
//�e�D
const vector<pp> river = { {-10,120 ,100,0,25},{190,120 ,100,180,220}, {24,9 ,100,-3,30}, {100,70 ,100,75,115}, {130,267 ,100,268,290},{200,80 ,100,93,112} };
const vector<pp> rock = { {-10,120 ,100,0,25},{190,120 ,100,180,220}, {24,9 ,100,-3,30},{100,70 ,100,75,96} ,{100,70 ,100,101,115}, {130,267 ,100,268,290},{200,80 ,100,93,112} };
//�W�l
const vector<pair<float, float>> pillar = { {110,100},{180,100},{110,160},{180,160} };
//�����m���y��
vector<pair<float, float>>limit;
node ball_cor(double r, int A, int B) {          //���y���ഫ
    node rt;
    rt.x = r * sin(A * 0.01745) * cos(B * 0.01745);
    rt.y = r * sin(A * 0.01745) * sin(B * 0.01745);
    rt.z = r * cos(A * 0.01745);
    return rt;
}
struct elf {
    void draw() {
        glPushMatrix();

        change_color(ROBOT_BLUE_MAIN);
        glPushMatrix();                 //�Y
        glutSolidSphere(1, 50, 50);
        glPopMatrix();

        change_color(ROBOT_BLUE_SUB);
        glPushMatrix();                 //�~�k��
        glTranslatef(0.5,0.6,-0.2);
        glRotatef(20,0,0,1);
        glScalef(0.8, 0.8, 0.4);
        draw_cube();

        glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);            //����
        glPushMatrix();
        glTranslatef(0.2, 0.2, 0.7);
        glScalef(0.6,0.6,0.4);
        draw_cube();
        glPopMatrix();
        glPopMatrix();

        change_color(ROBOT_BLUE_SUB);
        glPushMatrix();
        glTranslatef(-0.5, 0.6, 0.2);
        glRotatef(-20, 0, 0, 1);
        glRotatef(180, 0, 1, 0);
        glScalef(0.8, 0.8, 0.4);
        draw_cube();

        glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);            //����
        glPushMatrix();
        glTranslatef(0.2, 0.2, -0.1);
        glScalef(0.6, 0.6, 0.4);
        draw_cube();
        glPopMatrix();
        glPopMatrix();
     
        //����
        glColor3f(0,0,0);
        glPushMatrix();
        glTranslatef(0.4,0.15,1);
        glScalef(0.2,0.4,0.2);
        glutSolidSphere(0.5, 10, 10);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.4, 0.15, 1);
        glScalef(0.2, 0.4, 0.2);
        glutSolidSphere(0.5, 10, 10);
        glPopMatrix();

        //�|��
        //#FFAAD5
        glColor3f(1, 168/255.0, 212/255.0);
        glPushMatrix();
        glTranslatef(0.7, -0.2, 0.8);
        glScalef(0.4, 0.2, 0.2);
        glutSolidSphere(0.5, 10, 10);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.7, -0.2, 0.8);
        glScalef(0.4, 0.2, 0.2);
        glutSolidSphere(0.5, 10, 10);
        glPopMatrix();

        glPopMatrix();
    }
}camera;
struct magic_wand {
    float x = 0, y = 0, z = 0;                    //�ۤv���y��
    float angle_x = 0, angle_y = 0, angle_z = 0;
    float scale = 0;
    bool show = 1;   //�O�_���
    magic_wand(float s_ = 1, float x = 0, float y = 0, float z = 0) {
        scale = s_;
        angle_x = x;
        angle_y = y;
        angle_z = z;
    }
    void setPos(int x_, int y_, int z_) {
        x = x_;
        y = y_;
        z = z_;
    }
    void draw() {
        //x�k y�W z�e �����I:�k��������
        change_color(WAND_WOOD_COLOR);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glRotatef(angle_x, 1, 0, 0);
        glRotatef(angle_y, 0, 1, 0);
        glRotatef(angle_z, 0, 0, 1);
        glScalef(scale, scale, scale);

        glTranslatef(0, 0, -7);
        draw_cylinder(0.5, 0.8, 14);             //���׬�14 �e��0.8

        glColor3f(102 / 255.0, 34 / 255.0, 0);   //�k����������
        glPushMatrix();

        glTranslatef(0, 0, 0);                   //�k����������
        glRotatef(270, 1, 0, 0);                 //��ί���
        draw_circle(0.5, 1);
        glPopMatrix();

        glColor3f(102 / 255.0, 34 / 255.0, 0);   //�k����������
        glPushMatrix();
        glTranslatef(0, 0, 14);
        glRotatef(270, 1, 0, 0);                 //��ί���
        draw_circle(0.8, 1);
        glPopMatrix();

        glColor3f(1, 1, 168 / 255.0);              //����(��)
        glPushMatrix();
        glTranslatef(0, 0, 14);
        glutSolidSphere(1.5, 10, 10);              //9.5

        glColor3f(168 / 255.0, 1.0, 1);            //�{����(�Ŧ�)
        glPushMatrix();
        glRotatef(30, 0, 1, 0);
        glutSolidTorus(0.1, 2.5, 100, 100);
        glPopMatrix();

        glColor3f(168 / 255.0, 1.0, 1);           //�{����(�Ŧ�)
        glPushMatrix();
        glRotatef(330, 0, 1, 0);
        glutSolidTorus(0.1, 2.5, 100, 100);
        glPopMatrix();
        glPopMatrix();

    }
}myMagic_wand;
struct robot {
    magic_wand* magic_wand_carry = new magic_wand(0.34, 180, 0, 0); //�]�k�Ϊ���(���)
    magic_wand* magic_wand_sit = new magic_wand(0.5, 0, -90, 0);    //�]�k�Ϊ���(����)
    int mainColor = ROBOT_BLUE_MAIN, subColor = ROBOT_BLUE_SUB;    //�C��
    int moveMode = 0; //���ʼҦ� 0->walk  1->run  2->turn  3->fly
    robot() {
        stand();
    }
    float angle_x = 0.0, angle_y = 0.0;                         //���ਤ��
    bool carry_mw = 0;                                          //�ثe�����]�k�ζ�
    bool isMagician = 0;                                        //�O���O�]�k�v
    bool isJump_ready = 0;                                      //�ǳƸ������ն�
    float x = 0, y = 0, z = 0;                                  //��������H�y��
    struct hand {
        node tp;
        float shoulderAng_x = 180, shoulderAng_y = 0, shoulderAng_z = 35;    //�ӻH
        float elbowAng_x = 0, elbowAng_y = 0, elbowAng_z = 0;                //��y
        float fingerAng_x = 0, fingerAng_y = 0, fingerAng_z = 0;             //���
        int subColor = 0;
        void draw() {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //glRotatef(shoulderAng_x, 1, 0, 0);           //�������`���� 180��U
            glRotatef(shoulderAng_z, 0, 0, 1);             //-35��b������� ��z�౱��k
            glRotatef(shoulderAng_x, 1, 0, 0);
            change_color(subColor);
            glutSolidSphere(0.5, 10, 10);                  //�b�|�� 0.5 ���ӻH

            //forarms
            glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);
            glTranslatef(0, 0.75, 0);              //���� �ӻH�W��0.25 + �ꤤ��0.75(�e1.5�����u) - 0.25���|

            glPushMatrix();
            glScalef(0.7, 1.5, 0.7);
            glutSolidSphere(0.5, 10, 10);          //���|��1.5�����u
            glPopMatrix();

            glTranslatef(0, 0.75, 0);               //���u�e�ݤ���

            glRotatef(elbowAng_x, 1, 0, 0);         //�����y
            glRotatef(elbowAng_y, 0, 1, 0);
            glRotatef(elbowAng_z, 0, 0, 1);

            change_color(subColor);

            glutSolidSphere(0.25, 10, 10);          //���|�� 0.5 ����y      0.25���|

            //��e�u
            glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);
            glTranslatef(0, 0.75, 0);                //���� ��b�e��0.25 + �ꤤ��0.75(�e1.5����e�u) - 0.25���|

            glPushMatrix();
            glScalef(0.55, 1.5, 0.55);
            glutSolidSphere(0.5, 10, 10);           //���|��1.5����e�u
            glPopMatrix();

            glTranslatef(0, 0.75, 0);               //��e�u�e��   
            //�������V���Ӧb�o��
            glRotatef(fingerAng_y, 0, 1, 0);
            //������Y
            change_color(subColor);
            glPushMatrix();
            glTranslatef(-0.25, 0.2, 0);
            glRotatef(15, 0, 0, 1);               //�i�}����
            glScalef(0.3, 0.7, 0.3);              //�����: 0.7
            glutSolidSphere(0.5, 10, 10);
            glPopMatrix();

            //�k����Y
            glPushMatrix();
            glTranslatef(0.25, 0.2, 0);
            glRotatef(-15, 0, 0, 1);              //�i�}����
            glScalef(0.3, 0.7, 0.3);              //�����: 0.7
            glutSolidSphere(0.5, 10, 10);
            glPopMatrix();
            //glRotatef(-shoulderAng_z, 0, 0, 1);   //�ܦ^���`���y�Шt��
        }
    };
    struct foot {          //�L��2.75
        node tp;
        float hipJointAng_x = 180, hipJointAng_y = 0, hipJointAng_z = 0;   //�b���`
        float kneeAng_x = 0, kneeAng_y = 0, kneeAng_z = 0;
        float ankle_x = 0, ankle_y = 0, ankle_z = 0;
        int subColor = 0;
        void draw() {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glRotatef(hipJointAng_x, 1, 0, 0);          //�������`���� 180��U
            //glRotatef(-30, 0, 0, 1);            
            //glRotatef(90, 1, 0, 0);             //���e  ��x�b�౱���|�_��U
            change_color(subColor);
            glutSolidSphere(0.25, 10, 10);        //�j�L�W�����` 0.5���|

            //�j�L
            glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);
            glTranslatef(0, 0.625, 0);           //���� ���\0.125 + �ꤤ��0.5(�e1����)

            glPushMatrix();
            glScalef(0.7, 1, 0.7);               //�j�L��1
            glutSolidSphere(0.5, 10, 10);
            glPopMatrix();

            glTranslatef(0, 0.5, 0);             //�L�e�ݤ���  �M���\���|0.25

            glRotatef(kneeAng_x, 1, 0, 0);       //���\����
            //���\
            change_color(subColor);
            glutSolidSphere(0.25, 10, 10);       //���|0.5�����\

            glTranslatef(0, 0.5, 0);             //���\�e�� 0.25 + �L����0.5(�L��1) - 0.25(���|�a��)

            glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);
            glPushMatrix();
            glScalef(0.5, 1, 0.5);               //�p�L��1
            glutSolidSphere(0.5, 10, 10);
            glPopMatrix();

            glTranslatef(0, 0.5, 0);             //�p�L�e�ݤ��� 0.5 �M�L���|0.25
            glColor3f(0.5, 0.5, 0.5);
            glutSolidSphere(0.5, 10, 10);        //�} ���|1
        }
    };
    hand* right_h = new hand;
    hand* left_h = new hand;
    foot* left_f = new foot;
    foot* right_f = new foot;
    void setColor(int m, int s) {
        mainColor = m;
        subColor = s;
    };
    void draw_hat() {
        glPushMatrix();           //push1
        glRotatef(270, 1, 0, 0);  //�ઽ ���W�e
        glRotatef(-10, 1, 0, 0);
        //draw_cylinder(0,3,6); �Ȥl�ĪG
        //glRotatef(80, 1, 0, 0);

        glColor3f(38 / 255.0, 38 / 255.0, 38 / 255.0);   //�U�u
        glPushMatrix();           //push2
        draw_cylinder(3, 0, 1);

        glColor3f(61 / 255.0, 61 / 255.0, 61 / 255.0);  //�U�l
        draw_cylinder(2, 0, 4);

        //�᯾
        glColor3f(1, 1, 168 / 255.0);                    //�e
        glTranslatef(0, 0, 0.5);
        draw_cylinder(2, 1, 0.8);

        change_color(subColor);          //��
        glTranslatef(0, 0, 0.6);
        draw_cylinder(1.5, 0.8, 0.6);
        glPopMatrix();                  //pop2
        glPopMatrix();                  //pop1
    }
    void draw() {
        right_h->subColor = subColor;
        left_h->subColor = subColor;
        left_f->subColor = subColor;
        right_f->subColor = subColor;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //�{�l
        change_color(mainColor);

        glPushMatrix();                             //push1
        glScalef(2.5, 2.5, 2.5);
        glTranslatef(x, y, z);                      //�㰦���y��

        glRotatef(angle_y, 0, 1, 0);
        glRotatef(angle_x, 1, 0, 0);

        glTranslatef(0, 4.75, 0);                   //����{�l
        glutSolidSphere(2, 10, 10);                 //�e�{�l ���|4

        node tp = ball_cor(2, 90, 30);
        glPushMatrix();                             //push2
        glTranslatef(tp.x, tp.y, tp.z);             //����k�ӻH
        right_h->draw();

        glPushMatrix();                             //push3
        glTranslatef(0, 0.3, 0);
        if (carry_mw && isMagician) magic_wand_carry->draw();
        glPopMatrix();                              //pop3

        glPopMatrix();                              //pop2

        //���ӻH
        tp = ball_cor(2, 270, 330);
        glPushMatrix();                            //push2
        glTranslatef(tp.x, tp.y, tp.z);       //���쥪�ӻH
        left_h->draw();
        glPopMatrix();                             //pop2

        glPushMatrix();                       //���b�k���W  push2
        glTranslatef(0, -2, 0);
        if (isOnWand) magic_wand_sit->draw();
        glPopMatrix();                         //pop2

        //���j�L�W�������`  �I�b�����
        glPushMatrix();                       //push2
        glTranslatef(-0.4, -1.75, 0);
        left_f->draw();
        glPopMatrix();                        //pop2

        //�k�j�L�W�������`
        glPushMatrix();                       //push2
        glTranslatef(0.4, -1.75, 0);
        right_f->draw();
        glPopMatrix();                        //pop2

        //�Y
        change_color(mainColor);
        glTranslatef(0, 3, 0);               //�b�����Y  �M���魫�|0.5
        glPushMatrix();                      //push2
        glutSolidSphere(1.5, 10, 10);        //���|3

        if (isMagician)
            glColor3f(128 / 255.0, 128 / 255.0, 1);  //��
        else
            glColor3f(0, 0, 0);
        glPushMatrix();                          //����  push3
        glTranslatef(0.6, 0, 1.3);
        glScalef(0.4, 0.8, 0.4);
        glutSolidSphere(0.5, 10, 10);
        glColor3f(1, 1, 1);                      //����
        glTranslatef(0, 0.15, 0.15);
        glutSolidSphere(0.33, 10, 10);
        glPopMatrix();                           //pop3

        if (isMagician)
            glColor3f(1, 122 / 255.0, 189);      //��
        else
            glColor3f(0, 0, 0);
        glPushMatrix();                          //push3
        glTranslatef(-0.6, 0, 1.3);
        glScalef(0.4, 0.8, 0.4);
        glutSolidSphere(0.5, 10, 10);
        glColor3f(1, 1, 1);                      //����
        glTranslatef(0, 0.15, 0.15);
        glutSolidSphere(0.33, 10, 10);
        glPopMatrix();                           //pop3

        glColor3f(1, 0, 0);
        glPushMatrix();                           //push3
        glTranslatef(0, -0.6, 1.5);              //�L��
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex3f(-0.2, 0, 0);
        glVertex3f(0.2, 0, 0);
        glEnd();
        glPopMatrix();                          //pop3

        glTranslatef(0, 0.5, 0);                //�U�l���Шt
        if (isMagician) draw_hat();

        glPopMatrix();                       //���}�Y pop2

        glPopMatrix();                       //���}�{�l���Шt pop1
    }
    void stand() {                           //�w�q����(��l��)
        y = 0;
        x = 0;
        y = 0;
        angle_x = 0;
        right_f->hipJointAng_x = 180;
        right_f->kneeAng_x = 0;
        left_f->hipJointAng_x = 180;
        left_f->kneeAng_x = 0;
        left_h->shoulderAng_x = 180;
        right_h->shoulderAng_x = 180;
        left_h->elbowAng_x = 0;
        right_h->elbowAng_x = 0;
        left_h->shoulderAng_z = -35;
        right_h->shoulderAng_z = 35;
    }
    bool flag = 0, flag2 = 0;
    void move() {
        //(���\,�b���`)
        //�}��(0,180) ~ (35,200)     35 +7   20 +4
        //�}���e(0,180) ~ (35,130)   35 +7   50 -10
        //(�ӻH)
        //�⩹�e(180) ~ (200)        20 +4   100 +20
        //�⩹��(180) ~ (170)        20 -4
        //moveMode = walk
        int hipJointXLimit = 130;
        int hipJointFrontOffset = -10;
        int kneeFrontOffset = 7;
        int hipJointBackOffset = 4;
        int kneeBackOffset = 7;
        int shoulderOffset = -4;
        left_h->elbowAng_x = -20;
        right_h->elbowAng_x = -20;
        if (moveMode == RUN) {
            angle_x = 5;
            hipJointFrontOffset = -20;
            kneeFrontOffset = 14;
            hipJointBackOffset = 8;
            kneeBackOffset = 25;
            shoulderOffset = -20;
            left_h->elbowAng_x = -40;
            right_h->elbowAng_x = -40;
        }
        if (moveMode == TURN) {
            hipJointXLimit = 160;
        }
        if (flag == 0) {
            if (flag2 == 0) {  //�k�}���e�A���}����             
                right_f->hipJointAng_x += hipJointFrontOffset / 2.0;          //�k�}���e��
                right_f->kneeAng_x += kneeFrontOffset / 2.0;
                left_f->hipJointAng_x += hipJointBackOffset / 2.0;            //���}����
                left_f->kneeAng_x += kneeBackOffset / 2.0;
                left_h->shoulderAng_x += shoulderOffset / 2.0;                //���⩹�e
                right_h->shoulderAng_x -= shoulderOffset / 2.0;               //�k�⩹��
                if (right_f->hipJointAng_x <= hipJointXLimit) {               //��ɱ���
                    flag2 = 1;
                }
            }
            else { //�k�}�������I�A���}���e����I
                right_f->hipJointAng_x -= hipJointFrontOffset / 2.0;
                right_f->kneeAng_x -= kneeFrontOffset / 2.0;
                left_f->hipJointAng_x -= hipJointBackOffset / 2.0;
                left_f->kneeAng_x -= kneeBackOffset / 2.0;
                left_h->shoulderAng_x -= shoulderOffset / 2.0;
                right_h->shoulderAng_x += shoulderOffset / 2.0;
                if (right_f->hipJointAng_x >= 180) {
                    flag2 = 0;
                    flag = 1;
                }
            }
        }
        else {   //���}���e�A�k�}���� 
            if (flag2 == 0) {
                left_f->hipJointAng_x += hipJointFrontOffset / 2.0;
                left_f->kneeAng_x += kneeFrontOffset / 2.0;
                right_f->hipJointAng_x += hipJointBackOffset / 2.0;
                right_f->kneeAng_x += kneeBackOffset / 2.0;
                left_h->shoulderAng_x -= shoulderOffset / 2.0;
                right_h->shoulderAng_x += shoulderOffset / 2.0;
                if (left_f->hipJointAng_x <= hipJointXLimit) {
                    flag2 = 1;
                }
            }
            else { //���}�������I�A�k�}���e����I
                left_f->hipJointAng_x -= hipJointFrontOffset / 2.0;
                left_f->kneeAng_x -= kneeFrontOffset / 2.0;
                right_f->hipJointAng_x -= hipJointBackOffset / 2.0;
                right_f->kneeAng_x -= kneeBackOffset / 2.0;
                left_h->shoulderAng_x += shoulderOffset / 2.0;
                right_h->shoulderAng_x -= shoulderOffset / 2.0;
                if (left_f->hipJointAng_x >= 180) {
                    flag2 = 0;
                    flag = 0;
                }
            }
        }
    }
    void jump_ready() {               //�����w�ưʧ@
        left_f->kneeAng_x = 45;
        right_f->kneeAng_x = 45;

        left_f->hipJointAng_x = 150;
        right_f->hipJointAng_x = 150;

        left_h->shoulderAng_x = 200;
        right_h->shoulderAng_x = 200; //180 160
        isJump_ready = 1;
    }
    int jump_cmd = 0;              //fsm
    bool jump() {
        left_h->elbowAng_x = -15;
        right_h->elbowAng_x = -15;
        switch (jump_cmd) {
        case 0:              //���W��  ���`�ܥ��` �⩹�e
            y += 0.5;
            left_f->kneeAng_x -= 45 / 4.0;
            right_f->kneeAng_x -= 45 / 4.0;
            left_f->hipJointAng_x += 30 / 4.0;
            right_f->hipJointAng_x += 30 / 4.0;
            left_h->shoulderAng_x -= 40 / 4.0;
            right_h->shoulderAng_x -= 40 / 4.0;   //160
            if (y == 2) jump_cmd++;
            break;
        case 1:              //��a�O �s��
            y -= 1;
            left_f->kneeAng_x += 60 / 2.0;
            right_f->kneeAng_x += 60 / 2.0;
            left_f->hipJointAng_x -= 40 / 2.0;
            right_f->hipJointAng_x -= 40 / 2.0;
            left_h->shoulderAng_x += 40 / 2.0;
            right_h->shoulderAng_x += 40 / 2.0;  //200
            if (y == 0) jump_cmd++;
            break;
        case 2:
            y += 0.8;
            left_f->kneeAng_x -= 60 / 5.0;
            right_f->kneeAng_x -= 60 / 5.0;
            left_f->hipJointAng_x += 40 / 5.0;
            right_f->hipJointAng_x += 40 / 5.0;
            left_h->shoulderAng_x -= 40 / 5.0;
            right_h->shoulderAng_x -= 40 / 5.0;
            if (y == 4) jump_cmd++;
            break;
        case 3:
            y -= 1;
            left_f->kneeAng_x += 70 / 4.0;
            right_f->kneeAng_x += 70 / 4.0;
            left_f->hipJointAng_x -= 50 / 4.0;
            right_f->hipJointAng_x -= 50 / 4.0;
            left_h->shoulderAng_x += 40 / 4.0;
            right_h->shoulderAng_x += 40 / 4.0;
            if (y == 0) jump_cmd++;
            break;
        case 4:
        case 5:
            left_f->kneeAng_x -= 70 / 2.0;
            right_f->kneeAng_x -= 70 / 2.0;
            left_f->hipJointAng_x += 50 / 2.0;
            right_f->hipJointAng_x += 50 / 2.0;
            left_h->shoulderAng_x -= 20 / 2.0;
            right_h->shoulderAng_x -= 20 / 2.0;
            jump_cmd++;
            break;
        }
        if (jump_cmd == 6) {
            jump_cmd = 0;
            return 1;
        }
        return 0;
    }
    bool isOnWand = 0;             //�O�_���b�k���W
    bool jumpOnWand() {            //���W�k��
        carry_mw = 0;
        isOnWand = 1;
        left_h->elbowAng_x = -15;
        right_h->elbowAng_x = -15;
        switch (jump_cmd) {
        case 0:              //���W��  ���`�ܥ��` �⩹�e
            y += 0.5;
            left_f->kneeAng_x -= 45 / 4.0;
            right_f->kneeAng_x -= 45 / 4.0;
            left_f->hipJointAng_x += 30 / 4.0;
            right_f->hipJointAng_x += 30 / 4.0;
            left_h->shoulderAng_x -= 40 / 4.0;
            right_h->shoulderAng_x -= 40 / 4.0;   //160
            if (y == 2) jump_cmd++;
            break;
        case 1:
            sit();
            jump_cmd++;
            break;
        }
        if (jump_cmd == 2) {
            jump_cmd = 0;
            cout << y << " " << jump_cmd << " " << left_f->kneeAng_x << " " << left_f->hipJointAng_x << " " << left_h->shoulderAng_x << "\n";
            return 1;
        }
        return 0;
    }
    bool jumpToFloor() {           //����a�O
        //80 80 100 100 180 180
        left_h->elbowAng_x = -15;
        right_h->elbowAng_x = -15;
        switch (jump_cmd) {
        case 0:                 //��a�O �s��
            y -= 1;
            left_f->kneeAng_x += 60 / 2.0;
            right_f->kneeAng_x += 60 / 2.0;
            left_f->hipJointAng_x -= 40 / 2.0;
            right_f->hipJointAng_x -= 40 / 2.0;
            left_h->shoulderAng_x += 40 / 2.0;
            right_h->shoulderAng_x += 40 / 2.0;
            if (y == 0) jump_cmd++;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            left_f->kneeAng_x -= 140 / 4.0;
            right_f->kneeAng_x -= 140 / 4.0;
            left_f->hipJointAng_x += 120 / 4.0;
            right_f->hipJointAng_x += 120 / 4.0;
            left_h->shoulderAng_x -= 40 / 4.0;
            right_h->shoulderAng_x -= 40 / 4.0;
            jump_cmd++;
            break;
        }
        if (jump_cmd == 5) {
            jump_cmd = 0;
            carry_mw = 1;
            isOnWand = 0;
            return 1;
        }
        return 0;
    }
    void sit() {
        left_f->kneeAng_x = 80;
        right_f->kneeAng_x = 80;
        left_f->hipJointAng_x = 100;
        right_f->hipJointAng_x = 100;
        left_h->shoulderAng_x = 180;
        right_h->shoulderAng_x = 180;
        left_h->shoulderAng_z = -15;
        right_h->shoulderAng_z = 15;
    }
}myRobot, jakao, pupu;
struct big_chair {
    float x = 0, y = 0, z = 0;                    //�ۤv���y��
    float angle_z = 0.0;
    int flag = 0;
    void setPos(int x_, int y_, int z_) {
        x = x_;
        y = y_;
        z = z_;
    }
    void move() {                                 //�n�\
        if (flag == 0) {
            angle_z += 3;
            if (angle_z >= 20) flag = 1;
        }
        else {
            angle_z -= 3;
            if (angle_z <= -20) flag = 0;
        }
    }
    void draw() {
        //�Ȥl����25 �e10 ��26
        glColor3f(141 / 255.0, 84 / 255.0, 28 / 255.0);
        glPushMatrix();              //2�Ӭ[�l
        glTranslatef(0, 0, -20);
        glRotatef(90, 0, 1, 0);
        glRotatef(75, 1, 0, 0);
        draw_cylinder(0.5, 0.5, 30);
        glRotatef(30, 1, 0, 0);
        draw_cylinder(0.5, 0.5, 30);
        glPopMatrix();              //pop�[�l

        glPushMatrix();
        glTranslatef(0, 0, 20);
        glRotatef(90, 0, 1, 0);
        glRotatef(75, 1, 0, 0);
        draw_cylinder(0.5, 0.5, 30);
        glRotatef(30, 1, 0, 0);
        draw_cylinder(0.5, 0.5, 30);
        glPopMatrix();               //pop�[�l

        glPushMatrix();
        glRotatef(angle_z, 0, 0, 1);       //����Ȥl
        glTranslatef(0, 0, -20);        //���
        draw_cylinder(0.8, 0.8, 40);


        glColor3f(1, 1, 1);
        glPushMatrix();             //�u�u
        glTranslatef(0, 0, 7);
        glRotatef(90, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        draw_cylinder(0.3, 0.3, 20);
        glPopMatrix();

        glPushMatrix();             //�u�u
        glTranslatef(0, 0, 33);
        glRotatef(90, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        draw_cylinder(0.3, 0.3, 20);
        glPopMatrix();

        glColor3f(162 / 255.0, 92 / 255.0, 21 / 255.0);
        glTranslatef(0, -20, 7);

        glPushMatrix();               //�Ȥl
        glScalef(10, 2, 26);
        draw_cube();
        glPopMatrix();

        glColor3f(141 / 255.0, 84 / 255.0, 28 / 255.0);
        glPushMatrix();               //�ȭI
        glRotatef(110, 0, 0, 1);
        glScalef(10, 2, 26);
        draw_cube();
        glPopMatrix();

        jakao.sit();
        jakao.left_h->shoulderAng_x -= 20;
        jakao.right_h->shoulderAng_x -= 20;

        glPushMatrix();
        glTranslatef(8, -4.5, 6);   //���e�� ���U ���k
        glRotatef(90, 0, 1, 0);
        jakao.draw();
        glPopMatrix();

        pupu.sit();
        pupu.left_h->shoulderAng_x -= 20;
        pupu.right_h->shoulderAng_x -= 20;

        glPushMatrix();
        glTranslatef(8, -4.5, 19);   //���e�� ���U ���k
        glRotatef(90, 0, 1, 0);
        if (sitOnChair) pupu.draw();
        glPopMatrix();

        glPopMatrix();          //pop���

    }
}myBig_chair;
struct floor {
    int light[6] = { 0 }; //�W�U���k�e��
    void draw() {                  //�e����M�a�O
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //�W
        if (light[0] % 2 == 0)
            change_color(ICE_COLOR);
        else {
            glColor3f(1, 0, 0);
        }
        glPushMatrix();
        glTranslatef(0, 60, 0);
        draw_square(60, 60, 1);              //�e�Ѫ�O
        glPopMatrix();

        //�U
        change_color(ICE_COLOR);
        glPushMatrix();
        draw_square(60, 60, 1);              //�e�a�O
        glPopMatrix();

        //��
        if (light[2] % 2 == 0)
            change_color(ICE_COLOR);
        else
            glColor3f(1, 0, 0);
        glPushMatrix();                   //�O�s0,0
        glRotatef(90, 0, 0, 1);           //��z�b�f�ɰw��270��
        draw_square(60, 60, 1);
        if (light[2] % 2 == 1) {
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(0, 0, 60);
                glEnd();
                glTranslated(10, 0, 0);
            }
            glPopMatrix();
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(60, 0, 0);
                glEnd();
                glTranslated(0, 0, 10);
            }
            glPopMatrix();
        }
        glPopMatrix();                    //�^��0,0

        //�k
        if (light[3] % 2 == 0)
            change_color(ICE_COLOR);
        else
            glColor3f(1, 0, 0);
        glPushMatrix();
        glTranslatef(60, 0, 0);            //�O�s0,0
        glRotatef(90, 0, 0, 1);           //��z�b�f�ɰw��270��
        draw_square(60, 60, 1);
        if (light[3] % 2 == 1) {
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(0, 0, 60);
                glEnd();
                glTranslated(10, 0, 0);
            }
            glPopMatrix();
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(60, 0, 0);
                glEnd();
                glTranslated(0, 0, 10);
            }
            glPopMatrix();
        }
        glPopMatrix();
        

        //�e
        if (light[4] % 2 == 0)
            change_color(ICE_COLOR);
        else
            glColor3f(1, 0, 0);
        glPushMatrix();                   //�O�s0,0
        glTranslatef(0, 0, 60);
        glRotatef(270, 1, 0, 0);          //��x�b�f�ɰw��270��
        draw_square(60, 60, 1);
        if (light[4] % 2 == 1) {
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(0, 0, 60);
                glEnd();
                glTranslated(10, 0, 0);
            }
            glPopMatrix();
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(60, 0, 0);
                glEnd();
                glTranslated(0, 0, 10);
            }
            glPopMatrix();
        }
        glPopMatrix();

        //��
        if (light[5] % 2 == 0)
            change_color(ICE_COLOR);
        else
            glColor3f(1, 0, 0);
        glPushMatrix();                   //�O�s0,0
        glRotatef(270, 1, 0, 0);          //��x�b�f�ɰw��270��
        draw_square(60, 60, 1);
        if (light[5] % 2 == 1) {
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(0, 0, 60);
                glEnd();
                glTranslated(10, 0, 0);
            }
            glPopMatrix();
            glPushMatrix();
            for (int i = 0; i < 6; i++) {
                glBegin(GL_LINES);
                glVertex3f(0, 0, 0);
                glVertex3f(60, 0, 0);
                glEnd();
                glTranslated(0, 0, 10);
            }
            glPopMatrix();
        }
        glPopMatrix();                    //�^��0,0
    }
}myFloor;
void change_color(int value) {  //�]�w�C��
    switch (value) {
    case ICE_COLOR:
        glColor3f(235 / 255.0, 1.0, 1.0);
        break;
    case WAND_WOOD_COLOR:
        glColor3f(158 / 255.0, 79 / 255.0, 0);
        break;
    case ROBOT_BLUE_MAIN:
        glColor3f(219 / 255.0, 1, 1);
        break;
    case ROBOT_BLUE_SUB:
        glColor3f(173 / 255.0, 214 / 255.0, 1);
        break;
    case ROBOT_PINK_MAIN:
        glColor3f(1, 1, 1);
        break;
    case ROBOT_PINK_SUB:
        glColor3f(255 / 255.0, 219 / 255.0, 237 / 255.0);
        break;
    case HOME_COLOR:
        glColor3f(105 / 255.0, 66 / 255.0, 27 / 255.0);
        break;
    }
}
void init_camera() {
    eye[0] = Eye[0];
    eye[1] = Eye[1];
    eye[2] = Eye[2];
    zNear = 20;
    zFar = 50;
    aspect = width / (double)height;
    fovy = 70;
}
void myinit()
{
    
    glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */
    /*Clear the Depth & Color Buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glFlush();/*Enforce window system display the results*/
    
    /*---- Compute cos(5.0) and sin(5.0) ----*/
    cv = cos(5.0 * PI / 180.0);
    sv = sin(5.0 * PI / 180.0);

    //�Ӭ۾���m�]�m�A��v���׳]�m(��l��)
    init_camera();

    //�w�q�����H�C��
    myRobot.setColor(ROBOT_BLUE_MAIN, ROBOT_BLUE_SUB);
    pupu.setColor(ROBOT_BLUE_MAIN, ROBOT_BLUE_SUB);
    jakao.setColor(ROBOT_PINK_MAIN, ROBOT_PINK_SUB);
    pupu.isMagician = 1;   //���۪������H
    pupu.carry_mw = 0;
    //��ê���y�Ц���
    for (pp p : river) {
        for (int i = p.a1; i < p.a2; i++) {
            limit.push_back({ p.x + p.r * cos(i * 0.01745) ,p.z + p.r * sin(i * 0.01745) });
        }
    }
}
void draw_cube() {
    int i;
    glPolygonMode(GL_FRONT, GL_FILL);
    for (i = 0; i < 6; i++) {     /* draw the six faces one by one */
        glBegin(GL_POLYGON);  /* Draw the face */
        glVertex3fv(points[face[i][0]]);
        glVertex3fv(points[face[i][1]]);
        glVertex3fv(points[face[i][2]]);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
}
void draw_cube(int x) {
    int i;
    glPolygonMode(GL_FRONT, x);
    for (i = 0; i < 6; i++) {     /* draw the six faces one by one */
        glBegin(GL_POLYGON);  /* Draw the face */
        glVertex3fv(points[face[i][0]]);
        glVertex3fv(points[face[i][1]]);
        glVertex3fv(points[face[i][2]]);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
}
void draw_circle(double size, int wid) {    //�j�p �u�e��
    glLineWidth(wid);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 1440; ++i)
        glVertex3f(size * cos(i * 0.01745), 0, size * sin(i * 0.01745));
    glEnd();
}
void draw_square(int hei, int wid,int sz) {     //���b�a�W������� �w�q: x�b�V���e�Az�V����
    glLineWidth(sz);
    glPushMatrix();
    glScaled(wid, 0, hei);
    glBegin(GL_POLYGON);
    glVertex3f(0, 0.0, 1);
    glVertex3f(1, 0.0, 1);
    glVertex3f(1, 0.0, 0);
    glVertex3f(0, 0.0, 0);
    glEnd();
    glPopMatrix();
}
void draw_magic_wand() {
    myMagic_wand.draw();
}
void draw_home() {               //�������I
    float offsetx = 35;
    float offsetz = 30;
    change_color(HOME_COLOR);
    //�W�l 70 * 60
    glPushMatrix();
    glTranslatef(-offsetx, 0, -offsetz);
    glScalef(2.5, 40, 2.5);
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(offsetx, 0, -offsetz);
    glScalef(2.5, 40, 2.5);
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-offsetx, 0, offsetz);
    glScalef(2.5, 40, 2.5);
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(offsetx, 0, offsetz);
    glScalef(2.5, 40, 2.5);
    draw_cube();
    glPopMatrix();

    glPushMatrix();                               //��γ� 
    glTranslatef(-offsetx, 40, -offsetz);
    glScalef(offsetx * 2, 1, 1);
    draw_cube();
    glPopMatrix();

    glPushMatrix();                               //��γ�
    glTranslatef(-offsetx, 40, offsetz);
    glScalef(offsetx * 2, 1, 1);
    draw_cube();
    glPopMatrix();

    for (int i = -offsetx; i <= offsetx; i += 5) {   //���γ�
        glPushMatrix();
        glTranslatef(i, 40, -offsetz);
        glScalef(2, 1, 2 * offsetz);
        draw_cube();
        glPopMatrix();
    }
}
void draw_debug(){
    glPushMatrix();
    glColor3f(1, 122 / 255.0, 189); //��

    glPushMatrix();
    for (int i = 0; i < debugModeCmd; i++) {
        glPushMatrix();
        glScalef(5, 5, 5);
        draw_cube(GL_LINE);
        glPopMatrix();
        glTranslatef(5, 0, 0);
    }
    glPopMatrix();

    glColor3f(128 / 255.0, 128 / 255.0, 1); //��
    glPushMatrix();
    for (int i = 0; i < debugModeCmd; i++) {
        glPushMatrix();
        glScalef(5, 5, 5);
        draw_cube(GL_LINE);
        glPopMatrix();
        glTranslatef(0, 5, 0);
    }
    glPopMatrix();

    glColor3f(1, 1, 168 / 255.0);  //��
    glPushMatrix();
    for (int i = 0; i < debugModeCmd; i++) {
        glPushMatrix();
        glScalef(5, 5, 5);
        draw_cube(GL_LINE);
        glPopMatrix();
        glTranslatef(0, 0, 5);
    }
    glPopMatrix();

    glPopMatrix();
}
void draw_magic_field() {
    //�]�k�}
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRotatef(90, 0, 1, 0);                          //����90��(��y�b) -> ���ܤ�G�}�f
    draw_circle(30, 1);
    draw_circle(29, 1);
    draw_circle(27.8, 1); //1.2
    draw_circle(27.3, 1); //0.5

    glLineWidth(1);                        //���u
    for (int i = 0; i < 360; i += 15) {
        glBegin(GL_LINES);
        glVertex3f(25.3 * cos(i * 0.01745), 0, 25.3 * sin(i * 0.01745));
        glVertex3f(27.3 * cos(i * 0.01745), 0, 27.3 * sin(i * 0.01745));
        glEnd();
    }

    draw_circle(25.3, 1);   //2
    draw_circle(24.8, 1);   //0.5

    glLineWidth(2);
    for (int t = 0; t < 3; t++) {            //�P�P�}
        glBegin(GL_POLYGON);
        for (int i = (t * 30); i < 360; i += 90) {
            glVertex3f(24.8 * cos(i * 0.01745), 0, 24.8 * sin(i * 0.01745));
        }
        glEnd();
    }

    draw_circle(17, 1);       //6

    draw_circle(14, 4);
    for (float t = 0; t < 12; t += 0.5) {     //��G�}
        glLineWidth(4);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 1440; ++i)
            glVertex3f((14 - t * 0.2) * cos(i * 0.01745), 0, (t * 0.23) + (14 - t * 0.2) * sin(i * 0.01745));
        glEnd();
    }

    for (int i = 0, j = 0; i < 360; i += 15, j++) { //�Ӷ��}
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
    draw_circle(7.7, 2);         //0.5
}
void draw_scene(int mode) {
    if (mode == MAGICFIELD) {        //�]�k�} ��m(30,30) ��ɭ���(60,60) todo:�I�ɫG��
        if (!myRobot.isMagician) change_color(ICE_COLOR);
        else glColor3f(235 / 255.0, 244 / 255.0, 255 / 255.0);
        glPushMatrix();
        glTranslatef(30, 0, 30);       //�k�}�� lcs
        draw_magic_field();
        glPopMatrix();
        if (debugMode) {
            myFloor.draw();
            draw_debug();
        }

        glPushMatrix();
        glTranslatef(30, 7, 30);      //�k�M�� lcs  �Ʀb�Ť�
        myMagic_wand.setPos(30, 7, 30);
        if (myMagic_wand.show) draw_magic_wand();
        glPopMatrix();
    }
    else if (mode == GRASSLAND) {
        glColor3f(1 / 255.0, 152 / 255.0, 89 / 255.0); //��a
        glPushMatrix();
        glTranslatef(0, -10.5, 0);
        glScalef(200, 10, 200);
        draw_cube();
        glPopMatrix();

        glColor3f(204 / 255.0, 1, 204 / 255.0);     //���
        draw_square(200, 200,1);

        glPushMatrix();                             //�ಾ�k�}(17,12) 20*20
        glColor3f(188 / 255.0, 217 / 255.0, 246 / 255.0);
        glTranslatef(17, 0.3, 12);
        glScalef(1 / 3.0, 1 / 3.0, 1 / 3.0);
        draw_magic_field();
        glPopMatrix();

        //glColor3f(0, 0, 0);         //��l�u(�}�o��)
        //glPushMatrix();
        //for (int i = 0; i < 200; i += 10) {
        //    if(i%100 == 0) glLineWidth(3);
        //    else if (i%50  == 0) glLineWidth(2);
        //    else glLineWidth(1);
        //    glBegin(GL_LINES);
        //    glVertex3f(i, 0.1, 0);
        //    glVertex3f(i, 0.1, 200);
        //    glVertex3f(0, 0.1, i);
        //    glVertex3f(200, 0.1, i);
        //    glEnd();
        //}
        //glPopMatrix();

        //pool + river
        //{ {-10,120 ,100,0,30},{190,120 ,100,180,220}, {24,9 ,100,-3,30}, {100,70 ,100,80,120}, {130,267 ,100,260,290},{200,80 ,100,93,113} };
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glColor3f(188 / 255.0, 217 / 255.0, 246 / 255.0);
        glPushMatrix();
        glTranslatef(30, 0.5, 160);
        draw_circle(30, 1);
        glPopMatrix();
        for (const pp p : river) {
            for (int i = p.a1; i < p.a2; i++) {
                glPushMatrix();
                glTranslatef(p.x + p.r * cos(i * 0.01745), 0.5, p.z + p.r * sin(i * 0.01745));
                draw_circle(5, 1);
                glPopMatrix();
            }
        }

        //pool��rock
        for (int i = 10; i < 356; i += 2) {
            glPushMatrix();
            glTranslatef(30 + 30 * cos(i * 0.01745), 0.5, 160 + 30 * sin(i * 0.01745));
            if (i % 7 == 0) {
                glColor3f(92 / 255.0, 92 / 255.0, 92 / 255.0);
                glutSolidSphere(1.75, 10, 10);
            }
            else if (i % 5 == 0) {
                glColor3f(191 / 255.0, 191 / 255.0, 191 / 255.0);
                glutSolidSphere(1.5, 10, 10);
            }
            else if (i % 3 == 0) {
                glColor3f(122 / 255.0, 122 / 255.0, 122 / 255.0);
                glutSolidSphere(2, 10, 10);
            }
            else if (i % 2 == 0) {
                glColor3f(61 / 255.0, 61 / 255.0, 61 / 255.0);
                glutSolidSphere(1.25, 10, 10);
            }
            else {
                glColor3f(38 / 255.0, 38 / 255.0, 38 / 255.0);
                glutSolidSphere(0.5, 10, 10);
            }
            glPopMatrix();
        }
        //rock �~��
        for (int j = 0; j < rock.size(); j++) {
            pp p = rock[j];
            int l = p.a1, r = p.a2;
            float offset = 0;
            if (j == 2) l -= 3;
            if (j == 3) r += 5;
            if (j >= 3)offset += 0.4;
            for (int i = l; i < r; i++) {
                glPushMatrix();
                if (i % 7 == 0) {
                    glTranslatef(p.x + 3.75 + offset + p.r * cos(i * 0.01745), 0.5, p.z + 3.75 + offset + p.r * sin(i * 0.01745));
                    glColor3f(92 / 255.0, 92 / 255.0, 92 / 255.0);
                    glutSolidSphere(1.75, 10, 10);
                }
                else if (i % 5 == 0) {
                    glTranslatef(p.x + 4 + offset + p.r * cos(i * 0.01745), 0.5, p.z + 4 + offset + p.r * sin(i * 0.01745));
                    glColor3f(191 / 255.0, 191 / 255.0, 191 / 255.0);
                    glutSolidSphere(1.5, 10, 10);
                }
                else if (i % 3 == 0) {
                    glTranslatef(p.x + 3.5 + offset + p.r * cos(i * 0.01745), 0.5, p.z + 3.5 + offset + p.r * sin(i * 0.01745));
                    glColor3f(122 / 255.0, 122 / 255.0, 122 / 255.0);
                    glutSolidSphere(2, 10, 10);
                }
                else if (i % 2 == 0) {
                    glTranslatef(p.x + 3.75 + offset + p.r * cos(i * 0.01745), 0.5, p.z + 3.75 + offset + p.r * sin(i * 0.01745));
                    glColor3f(61 / 255.0, 61 / 255.0, 61 / 255.0);
                    glutSolidSphere(1.25, 10, 10);
                }
                else {
                    glTranslatef(p.x + 4 + offset + p.r * cos(i * 0.01745), 0.5, p.z + 4 + offset + p.r * sin(i * 0.01745));
                    glColor3f(38 / 255.0, 38 / 255.0, 38 / 255.0);
                    glutSolidSphere(0.5, 10, 10);
                }
                glPopMatrix();
            }
        }
        //rock ����       
        for (int j = 0; j < rock.size(); j++) {
            pp p = rock[j];
            int l = p.a1, r = p.a2;
            float offset = 0;
            if (j == 4) {
                r -= 4;
                offset -= 1;
            }
            if (j == 0) r += 2;
            if (j >= 3) offset -= 0.5;
            if (j == 6) l -= 5;
            for (int i = l; i < r; i++) {
                glPushMatrix();
                if (i % 7 == 0) {
                    glTranslatef(p.x - 3.75 + offset + p.r * cos(i * 0.01745), 0.5, p.z - 3.75 + offset + p.r * sin(i * 0.01745));
                    glColor3f(92 / 255.0, 92 / 255.0, 92 / 255.0);
                    glutSolidSphere(1.75, 10, 10);
                }
                else if (i % 5 == 0) {
                    glTranslatef(p.x - 4 + offset + p.r * cos(i * 0.01745), 0.5, p.z - 4 + offset + p.r * sin(i * 0.01745));
                    glColor3f(191 / 255.0, 191 / 255.0, 191 / 255.0);
                    glutSolidSphere(1.5, 10, 10);
                }
                else if (i % 3 == 0) {
                    glTranslatef(p.x - 3.5 + offset + p.r * cos(i * 0.01745), 0.5, p.z - 3.5 + offset + p.r * sin(i * 0.01745));
                    glColor3f(122 / 255.0, 122 / 255.0, 122 / 255.0);
                    glutSolidSphere(2, 10, 10);
                }
                else if (i % 2 == 0) {
                    glTranslatef(p.x - 3.75 + offset + p.r * cos(i * 0.01745), 0.5, p.z - 3.75 + offset + p.r * sin(i * 0.01745));
                    glColor3f(61 / 255.0, 61 / 255.0, 61 / 255.0);
                    glutSolidSphere(1.25, 10, 10);
                }
                else {
                    glTranslatef(p.x - 4 + offset + p.r * cos(i * 0.01745), 0.5, p.z - 4 + offset + p.r * sin(i * 0.01745));
                    glColor3f(38 / 255.0, 38 / 255.0, 38 / 255.0);
                    glutSolidSphere(0.5, 10, 10);
                }
                glPopMatrix();
            }
        }
        //road
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for (int i = 115; i < 168; i += 6) {              //180�q�Y
            glBegin(GL_POLYGON);
            glColor3f(133 / 255.0, 66 / 255.0, 0 / 255.0);
            glVertex3f(150 + 150 * cos(i * 0.01745), 0.4, 0 + 100 * sin(i * 0.01745));
            glVertex3f(150 + 150 * cos((i + 3) * 0.01745), 0.4, 0 + 100 * sin((i + 3) * 0.01745));
            glColor3f(96 / 255.0, 47 / 255.0, 47 / 255.0);
            glVertex3f(150 + 120 * cos((i + 3) * 0.01745), 0.4, 0 + 80 * sin((i + 3) * 0.01745));
            glVertex3f(150 + 120 * cos(i * 0.01745), 0.4, 0 + 80 * sin(i * 0.01745));
            glEnd();
        }
        for (int i = -46; i < 30; i += 6) {
            glBegin(GL_POLYGON);
            glColor3f(96 / 255.0, 47 / 255.0, 47 / 255.0);
            glVertex3f(4 + 150 * cos(i * 0.01745), 0.4, 150 + 100 * sin(i * 0.01745));
            glVertex3f(4 + 150 * cos((i + 3) * 0.01745), 0.4, 150 + 100 * sin((i + 3) * 0.01745));
            glColor3f(133 / 255.0, 66 / 255.0, 0 / 255.0);
            glVertex3f(4 + 120 * cos((i + 3) * 0.01745), 0.4, 150 + 80 * sin((i + 3) * 0.01745));
            glVertex3f(4 + 120 * cos(i * 0.01745), 0.4, 150 + 80 * sin(i * 0.01745));
            glEnd();
        }
        //home   
        glPushMatrix();
        glTranslatef(145, 0, 130);               //70*50
        draw_home();
        glPopMatrix();

        //big chair
        myBig_chair.setPos(160, 0, 110);
        glPushMatrix();                        //40*15
        glTranslatef(160, 25, 110);            //����
        glRotatef(45, 0, 1, 0);
        glRotatef(180, 0, 1, 0);
        myBig_chair.draw();
        glPopMatrix();

    }
}
void draw_cylinder(double up, double down, double height) {
    if (cylind == NULL) {
        cylind = gluNewQuadric();
    }
    /*--- Draw a cylinder ---*/
    gluCylinder(cylind, up, down, /* radius of top and bottom circle */
        height,                   /* height of the cylinder */
        20,                       /* use 12-side polygon approximating circle*/
        4);                       /* Divide it into 3 sections */
}
void draw_robot() {
    if (sitOnChair) return;  //���b�Ȥl�W���n�e
    myRobot.draw();
}
void draw_camera() {
    glPushMatrix();

    glPushMatrix();
    glScalef(3,3,3);
    camera.draw();
    glPopMatrix();

    double z1, x1, y1, z2, x2, y2;
    z1 = zNear / cos((fovy / 2.0) * PI / 180.0); //����
    y1 = z1 * sin((fovy / 2.0) * PI / 180.0);    //�e
    x1 = y1 * aspect;

    z2 = zFar / cos((fovy / 2.0) * PI / 180.0); //����
    y2 = z2 * sin((fovy / 2.0) * PI / 180.0);    //�e
    x2 = y2 * aspect;
    glColor4f(1,1,0,0.5);
    glBegin(GL_TRIANGLES);
    //��
    glVertex3f(0, 0, 0);
    glVertex3f(-x1, y1, zNear);
    glVertex3f(x1, y1, zNear);

    glVertex3f(0, 0, 0);
    glVertex3f(-x1, y1, zNear);
    glVertex3f(-x1, -y1, zNear);

    glVertex3f(0, 0, 0);
    glVertex3f(x1, y1, zNear);
    glVertex3f(x1, -y1, zNear);

    glVertex3f(0, 0, 0);
    glVertex3f(-x1, -y1, zNear);
    glVertex3f(x1, -y1, zNear);
    glColor4f(1, 1, 1, 0.5);
    glEnd();
    //�~
    glBegin(GL_QUADS);
    glVertex3f(-x1, y1, zNear);
    glVertex3f(x1, y1, zNear);
    glVertex3f(x2, y2, zFar);
    glVertex3f(-x2 , y2 , zFar);

    glVertex3f(-x1, y1, zNear);
    glVertex3f(-x1, -y1, zNear);
    glVertex3f(-x2, -y2, zFar);
    glVertex3f(-x2, y2, zFar);

    glVertex3f(x1, y1, zNear);
    glVertex3f(x1, -y1, zNear);
    glVertex3f(x2 , -y2, zFar);
    glVertex3f(x2, y2, zFar);

    glVertex3f(-x1, -y1, zNear);
    glVertex3f(x1, -y1, zNear);
    glVertex3f(x2, -y2, zFar);
    glVertex3f(-x2 , -y2, zFar);
    glEnd();
    /*
    glColor4f(1, 0, 0, 0.5);
    glBegin(GL_POLYGON);
    glVertex3f(x1 , y1 , zNear);
    glVertex3f(x1 , -y1, zNear);
    glVertex3f(-x1, -y1, zNear);
    glVertex3f(-x1, y1 , zNear);
    glEnd();
    glColor4f(1, 0, 1, 0.5);
    glBegin(GL_POLYGON);
    glVertex3f(x2 , y2 , zFar);
    glVertex3f(x2 , -y2, zFar);
    glVertex3f(-x2, -y2, zFar);
    glVertex3f(-x2, y2 , zFar);
    glEnd();
    */
    glPopMatrix();
}
void draw_view() {
    draw_scene(scene);
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);      //�����H
    draw_robot();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(eye[0], eye[1], eye[2]);
    glRotatef(eyeAngx, 1, 0, 0);
    glRotatef(eyeAngy, 0, 1, 0);
    glRotatef(eyeAngz, 0, 0, 1);
    glRotatef(180, 0, 1, 0);
    draw_camera();
    glPopMatrix();
}
void make_projection(int x)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (x == 3) {
        gluPerspective(fovy, aspect, zNear, zFar); 
    }
    else {
        glOrtho(-40.0, 40.0, -40.0, 40.0, -100.0, 200);
    }
    glMatrixMode(GL_MODELVIEW);
}
void make_view(int x)
{
    //�۾���m    �۾���Ǫ���m   �۾��V�W������
    //gluLookAt(pos[0]-5, 30, pos[2] + 30, pos[0], 15, pos[2], 0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    switch (x) {
    case 0:       /* X direction parallel viewing */
        gluLookAt(0.0, 0.0, 30.0, 30.0, 0.0, 30.0, 0.0, 1.0, 0.0);
        break;
    case 1:       /* Y direction parallel viewing */
        gluLookAt(30.0, 30.0, 30.0, 30.0, 0.0, 30.0, 0.0, 0.0, -1.0);
        break;
    case 2:       /* Z direction parallel viewing */
        gluLookAt(30.0, 0.0, 60.0, 30.0, 0.0, 30.0, 0.0, 1.0, 0.0);
        break;
    case 3:       /* Perspective */
        /* In this sample program, eye position and Xe, Ye, Ze are computed
           by ourselves. Therefore, use them directly; no trabsform is
           applied upon eye coordinate system
           */
        //gluLookAt(30.0, 30.0, 80.0, 30.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        //u�Oeye���y�Шt��
        gluLookAt(eye[0], eye[1], eye[2], eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2], u[1][0], u[1][1], u[1][2]);
        break;
    case 5:
        gluLookAt(pos[0] - 5, 30, pos[2] + 30, pos[0], 15, pos[2], 0.0, 1.0, 0.0);
    }
}
void display()
{
    //printf("display\n");
    /*Clear previous frame and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*----Define the current eye position and the eye-coordinate system---*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    switch (viewStyle) {
    case 0:
        glViewport(0, 0, width, height);
        make_projection(0);
        make_view(0);
        draw_view();
        break;
    case 1:
        glViewport(0, 0, width, height);
        make_projection(0);
        make_view(1);
        draw_view();
        break;
    case 2:
        glViewport(0, 0, width, height);
        make_projection(0);
        make_view(2);
        draw_view();
        break;
    case 3:
        glViewport(0, 0, width, height);
        make_projection(3);
        make_view(3);
        draw_view();
        break;
    case 4:
        make_projection(0);

        glViewport(0, height / 2, width / 2, height / 2);  //���U �e��
        make_view(0);
        draw_view();

        glViewport(width / 2, height / 2, width / 2, height / 2);
        make_view(1);
        draw_view();

        glViewport(0, 0, width / 2, height / 2);
        make_view(2);
        draw_view();

        glViewport(width / 2, 0, width / 2, height / 2);
        make_projection(3);
        make_view(3);
        draw_view();
        break;
    case 5:
        glViewport(0, 0, width, height);
        make_projection(0);
        make_view(5);
        draw_view();
        break;
    }

    //�ʧ@
    //�۾���m    �۾���Ǫ���m   �۾��V�W������

    //�}�o�ΰѼ�
    //if(see)
    //    gluLookAt(35.0, 30.0, 80.0,       20.0, 0.0, 0.0,         0.0, 1.0, 0.0);         //�ʧ@
    //else
    //    gluLookAt(40.0, 70.0, 55.0,       25.0, 0.0, 25.0,        0.0, 1.0, 0.0);         //����
    //if (see)
    //gluLookAt(pos[0] - 5, 30, pos[2] + 30, pos[0], 15, pos[2], 0.0, 1.0, 0.0);
    //else
    //  gluLookAt(100, 150, 100, 100, 0, 100, 0.0, 0.0, -1.0);

    //draw_view();




    glutSwapBuffers();
    return;
}
void my_reshape(int w, int h)
{
    printf("res\n");
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(-40.0, 50.0, -40.0, 40.0, 0.0, 120); 

   //if(see)
    glOrtho(-40.0, 40.0, -40.0, 40.0, -100.0, 200);
    //else 
    //   glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 200);
    width = w; height = h;
    aspect = width / (double)height;  //��s�e����
}
void timerFunc(int nTimerID) {
    switch (nTimerID) {
    case RUNTIMER:                //�����]
        preKey = -1;
        break;
    case JUMPTIMER:               //���D
        if (!myRobot.jump()) {
            isLock = LOCK;        //���ɤ��i�H��
            glutTimerFunc(100, timerFunc, JUMPTIMER);
        }
        else {
            myRobot.stand();
            isLock = UNLOCK;
        }
        glutPostRedisplay();
        break;
    case JUMPONWANDTIMER:           //���W�k��
        if (!myRobot.jumpOnWand()) {
            isLock = LOCK;
            glutTimerFunc(100, timerFunc, JUMPONWANDTIMER);
        }
        else {
            isLock = UNLOCK;
        }
        glutPostRedisplay();
        break;
    case JUMPTOFLOORTIMER:          //���^�a��
        if (!myRobot.jumpToFloor()) {
            isLock = LOCK;
            glutTimerFunc(100, timerFunc, JUMPTOFLOORTIMER);
        }
        else {
            isLock = UNLOCK;
            myRobot.stand();
        }
        glutPostRedisplay();
        break;
    case CHAIR_MOVE:                //�n���\�\�\
        myBig_chair.move();
        if (scene == GRASSLAND) {
            glutTimerFunc(100, timerFunc, CHAIR_MOVE);
        }
        glutPostRedisplay();
        break;
    case DEBUG_MODE:
        if (debugModeCmd == 12) return;
        else glutTimerFunc(100 - debugModeCmd*4, timerFunc, DEBUG_MODE);
        debugModeCmd++;
        glutPostRedisplay();
        break;
    case OUT_LINE_LEFT:
        if (myFloor.light[2] == 12) {
            myFloor.light[2] = 0;
            return;
        }
        myFloor.light[2]++;
        glutTimerFunc(100, timerFunc, OUT_LINE_LEFT);
        glutPostRedisplay();
        break;
    case OUT_LINE_RIGHT:
        if (myFloor.light[3] == 12) {
            myFloor.light[3] = 0;
            return;
        }
        myFloor.light[3]++;
        glutTimerFunc(100, timerFunc, OUT_LINE_RIGHT);
        glutPostRedisplay();
        break;
    case OUT_LINE_FRONT:
        if (myFloor.light[4] == 12) {
            myFloor.light[4] = 0;
            return;
        }
        myFloor.light[4]++;
        glutTimerFunc(100, timerFunc, OUT_LINE_FRONT);
        glutPostRedisplay();
        break;
    case OUT_LINE_BACK:
        if (myFloor.light[5] == 12) {
            myFloor.light[5] = 0;
            return;
        }
        myFloor.light[5]++;
        glutTimerFunc(100, timerFunc, OUT_LINE_BACK);
        glutPostRedisplay();
        break;
    }
}
bool detectCollision(int x, int y, int z) { //�����I��
    //�P�_�I���ê��
    for (int i = -7; myMagic_wand.show && i < 7; i++) {
        if (getDis(x, z, myMagic_wand.x, myMagic_wand.z + i) < 3.5) return 1;  //magic wand
    }
    if (scene == GRASSLAND) {
        if (myRobot.moveMode != FLY) {
            for (auto i : limit) {
                if (getDis(x, z, i.first, i.second) < 5) return 1;     //river
            }
            if (getDis(x, z, 30, 160) < 30) return 1;                  //pool
        }
        for (auto i : pillar) {
            if (getDis(x, z, i.first, i.second) < 1.25 + 2 * 2.5) return 1;     //pillar �W�l    
        }
        if (getDis(x, z, myBig_chair.x, myBig_chair.z) < 18 + 2 * 2.5) return 1;  //chair
    }

    //�P�_���
    if (scene == MAGICFIELD) {
        if (debugMode) {
            if (x < 0) { //��
                if(!myFloor.light[2]) glutTimerFunc(100, timerFunc, OUT_LINE_LEFT);
            }
            else if (x > 60) { //�k
                if (!myFloor.light[3])glutTimerFunc(100, timerFunc, OUT_LINE_RIGHT);
            }
            else if (z < 0) { //��
                if (!myFloor.light[5])glutTimerFunc(100, timerFunc, OUT_LINE_BACK);
            }
            else if (z > 60) { //�e
                if(!myFloor.light[4]) glutTimerFunc(100, timerFunc, OUT_LINE_FRONT);
            }
        }
        if (x < 0 || x > 60 || z < 0 || z > 60) return 1;
    }
    else if (scene == GRASSLAND) {
        if (x < 0 || x > 200 || z < 0 || z > 200) return 1;
    }
    return 0;
}
void my_move_order(unsigned char key) {        //�򲾰ʬ������P�_
    if (sitOnChair) return; //�p�G���b�Ȥl�W�N�����
    float tpPos[3] = { pos[0], pos[1], pos[2] };
    float offset = 0;
    if (myRobot.moveMode == WALK && preKey == key && (key == 'W' || key == 'w' || key == 'A' || key == 'a' || key == 'S' || key == 's' || key == 'D' || key == 'd'))
        myRobot.moveMode = RUN;                       //0.3���s��� �N�ܦ��]�]
    if (myRobot.moveMode == RUN) offset = 2;          //�]�B�@����2
    else if (myRobot.moveMode == WALK) offset = 0.5;  //�����@��0.5
    else if (myRobot.moveMode == FLY) offset = 3;     //����@��5

    if (key == 'S' || key == 's') {
        if (myRobot.isOnWand) {        //����Ҧ�
            myRobot.angle_y = 90;
        }
        else {                         //�@�먫���Ҧ�
            myRobot.angle_y = 0;
            myRobot.move();
        }
        tpPos[2] += offset;
    }
    else if (key == 'W' || key == 'w') {
        if (myRobot.isOnWand) {
            myRobot.angle_y = 90;
        }
        else {
            myRobot.angle_y = 180;
            myRobot.move();
        }
        tpPos[2] -= offset;
    }
    else if (key == 'A' || key == 'a') {
        if (myRobot.isOnWand) {
            myRobot.angle_y = 0;
        }
        else {
            myRobot.angle_y = 270;
            myRobot.move();
        }
        tpPos[0] -= offset;
    }
    else if (key == 'D' || key == 'd') {
        if (myRobot.isOnWand) {
            myRobot.angle_y = 0;
        }
        else {
            myRobot.angle_y = 90;
            myRobot.move();
        }
        tpPos[0] += offset;
    }
    else if (key == 'r' || key == 'R') {            //����
        myRobot.angle_y += 5;
        angley++;
        if (!myRobot.isOnWand) {
            myRobot.moveMode = TURN;
            myRobot.move();    //�b�a�O�~�n�ʸ}
        }
    }
    if (detectCollision(tpPos[0], tpPos[1], tpPos[2])) return;
    for (int i = 0; i < 3; i++) pos[i] = tpPos[i];
    display();
}
bool change_view_order(int key) {
    if (key == 103) {       //�W
        eyeDy += 0.5;       /* move up */
        for (int i = 0; i < 3; i++) eye[i] -= 0.5 * u[1][i];
    }
    else if (key == 101) {   //�U
        eyeDy += -0.5;       /* move down */
        for (int i = 0; i < 3; i++) eye[i] += 0.5 * u[1][i];
    }
    else if (key == 102) {   //��
        eyeDx += -0.5;       /* move left */
        for (int i = 0; i < 3; i++) eye[i] += 0.5 * u[0][i];
    }
    else if (key == 100) {   //�k
        eyeDx += 0.5;        /* move right */
        for (int i = 0; i < 3; i++) eye[i] -= 0.5 * u[0][i];
    }
    else return 0;
    display();
    return 1;
}
bool change_view_order(unsigned char key) {
    cout << key << "\n";
    if (key == 'y' || key == 'Y') {
        viewStyle++;
        viewStyle %= 6;
        display();
        return 1;
    }
    if (key == 'x' || key == 'X') {
        if (scene != MAGICFIELD) return 1;   //�u���b�]�k�}���o�ӼҦ�
        if (debugModeCmd != 0 && debugModeCmd != 12) return 1;  //�e���ʵe�S�]�� ���n�@�Ȩ�s
        debugMode ^= 1;
        debugModeCmd = 0;
        if (debugMode) {
            glutTimerFunc(100, timerFunc, DEBUG_MODE);
        }
        return 1;
    }
    float  x[3], y[3], z[3];
    int i;
    if (key == 19) {       //�U ctrl + w
        //eyeDy += -0.5;    
        for (int i = 0; i < 3; i++) eye[i] -= 0.5 * u[1][i];
    }
    else if (key == 23) {   //�W ctrl + s
        //eyeDy += 0.5;     
        for (int i = 0; i < 3; i++) eye[i] += 0.5 * u[1][i];
    }
    else if (key == 4) {   //�k ctrl + d
        //eyeDx += -0.5;     
        for (int i = 0; i < 3; i++) eye[i] += 0.5 * u[0][i];
    }
    else if (key == 1) {   //�� ctrl + a
        //eyeDx += 0.5;      
        for (int i = 0; i < 3; i++) eye[i] -= 0.5 * u[0][i];
    }
    if (key == 17) {
        //eyeDz += 0.5;    //���e ctrl + q
        for (i = 0; i < 3; i++) eye[i] -= 0.5 * u[2][i];
    }
    else if (key == 5) {
        //eyeDz += -0.5;   //���� ctrl + e
        for (i = 0; i < 3; i++) eye[i] += 0.5 * u[2][i];
    }
    else
    if (key == 24) {             //ctrl + x pitching 
        eyeAngx += 5.0;
        if (eyeAngx > 360.0) eyeAngx -= 360.0;
        y[0] = u[1][0] * cv - u[2][0] * sv;
        y[1] = u[1][1] * cv - u[2][1] * sv;
        y[2] = u[1][2] * cv - u[2][2] * sv;

        z[0] = u[2][0] * cv + u[1][0] * sv;
        z[1] = u[2][1] * cv + u[1][1] * sv;
        z[2] = u[2][2] * cv + u[1][2] * sv;

        for (i = 0; i < 3; i++) {
            u[1][i] = y[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 25) {            // heading ctrl + y
        eyeAngy += 5.0;
        if (eyeAngy > 360.0) eyeAngy -= 360.0;
        for (i = 0; i < 3; i++) {
            x[i] = cv * u[0][i] - sv * u[2][i];
            z[i] = sv * u[0][i] + cv * u[2][i];
        }
        for (i = 0; i < 3; i++) {
            u[0][i] = x[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 26) {            //ctrl + z rolling
        eyeAngz += 5.0;
        if (eyeAngz > 360.0) eyeAngz -= 360.0;
        for (i = 0; i < 3; i++) {
            x[i] = cv * u[0][i] - sv * u[1][i];
            y[i] = sv * u[0][i] + cv * u[1][i];
        }
        for (i = 0; i < 3; i++) {
            u[0][i] = x[i];
            u[1][i] = y[i];
        }
    }else return 0;
    display();
}
void special_func(int key, int x, int y) {
    //cout <<"special: " << key << "\n";
    if(change_view_order(key)) return;
}
void keyboardUp_func(unsigned char key, int x, int y) {
    if (isLock == LOCK) return;
    glutTimerFunc(200, timerFunc, RUNTIMER);
    if (myRobot.moveMode != FLY && preKey != key)  myRobot.moveMode = WALK;
    preKey = key;
    if (myRobot.moveMode != FLY && (key == 'W' || key == 'w' || key == 'A' || key == 'a' || key == 'S' || key == 's' || key == 'D' || key == 'd' || key == 'r' || key == 'R')) myRobot.stand();
    display();
}
void keybaord_fun(unsigned char key, int x, int y) {
    printf("key: %d\n", key);
    if(change_view_order(key)) return;
    if (isLock == LOCK) return;
    my_move_order(key);
    if ((key == 'p' || key == 'P')) {                     //���ʪF��
        if (myMagic_wand.show) {
            for (int i = -7; i < 7; i++) {
                if (getDis(pos[0], pos[2], myMagic_wand.x, myMagic_wand.z + i) < 3.5 + 5) {           //���k��
                    myMagic_wand.show = 0;
                    myRobot.carry_mw = 1;
                    myRobot.isMagician = 1;
                }
            }
        }
        if (scene == GRASSLAND && getDis(pos[0], pos[2], myBig_chair.x, myBig_chair.z) < 18 + 2 * 2.5 + 2) {   //���W�Ȥl
            sitOnChair = !sitOnChair;
        }
    }
    if (key == ' ') {                   //��
        if (myRobot.isMagician) {
            if (myRobot.isOnWand) {
                glutTimerFunc(100, timerFunc, JUMPTOFLOORTIMER);
                myRobot.moveMode = WALK;
            }
            else {
                myRobot.jump_ready();
                glutTimerFunc(100, timerFunc, JUMPONWANDTIMER);
                myRobot.moveMode = FLY;
            }
        }
        else {
            myRobot.jump_ready();
            glutTimerFunc(100, timerFunc, JUMPTIMER);
        }
    }
    else if ((int)key == 13) {          //enter �i�J��a(�_�l�I17 12)
        if (scene == MAGICFIELD && myRobot.isMagician && getDis(pos[0], pos[2], 30, 30) < 30) {
            pos[0] = 17;
            pos[2] = 12;
            scene = GRASSLAND;
            glutTimerFunc(100, timerFunc, CHAIR_MOVE);
        }                               //enter �i�J�]�k�}(�_�l�I30 30)
        else if (scene == GRASSLAND && getDis(pos[0], pos[2], 17, 12) <= 10) {
            pos[0] = pos[2] = 30;
            scene = MAGICFIELD;
        }
    }
    //�}�o�� ���䴩!!
    //if (key == 'u') glutTimerFunc(100, timerFunc, CHAIR_MOVE);
    //if (key == 'o' || key == 'O') {
    //    cout << ++myRobot.magic_wand_carry->angle_x << "\n";
    //}
    if (key == 'Y' || key == 'y') {
        see = ~see;
    }
    //if (key == '1') {
    //    myRobot.right_h->shoulderAng_x+=3;
    //    if (myRobot.right_h->shoulderAng_x > 360) myRobot.right_h->shoulderAng_x -= 360;
    //    cout << myRobot.right_h->shoulderAng_x << "\n";
    //}
    //if (key == '2') {
    //    myRobot.right_h->fingerAng_y+=3;
    //    if (myRobot.right_h->fingerAng_y > 360) myRobot.right_h->fingerAng_y -= 360;
    //    cout << myRobot.right_h->fingerAng_y << "\n";
    //}
    //if (key == '5') {
    //    myRobot.left_h->shoulderAng_x += 3;
    //    if (myRobot.left_h->shoulderAng_x > 360) myRobot.left_h->shoulderAng_x -= 360;
    //    cout << myRobot.left_h->shoulderAng_x << "\n";
    //}
    //if (key == '6') {
    //    myRobot.left_h->fingerAng_y += 3;
    //    if (myRobot.left_h->fingerAng_y > 360) myRobot.left_h->fingerAng_y -= 360;
    //    cout << myRobot.left_h->fingerAng_y << "\n";
    //}
    //if (key == '3') {
    //    myRobot.right_h->shoulderAng_z+= 3;
    //    if (myRobot.right_h->shoulderAng_z > 360) myRobot.right_h->shoulderAng_z -= 360;
    //    cout << myRobot.right_h->shoulderAng_z << "\n";
    //}
    //if (key == '3') {
    //    myRobot.left_f->kneeAng_x += 3;
    //    if (myRobot.left_f->kneeAng_x > 360)  myRobot.left_f->kneeAng_x -= 360;
    //    cout << myRobot.left_f->kneeAng_x << "\n";
    //}
    //else if (key == '4') {
    //    myRobot.left_f->hipJointAng_x++;
    //    if (myRobot.right_f->hipJointAng_x > 360)  myRobot.left_f->hipJointAng_x -= 360;
    //    cout << myRobot.left_f->hipJointAng_x << "\n";
    //}
    //else if (key == '3') {
    //    myRobot.right_f->ankle_x++;
    //    if (myRobot.right_f->ankle_x > 360)  myRobot.right_f->ankle_x -= 360;
    //    cout << myRobot.right_f->ankle_x << "\n";
    //}
    //else if (key == '4') {
    //    myRobot.angle_x += 5;
    //    if (myRobot.angle_x > 360)  myRobot.angle_x -= 360;
    //    cout << myRobot.angle_x << "\n";
    //}
    display();
}
void motion_func(int  x, int y) {};
void passive_motion_func(int x, int y) {};
void mouse_func(int button, int state, int x, int y) {};
void idle_func(void) {};
void main(int argc, char** argv)
{
    /*-----Initialize the GLUT environment-------*/
    glutInit(&argc, argv);

    /*-----Depth buffer is used, be careful !!!----*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(700, 700);
    glutCreateWindow("RobotView");

    myinit();      /*---Initialize other state varibales----*/

    /*----Associate callback func's whith events------*/
    glutDisplayFunc(display);

    glutReshapeFunc(my_reshape);
    glutKeyboardFunc(keybaord_fun);

    glutIdleFunc(idle_func);
    glutMouseFunc(mouse_func);  /* Mouse Button Callback func */
    glutMotionFunc(motion_func);/* Mouse motion event callback func */
    glutPassiveMotionFunc(passive_motion_func);
    glutSpecialFunc(special_func);
    glutKeyboardUpFunc(keyboardUp_func);
    glutMainLoop();
}
