/***************************************************
 * Example program: Draw a moving cube
 *   Author: S. K. Ueng
 *   National Taiwan Ocean Univ. Comp. Sci. Dept.
 */
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glut.h>


#define   PI   3.1415927
 //Define polygon drwaing models, filled or outlined.
#define   FILL   1
#define   LINE  2   

#define ICE 100
#define FLOOR 101
#define WOOD 102

#define WALK 0
#define RUN 1

#define RUNTIMER 50
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

int see = 0;
int preKey = 0;
int moveMode = 0;
void draw_magic_field();
void draw_cylinder(double up, double down, int height);
void change_color(int value);
void draw_circle(double size, int wid);
void draw_square(int hei, int wid);
struct node {
    double x = 0, y = 0, z = 0;
};
node ball_cor(double r, int A, int B) {
    node rt;
    rt.x = r * sin(A * 0.01745) * cos(B * 0.01745);
    rt.y = r * sin(A * 0.01745) * sin(B * 0.01745);
    rt.z = r * cos(A * 0.01745);
    return rt;
}
struct magic_wand {
    //float x1 = 0, y1 = 0, x2 = 0;                    //自己的座標
    float angle_x = 0, angle_y = 0, angle_z = 0;
    bool show = 1;
    magic_wand(float x = 0,float y = 0,float z = 0){
        angle_x = x;
        angle_y = y;
        angle_z = z;
    }
   /* void setPos(int x_, int y_, int z_) {
        x = x_;
        y = y_;
        z = z_;
    }*/
    void draw() {
        //x右 y上 z前 中心點:法杖的中間
        change_color(WOOD);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glRotatef(angle_x, 1, 0, 0);
        glRotatef(angle_y, 0, 1, 0);
        glRotatef(angle_z, 0, 0, 1);
        glPushMatrix();
        glTranslatef(0, 0, -4);
        draw_cylinder(0.5, 0.8, 8);
        glPopMatrix();

        glColor3f(102 / 255.0, 34 / 255.0, 0);  //法杖底部的圓
        glPushMatrix();
        glTranslatef(0, 0, -4);
        glRotatef(270, 1, 0, 0);            //圓形站立
        draw_circle(0.5, 1);
        glPopMatrix();

        glColor3f(102 / 255.0, 34 / 255.0, 0);   //法杖頂部的圓
        glPushMatrix();
        glTranslatef(0, 0, 4);
        glRotatef(270, 1, 0, 0);                 //圓形站立
        draw_circle(0.8, 1);
        glPopMatrix();

        glColor3f(168 / 255.0, 1.0, 1);         //閃光圈(藍色)
        glPushMatrix();
        glTranslatef(0, 0, 5);
        glRotatef(30, 0, 1, 0);
        glutSolidTorus(0.1, 2.5, 100, 100);
        glPopMatrix();

        glColor3f(168 / 255.0, 1.0, 1);          //閃光圈(藍色)
        glPushMatrix();
        glTranslatef(0, 0, 5);
        glRotatef(330, 0, 1, 0);
        glutSolidTorus(0.1, 2.5, 100, 100);
        glPopMatrix();

        glColor3f(1, 1, 168 / 255.0);              //水晶(黃)
        glPushMatrix();
        glTranslatef(0, 0, 5.8);
        glutSolidSphere(1.5, 10, 10);
        glPopMatrix();
    }
}myMagic_wand;
struct robot {
    magic_wand* magic_wand_r = new magic_wand(180,0,0);
    struct hand {
        node tp;
        bool carry_mw = 0;
        float shoulderAng_x = 180, shoulderAng_y = 0, shoulderAng_z = -35;
        float elbowAng_x = 0, elbowAng_y = 0, elbowAng_z = 0;
        float fingerAng_x = 0, fingerAng_y = 0, fingerAng_z = 0;
        void draw() {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glRotatef(shoulderAng_x, 1, 0, 0);           //移動關節角度 180放下
            glRotatef(shoulderAng_z, 0, 0, 1);           //-35放在身體旁邊 對z轉控制左右
            glColor3f(173 / 255.0, 214 / 255.0, 1);
            glutSolidSphere(0.5, 10, 10);                //半徑為 0.5 的肩膀

            //forarms
            glColor3f(167/255.0,167/255.0,167/255.0);
            glTranslatef(0, 0.75, 0);              //走到 肩膀上方0.25 + 圓中心0.75(畫1.5的手臂) - 0.25重疊

            glPushMatrix();
            glScalef(0.7, 1.5, 0.7);
            glutSolidSphere(0.5, 10, 10);          //直徑為1.5的手臂
            glPopMatrix();

            glTranslatef(0, 0.75, 0);               //手臂前端中心
            glRotatef(elbowAng_x, 1, 0, 0);
            glRotatef(elbowAng_y, 0, 1, 0);
            glRotatef(elbowAng_z, 0, 0, 1);
            glColor3f(173 / 255.0, 214 / 255.0, 1);

            glutSolidSphere(0.25, 10, 10);          //直徑為 0.5 的手肘      0.25重疊

            //手前臂
            glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);
            glTranslatef(0, 0.75, 0);                //走到 手軸前端0.25 + 圓中心0.75(畫1.5的手前臂) - 0.25重疊

