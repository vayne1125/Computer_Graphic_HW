/***************************************************
 * Example program: Draw a moving cube
 *   Author: S. K. Ueng
 *   National Taiwan Ocean Univ. Comp. Sci. Dept.
 */
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glut.h>
#include <vector>

#define   PI   3.1415927
 //Define polygon drwaing models, filled or outlined.
#define   FILL   1
#define   LINE  2   

#define ICE 100
#define FLOOR 101
#define WOOD 102

#define WALK 0
#define RUN 1
#define TURN 2
#define FLY 3

#define RUNTIMER 50
#define JUMPTIMER 51
#define JUMPONWANDTIMER 52
#define JUMPTOFLOORTIMER 53

#define LOCK true
#define UNLOCK false

#define MAGICFIELD 0
#define GRASSLAND 1

using namespace std;

//coordinates of the 8 corners.(0,0,0)-(1,1,1)
float  points[][3] = { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
                      {1.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
                      {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
{1.0, 1, 1.0}, {0.0, 1.0, 1.0} };
//Define 6 faces using the 8 corners (vertices)
int    face[][4] = { {0, 1, 2, 3}, {7, 6, 5, 4}, {0, 4, 5, 1},
                    {1, 5, 6, 2}, {3, 2, 6, 7}, {0, 3, 7, 4} };
//Define the cube using the 6 faces.
int    cube[6] = { 0, 1, 2, 3, 4, 5 };

/* window shape */
int    width = 700, height = 700;
//define a base position in the z-x plane
float  pos[3] = { 0.0, 0.0, 0.0 };
//declare the rotational angle.
float  anglex = 0.0, angley = 0.0;

//Quodri objects for drawing the world coordinate system.
/*-----Define GLU quadric objects, a sphere and a cylinder----*/
GLUquadricObj* sphere = NULL, * cylind = NULL, * mycircle = NULL;
int polygonMode = FILL;
int see = 0;
int preKey = 0;
int scene = MAGICFIELD;
bool isLock = 0;
void draw_magic_field();
void draw_cube();
void draw_cylinder(double up, double down, double height);
void change_color(int value);
void draw_circle(double size, int wid);
void draw_square(int hei, int wid);
float getDis(float x1, float y1, float x2, float y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
struct node {
    double x = 0, y = 0, z = 0;
};
struct pp {
    float x1 = 0, x2 = 0, z1 = 0, z2 = 0;
    float x = 0, z = 0, r = 0;
    int a1 = 0, a2 = 0;
    pp(float x1_, float z1_, float x2_, float z2_) {
        x1 = x1_;
        x2 = x2_;
        z1 = z1_;
        z2 = z2_;
    }
    pp(float x_, float z_, float r_, int a1_, int a2_) {
        x = x_;
        z = z_;
        r = r_;
        a1 = a1_;
        a2 = a2_;
    }
};
const vector<pp> river = { {-10,120 ,100,0,30},{190,120 ,100,180,220}, {24,9 ,100,-3,30}, {100,70 ,100,80,120}, {130,267 ,100,260,290},{200,80 ,100,93,110} };
const vector<pair<float, float>> pillar = { {110,100},{180,100},{110,160},{180,160} };
vector<pair<float, float>>limit;
node ball_cor(double r, int A, int B) {
    node rt;
    rt.x = r * sin(A * 0.01745) * cos(B * 0.01745);
    rt.y = r * sin(A * 0.01745) * sin(B * 0.01745);
    rt.z = r * cos(A * 0.01745);
    return rt;
}
struct pupu {
    int scale = 0;
    pupu(int s = 1) {
        scale = s;
    }
    void draw() {

        glColor3f(1, 1, 0);

        glScalef(9, 9, 9);

        glTranslatef(0, 0.5, 0);

        glPushMatrix();
        glScalef(1.5, 1, 1);
        glutSolidSphere(0.5, 10, 10);
        glPopMatrix();

        glPushMatrix();                    //����

        glPopMatrix();
    }
}jakao;
struct magic_wand {
    float x = 0, y = 0, z = 0;                    //�ۤv���y��
    float angle_x = 0, angle_y = 0, angle_z = 0;
    float scale = 0;
    bool show = 1;
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
        change_color(WOOD);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glRotatef(angle_x, 1, 0, 0);
        glRotatef(angle_y, 0, 1, 0);
        glRotatef(angle_z, 0, 0, 1);
        glScalef(scale, scale, scale);
        //f
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

        glColor3f(168 / 255.0, 1.0, 1);         //�{����(�Ŧ�)
        glPushMatrix();
        glRotatef(30, 0, 1, 0);
        glutSolidTorus(0.1, 2.5, 100, 100);
        glPopMatrix();

        glColor3f(168 / 255.0, 1.0, 1);          //�{����(�Ŧ�)
        glPushMatrix();
        glRotatef(330, 0, 1, 0);
        glutSolidTorus(0.1, 2.5, 100, 100);
        glPopMatrix();
        glPopMatrix();

    }
}myMagic_wand;
struct robot {
    magic_wand* magic_wand_carry = new magic_wand(0.34, 180, 0, 0); //�]�k�Ϊ���(���)
    magic_wand* magic_wand_sit = new magic_wand(0.5, 0, -90, 0); //�]�k�Ϊ���(����)
    int moveMode = 0;
    //0->walk  1->run  2->turn  3->fly
    robot() {
        stand();
    }
    float angle_x = 0.0, angle_y = 0.0;                         //���ਤ��
    bool carry_mw = 0;                                          //�ثe�����]�k�ζ�
    bool isMagician = 0;                                        //�O���O�]�k�v
    bool isJump_ready = 0;                                      //�ǳƸ������ն� (���e��})
    float x = 0, y = 0, z = 0;                                  //��������H�y��
    struct hand {
        node tp;
        float shoulderAng_x = 180, shoulderAng_y = 0, shoulderAng_z = 35;   //�ӻH
        float elbowAng_x = 0, elbowAng_y = 0, elbowAng_z = 0;                //��y
        float fingerAng_x = 0, fingerAng_y = 0, fingerAng_z = 0;             //���
        void draw() {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //glRotatef(shoulderAng_x, 1, 0, 0);           //�������`���� 180��U
            glRotatef(shoulderAng_z, 0, 0, 1);             //-35��b������� ��z�౱��k
            glRotatef(shoulderAng_x, 1, 0, 0);
            glColor3f(173 / 255.0, 214 / 255.0, 1);
            glutSolidSphere(0.5, 10, 10);                //�b�|�� 0.5 ���ӻH

            //forarms
            glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);
            glTranslatef(0, 0.75, 0);              //���� �ӻH�W��0.25 + �ꤤ��0.75(�e1.5�����u) - 0.25���|

            glPushMatrix();
            glScalef(0.7, 1.5, 0.7);
            glutSolidSphere(0.5, 10, 10);          //���|��1.5�����u
            glPopMatrix();

            glTranslatef(0, 0.75, 0);               //���u�e�ݤ���

            glRotatef(elbowAng_x, 1, 0, 0);
            glRotatef(elbowAng_y, 0, 1, 0);
            glRotatef(elbowAng_z, 0, 0, 1);

            glColor3f(173 / 255.0, 214 / 255.0, 1);

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
            //������Y
            glColor3f(173 / 255.0, 214 / 255.0, 1);
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
    }right_h, left_h;
    struct foot {          //�L��2.75
        node tp;
        float hipJointAng_x = 180, hipJointAng_y = 0, hipJointAng_z = 0;   //�b���`
        float kneeAng_x = 0, kneeAng_y = 0, kneeAng_z = 0;
        float ankle_x = 0, ankle_y = 0, ankle_z = 0;
        void draw() {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glRotatef(hipJointAng_x, 1, 0, 0);          //�������`���� 180��U
            //glRotatef(-30, 0, 0, 1);            
            //glRotatef(90, 1, 0, 0);             //���e  ��x�b�౱���|�_��U
            glColor3f(173 / 255.0, 214 / 255.0, 1);
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
            glColor3f(173 / 255.0, 214 / 255.0, 1);
            glPushMatrix();
            glutSolidSphere(0.25, 10, 10);       //���|0.5�����\
            glPopMatrix();

            glTranslatef(0, 0.5, 0);             //���\�e�� 0.25 + �L����0.5(�L��1) - 0.25(���|�a��)

            glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);
            glPushMatrix();
            glScalef(0.5, 1, 0.5);               //�p�L��1
            glutSolidSphere(0.5, 10, 10);
            glPopMatrix();

            glTranslatef(0, 0.5, 0);             //�p�L�e�ݤ��� 0.5 �M�L���|0.25
            glRotatef(ankle_x, 1, 0, 0);

            glColor3f(0.5, 0.5, 0.5);
            glutSolidSphere(0.5, 10, 10);        //�} ���|1
        }
    }left_f, right_f;
    void draw_hat() {
        glPushMatrix();
        glRotatef(270, 1, 0, 0);  //�ઽ ���W�e
        glRotatef(-10, 1, 0, 0);
        //draw_cylinder(0,3,6); �Ȥl�ĪG
        //glRotatef(80, 1, 0, 0);

        glColor3f(38 / 255.0, 38 / 255.0, 38 / 255.0);          //�U�u
        glPushMatrix();
        draw_cylinder(3, 0, 1);

        glColor3f(61 / 255.0, 61 / 255.0, 61 / 255.0);  //�U�l
        draw_cylinder(2, 0, 4);

        //�᯾
        glColor3f(1, 1, 168 / 255.0);                      //�e
        glTranslatef(0, 0, 0.5);
        draw_cylinder(2, 1, 0.8);

        glColor3f(173 / 255.0, 214 / 255.0, 1);          //��
        glTranslatef(0, 0, 0.6);
        draw_cylinder(1.5, 0.8, 0.6);
        glPopMatrix();
        glPopMatrix();
    }
    void draw() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //�{�l
        glColor3f(219 / 255.0, 1, 1);

        glPushMatrix();
        glScalef(2.5, 2.5, 2.5);
        glTranslatef(x, y, z);                      //�㰦���y��

        glRotatef(angle_y, 0, 1, 0);
        glRotatef(angle_x, 1, 0, 0);

        glTranslatef(0, 4.75, 0);                   //����{�l
        //glTranslatef(0, 14.25, 0);
        //glRotatef(180,1,0,0);
        glutSolidSphere(2, 10, 10);                 //�e�{�l ���|4

        node tp = ball_cor(2, 90, 30);
        glPushMatrix();
        glTranslatef(tp.x, tp.y, tp.z);         //����k�ӻH
        right_h.draw();

        glPushMatrix();
        glTranslatef(0, 0.3, 0);
        if (carry_mw && isMagician) magic_wand_carry->draw();
        glPopMatrix();

        glPopMatrix();

        //���ӻH
        tp = ball_cor(2, 270, 330);
        glPushMatrix();
        glTranslatef(tp.x, tp.y, tp.z);       //���쥪�ӻH
        left_h.draw();
        glPopMatrix();

        glPushMatrix();                       //���b�k���W
        glTranslatef(0, -2, 0);
        if (isOnWand) magic_wand_sit->draw();
        glPopMatrix();

        //�k�j�L�W�������`  �Z�b�����
        glPushMatrix();
        glTranslatef(-0.4, -1.75, 0);
        left_f.draw();
        glPopMatrix();

        //���j�L�W�������`
        glPushMatrix();
        glTranslatef(0.4, -1.75, 0);
        right_f.draw();
        glPopMatrix();

        //�Y
        glColor3f(219 / 255.0, 1, 1);
        glTranslatef(0, 3, 0);               //�b�����Y  �M���魫�|0.5
        glPushMatrix();
        glutSolidSphere(1.5, 10, 10);        //���|3

        glColor3f(128 / 255.0, 128 / 255.0, 1);  //��
        glPushMatrix();                      //����
        glTranslatef(0.6, 0.5, 1.2);
        glScalef(0.4, 0.8, 0.4);
        glutSolidSphere(0.5, 10, 10);
        glColor3f(1, 1, 1);                  //����
        glTranslatef(0, 0.2, 0.15);
        glutSolidSphere(0.33, 10, 10);
        glPopMatrix();

        glColor3f(1, 122 / 255.0, 189);        //����
        glPushMatrix();
        glTranslatef(-0.6, 0.5, 1.2);
        glScalef(0.4, 0.8, 0.4);
        glutSolidSphere(0.5, 10, 10);
        glColor3f(1, 1, 1);                  //����
        glTranslatef(0, 0.2, 0.15);
        glutSolidSphere(0.33, 10, 10);
        glPopMatrix();

        glColor3f(1, 0, 0);
        glPushMatrix();
        glTranslatef(0, -0.15, 1.5);         //�L��
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex3f(-0.2, 0, 0);
        glVertex3f(0.2, 0, 0);
        glEnd();
        glPopMatrix();
        glPopMatrix();

        glTranslatef(0, 1, 0);               //�U�l���Шt
        if (isMagician) draw_hat();

        glPopMatrix();                       //���}�Y

        glPopMatrix();                       //���}�{�l���Шt
    }
    void stand() {                  //�w�q����(��l��)
        y = 0;
        x = 0;
        y = 0;
        angle_x = 0;
        right_f.hipJointAng_x = 180;
        right_f.kneeAng_x = 0;
        left_f.hipJointAng_x = 180;
        left_f.kneeAng_x = 0;
        left_h.shoulderAng_x = 180;
        right_h.shoulderAng_x = 180;
        left_h.elbowAng_x = 0;
        right_h.elbowAng_x = 0;
        left_h.shoulderAng_z = -35;
        right_h.shoulderAng_z = 35;
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
        left_h.elbowAng_x = -20;
        right_h.elbowAng_x = -20;
        if (moveMode == RUN) {
            angle_x = 5;
            hipJointFrontOffset = -20;
            kneeFrontOffset = 14;
            hipJointBackOffset = 8;
            kneeBackOffset = 25;
            shoulderOffset = -20;
            left_h.elbowAng_x = -40;
            right_h.elbowAng_x = -40;
        }
        if (moveMode == TURN) {
            hipJointXLimit = 160;
        }
        if (flag == 0) {
            if (flag2 == 0) {  //�k�}���e�A���}����             
                right_f.hipJointAng_x += hipJointFrontOffset / 2.0;         //�k�}���e��
                right_f.kneeAng_x += kneeFrontOffset / 2.0;
                left_f.hipJointAng_x += hipJointBackOffset / 2.0;            //���}����
                left_f.kneeAng_x += kneeBackOffset / 2.0;
                left_h.shoulderAng_x += shoulderOffset / 2.0;                //���⩹�e
                right_h.shoulderAng_x -= shoulderOffset / 2.0;              //�k�⩹��
                if (right_f.hipJointAng_x <= hipJointXLimit) {             //��ɱ���
                    flag2 = 1;
                }
            }
            else { //�k�}�������I�A���}���e����I
                right_f.hipJointAng_x -= hipJointFrontOffset / 2.0;         //�k�}���e��
                right_f.kneeAng_x -= kneeFrontOffset / 2.0;
                left_f.hipJointAng_x -= hipJointBackOffset / 2.0;           //���}����
                left_f.kneeAng_x -= kneeBackOffset / 2.0;
                left_h.shoulderAng_x -= shoulderOffset / 2.0;               //���⩹�e
                right_h.shoulderAng_x += shoulderOffset / 2.0;              //�k�⩹��      
                if (right_f.hipJointAng_x >= 180) {      //�}�b�̫e��
                    flag2 = 0;
                    flag = 1;
                }
            }
        }
        else {   //���}���e�A�k�}���� 
            if (flag2 == 0) {                        //�}���e��
                left_f.hipJointAng_x += hipJointFrontOffset / 2.0;
                left_f.kneeAng_x += kneeFrontOffset / 2.0;
                right_f.hipJointAng_x += hipJointBackOffset / 2.0;
                right_f.kneeAng_x += kneeBackOffset / 2.0;
                left_h.shoulderAng_x -= shoulderOffset / 2.0;
                right_h.shoulderAng_x += shoulderOffset / 2.0;
                if (left_f.hipJointAng_x <= hipJointXLimit) {      //�}�b�̫e��
                    flag2 = 1;
                }
            }
            else { //���}�������I�A�k�}���e����I
                left_f.hipJointAng_x -= hipJointFrontOffset / 2.0;
                left_f.kneeAng_x -= kneeFrontOffset / 2.0;
                right_f.hipJointAng_x -= hipJointBackOffset / 2.0;
                right_f.kneeAng_x -= kneeBackOffset / 2.0;
                left_h.shoulderAng_x += shoulderOffset / 2.0;
                right_h.shoulderAng_x -= shoulderOffset / 2.0;
                if (left_f.hipJointAng_x >= 180) {      //�}�b�̫e��
                    flag2 = 0;
                    flag = 0;
                }
            }
        }
    }
    void jump_ready() {
        left_f.kneeAng_x = 45;
        right_f.kneeAng_x = 45;

        left_f.hipJointAng_x = 150;           //110
        right_f.hipJointAng_x = 150;

        left_h.shoulderAng_x = 200;
        right_h.shoulderAng_x = 200; //180 160
        isJump_ready = 1;
    }
    int jump_cmd = 0;
    bool jump() {
        left_h.elbowAng_x = -15;
        right_h.elbowAng_x = -15;
        switch (jump_cmd) {
        case 0:              //���W��  ���`�ܥ��` �⩹�e
            y += 0.5;
            left_f.kneeAng_x -= 45 / 4.0;
            right_f.kneeAng_x -= 45 / 4.0;
            left_f.hipJointAng_x += 30 / 4.0;
            right_f.hipJointAng_x += 30 / 4.0;
            left_h.shoulderAng_x -= 40 / 4.0;
            right_h.shoulderAng_x -= 40 / 4.0;   //160
            if (y == 2) jump_cmd++;
            break;
        case 1:              //��a�O �s��
            y -= 1;
            left_f.kneeAng_x += 60 / 2.0;
            right_f.kneeAng_x += 60 / 2.0;
            left_f.hipJointAng_x -= 40 / 2.0;
            right_f.hipJointAng_x -= 40 / 2.0;
            left_h.shoulderAng_x += 40 / 2.0;
            right_h.shoulderAng_x += 40 / 2.0;  //200
            if (y == 0) jump_cmd++;
            break;
        case 2:
            y += 0.8;
            left_f.kneeAng_x -= 60 / 5.0;
            right_f.kneeAng_x -= 60 / 5.0;
            left_f.hipJointAng_x += 40 / 5.0;
            right_f.hipJointAng_x += 40 / 5.0;
            left_h.shoulderAng_x -= 40 / 5.0;
            right_h.shoulderAng_x -= 40 / 5.0;
            if (y == 4) jump_cmd++;
            break;
        case 3:
            y -= 1;
            left_f.kneeAng_x += 70 / 4.0;
            right_f.kneeAng_x += 70 / 4.0;
            left_f.hipJointAng_x -= 50 / 4.0;
            right_f.hipJointAng_x -= 50 / 4.0;
            left_h.shoulderAng_x += 40 / 4.0;
            right_h.shoulderAng_x += 40 / 4.0;
            if (y == 0) jump_cmd++;
            break;
        case 4:
        case 5:
            left_f.kneeAng_x -= 70 / 2.0;
            right_f.kneeAng_x -= 70 / 2.0;
            left_f.hipJointAng_x += 50 / 2.0;
            right_f.hipJointAng_x += 50 / 2.0;
            left_h.shoulderAng_x -= 20 / 2.0;
            right_h.shoulderAng_x -= 20 / 2.0;
            jump_cmd++;
            break;
        }
        if (jump_cmd == 6) {
            jump_cmd = 0;
            return 1;
        }
        return 0;
    }
    bool isOnWand = 0;
    bool jumpOnWand() {
        carry_mw = 0;
        isOnWand = 1;
        left_h.elbowAng_x = -15;
        right_h.elbowAng_x = -15;
        switch (jump_cmd) {
        case 0:              //���W��  ���`�ܥ��` �⩹�e
            y += 0.5;
            left_f.kneeAng_x -= 45 / 4.0;
            right_f.kneeAng_x -= 45 / 4.0;
            left_f.hipJointAng_x += 30 / 4.0;
            right_f.hipJointAng_x += 30 / 4.0;
            left_h.shoulderAng_x -= 40 / 4.0;
            right_h.shoulderAng_x -= 40 / 4.0;   //160
            if (y == 2) jump_cmd++;
            break;
        case 1:
            left_f.kneeAng_x = 80;
            right_f.kneeAng_x = 80;
            left_f.hipJointAng_x = 100;
            right_f.hipJointAng_x = 100;
            left_h.shoulderAng_x = 180;
            right_h.shoulderAng_x = 180;
            left_h.shoulderAng_z = -15;
            right_h.shoulderAng_z = 15;
            jump_cmd++;
            break;
        }
        if (jump_cmd == 2) {
            jump_cmd = 0;
            cout << y << " " << jump_cmd << " " << left_f.kneeAng_x << " " << left_f.hipJointAng_x << " " << left_h.shoulderAng_x << "\n";
            return 1;
        }
        return 0;
    }
    bool jumpToFloor() {
        //80 80 100 100 180 180
        left_h.elbowAng_x = -15;
        right_h.elbowAng_x = -15;
        switch (jump_cmd) {
        case 0:              //��a�O �s��
            y -= 1;
            left_f.kneeAng_x += 60 / 2.0;
            right_f.kneeAng_x += 60 / 2.0;
            left_f.hipJointAng_x -= 40 / 2.0;
            right_f.hipJointAng_x -= 40 / 2.0;
            left_h.shoulderAng_x += 40 / 2.0;
            right_h.shoulderAng_x += 40 / 2.0;
            if (y == 0) jump_cmd++;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            left_f.kneeAng_x -= 140 / 4.0;
            right_f.kneeAng_x -= 140 / 4.0;
            left_f.hipJointAng_x += 120 / 4.0;
            right_f.hipJointAng_x += 120 / 4.0;
            left_h.shoulderAng_x -= 40 / 4.0;
            right_h.shoulderAng_x -= 40 / 4.0;
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
}myRobot;
struct big_chair {
    float angle_x = 0.0;
    void draw() {            
        glPushMatrix();              //���
        glTranslatef(0, 0, -20);
        draw_cylinder(0.8,0.8,40);
        glPopMatrix();

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
        glPopMatrix();              //pop�[�l

        glColor3f(1, 1, 0);
        glPushMatrix();             //�u�u
        glTranslatef(0, 0, -13);
        glRotatef(90, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        draw_cylinder(0.3, 0.3, 20);
        glPopMatrix();              

        glColor3f(1, 1, 0);
        glPushMatrix();             //�u�u
        glTranslatef(0, 0, 13);
        glRotatef(90, 0, 1, 0);
        glRotatef(90, 1, 0, 0);     //�̰ʧ��
        draw_cylinder(0.3, 0.3, 20);

        glColor3f(1, 0, 1);
        glPushMatrix();
        glTranslatef(0, 0, 20);
        glScalef(26, 10, 1);
        draw_cube();
        glPopMatrix();

        glPopMatrix();

    }
}myBig_chair;
void change_color(int value) {  //�]�w�e���C��
    switch (value) {
    case ICE:
        glColor3f(235 / 255.0, 1.0, 1.0);
        break;
    case FLOOR:
        glColor3f(50 / 255.0, 50 / 255.0, 50 / 255.0);
        break;
    case WOOD:
        glColor3f(158 / 255.0, 79 / 255.0, 0);
    }
}
void init() {
    //��ê���y�Ц���
    for (pp p : river) {
        for (int i = p.a1; i < p.a2; i++) {
            limit.push_back({ p.x + p.r * cos(i * 0.01745) ,p.z + p.r * sin(i * 0.01745) });
        }
    }
}
void myinit()
{
    init();
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
void draw_circle(double size, int wid) {    // Procedure to draw a circle
    glLineWidth(wid);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 1440; ++i)
        glVertex3f(size * cos(i * 0.01745), 0, size * sin(i * 0.01745));
    glEnd();
}
void draw_square(int hei, int wid) {     //���b�a�W������� �w�q: x�b�V���e�Az�V����
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
    // (110,100
    //  180,100
    //  110,150
    //  180,150) 
    float offsetx = 35;
    float offsetz = 30;
    glColor3f(105 / 255.0, 66 / 255.0, 27 / 255.0);
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
    glScalef(offsetx*2, 1, 1);
    draw_cube();
    glPopMatrix();

    glPushMatrix();                               //��γ�
    glTranslatef(-offsetx, 40, offsetz);
    glScalef(offsetx * 2, 1, 1);
    draw_cube();
    glPopMatrix();

    for (int i = -offsetx; i <= offsetx; i+=5) {            //���γ�
        glPushMatrix();
        glTranslatef(i, 40, -offsetz);
        glScalef(2, 1, 2*offsetz);
        draw_cube();
        glPopMatrix();
    }
}
void draw_floor() {           //�e����M�a�O
    change_color(ICE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glPushMatrix();                   //�O�s0,0
    glRotatef(270, 1, 0, 0);          //��x�b�f�ɰw��270��
    draw_square(60, 60);
    glPopMatrix();                    //�^��0,0

    glPushMatrix();                   //�O�s0,0
    glRotatef(90, 0, 0, 1);           //��z�b�f�ɰw��270��
    draw_square(60, 60);
    glPopMatrix();                    //�^��0,0

    glPushMatrix();
    draw_square(60, 60);              //�e�a�O
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
    //glColor3f(1, 0, 0);

    draw_circle(17, 1);       //6

    draw_circle(14, 4);
    for (float t = 0; t < 12; t += 0.5) {     //��G�}
        glLineWidth(4);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 1440; ++i)
            glVertex3f((14 - t * 0.2) * cos(i * 0.01745), 0, (t * 0.23) + (14 - t * 0.2) * sin(i * 0.01745));
        glEnd();
    }
    //change_color(ICE);
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
    draw_circle(7.7, 2);          //0.5

}
void draw_scene(int mode) {
    //draw_floor();
    if (mode == MAGICFIELD) {
        if (!myRobot.isMagician) change_color(ICE);
        else glColor3f(188 / 255.0, 217 / 255.0, 246 / 255.0);
        glPushMatrix();
        glTranslatef(30, 0, 30);       //�k�}�� lcs
        draw_magic_field();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(30, 7, 30);      //�k�M�� lcs  �Ʀb�Ť�
        myMagic_wand.setPos(30, 7, 30);
        if (myMagic_wand.show) draw_magic_wand();
        glPopMatrix();
    }
    else if (mode == GRASSLAND) {
        glColor3f(1 / 255.0, 152/255.0, 89 / 255.0); //��a
        glPushMatrix();
        glTranslatef(0,-10.5,0);
        glScalef(200,10,200);
        draw_cube();
        glPopMatrix();

        glColor3f(204 / 255.0, 1, 204 / 255.0); //��a
        draw_square(200, 200);

        //glColor3f(0, 0, 0);
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
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3f(188 / 255.0, 217 / 255.0, 246 / 255.0);
        glPushMatrix();
        glTranslatef(30, 0.5, 160);
        draw_circle(30, 1);
        glPopMatrix();
        for (pp p : river) {
            for (int i = p.a1; i < p.a2; i++) {
                glPushMatrix();
                glTranslatef(p.x + p.r * cos(i * 0.01745), 0.5, p.z + p.r * sin(i * 0.01745));
                draw_circle(5, 1);
                glPopMatrix();
            }
        }
        //road
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(133 / 255.0, 66 / 255.0, 0 / 255.0);
        for (int i = 115; i < 180; i+=6) {
            glBegin(GL_POLYGON);
            glVertex3f(150 + 150 * cos(i * 0.01745),      0.4, 0 + 100 * sin(i * 0.01745));
            glVertex3f(150 + 150 * cos((i+3) * 0.01745), 0.4, 0 + 100 * sin((i+3) * 0.01745));
            glVertex3f(150 + 120 * cos((i+3) * 0.01745), 0.4, 0 + 80 * sin((i+3) * 0.01745));
            glVertex3f(150 + 120 * cos(i * 0.01745), 0.4, 0 + 80 * sin(i * 0.01745));
            glEnd();
        }
        for (int i = -46; i < 30; i += 6) {
            glBegin(GL_POLYGON);
            glVertex3f( 4 + 150 * cos(i * 0.01745), 0.4, 150 + 100 * sin(i * 0.01745));
            glVertex3f( 4 + 150 * cos((i + 3) * 0.01745), 0.4, 150 + 100 * sin((i + 3) * 0.01745));
            glVertex3f( 4 + 120 * cos((i + 3) * 0.01745), 0.4, 150 + 80 * sin((i + 3) * 0.01745));
            glVertex3f( 4 + 120 * cos(i * 0.01745), 0.4, 150 + 80 * sin(i * 0.01745));
            glEnd();
        }
        //home 
        //todo �P�_home���i���� �W�l���i�I��   
        glPushMatrix();
        glTranslatef(145,0,130);               //70*50
        draw_home();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(50, 25, 50);    
        glRotatef(angley,0,1,0);
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
    myRobot.draw();
}
void draw_pupu() {
    jakao.draw();
}
void display()
{
    //printf("display\n");
    /*Clear previous frame and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*----Define the current eye position and the eye-coordinate system---*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //�ʧ@
    //�۾���m    �۾���Ǫ���m   �۾��V�W������

    //if(see)
    //    gluLookAt(35.0, 30.0, 80.0,       20.0, 0.0, 0.0,         0.0, 1.0, 0.0);         //�ʧ@
    //else
    //    gluLookAt(40.0, 70.0, 55.0,       25.0, 0.0, 25.0,        0.0, 1.0, 0.0);         //����
    if (see)
        gluLookAt(pos[0]-5, 30, pos[2] + 30, pos[0], 15, pos[2], 0.0, 1.0, 0.0);
    else
        gluLookAt(100, 150, 100, 100, 0, 100, 0.0, 0.0, -1.0);
    /*-------Draw the floor------*/
    scene = GRASSLAND;
    draw_scene(scene);
    //draw_scene(scene);

    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    draw_robot();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10, 0, 10);
    draw_pupu();
    glPopMatrix();



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

    if(see)
        glOrtho(-40.0, 40.0, -40.0, 40.0, -100.0, 200);
    else 
        glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 200);
    width = w; height = h;
}
void timerFunc(int nTimerID) {
    switch (nTimerID) {
    case RUNTIMER:
        preKey = -1;
        break;
        //cout << "time out\n";
    case JUMPTIMER:
        if (!myRobot.jump()) {
            isLock = LOCK;
            glutTimerFunc(100, timerFunc, JUMPTIMER);
        }
        else {
            myRobot.stand();
            isLock = UNLOCK;
        }
        glutPostRedisplay();
        break;
    case JUMPONWANDTIMER:
        if (!myRobot.jumpOnWand()) {
            isLock = LOCK;
            glutTimerFunc(100, timerFunc, JUMPONWANDTIMER);
        }
        else {
            isLock = UNLOCK;
        }
        glutPostRedisplay();
        break;
    case JUMPTOFLOORTIMER:
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
    }
}
void my_move_order(unsigned char key) {        //�򲾰ʬ������P�_
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
        angley++;
        myRobot.angle_y += 5;
        if (!myRobot.isOnWand) {
            myRobot.moveMode = TURN;
            myRobot.move();    //�b�a�O�~�n�ʸ}
        }
    }

    //�P�_�I���ê��
    for (int i = -7; myMagic_wand.show && i < 7; i++) {
        if (getDis(tpPos[0], tpPos[2], myMagic_wand.x, myMagic_wand.z + i) < 3.5) return;  //magic wand
    }
    if (myRobot.moveMode != FLY && scene == GRASSLAND) {
        for (auto i : limit) {
            if (getDis(tpPos[0], tpPos[2], i.first, i.second) < 5) return;    //river
        }
        if (getDis(tpPos[0], tpPos[2], 30, 160) < 30) return;                 //pool
    }
    for (auto i : pillar) {
        if (getDis(tpPos[0], tpPos[2], i.first, i.second) < 1.25 + 2) return;     //pillar �W�l    
    }

    //�P�_���
    if (scene == MAGICFIELD) {
        if (tpPos[0] < 0 || tpPos[0] > 60 || tpPos[2] < 0 || tpPos[2] > 60) return;
    }
    else if (scene == GRASSLAND) {
        if (tpPos[0] < 0 || tpPos[0] > 200 || tpPos[2] < 0 || tpPos[2] > 200) return;
    }
    for (int i = 0; i < 3; i++) pos[i] = tpPos[i];
    display();
}
void special_func(int key, int x, int y) {
    cout << key << "\n";
}
void keyboardUp_func(unsigned char key, int x, int y) {
    if (isLock == LOCK) return;
    glutTimerFunc(200, timerFunc, RUNTIMER);
    if (myRobot.moveMode != FLY && preKey != key)  myRobot.moveMode = WALK;
    preKey = key;
    if (!myRobot.isOnWand && (key == 'W' || key == 'w' || key == 'A' || key == 'a' || key == 'S' || key == 's' || key == 'D' || key == 'd' || key == 'r' || key == 'R')) myRobot.stand();
    display();
}
void keybaord_fun(unsigned char key, int x, int y) {
    printf("key: %d\n", key);
    if (isLock == LOCK) return;
    my_move_order(key);
    if (key == 'o' || key == 'O') {
        cout << ++myRobot.magic_wand_carry->angle_x << "\n";
    }
    if (myMagic_wand.show && (key == 'p' || key == 'P')) {
        for (int i = -7; i < 7; i++) {
            if (getDis(pos[0], pos[2], myMagic_wand.x, myMagic_wand.z + i) < 3.5 + 5) {
                myMagic_wand.show = 0;
                myRobot.carry_mw = 1;
                myRobot.isMagician = 1;
            }
        }
    }
    if (key == 'Y' || key == 'y') {
        see = ~see;
    }
    if (key == 'Q' || key == 'q') {
        myRobot.right_h.shoulderAng_x++;
        myRobot.left_h.shoulderAng_x++;
        if (myRobot.right_h.shoulderAng_x > 360) myRobot.right_h.shoulderAng_x -= 360;
        if (myRobot.left_h.shoulderAng_x > 360) myRobot.right_h.shoulderAng_x -= 360;
        cout << myRobot.right_h.shoulderAng_x << "\n";
    }
    if (key == '1') {
        myRobot.right_f.kneeAng_x++;
        myRobot.left_f.kneeAng_x++;
        if (myRobot.right_f.kneeAng_x > 360) myRobot.right_f.kneeAng_x -= 360;
        cout << myRobot.right_f.kneeAng_x << "\n";
    }
    else if (key == '2') {
        myRobot.right_f.hipJointAng_x++;
        myRobot.left_f.hipJointAng_x++;
        if (myRobot.right_f.hipJointAng_x > 360)  myRobot.right_f.hipJointAng_x -= 360;
        cout << myRobot.right_f.hipJointAng_x << "\n";
    }
    else if (key == '3') {
        myRobot.right_f.ankle_x++;
        if (myRobot.right_f.ankle_x > 360)  myRobot.right_f.ankle_x -= 360;
        cout << myRobot.right_f.ankle_x << "\n";
    }
    else if (key == '4') {
        myRobot.angle_x += 5;
        if (myRobot.angle_x > 360)  myRobot.angle_x -= 360;
        cout << myRobot.angle_x << "\n";
    }
    else if (key == ' ') {
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
    else if ((int)key == 13) {          //enter �i�J��a
        if (myRobot.isMagician && getDis(pos[0], pos[2], 30, 30) < 30) {
            pos[0] = pos[2] = 0;
            scene = GRASSLAND;
        }
    }
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
    glutCreateWindow("Robot");

    myinit();      /*---Initialize other state varibales----*/

    /*----Associate callback func's whith events------*/
    glutDisplayFunc(display);

    glutReshapeFunc(my_reshape);
    glutKeyboardFunc(keybaord_fun);

    glutIdleFunc(idle_func);
    glutMouseFunc(mouse_func);  /* Mouse Button Callback func */
    glutMotionFunc(motion_func);/* Mouse motion event callback func */
    glutPassiveMotionFunc(passive_motion_func);
    glutTimerFunc(1000, timerFunc, 1);
    glutSpecialFunc(special_func);
    glutKeyboardUpFunc(keyboardUp_func);
    glutMainLoop();
}