            glPushMatrix();
            glScalef(0.55, 1.5, 0.55);
            glutSolidSphere(0.5, 10, 10);           //直徑為1.5的手前臂
            glPopMatrix();

            glTranslatef(0, 0.75, 0);               //手前臂前端   
            //換手指方向應該在這轉
            //左手指頭
            glColor3f(173 / 255.0, 214 / 255.0, 1);
            glPushMatrix();
            glTranslatef(-0.25, 0.2, 0);
            glRotatef(15, 0, 0, 1);               //張開角度
            glScalef(0.3, 0.7, 0.3);              //手指長: 0.7
            glutSolidSphere(0.5, 10, 10);
            glPopMatrix();

            //右手指頭
            glPushMatrix();
            glTranslatef(0.25, 0.2, 0);
            glRotatef(-15, 0, 0, 1);              //張開角度
            glScalef(0.3, 0.7, 0.3);              //手指長: 0.7
            glutSolidSphere(0.5, 10, 10);
            glPopMatrix();
            glRotatef(-shoulderAng_z, 0, 0, 1);   //變回正常的座標系統
        }
    }right_h, left_h;
    struct foot {          //腿長2.75
        node tp;
        float hipJointAng_x = 180, hipJointAng_y = 0, hipJointAng_z = 0;   //髖關節
        float kneeAng_x = 0, kneeAng_y = 0, kneeAng_z = 0;
        void draw() {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glRotatef(hipJointAng_x, 1, 0, 0);          //移動關節角度 180放下
            //glRotatef(-30, 0, 0, 1);            
            //glRotatef(90, 1, 0, 0);             //往前  對x軸轉控制舉起放下
            glColor3f(173 / 255.0, 214 / 255.0, 1);
            glutSolidSphere(0.25, 10, 10);        //大腿上的關節 0.5直徑

            //大腿
            glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);
            glTranslatef(0, 0.625, 0);           //走到 膝蓋0.125 + 圓中心0.5(畫1的圓)

            glPushMatrix();
            glScalef(0.7, 1, 0.7);               //大腿長1
            glutSolidSphere(0.5, 10, 10); 
            glPopMatrix();

            glTranslatef(0, 0.5, 0);             //腿前端中心  和膝蓋重疊0.25

            glRotatef(kneeAng_x, 1, 0, 0);       //膝蓋角度
            //膝蓋
            glColor3f(173 / 255.0, 214 / 255.0, 1);
            glPushMatrix();
            glutSolidSphere(0.25, 10, 10);       //直徑0.5的膝蓋
            glPopMatrix();

            glTranslatef(0, 0.5, 0);             //膝蓋前端 0.25 + 腿中間0.5(腿長1) - 0.25(重疊地方)

            glColor3f(167 / 255.0, 167 / 255.0, 167 / 255.0);
            glPushMatrix();
            glScalef(0.5, 1, 0.5);               //小腿長1
            glutSolidSphere(0.5, 10, 10);
            glPopMatrix();

            glTranslatef(0, 0.5, 0);             //小腿前端中心 0.5 和腿重疊0.25

            glColor3f(0.5, 0.5, 0.5);
            glutSolidSphere(0.5, 10, 10);        //腳 直徑1
        }
    }left_f, right_f;
    void draw() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //肚子
        glColor3f(219/255.0,1,1);
        glPushMatrix();
        if (moveMode == RUN) glRotatef(5, 1, 0, 0);
        //glTranslatef(0, 4.75, 0);              //走到肚子

        glTranslatef(0, 14.25, 0);
        glScalef(3, 3, 3);
        //glRotatef(180,1,0,0);
        glutSolidSphere(2, 10, 10);             //畫肚子 直徑4

        //右肩膀
        node tp = ball_cor(2, 90, 30);
        glPushMatrix();
        glTranslatef(tp.x, tp.y, tp.z);        //走到右肩膀
        right_h.carry_mw = 1;
        right_h.draw();

        glPushMatrix();
        glScalef(1 / 3.0, 1 / 3.0, 1 / 3.0);
        magic_wand_r -> draw();
        glPopMatrix();

        glPopMatrix();

        //左肩膀
        tp = ball_cor(2, 270, 330);
        glPushMatrix();
        glTranslatef(tp.x, tp.y, tp.z);       //走到左肩膀
        left_h.shoulderAng_z = 30;
        left_h.draw();
        glPopMatrix();

        //右大腿上面的關節  蠻在身體裡
        glPushMatrix();
        glTranslatef(-0.4,-1.75,0);     
        left_f.draw();
        glPopMatrix();

        //左大腿上面的關節
        glPushMatrix();
        glTranslatef(0.4, -1.75, 0);     
        right_f.draw();
        glPopMatrix();

        //頭
        glColor3f(219 / 255.0, 1, 1);
        glPushMatrix();
        glTranslatef(0, 3, 0);               //在走到頭  和身體重疊0.5
        glutSolidSphere(1.5, 10, 10);        //直徑3

        glColor3f(128/255.0, 128/255.0, 1);  //藍
        glPushMatrix();                      //眼睛
        glTranslatef(0.6, 0.5, 1.2);
        glScalef(0.4,0.8,0.4);
        glutSolidSphere(0.5, 10, 10);
        glColor3f(1, 1, 1);                  //眼白
        glTranslatef(0, 0.2, 0.15);
        glutSolidSphere(0.33, 10, 10);
        glPopMatrix();

        glColor3f(1, 122/255.0, 189);        //粉眼
        glPushMatrix();
        glTranslatef(-0.6, 0.5, 1.2);
        glScalef(0.4, 0.8, 0.4);
        glutSolidSphere(0.5, 10, 10);
        glColor3f(1, 1, 1);                  //眼白
        glTranslatef(0, 0.2, 0.15);
        glutSolidSphere(0.33, 10, 10);
        glPopMatrix();            
        
        glColor3f(1, 0, 0);
        glPushMatrix();                      
        glTranslatef(0, -0.15, 1.5);         //嘴巴
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex3f(-0.2,0,0);
        glVertex3f(0.2,0,0);
        glEnd();
        glPopMatrix();

        glPopMatrix();
        glPopMatrix();                       //離開肚子坐標系
    }
    bool flag = 0,flag2 = 0;
    void stand() {
        right_f.hipJointAng_x = 180;
        right_f.kneeAng_x = 0;
        left_f.hipJointAng_x = 180;
        left_f.kneeAng_x = 0;
        left_h.shoulderAng_x = 180;
        right_h.shoulderAng_x = 180;
    }
    void walk(int mode) {
        //(膝蓋,髖關節)
        //腳後(0,180) ~ (35,200)     35 +7   20 +4
        //腳往前(0,180) ~ (35,130)   35 +7   50 -10
        //(肩膀)
        //手往前(180) ~ (200)        20 +4   100 +20
        //手往後(180) ~ (170)        20 -4
        int hipJointFrontOffset = -10;
        int kneeFrontOffset = 7;
        int hipJointBackOffset = 4;
        int kneeBackOffset = 7;
        int shoulderOffset = -4;
        left_h.elbowAng_x = -20;
        right_h.elbowAng_x = -20;
        if (mode == RUN) {
            hipJointFrontOffset = -20;
            kneeFrontOffset = 14;
            hipJointBackOffset = 8;
            kneeBackOffset = 25;
            shoulderOffset = -20;
            left_h.elbowAng_x = -40;
            right_h.elbowAng_x = -40;
        }
        if (flag == 0) {  
            if (flag2 == 0) {  //右腳往前，左腳往後             
                right_f.hipJointAng_x += hipJointFrontOffset /2.0;         //右腳往前伸
                right_f.kneeAng_x += kneeFrontOffset/2.0;
                left_f.hipJointAng_x += hipJointBackOffset/2.0;           //左腳往後
                left_f.kneeAng_x += kneeBackOffset /2.0;
                left_h.shoulderAng_x += shoulderOffset/2.0;               //左手往前
                right_h.shoulderAng_x -= shoulderOffset/ 2.0;             //右手往後
                if (right_f.hipJointAng_x <= 130) {      //邊界條件
                    flag2 = 1;                           
                }
            }
            else { //右腳往後到原點，左腳往前到原點
                right_f.hipJointAng_x -= hipJointFrontOffset / 2.0;         //右腳往前伸
                right_f.kneeAng_x -= kneeFrontOffset / 2.0;
                left_f.hipJointAng_x -= hipJointBackOffset / 2.0;           //左腳往後
                left_f.kneeAng_x -= kneeBackOffset / 2.0;
                left_h.shoulderAng_x -= shoulderOffset / 2.0;               //左手往前
                right_h.shoulderAng_x += shoulderOffset / 2.0;             //右手往後      
                if (right_f.hipJointAng_x >= 180) {      //腳在最前面
                    flag2 = 0;
                    flag = 1;
                }
            }
        }
        else {   //左腳往前，右腳往後 
            if (flag2 == 0) {                        //腳往前伸
                left_f.hipJointAng_x += hipJointFrontOffset / 2.0;      
                left_f.kneeAng_x += kneeFrontOffset / 2.0;
                right_f.hipJointAng_x += hipJointBackOffset / 2.0;           
                right_f.kneeAng_x += kneeBackOffset / 2.0;
                left_h.shoulderAng_x -= shoulderOffset / 2.0;              
                right_h.shoulderAng_x += shoulderOffset / 2.0;           
                if (left_f.hipJointAng_x <= 130) {      //腳在最前面
                    flag2 = 1;
                }
            }
            else { //左腳往後到原點，右腳往前到原點
                left_f.hipJointAng_x -= hipJointFrontOffset / 2.0;
                left_f.kneeAng_x -= kneeFrontOffset / 2.0;
                right_f.hipJointAng_x -= hipJointBackOffset / 2.0;
                right_f.kneeAng_x -= kneeBackOffset / 2.0;
                left_h.shoulderAng_x += shoulderOffset / 2.0;
                right_h.shoulderAng_x -= shoulderOffset / 2.0;
                if (left_f.hipJointAng_x >= 180) {      //腳在最前面
                    flag2 = 0;
                    flag = 0;
                }
            }
        }
        
    }
}myRobot;
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
    glPushMatrix();
    glScaled(wid,0,hei);
    glBegin(GL_POLYGON);
    glVertex3f(0, 0.0, 1);           
    glVertex3f(1, 0.0, 1);
    glVertex3f(1, 0.0, 0);
    glVertex3f(0, 0.0, 0);
    glEnd();
    glPopMatrix();
}
void draw_magic_wand(){                 
   myMagic_wand.draw();
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
    //glColor3f(1, 0, 0);
   
    draw_circle(17, 1);       //6

    draw_circle(14, 4);
    for (float t = 0; t < 12; t += 0.5) {     //月亮陣
        glLineWidth(4);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 1440; ++i)
            glVertex3f((14 - t * 0.2) * cos(i * 0.01745), 0, (t * 0.23) + (14 - t * 0.2) * sin(i * 0.01745));
        glEnd();
    }
    //change_color(ICE);
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
    //draw_floor();

    glPushMatrix();
    glTranslatef(30, 0, 30);       //法陣的 lcs
    draw_magic_field();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(30 ,7, 30);      //法仗的 lcs  飄在空中
   // myMagic_wand.setPos(30,7,30);
    if(myMagic_wand.show) draw_magic_wand();
    glPopMatrix();
}
void draw_cylinder(double up,double down,int height){
    if (cylind == NULL) {
        cylind = gluNewQuadric();
    }
    /*--- Draw a cylinder ---*/
    gluCylinder(cylind, up, down, /* radius of top and bottom circle */
        height,              /* height of the cylinder */
        20,               /* use 12-side polygon approximating circle*/
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
void draw_robot() {
    myRobot.draw();
}
void display()
{
    //printf("display\n");
    /*Clear previous frame and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*----Define the current eye position and the eye-coordinate system---*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //動作
    //相機位置    相機對準的位置   相機向上的角度
    
    if(see)
        gluLookAt(35.0, 30.0, 60.0,       20.0, 0.0, 0.0,         0.0, 1.0, 0.0);      //動作
    else
        gluLookAt(40.0, 70.0, 55.0, 25.0, 0.0, 25.0, 0.0, 1.0, 0.0);                    //場景


    /*-------Draw the floor------*/
    
    draw_scene1();

    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glRotatef(angle,0,1,0);
    draw_robot();
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

    glOrtho(-40.0, 40.0, -40.0, 40.0, 0.0, 120);       //場景
    width = w; height = h;
}
void timerFunc(int nTimerID){
    switch (nTimerID){
    case 1:
        //printf("h\n");
        //glutPostRedisplay();
        //glutTimerFunc(1000, timerFunc, 1);
        break;
    case RUNTIMER:
        preKey = -1;
        cout << "time out\n";
    }
}
void my_move_order(unsigned char key) {        //跟移動相關的判斷
    float tpPos[3] = { pos[0], pos[1], pos[2] };
    //cout << key << "\n";

    if (preKey == key) moveMode = RUN;  //0.3秒內連續按

    float offset = 0;
    if (moveMode == RUN) offset = 2;
    else if (moveMode == WALK) offset = 0.5;

    if (key == 'S' || key == 's') {  
        angle = 0;
        tpPos[2] += offset;   
        myRobot.walk(moveMode);
    }
    else if (key == 'W' || key == 'w') {
        angle = 180;
        tpPos[2] -= offset;    
        myRobot.walk(moveMode);
    }
    else if (key == 'A' || key == 'a') {
        angle = 270;
        tpPos[0] -= offset;
        myRobot.walk(moveMode);
    }
    else if (key == 'D' || key == 'd') {
        angle = 90;
        tpPos[0] += offset;
        myRobot.walk(moveMode);
    }
    for (int i = 0; i < 3; i++) pos[i] = tpPos[i];
    display();
}
void special_func(int key, int x, int y) {
    cout << key << "\n";
    
}
void keyboardUp_func(unsigned char key, int x, int y) {
    glutTimerFunc(300, timerFunc, RUNTIMER);
    if(preKey != key) moveMode = WALK;
    preKey = key;
    myRobot.stand();
    display();
}
void keybaord_fun(unsigned char key, int x, int y) {
    //printf("key: %c\n", key);
    my_move_order(key);
    if (key == 'o' || key == 'O') {     
        cout << ++myRobot.magic_wand_r->angle_x << "\n";
    }
    if (key == 'p' || key == 'P') {
        myMagic_wand.show = !myMagic_wand.show;
    }
    if (key == 'Y' || key == 'y') {
        see = ~see;
    }
    if (key == 'Q' || key == 'q') {
        myRobot.right_h.shoulderAng_x++;
        myRobot.left_h.shoulderAng_x++;
        if (myRobot.right_h.shoulderAng_x > 360) myRobot.right_h.shoulderAng_x -= 360;
        if (myRobot.left_h.shoulderAng_x > 360) myRobot.right_h.shoulderAng_x -= 360;
    }
    if (key == '1') {
        myRobot.right_f.kneeAng_x++;
        if (myRobot.right_f.kneeAng_x > 360) myRobot.right_f.kneeAng_x -= 360;
        cout << myRobot.right_f.kneeAng_x << "\n";
    }
    else if (key == '2') {
        myRobot.right_f.hipJointAng_x++;
        if (myRobot.right_f.hipJointAng_x > 360)  myRobot.right_f.hipJointAng_x -= 360;
        cout << myRobot.right_f.hipJointAng_x << "\n";
    }
    else if (key == '3') {
        myRobot.left_f.kneeAng_x++;
        if (myRobot.left_f.kneeAng_x > 360) myRobot.left_f.kneeAng_x -= 360;
        cout << myRobot.left_f.kneeAng_x << "\n";
    }
    else if (key == '4') {
        myRobot.left_f.hipJointAng_x++;
        if (myRobot.left_f.hipJointAng_x > 360)  myRobot.left_f.hipJointAng_x -= 360;
        cout << myRobot.left_f.hipJointAng_x << "\n";
    }
    //if (myMagic_wand.x ) {
    //}

    if (key == 'R' || key == 'r') angle += 3.0;
    else if (key == 'L' || key == 'l') angle -= 3.0;

    if (angle >= 360.0) angle -= 360.0;
    else if (angle < 0.0) angle += 360.0;

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

    glutInitWindowSize(500, 500);
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
