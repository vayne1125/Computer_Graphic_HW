/***************************************************
 * Example program: Draw a trunk.moving cube
 *   Author: S. K. Ueng
 *   National Taiwan Ocean Univ. Comp. Sci. Dept.
 */
#include<vector>
#include<iostream>
#include <GL/glut.h>
using namespace std;

#define RIGHT		 0
#define LEFT		 1


#define BWALK		-4
#define RWALK		-5
#define LWALK		-4
#define TUN			-3
#define LIFT		-2
#define LEAP		-1
#define RIS			 0
#define WALK		 1
#define RWLK		 2
#define LWLK		 3
#define SWLK		 4
#define RUN			 5
#define ROT			 6
#define TUR			 7
#define JMP			 8
#define TURN		 9
#define LEAP_JUMP	10
#define LIFT_JUMP	11
#define TUN_JUMP    12
#define LWALK_JUMP  13
#define RWALK_JUMP  14
#define BWALK_JUMP  15


//Define polygon drwaing models, filled or outlined.
#define GLOOMY		 0   
#define NORMAL		 1

//coordinates of the 8 corners.(0,0,0)-(1,1,1)
float  points[][3] = {{0.0, 0.0, 0.0}, {10.0, 0.0, 0.0}, 
					  {10.0, 0.0, 10.0}, {0.0, 0.0, 10.0},
                      {0.0, 10.0, 0.0}, {10.0, 10.0, 0.0},
					  {10.0, 10.0, 10.0}, {0.0, 10.0, 10.0}};
//Define 6 faces using the 8 corners (vertices)
int    face[][4] = {{0, 1, 2, 3}, {7, 6, 5, 4}, {0, 4, 5, 1}, 
                    {1, 5, 6, 2}, {3, 2, 6, 7}, {0, 3, 7, 4}};
//Define the cube using the 6 faces.
int    cube[6] = {0, 1, 2, 3, 4, 5};
//Define colors
float  color[][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 0.0},
{1.0, 0.0, 1.0},{0.0, 1.0, 1.0}};

 /* window shape */
int    width = 500, height = 500;   
//define a base position in the z-x plane
float  pos[3] = {0.0, 0.0, 0.0};
//declare the rotational angle.
float  angle = 0.0;

//Quodri objects for drawing the world coordinate system.
/*-----Define GLU quadric objects, a sphere and a cylinder----*/
GLUquadricObj  *sphere=NULL, *cylind=NULL;
int    polygonMode=GLOOMY;


struct Handlebar {
	struct Pos{
		double dx = 0, dy = 0, dz = 5;
	}	pos;
	struct Rotate {
		double ang = 0, rx, ry = 0, rz = 0;
	}	rot;
	void draw() {
		//定位點
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(206 / 255.0, 209 / 225.0, 219 / 255.0);
		glPushMatrix();
		glTranslatef(pos.dx, pos.dz, pos.dy);//右,前,上
		glRotatef(rot.ang, rot.rx, rot.ry, rot.rz);
		if (sphere == NULL)
			sphere = gluNewQuadric();
		gluSphere(sphere,	1.0,   /* radius=0.5 */
							12,            /* composing of 12 slices*/
							12);           /* composing of 12 stacks */
		
		//底桿
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(206 / 255.0, 209 / 225.0, 219 / 255.0);
		glPushMatrix();
		glTranslatef(0, 0, -15);//右,上,前
		if (cylind == NULL) cylind = gluNewQuadric();
		gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
							30,      /* height of the cylinder */
							12,       /* use 12-side polygon approximating circle*/
							4);       /* Divide it into 3 sections */
		
		//下支架(前)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(206 / 255.0, 209 / 225.0, 219 / 255.0);
		glPushMatrix();
		glTranslatef(5, 0, 30);//右,上,前
		glRotatef(-90, 0, 1, 0);//前,上,左
		if (cylind == NULL) cylind = gluNewQuadric();
		gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
							10,      /* height of the cylinder */
							12,       /* use 12-side polygon approximating circle*/
							4);       /* Divide it into 3 sections */
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(206 / 255.0, 209 / 225.0, 219 / 255.0);
		glTranslatef(0, 1, 0);//前,上,左
		glRotatef(90, 1, 0, 0);//前,左,下
		if (cylind == NULL) cylind = gluNewQuadric();
		gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
							6,      /* height of the cylinder */
							12,       /* use 12-side polygon approximating circle*/
							4);       /* Divide it into 3 sections */
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(206 / 255.0, 209 / 225.0, 219 / 255.0);
		glTranslatef(0, 10, 0);//前,左,下
		if (cylind == NULL) cylind = gluNewQuadric();
		gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
							6,      /* height of the cylinder */
							12,       /* use 12-side polygon approximating circle*/
							4);       /* Divide it into 3 sections */
		glPopMatrix();

		//下支架(後)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(206 / 255.0, 209 / 225.0, 219 / 255.0);
		glPushMatrix();
		glTranslatef(5, 0, 0);//右,上,前
		glRotatef(-90, 0, 1, 0);//前,上,左
		if (cylind == NULL) cylind = gluNewQuadric();
		gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
							10,      /* height of the cylinder */
							12,       /* use 12-side polygon approximating circle*/
							4);       /* Divide it into 3 sections */
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(206 / 255.0, 209 / 225.0, 219 / 255.0);
		glTranslatef(0, 1, 0);//前,上,左
		glRotatef(90, 1, 0, 0);//前,左,下
		if (cylind == NULL) cylind = gluNewQuadric();
		gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
							6,      /* height of the cylinder */
							12,       /* use 12-side polygon approximating circle*/
							4);       /* Divide it into 3 sections */
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(206 / 255.0, 209 / 225.0, 219 / 255.0);
		glTranslatef(0, 10, 0);//前,左,下
		if (cylind == NULL) cylind = gluNewQuadric();
		gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
							6,      /* height of the cylinder */
							12,       /* use 12-side polygon approximating circle*/
							4);       /* Divide it into 3 sections */
		glPopMatrix();
		
		//頂桿
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(206 / 255.0, 209 / 225.0, 219 / 255.0);
		glPushMatrix();
		glTranslatef(0, 13, 0);//右,上,前
		if (cylind == NULL) cylind = gluNewQuadric();
		gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
							30,      /* height of the cylinder */
							12,       /* use 12-side polygon approximating circle*/
							4);       /* Divide it into 3 sections */
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(206 / 255.0, 209 / 225.0, 219 / 255.0);
		glRotatef(90, 1, 0, 0);//r,f,d
		if (cylind == NULL) cylind = gluNewQuadric();
		gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
							14,      /* height of the cylinder */
							12,       /* use 12-side polygon approximating circle*/
							4);       /* Divide it into 3 sections */
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(206 / 255.0, 209 / 225.0, 219 / 255.0);
		glTranslatef(0, 30, 0);//r,f,d
		if (cylind == NULL) cylind = gluNewQuadric();
		gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
							14,      /* height of the cylinder */
							12,       /* use 12-side polygon approximating circle*/
							4);       /* Divide it into 3 sections */
		glPopMatrix();

		//中桿
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(206 / 255.0, 209 / 225.0, 219 / 255.0);
		glPushMatrix();
		glTranslatef(0, 10, 0);//右,上,前
		if (cylind == NULL) cylind = gluNewQuadric();
		gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
							30,      /* height of the cylinder */
							12,       /* use 12-side polygon approximating circle*/
							4);       /* Divide it into 3 sections */
		glPopMatrix();

		glPopMatrix();
		glPopMatrix();
	}
}	handlebar;

struct Robot{
	struct Act{
		int action = 0, fir = -2, idiomatic = 0, inc = -1;
	}act;
	struct Joint {
		struct Rot {
			double ang = 0, rx = 0, ry = 0, rz = 0;
		}rot, swp;
		struct Mov {
			double dx = 0, dy = 0, dz = 0, dh = 0;
		}mov;

		void draw(double r,
			double px, double py, double pz,
			double ang, double rx, double ry, double rz,
			int cr, int cg, int cb) {

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor3f(cr / 255.0, cg / 225.0, cb / 255.0);
			glPushMatrix();
			glScalef(1, 1, 1);
			glTranslatef(px, py, pz);
			glRotatef(swp.ang, swp.rx, swp.ry, swp.rz);//swap
			glRotatef(ang, rx, ry, rz);//用以控制枝幹方向
			if (sphere == NULL)
				sphere = gluNewQuadric();
			gluSphere(sphere, r,   /* radius=0.5 */
				12,            /* composing of 12 slices*/
				12);           /* composing of 12 stacks */
		}
	}	chest, belly, waist,
		hip_r, hip_l, knee_l, knee_r, ankle_l, ankle_r,
		shouder_l, shouder_r, elbow_l, elbow_r, wrist_l, wrist_r,
		knuckle1_l, knuckle1_r, knuckle4_l, knuckle4_r;
	struct Skirt {
		struct Cyl {
			double tr = 4.0, br = 5.0, h = 3.5;
		}	cyl;
		void draw(int cr, int cg, int cb) {

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor3f(cr / 255.0, cg / 225.0, cb / 255.0);
			glScalef(1, 1, 1);
			glTranslatef(0, 0, 2.5);
			glRotatef(0, 0, 0, 0);
			if (cylind == NULL) cylind = gluNewQuadric();
			gluCylinder(cylind, cyl.tr, cyl.br, /* radius of top and bottom circle */
								cyl.h,      /* height of the cylinder */
								12,       /* use 12-side polygon approximating circle*/
								4);       /* Divide it into 3 sections */
		}
	}	skirt;

	struct Trunk {
		struct Mov {
			double dx = 0, dy = 0, dz = 0, dh = 0;
		}mov;
		void draw(double tr, double br, double h,
			double px, double py, double pz,
			double sx, double sy, double sz,
			int cr, int cg, int cb) {

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor3f(cr / 255.0, cg / 225.0, cb / 255.0);
			glScalef(sx, sy, sz);
			glTranslatef(px, py, pz);
			glRotatef(0, 0, 0, 0);
			if (cylind == NULL) cylind = gluNewQuadric();
			gluCylinder(cylind, tr, br, /* radius of top and bottom circle */
				h,      /* height of the cylinder */
				12,       /* use 12-side polygon approximating circle*/
				4);       /* Divide it into 3 sections */
		}
	}	trunk, shoes;

	void init() {
		belly.rot = { 90.0, 1.0, 0.0, 0.0 }; chest.rot = { -180, 1, 0, 0 }; waist.rot = { 0, 0, 0, 0 };
		hip_l.rot = hip_r.rot = knee_l.rot = knee_r.rot = { 0, 0, 0, 0 };
		ankle_l.rot = ankle_r.rot = { -90, 1, 0, 0 }; 
		shouder_l.rot = { 165.0, 0, 1, 0 }; shouder_r.rot = { -165.0, 0, -1, 0 };
		elbow_l.rot = { -15, 0, 1, 0 }; elbow_r.rot = { 15, 0, -1, 0 };
		wrist_l.rot = { 0, 0, 0, 0 }, wrist_r.rot = { 0, 0, 0, 0 };
		knuckle1_l.rot = { -30, 0, 1, 0 }, knuckle1_r.rot = { -30, 0, 1, 0 };
		knuckle4_l.rot = { 0, 0, 0, 0 }, knuckle4_r.rot = { 0, 0, 0, 0 };

		chest.mov = { 0, 0, 4.5 }; belly.mov = { 0, 13.25, 0 }; waist.mov = { 0, 0, 0 };
		hip_l.mov = { 1.6, 0, 1.8 }; hip_r.mov = { -1.6, 0, 1.8 };
		knee_l.mov = knee_r.mov = { 0, 0.4, 4.0 };
		ankle_l.mov = ankle_r.mov = { 0, -0.1, 3.25 };
		shouder_l.mov = { 3.0, 0, 0.5 }; shouder_r.mov = { -3.0, 0, 0.5 };
		elbow_l.mov = elbow_r.mov = { 0, 0, 6 };
		wrist_l.mov = wrist_r.mov = { 0, 0, 4 };
		knuckle1_l = { 0, -0.75, 1.5 }; knuckle1_r = { 0, 0.75, 1.5 };
		knuckle4_l = { 0, -1.0, 1.5 }; knuckle4_r = { 0, 1.0, 1.5 };
	}
	
	void jmp() {
		if (robot.act.inc == 0 || robot.act.inc == 6) {
			robot.hip_r.rot = { -15.0, 1.0, 0.0, 0.0 };
			robot.hip_l.rot = { -15.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 30.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 30.0, 1.0, 0.0, 0.0 };
			robot.elbow_r.rot = { -60.0 , 1.0, 0.0, 0.0 };
			robot.elbow_l.rot = { -60.0 , 1.0, 0.0, 0.0 };
			robot.belly.mov = { 0, 12.75, 0 };
		}
		if (robot.act.inc == 1 || robot.act.inc == 5) {
			robot.hip_r.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.hip_l.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 30.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 30.0, 1.0, 0.0, 0.0 };
			robot.elbow_r.rot = { -90.0 , 1.0, 0.0, 0.0 };
			robot.elbow_l.rot = { -90.0 , 1.0, 0.0, 0.0 };
			robot.belly.mov = { 0, 12.0, 0 };
		}
		if (robot.act.inc == 2 || robot.act.inc == 4) {
			robot.hip_r.rot = { -15.0, 1.0, 0.0, 0.0 };
			robot.hip_l.rot = { -15.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 30.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 30.0, 1.0, 0.0, 0.0 };
			robot.elbow_r.rot = { -30.0 , 1.0, 0.0, 0.0 };
			robot.elbow_l.rot = { -30.0 , 1.0, 0.0, 0.0 };
			robot.belly.mov = { 0, 14.5, 0 };
		}
		if (robot.act.inc == 3) {
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.elbow_r.rot = { 0.0 , 0.0, 0.0, 0.0 };
			robot.elbow_l.rot = { 0.0 , 0.0, 0.0, 0.0 };
			robot.belly.mov = { 0, 15.0, 0 };
		}
		if (robot.act.inc < 0) {
			belly.mov = { 0, 13.25, 0 };
			robot.hip_l.rot = robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.elbow_l.swp = robot.elbow_r.swp = { 0.0, 0.0, 0.0, 0.0 };
		}
	}

	void run() {
		//0:right_lift, 1:left_lift 2:right_down 3:left_down
		if (robot.act.fir == 0) {
			robot.hip_r.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.hip_l.rot = { -15.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 45.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { -30.0, 1.0, 0.0, 0.0 };
			robot.belly.mov = { 0, 12.25, 0 };
			robot.belly.rot = { 105, 1, 0, 0 };
		}
		if (robot.act.fir == 1) {
			robot.hip_l.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.hip_r.rot = { -15.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 45.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { -30.0, 1.0, 0.0, 0.0 };
		}
		if (robot.act.fir == 2) {
			robot.hip_r.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.hip_l.rot = { -15.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 30.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 60.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 0.0, 0.0, 0.0, 0.0 };
		}
		if (robot.act.fir == 3) {
			robot.hip_l.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.hip_r.rot = { -15.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 30.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 60.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
		}
		if (robot.act.fir < 0) {
			belly.mov = { 0, 13.25, 0 };
			robot.hip_l.rot  = robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_l.swp = robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.belly.rot = { 90, 1, 0, 0 };
		}
	}

	void leap_jump() {
		//0:right_lift, 1:left_lift 2:right_down 3:left_down
		if (robot.act.fir == 4) {
			robot.waist.rot = { -15.0, 1, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.2 , 3.0 };
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_l.rot = { -45.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0, 0, 0, 0 };
			robot.knee_l.rot = { 0, 0, 0, 0 };
			robot.shouder_r.swp = { -30.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.ankle_r = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l = { 0.0, 0.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 13.25, 0.0 };
		}
		if (robot.act.fir == 5) {
			robot.waist.rot = { -15.0, 1, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.2 , 3.0 };
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_r.rot = { -45.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0, 0, 0, 0 };
			robot.knee_r.rot = { 0, 0, 0, 0 };
			robot.shouder_l.swp = { -30.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.ankle_l = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_r = { 0.0, 0.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 13.25, 0.0 };
		}
		if (robot.act.fir == 6) {
			robot.waist.rot = { -30.0, 1, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.5 , 1.0 };
			robot.hip_r.rot = { 45.0, 1.0, 0.0, 0.0 };
			robot.hip_l.rot = { -60.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0, 0, 0, 0 };
			robot.knee_l.rot = { 0, 0, 0, 0 };
			robot.shouder_r.swp = { -60.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 40.0, 1.0, 0.0, 0.0 };
			robot.ankle_r = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l = { 0.0, 0.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 15.0, 0.0 };
		}
		if (robot.act.fir == 7) {
			robot.waist.rot = { -30.0, 1, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.5 , 1.0 };
			robot.hip_l.rot = { 45.0, 1.0, 0.0, 0.0 };
			robot.hip_r.rot = { -60.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0, 0, 0, 0 };
			robot.knee_r.rot = { 0, 0, 0, 0 };
			robot.shouder_l.swp = { -60.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 40.0, 1.0, 0.0, 0.0 };
			robot.ankle_l = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r = { 0.0, 0.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 15.0, 0.0 };
		}
		if (robot.act.fir == 8) {
			robot.waist.rot = { 0, 0, 0, 0 };
			robot.skirt.cyl = { 4.0, 6.0 , 0.2 };
			robot.hip_r.rot = { 70.0, 1.0, 0.0, 0.0 };
			robot.hip_l.rot = { -80.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0, 0, 0, 0 };
			robot.knee_l.rot = { 0, 0, 0, 0 };
			robot.shouder_r.swp = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 75.0, 1.0, 0.0, 0.0 };
			robot.ankle_r = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l = { 0.0, 0.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 15.5, 0.0 };
		}
		if (robot.act.fir == 9) {
			robot.waist.rot = { 0, 0, 0, 0 };
			robot.skirt.cyl = { 4.0, 6.0 , 0.2 };
			robot.hip_l.rot = { 65.0, 1.0, 0.0, 0.0 };
			robot.hip_r.rot = { -75.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0, 0, 0, 0 };
			robot.knee_l.rot = { 0, 0, 0, 0 };
			robot.shouder_l.swp = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 75.0, 1.0, 0.0, 0.0 };
			robot.ankle_l = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r = { 0.0, 0.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 15.5, 0.0 };
		}
		if (robot.act.fir == 10) {
			robot.waist.rot = { 30, 1, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.5 , 1.0 };
			robot.hip_r.rot = { 60.0, 1.0, 0.0, 0.0 };
			robot.hip_l.rot = { -45.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0, 0, 0, 0 };
			robot.knee_l.rot = { 0, 0, 0, 0 };
			robot.shouder_r.swp = { -60.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 40.0, 1.0, 0.0, 0.0 };
			robot.ankle_r = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l = { 0.0, 0.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 15.0, 0.0 };
		}
		if (robot.act.fir == 11) {
			robot.waist.rot = { 30, 1, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.5 , 1.0 };
			robot.hip_l.rot = { 60.0, 1.0, 0.0, 0.0 };
			robot.hip_r.rot = { -45.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0, 0, 0, 0 };
			robot.knee_r.rot = { 0, 0, 0, 0 };
			robot.shouder_l.swp = { -60.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 40.0, 1.0, 0.0, 0.0 };
			robot.ankle_l = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r = { 0.0, 0.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 15.0, 0.0 };
		}
		if (robot.act.fir == 12) {
			robot.waist.rot = { 15, 1, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.2 , 3.0 };
			robot.hip_r.rot = { 45.0, 1.0, 0.0, 0.0 };
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0, 0, 0, 0 };
			robot.knee_l.rot = { 0, 0, 0, 0 };
			robot.shouder_r.swp = { -30.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.ankle_r = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l = { -90.0, 1.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 13.25, 0.0 };
		}
		if (robot.act.fir == 13) {
			robot.waist.rot = { 15, 1, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.2 , 3.0 };
			robot.hip_l.rot = { 45.0, 1.0, 0.0, 0.0 };
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0, 0, 0, 0 };
			robot.knee_r.rot = { 0, 0, 0, 0 };
			robot.shouder_l.swp = { -30.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.ankle_l = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r = { -90.0, 1.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 13.25, 0.0 };
		}
		if (robot.act.fir < 0) {
			cout << "in";
			robot.waist.rot = { 0, 0, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.0 , 3.5 };
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0, 0, 0, 0 };
			robot.knee_r.rot = { 0, 0, 0, 0 };
			robot.shouder_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_r = { -90.0, 1.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 13.25, 0.0 };
		}
	}

	void lift_jump() {
		//0:right_lift, 1:left_lift 2:right_down 3:left_down
		if (robot.act.fir == 2 || robot.act.fir == 6) {
			robot.waist.rot = { -30.0, 1, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.5 , 3.0 };
			robot.hip_r.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.hip_l.rot = { -45.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 30.0, 1, 0, 0 };
			robot.knee_l.rot = { 90.0, 0, 0, 0 };
			robot.shouder_r.swp = { -60.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 30.0, 1.0, 0.0, 0.0 };
			robot.ankle_r = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l = { 0.0, 0.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 12.5, 0.0 };
		}
		if (robot.act.fir == 3 || robot.act.fir == 7) {
			robot.waist.rot = { -30.0, 1, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.5 , 3.0 };
			robot.hip_l.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.hip_r.rot = { -45.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 30.0, 1, 0, 0 };
			robot.knee_r.rot = { 90.0, 0, 0, 0 };
			robot.shouder_l.swp = { -60.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 30.0, 1.0, 0.0, 0.0 };
			robot.ankle_l = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_r = { 0.0, 0.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 12.5, 0.0 };
		}
		if (robot.act.fir == 4) {
			robot.waist.rot = { -60.0, 1, 0, 0 };
			robot.skirt.cyl = { 4.0, 6.0 , 2.8 };
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0.0, 0, 0, 0 };
			robot.knee_l.rot = { 90.0, 0, 0, 0 };
			robot.shouder_r.swp = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 45.0, 1.0, 0.0, 0.0 };
			robot.ankle_r = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l = { 0.0, 0.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 15.0, 0.0 };
		}
		if (robot.act.fir == 5) {
			robot.waist.rot = { -60.0, 1, 0, 0 };
			robot.skirt.cyl = { 4.0, 6.0 , 2.8 };
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0, 0, 0 };
			robot.knee_r.rot = { 90.0, 0, 0, 0 };
			robot.shouder_l.swp = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 45.0, 1.0, 0.0, 0.0 };
			robot.ankle_l = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r = { 0.0, 0.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 15.0, 0.0 };
		}
		if (robot.act.fir < 0) {
			robot.waist.rot = { 0, 0, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.0 , 3.5 };
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0, 0, 0, 0 };
			robot.knee_r.rot = { 0, 0, 0, 0 };
			robot.shouder_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_r = { -90.0, 1.0, 0.0, 0.0 };
			robot.belly.mov = { 0.0, 13.25, 0.0 };
		}
	}

	void tun_jump(int pos) {
		if (robot.act.fir == 2) {//非軸心角定位
			robot.hip_l.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.hip_r.rot = { -45.0, 5.0, -5.0, 1.0 };
			robot.knee_l.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 45.0, 5.0, 5.0, 1.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { -5, 1, 0, 0 };
			robot.shouder_l.swp = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 90.0, 1.0, 0.0, 0.0 };
			robot.elbow_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.elbow_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { -5, 1, 0, 0 };
			robot.waist.mov = { 0, 0, -1.5 };
			skirt.cyl = { 4.2, 5.5, 4 };
			robot.belly.mov = { 0, 13.25 + 1.2 * pos - 0.1 * pos * pos, 0 };
		}
		if (robot.act.fir == 3) {//非軸心角定位
			robot.hip_r.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.hip_l.rot = { -45.0, 5.0, 5.0, 1.0 };
			robot.knee_r.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 45.0, 5.0, -5.0, 1.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 90.0, 0.0, 1.0, 0.0 };
			robot.shouder_l.swp = { -90.0, 0.0, 1.0, 0.0 };
			robot.elbow_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.elbow_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { -5, 1, 0, 0 };
			robot.waist.mov = { 0, 0, -1.5 };
			skirt.cyl = { 4.2, 5.5, 4 };
			robot.belly.mov = { 0, 13.25 + 3.6 * pos - 0.3 * pos * pos, 0 };
		}
		if (robot.act.fir < 0) {
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.elbow_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.elbow_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { 0, 0, 0, 0 };
			robot.waist.mov = { 0, 0, 0 };
			skirt.cyl = { 4.0, 5.0, 3.5 };
			robot.belly.mov = { 0, 13.25, 0 };
		}
	}

	void walk() {
		//0:right_lift, 1:left_lift 2:right_down 3:left_down
		if (robot.act.fir == 0) {
			robot.hip_r.rot = { -30.0, 1.0, -0.4, 0.0 };
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -60.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { -30.0, 1.0, 0.0, 0.0 };
			robot.waist.rot = { -15,1,0,0 };
			robot.belly.mov = { 0, 12.25, 0 };
		}
		if (robot.act.fir == 1) {
			robot.hip_l.rot = { -30.0, 1.0, 0.4, 0.0 };
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -60.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { -30.0, 1.0, 0.0, 0.0 };
			robot.waist.rot = { -15,1,0,0 };
			robot.belly.mov = { 0, 12.25, 0 };
		}
		if (robot.act.fir == 2) {
			robot.hip_r.rot = { -10.0, 1.0, -0.4, 0.0 };
			robot.hip_l.rot = { -5.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 10.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 10.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -80.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -80.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { 0,0,0,0 }; robot.skirt.cyl = { 4.0, 5.5 , 3.0 };
			robot.belly.mov = { 0, 15.00, 0 };
		}
		if (robot.act.fir == 3) {
			robot.hip_l.rot = { -10.0, 1.0, 0.4, 0.0 };
			robot.hip_r.rot = { -5.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 10.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 10.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -80.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -80.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { 0, 0, 0, 0 }; robot.skirt.cyl = { 4.0, 5.5 , 3.0 };
			robot.belly.mov = { 0, 15.00, 0 };
		}
		if (robot.act.fir == 4) {
			robot.hip_r.rot = { -30.0, 1.0, -0.4, 0.0 };
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -60.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { -30.0, 1.0, 0.0, 0.0 };
			robot.waist.rot = { -15, 1, 0, 0 }; robot.skirt.cyl = { 4.0, 5.0 , 3.5 };
			robot.belly.mov = { 0, 12.25, 0 };
		}
		if (robot.act.fir == 5) {
			robot.hip_l.rot = { -30.0, 1.0, 0.4, 0.0 };
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -60.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { -30.0, 1.0, 0.0, 0.0 };
			robot.waist.rot = { -15, 1, 0, 0 }; robot.skirt.cyl = { 4.0, 5.0 , 3.5 };
			robot.belly.mov = { 0, 12.25, 0 };
		}
		if (robot.act.fir < 0) {
			robot.hip_l.rot = robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l.rot = robot.ankle_r.rot = { -90, 1, 0, 0 };
			robot.shouder_l.swp = robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { 0, 0, 0, 0 };  robot.skirt.cyl = { 4.0, 5.0 , 3.5 };
			robot.belly.mov = { 0, 13.25, 0 };
		}
	}

	void lwalk() {
		//0:right_lift, 1:left_lift 2:right_down 3:left_down
		if (robot.act.inc == 0) {
			robot.hip_r.rot = { -45.0, 1.0, -1.0, 0.0 };
			robot.hip_l.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 60.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 75.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { -30.0, 1.0, 0.0, 0.0 };
			robot.waist.rot = { -10, 1, 0, 0 };
			robot.chest.swp = { 5, 0, 0, 1 };
			robot.belly.swp = { 10, 1, 0, 0 };
			robot.belly.mov = { 0, 12.25, 0 };
		}
		if (robot.act.inc == 1 || robot.act.inc == 3) {
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { 0, 0, 0, 0 };
			robot.chest.swp = { 0, 0, 0, 0 };
			robot.belly.swp = { 0, 0, 0, 0 };
			robot.belly.mov = { 0, 13.25, 0 };
		}
		if (robot.act.inc == 2) {
			robot.hip_r.rot = { 20.0, -0.5, 1.0, 0.0 };
			robot.hip_l.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 60.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { -30.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.waist.rot = { -10, 1, 0, 0 };
			robot.chest.swp = { -5, 0, 0, 1 };
			robot.belly.swp = { 10, 1, 0, 0 };
			robot.belly.mov = { 0, 12.25, 0 };
		}
		if (robot.act.inc < 0) {
			robot.hip_l.rot = robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_l.swp = robot.hip_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l.rot = robot.ankle_r.rot = { -90, 1, 0, 0 };
			robot.shouder_l.swp = robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.chest.swp = { 0, 0, 0, 0 }; robot.waist.rot = { 0, 0, 0, 0 };
			skirt.cyl = { 4.0, 5.0, 3.5 };
			robot.belly.swp = { 10, 1, 0, 0 }; robot.belly.mov = { 0, 13.25, 0 };
		}
	
	}

	void lwalk_jump(int cnt) {
		robot.hip_r.rot = { -45.0 + 15 * cnt - 1.25 * cnt * cnt, 1.0, -1.0, 0.0 };
		robot.hip_l.swp = { 30 * cnt - 2.5 * cnt * cnt, 0.0, 1.0, 0.0 };
		robot.hip_l.rot = { -30.0 + 10 * cnt - 1.25 * cnt * cnt, 1.0, 0.0, 0.0 };
		robot.knee_r.rot = { 60.0 - 20 * cnt + 1.7 * cnt * cnt, 1.0, 0.0, 0.0 };
		robot.knee_l.rot = { 75.0 - 25 * cnt + 2.08* cnt * cnt, 1.0, 0.0, 0.0 };
		robot.ankle_r.rot = { -90.0 + 30 * cnt - 1.25 * cnt * cnt, 1.0, 0.0, 0.0 };
		robot.ankle_l.rot = { 0, 0.0, 0.0, 0.0 };
		robot.shouder_r.swp = { 30 * cnt - 2.5 * cnt * cnt , 0.0, 1.0, 0.0 };
		robot.shouder_l.swp = { -30.0 -20 * cnt + 1.67 * cnt * cnt, 1.0, 0.0, 0.0 };
		robot.waist.rot = { 10 * cnt - 0.8 * cnt * cnt, 0, 1, 0 };
		robot.waist.mov = { 0, 0, -0.25 * cnt + 0.042 * cnt * cnt };
		skirt.cyl = { 4.0, 5.0 + 0.3 * cnt - 0.28 * cnt * cnt, 3.5 - 0.84 * cnt + 0.07 * cnt * cnt};
		robot.chest.swp = { 0, 0, 0, 0 };
		robot.belly.swp = { 10 - 1.67 * cnt - 0.14 * cnt * cnt, 1, 0, 0 };
		robot.belly.mov = { 0, 13.25 + 3 * cnt - 0.25 * cnt * cnt, 0 };
	}

	void rwalk() {
		//0:right_lift, 1:left_lift 2:right_down 3:left_down
		if (robot.act.inc == 0) {
			robot.hip_l.rot = { -45.0, 1.0, 1.0, 0.0 };
			robot.hip_r.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 60.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 75.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { -30.0, 1.0, 0.0, 0.0 };
			robot.waist.rot = { 10, 1, 0, 0 };
			robot.chest.swp = { -5, 0, 0, 1 };
			robot.belly.swp = { 10, 1, 0, 0 };
			robot.belly.mov = { 0, 12.25, 0 };
		}
		if (robot.act.inc == 1 || robot.act.inc == 3) {
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { 0, 0, 0, 0 };
			robot.chest.swp = { 0, 0, 0, 0 };
			robot.belly.swp = { 0, 0, 0, 0 };
			robot.belly.mov = { 0, 13.25, 0 };
		}
		if (robot.act.inc == 2) {
			robot.hip_l.rot = { 20.0, -0.5, -1.0, 0.0 };
			robot.hip_r.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 60.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { -30.0, 1.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 15.0, 1.0, 0.0, 0.0 };
			robot.waist.rot = { 10, 1, 0, 0 };
			robot.chest.swp = { 5, 0, 0, 1 };
			robot.belly.swp = { 10, 1, 0, 0 };
			robot.belly.mov = { 0, 12.25, 0 };
		}
		if (robot.act.inc < 0) {
			robot.hip_l.rot = robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_l.swp = robot.hip_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l.rot = robot.ankle_r.rot = { -90, 1, 0, 0 };
			robot.shouder_l.swp = robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.chest.swp = { 0, 0, 0, 0 }; robot.waist.rot = { 0, 0, 0, 0 };
			skirt.cyl = { 4.0, 5.0, 3.5 };
			robot.belly.swp = { 10, 1, 0, 0 }; robot.belly.mov = { 0, 13.25, 0 };
		}

	}

	void rwalk_jump(int cnt) {
		robot.hip_l.rot = { -45.0 + 15 * cnt - 1.25 * cnt * cnt, 1.0, -1.0, 0.0 };
		robot.hip_r.swp = { 30 * cnt - 2.5 * cnt * cnt, 0.0, 1.0, 0.0 };
		robot.hip_r.rot = { -30.0 + 10 * cnt - 1.25 * cnt * cnt, 1.0, 0.0, 0.0 };
		robot.knee_l.rot = { 60.0 - 20 * cnt + 1.7 * cnt * cnt, 1.0, 0.0, 0.0 };
		robot.knee_r.rot = { 75.0 - 25 * cnt + 2.08* cnt * cnt, 1.0, 0.0, 0.0 };
		robot.ankle_l.rot = { -90.0 + 30 * cnt - 1.25 * cnt * cnt, 1.0, 0.0, 0.0 };
		robot.ankle_r.rot = { 0, 0.0, 0.0, 0.0 };
		robot.shouder_l.swp = { 30 * cnt - 2.5 * cnt * cnt , 0.0, 1.0, 0.0 };
		robot.shouder_r.swp = { -30.0 - 20 * cnt + 1.67 * cnt * cnt, 1.0, 0.0, 0.0 };
		robot.waist.rot = { 10 * cnt - 0.8 * cnt * cnt, 0, 1, 0 };
		robot.waist.mov = { 0, 0, -0.25 * cnt + 0.042 * cnt * cnt };
		skirt.cyl = { 4.0, 5.0 + 0.3 * cnt - 0.28 * cnt * cnt, 3.5 - 0.84 * cnt + 0.07 * cnt * cnt };
		robot.chest.swp = { 0, 0, 0, 0 };
		robot.belly.swp = { 10 - 1.67 * cnt - 0.14 * cnt * cnt, 1, 0, 0 };
		robot.belly.mov = { 0, 13.25 + 3 * cnt - 0.25 * cnt * cnt, 0 };
	}

	void rot() {
		if (robot.act.fir == 0) {//非軸心角移動
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_r.rot = { 30.0, 0.0, -1.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { 15, 0, -1, 0 };
			robot.waist.mov = { 0, 0, -1.5 };
			robot.skirt.cyl = { 4.2, 5.5, 4 };
			robot.belly.mov = { 0, 13.25, 0 };
		}
		if (robot.act.fir == 1) {//非軸心角移動
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_l.rot = { 30.0, 0.0, 1.0, 1.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { 5, 0, 1, 0 };
			robot.waist.mov = { 0, 0, -1.5 };
			robot.skirt.cyl = { 4.2, 5.5, 4.0 };
			robot.belly.mov = { 0, 13.25, 0 };
		}
		if (robot.act.fir == 2 || robot.act.fir == 11) {//非軸心角定位
			robot.hip_l.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.hip_r.rot = { -20.0, 5.0, -4.0, 1.0 };
			robot.knee_l.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 45.0, 5.0, 4.0, 1.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { -5, 1, 0, 0 };
			robot.waist.mov = { 0, 0, -1.5 };
			robot.skirt.cyl = { 4.2, 5.5, 4 };
			robot.belly.mov = { 0, 12.5, 0 };
		}
		if (robot.act.fir == 3 || robot.act.fir == 10) {//非軸心角定位
			robot.hip_r.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.hip_l.rot = { -20.0, 5.0, 4.0, 1.0 };
			robot.knee_r.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 45.0, 5.0, -4.0, 1.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { -5, 1, 0, 0 };
			robot.waist.mov = { 0, 0, -1.5 };
			robot.skirt.cyl = { 4.2, 5.5, 4 };
			robot.belly.mov = { 0, 12.5, 0 };
		}
		if (robot.act.fir == 4 || robot.act.fir == 9) {//非軸心角定位
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_r.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 15.0, 1.0, 0.0, 0.00 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { -5, 1, 0, 0 };
			robot.waist.mov = { 0, 0, -1.5 };
			robot.skirt.cyl = { 4.2, 5.5, 4 };
			robot.belly.mov = { 0, 12.75, 0 };
		}
		if (robot.act.fir == 5 || robot.act.fir == 8) {//非軸心角定位
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_l.rot = { -30.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 15.0, 1.0, 0.0, 0.00 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { -5, 1, 0, 0 };
			robot.waist.mov = { 0, 0, -1.5 };
			robot.skirt.cyl = { 4.2, 5.5, 4 };
			robot.belly.mov = { 0, 12.75, 0 };
			robot.belly.mov = { 0, 12.25, 0 };
		}
		if (robot.act.fir == 6 || robot.act.fir == 7) {//非軸心角定位
			robot.hip_l.rot = { -15.0, 1.0, 0.0, 0.0 };
			robot.hip_r.rot = { -15.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 15.0, 1.0, 0.0, 0.00 };
			robot.ankle_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { 0, 0, 0, 0 };
			robot.waist.mov = { 0, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.0, 3.5 };
			robot.belly.mov = { 0, 13.25, 0 };
		}
		if (robot.act.fir < 0) {//非軸心角定位
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.00 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.waist.rot = { 0, 0, 0, 0 };
			robot.waist.mov = { 0, 0, 0 };
			robot.skirt.cyl = { 4.0, 5.0, 3.5 };
			robot.belly.mov = { 0, 13.25, 0 };
		}
	}

	void turn() {
		if (robot.act.fir == 0) {//非軸心角移動
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_r.rot = { 30.0, 0.0, -1.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_l.swp = { -90.0, 0.0, 1.0, 0.0 };
			robot.shouder_r.swp = { 90.0, 0.0, 1.0, 0.0 };
			robot.elbow_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.elbow_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { 15, 0, -1, 0 };
			robot.waist.mov = { 0, 0, -1.5 };
			skirt.cyl = { 4.2, 5.5, 4 };
			robot.belly.mov = { 0, 13.25, 0 };
		}
		if (robot.act.fir == 1) {//非軸心角移動
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_l.rot = { 30.0, 0.0, 1.0, 1.0 };
			robot.knee_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 90.0, 0.0, 1.0, 0.0 };
			robot.shouder_l.swp = { -90.0, 0.0, 1.0, 0.0 };
			robot.elbow_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.elbow_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { 5, 0, 1, 0 };
			robot.waist.mov = { 0, 0, -1.5 };
			robot.waist.rot = { 4.2, 5.5, 4.0 };
			robot.belly.mov = { 0, 13.25, 0 };
		}
		if (robot.act.fir == 2) {//非軸心角定位
			robot.hip_l.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.hip_r.rot = { -45.0, 5.0, -5.0, 1.0 };
			robot.knee_l.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.knee_r.rot = { 45.0, 5.0, 5.0, 1.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { -5, 1, 0, 0 };
			robot.shouder_l.swp = { -45.0, 0.0, 1.0, 0.0 };
			robot.shouder_r.swp = { 45.0, 0.0, 1.0, 0.0 };
			robot.elbow_l.swp = { 60.0, 0.0, 1.0, 0.0 };
			robot.elbow_r.swp = { -60.0, 0.0, 1.0, 0.0 };
			robot.waist.rot = { -5, 1, 0, 0 };
			robot.waist.mov = { 0, 0, -1.5 };
			skirt.cyl = { 4.2, 5.5, 4 };
			robot.belly.mov = { 0, 12.5, 0 };
		}
		if (robot.act.fir == 3) {//非軸心角定位
			robot.hip_r.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.hip_l.rot = { -45.0, 5.0, 5.0, 1.0 };
			robot.knee_r.rot = { 15.0, 1.0, 0.0, 0.0 };
			robot.knee_l.rot = { 45.0, 5.0, -5.0, 1.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 45.0, 0.0, 1.0, 0.0 };
			robot.shouder_l.swp = { -45.0, 0.0, 1.0, 0.0 };
			robot.elbow_l.swp = { 60.0, 0.0, 1.0, 0.0 };
			robot.elbow_r.swp = { -60.0, 0.0, 1.0, 0.0 };
			robot.waist.rot = { -5, 1, 0, 0 };
			robot.waist.mov = { 0, 0, -1.5 };
			skirt.cyl = { 4.2, 5.5, 4 };
			robot.belly.mov = { 0, 12.5, 0 };
		}
		if (robot.act.fir < 0) {
			robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.hip_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.knee_r.rot = { 0, 0.0, 0.0, 0.0 };
			robot.knee_l.rot = { 0.0, 0.0, 0.0, 0.0 };
			robot.ankle_r.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.ankle_l.rot = { -90.0, 1.0, 0.0, 0.0 };
			robot.shouder_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.shouder_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.elbow_l.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.elbow_r.swp = { 0.0, 0.0, 0.0, 0.0 };
			robot.waist.rot = { 0, 0, 0, 0 };
			robot.waist.mov = { 0, 0, 0 };
			skirt.cyl = { 4.0, 5.0, 3.5 };
			robot.belly.mov = { 0, 13.25, 0 };
		}
	}
	
	void rot_mat(double ang, double rx, double ry, double rz) {
		vector<vector<double>>mat_rot;
		mat_rot.resize(4);
		for (int i = 0; i < 4; ++i) mat_rot[i].resize(4);
		ang = ang * acos(-1) / 180.0;
		mat_rot[0][0] = cos(ang) + rx * rx * (1 - cos(ang));
		mat_rot[0][1] = rx * ry * (1 - cos(ang)) - rz * sin(ang);
		mat_rot[0][2] = rx * rz * (1 - cos(ang)) + ry * sin(ang);
		mat_rot[1][0] = ry * rx * (1 - cos(ang)) + rz * sin(ang);
		mat_rot[1][1] = cos(ang) + ry * ry * (1 - cos(ang));
		mat_rot[1][2] = ry * rz * (1 - cos(ang)) - rx * sin(ang);
		mat_rot[2][0] = rz * rx * (1 - cos(ang)) - ry * sin(ang);
		mat_rot[2][1] = rz * ry * (1 - cos(ang)) + rx * sin(ang);
		mat_rot[2][2] = cos(ang) + rz * rz * (1 - cos(ang));
		mat_rot[0][3] = mat_rot[1][3] = mat_rot[2][3] = 0;
		mat_rot[3][0] = mat_rot[3][1] = mat_rot[3][2] = 0;
		mat_rot[3][3] = 1;
		double cmp[4] = { 0, 0, 0, 0 };
		for (int i = 0; i < 4; ++i) cmp[i] += mat_rot[i][0] * trunk.mov.dx;
		for (int i = 0; i < 4; ++i) cmp[i] += mat_rot[i][1] * trunk.mov.dy;
		for (int i = 0; i < 4; ++i) cmp[i] += mat_rot[i][2] * trunk.mov.dz;
		for (int i = 0; i < 4; ++i) cmp[i] += mat_rot[i][3] * trunk.mov.dh;
		trunk.mov = { cmp[0], cmp[1], cmp[2], cmp[3] };
	}
	
	void draw() {
		//肚子
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(174 / 255.0, 203 / 225.0, 213 / 255.0);
		glPushMatrix();
		glScalef(1, 1, 0.75);
		glTranslatef(belly.mov.dx, belly.mov.dy, belly.mov.dz);//右,前,上
		glRotatef(belly.swp.ang, belly.swp.rx, belly.swp.ry, belly.swp.rz);
		glRotatef(belly.rot.ang, belly.rot.rx, belly.rot.ry, belly.rot.rz);
		if (cylind == NULL) cylind = gluNewQuadric();
		gluCylinder(cylind, 3.0, 4.0, /* radius of top and bottom circle */
							3.5,      /* height of the cylinder */
							12,       /* use 12-side polygon approximating circle*/
							4);       /* Divide it into 3 sections */
		
		//腿_右
		hip_r.draw(1.8, hip_r.mov.dx, hip_r.mov.dy, hip_r.mov.dz, hip_r.rot.ang, hip_r.rot.rx, hip_r.rot.ry, hip_r.rot.rz, 237, 225, 211);
		trunk.draw(1.8, 1.3, 4.0, 0, 0, 1.5, 1, 1, 1, 255, 242, 228);
		knee_r.draw(1.2, knee_r.mov.dx, knee_r.mov.dy, knee_r.mov.dz, knee_r.rot.ang, knee_r.rot.rx, knee_r.rot.ry, knee_r.rot.rz, 237, 225, 211);
		trunk.draw(1.2, 1.25, 1.25, 0, -0.5, 0.25, 1, 1, 1, 255, 242, 228);
		trunk.draw(1.25, 0.3, 3.5, 0, 0, 1.25, 1, 1, 1, 0, 0, 0);
		ankle_r.draw(0.5, ankle_r.mov.dx, ankle_r.mov.dy, ankle_r.mov.dz, ankle_r.rot.ang, ankle_r.rot.rx, ankle_r.rot.ry, ankle_r.rot.rz, 237, 225, 211);//右上前
		shoes.draw(0.8, 0.5, 3.0, 0, -0.3, -0.8, 1, 0.7, 1, 174, 203, 213);
		glPopMatrix(); //回膝蓋
		glPopMatrix(); //回髖關節
		glPopMatrix(); //回肚子

		//腿_左
		hip_l.draw(1.8, hip_l.mov.dx, hip_l.mov.dy, hip_l.mov.dz, hip_l.rot.ang, hip_l.rot.rx, hip_l.rot.ry, hip_l.rot.rz, 237, 225, 211);
		trunk.draw(1.8, 1.3, 4.0, 0, 0, 1.5, 1, 1, 1, 255, 242, 228);
		knee_l.draw(1.2, knee_l.mov.dx, knee_l.mov.dy, knee_l.mov.dz, knee_l.rot.ang, knee_l.rot.rx, knee_l.rot.ry, knee_l.rot.rz, 237, 225, 211);
		trunk.draw(1.2, 1.25, 1.25, 0, -0.5, 0.25, 1, 1, 1, 255, 242, 228);
		trunk.draw(1.25, 0.3, 3.25, 0, 0, 1.25, 1, 1, 1, 255, 255, 255);
		ankle_l.draw(0.5, ankle_l.mov.dx, ankle_l.mov.dy, ankle_l.mov.dz, ankle_l.rot.ang, ankle_l.rot.rx, ankle_l.rot.ry, ankle_l.rot.rz, 237, 225, 211);//右上前
		shoes.draw(0.8, 0.5, 3.0, 0, -0.3, -0.8, 1, 0.7, 1, 174, 203, 213);
		glPopMatrix(); //回膝蓋
		glPopMatrix(); //回髖關節
		glPopMatrix(); //回肚子

		waist.draw(0.5, waist.mov.dx, waist.mov.dy, waist.mov.dz, waist.rot.ang, waist.rot.rx, waist.rot.ry, waist.rot.rz, 237, 225, 211);//右後下
		skirt.draw(194, 226, 237);
		glPopMatrix();

		//胸
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(174 / 255.0, 203 / 225.0, 213 / 255.0);
		glPushMatrix();
		glScalef(1, 1, 0.75);
		glTranslatef(chest.mov.dx, chest.mov.dy, chest.mov.dz);//右,下,前
		glRotatef(chest.swp.ang, chest.swp.rx, chest.swp.ry, chest.swp.rz);
		glRotatef(chest.rot.ang, chest.rot.rx, chest.rot.ry, chest.rot.rz);
		if (cylind == NULL) cylind = gluNewQuadric();
		gluCylinder(cylind, 3.0, 3.5, /* radius of top and bottom circle */
							1.5,      /* height of the cylinder */
							12,       /* use 12-side polygon approximating circle*/
							4);       /* Divide it into 3 sections */
		trunk.draw(3.0, 3.5, 6.0, 0.0, 0.0, 6, 1, 1, 0.75, 174, 203, 213);
		trunk.draw(3.5, 2.5, 1.5, 0, 0, 7.5, 1, 1, 0.75, 255, 242, 228);

		//手_右
		shouder_r.draw(1.0, shouder_r.mov.dx, shouder_r.mov.dy, shouder_r.mov.dz, -shouder_r.rot.ang, shouder_r.rot.rx, shouder_r.rot.ry, shouder_r.rot.rz, 237, 225, 211);
		trunk.mov = { -0.1, -0.1, -0.1, 1 }; rot_mat(shouder_r.rot.ang, shouder_r.rot.rx, shouder_r.rot.ry, shouder_r.rot.rz);
		trunk.draw(1.0, 0.8, 6, trunk.mov.dx, trunk.mov.dy, trunk.mov.dz, 1, 1, 1, 255, 242, 228);
		elbow_r.draw(1.0, elbow_r.mov.dx, elbow_r.mov.dy, elbow_r.mov.dz, elbow_r.rot.ang, elbow_r.rot.rx, elbow_r.rot.ry, elbow_r.rot.rz, 237, 225, 211);
		trunk.mov = { -0.1, -0.1, -0.1, 1 }; rot_mat(elbow_r.rot.ang, elbow_r.rot.rx, elbow_r.rot.ry, elbow_r.rot.rz);
		trunk.draw(0.8, 0.5, 4, trunk.mov.dx, trunk.mov.dy, trunk.mov.dz, 1, 1, 1, 255, 242, 228);
		wrist_r.draw(0.5, wrist_r.mov.dx, wrist_r.mov.dy, wrist_r.mov.dz, wrist_r.rot.ang, wrist_r.rot.rx, wrist_r.rot.ry, wrist_r.rot.rz, 237, 225, 211);
		trunk.mov = { -0.1, -0.1, -0.1, 1 }; rot_mat(wrist_r.rot.ang, wrist_r.rot.rx, wrist_r.rot.ry, wrist_r.rot.rz);
		trunk.draw(0.55, 1.0, 1.5, trunk.mov.dx, trunk.mov.dy, trunk.mov.dz, 0.5, 1, 1, 237, 242, 228);
		knuckle1_r.draw(0.3, knuckle1_r.mov.dx, knuckle1_r.mov.dy, knuckle1_r.mov.dz, knuckle1_r.rot.ang, knuckle1_r.rot.rx, knuckle1_r.rot.ry, knuckle1_r.rot.rz, 237, 225, 211);
		trunk.mov = { -0.1, -0.1, -0.1, 1 }; rot_mat(knuckle1_r.rot.ang, knuckle1_r.rot.rx, knuckle1_r.rot.ry, knuckle1_r.rot.rz);
		trunk.draw(0.3, 0.3, 1.0, trunk.mov.dx, trunk.mov.dy, trunk.mov.dy, 1, 1, 1, 237, 242, 228);
		glPopMatrix(); //回手腕
		knuckle4_r.draw(1.2, knuckle4_r.mov.dx, knuckle4_r.mov.dy, knuckle4_r.mov.dz, knuckle4_r.rot.ang, knuckle4_r.rot.rx, knuckle4_r.rot.ry, knuckle4_r.rot.rz,  237, 225, 211);
		trunk.mov = { -0.1, -0.1, -0.1, 1 }; rot_mat(knuckle4_r.rot.ang, knuckle4_r.rot.rx, knuckle4_r.rot.ry, knuckle4_r.rot.rz);
		trunk.draw(1.2, 1.2, 1.5, trunk.mov.dx, trunk.mov.dy, trunk.mov.dy, 1, 0.5, 1, 237, 242, 228);
		glPopMatrix(); //回手腕
		glPopMatrix(); //回手肘
		glPopMatrix(); //回肩膀
		glPopMatrix(); //回胸

		//手_左
		shouder_l.draw(1.0, shouder_l.mov.dx, shouder_l.mov.dy, shouder_l.mov.dz, shouder_l.rot.ang, shouder_l.rot.rx, shouder_l.rot.ry, shouder_l.rot.rz, 237, 225, 211);
		trunk.mov = { -0.1, -0.1, -0.1, 1 }; rot_mat(shouder_l.rot.ang, shouder_l.rot.rx, shouder_l.rot.ry, shouder_l.rot.rz);
		trunk.draw(1.0, 0.8, 5.5, trunk.mov.dx, trunk.mov.dy, trunk.mov.dz, 1, 1, 1, 255, 242, 228);
		elbow_l.draw(1.0, elbow_r.mov.dx, elbow_l.mov.dy, elbow_l.mov.dz, -elbow_l.rot.ang, elbow_l.rot.rx, elbow_l.rot.ry, elbow_l.rot.rz, 237, 225, 211);
		trunk.mov = { -0.1, -0.1, -0.1, 1 }; rot_mat(elbow_l.rot.ang, elbow_l.rot.rx, elbow_l.rot.ry, elbow_l.rot.rz);
		trunk.draw(0.8, 0.5, 3, trunk.mov.dx, trunk.mov.dy, trunk.mov.dz, 1, 1, 1, 255, 242, 228);
		wrist_l.draw(0.5, wrist_l.mov.dx, wrist_l.mov.dy, wrist_l.mov.dz, -wrist_l.rot.ang, wrist_l.rot.rx, wrist_l.rot.ry, wrist_l.rot.rz, 237, 225, 211);
		trunk.mov = { -0.1, -0.1, -0.1, 1 }; rot_mat(wrist_l.rot.ang, wrist_l.rot.rx, wrist_l.rot.ry, wrist_l.rot.rz);
		trunk.draw(0.55, 1.0, 1.2, trunk.mov.dx, trunk.mov.dy, trunk.mov.dz, 1, 0.5, 1, 237, 242, 228);
		knuckle1_l.draw(0.24, knuckle1_l.mov.dx, knuckle1_l.mov.dy, knuckle1_l.mov.dz, -knuckle1_l.rot.ang, knuckle1_l.rot.rx, knuckle1_l.rot.ry, knuckle1_l.rot.rz, 237, 225, 211);
		trunk.mov = { -0.1, -0.1, -0.1, 1 }; rot_mat(knuckle1_l.rot.ang, knuckle1_l.rot.rx, knuckle1_l.rot.ry, knuckle1_l.rot.rz);
		trunk.draw(0.24, 0.24, 1, trunk.mov.dx, trunk.mov.dy, trunk.mov.dy, 1, 1, 1, 237, 242, 228);
		glPopMatrix(); //回手腕
		knuckle4_l.draw(0.96, knuckle4_l.mov.dx, knuckle4_l.mov.dy, knuckle4_l.mov.dz, -knuckle4_l.rot.ang, knuckle4_l.rot.rx, knuckle4_l.rot.ry, knuckle4_l.rot.rz, 237, 225, 211);
		trunk.mov = { -0.1, -0.1, -0.1, 1 }; rot_mat(knuckle4_l.rot.ang, knuckle4_l.rot.rx, knuckle4_l.rot.ry, knuckle4_l.rot.rz);
		trunk.draw(0.96, 0.96, 0.75, trunk.mov.dx, trunk.mov.dy, trunk.mov.dy, 0.5, 1, 1, 237, 242, 228);
		glPopMatrix(); //回手腕
		glPopMatrix(); //回手肘
		glPopMatrix(); //回肩膀
		glPopMatrix(); //回胸
		
		//脖子
		trunk.draw(1.5, 1.2, 2.5, 0.0, 0.0, 0.25, 1, 1, 1, 237, 242, 228);
		//頭
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(237 / 255.0, 242 / 225.0, 228 / 255.0);
		glScalef(0.8, 1, 1);
		glTranslatef(0, 0, 6);
		if (sphere == NULL)
			sphere = gluNewQuadric();
		gluSphere(sphere,	5,   /* radius=0.5 */
							12,            /* composing of 12 slices*/
							12);           /* composing of 12 stacks */
		//包包頭
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(255 / 255.0, 190 / 225.0, 150 / 255.0);
		glScalef(1, 1, 1);
		glTranslatef(0, 0.5, 0.3);
		if (sphere == NULL)
			sphere = gluNewQuadric();
		gluSphere(sphere,	5.2,   /* radius=0.5 */
							12,            /* composing of 12 slices*/
							12);           /* composing of 12 stacks */
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(255 / 255.0, 190 / 225.0, 150 / 255.0);
		glScalef(1, 1, 1);
		glTranslatef(0, 4.15, 3.25);
		if (sphere == NULL)
			sphere = gluNewQuadric();
		gluSphere(sphere,	1.8,   /* radius=0.5 */
							12,            /* composing of 12 slices*/
							12);           /* composing of 12 stacks */
		//雙眼
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(0 / 255.0, 0 / 225.0, 0 / 255.0);
		glScalef(1, 1, 1);
		glTranslatef(1.75, -9.0, -3.5);
		if (sphere == NULL)
			sphere = gluNewQuadric();
		gluSphere(sphere,	0.75,   /* radius=0.5 */
							12,            /* composing of 12 slices*/
							12);           /* composing of 12 stacks */
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(0 / 255.0, 0 / 225.0, 0 / 255.0);
		glScalef(1, 1, 1);
		glTranslatef(-3.5, 0, 0);
		if (sphere == NULL)
			sphere = gluNewQuadric();
		gluSphere(sphere,	0.75,   /* radius=0.5 */
							12,            /* composing of 12 slices*/
							12);           /* composing of 12 stacks */
		
		glPopMatrix(); //回肚子
		glPopMatrix(); //return
	}
}	robot;

/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
	robot.init();
	glClearColor(23 / 255.0, 17 / 255.0, 30 / 255.0, 1.0);      /*set the background color BLACK */
                   /*Clear the Depth & Color Buffers */
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, 499, 499);

	/*-----Set a parallel projection mode-----*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-8.0, 8.0 , -8.0, 8.0, 0.0, 20.0);
  
	glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */
   
	glFlush();/*Enforce window system display the results*/
}

/*--------------------------------------------------------
 * Procedure to draw the cube. The geometrical data of the cube
 * are defined above.
 */
void draw_cube()
{
  int    i;

  glPolygonMode(GL_FRONT, GL_FILL);
  for(i=0;i<6;i++){     /* draw the six faces one by one */
    glColor3fv(color[i]);  /* Set the current color */

    glBegin(GL_POLYGON);  /* Draw the face */
      glVertex3fv(points[face[i][0]]);
      glVertex3fv(points[face[i][1]]);
      glVertex3fv(points[face[i][2]]);
      glVertex3fv(points[face[i][3]]);
    glEnd();
  }
}

/*------------------------------------------------------------
 * Procedure to draw a floor of black-and-white tiles.
  */
float wood[4][6][3] = { {	{ 30 / 255.0,  10 / 255.0,  10 / 255.0},
							{ 50 / 255.0,  15 / 255.0,  15 / 255.0},
							{ 70 / 255.0,  25 / 255.0,  25 / 255.0},
							{ 80 / 255.0,  30 / 255.0,  30 / 255.0},
							{ 90 / 255.0,  40 / 255.0,  40 / 255.0},
							{ 95 / 255.0,  40 / 255.0,  40 / 255.0} },
						{	{ 80 / 255.0,  20 / 255.0,  20 / 255.0},
							{135 / 255.0,  40 / 255.0,  40 / 255.0},
							{160 / 255.0,  80 / 255.0,  75 / 255.0},
							{180 / 255.0, 110 / 255.0, 105 / 255.0},
							{190 / 255.0, 130 / 255.0, 115 / 255.0},
							{210 / 255.0, 150 / 255.0, 130 / 255.0} },
						{	{ 95 / 255.0,  20 / 255.0,  20 / 255.0},
							{140 / 255.0,  50 / 255.0,  50 / 255.0},
							{170 / 255.0,  80 / 255.0,  70 / 255.0},
							{190 / 255.0, 110 / 255.0, 100 / 255.0},
							{210 / 255.0, 160 / 255.0, 140 / 255.0},
							{210 / 255.0, 160 / 255.0, 140 / 255.0} },
						{	{140 / 255.0,  50 / 255.0,  50 / 255.0},
							{160 / 255.0,  60 / 255.0,  55 / 255.0},
							{170 / 255.0,  70 / 255.0,  55 / 255.0},
							{180 / 255.0,  90 / 255.0,  70 / 255.0},
							{185 / 255.0,  95 / 255.0,  75 / 255.0},
							{185 / 255.0,  95 / 255.0,  75 / 255.0} } };
int point[3][4] = { {0,20,30,50}, {0,18,32,50}, {0,15,35,50} };
void draw_floor(){//1:歡樂 2:恐怖
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//right (-0 %5) 
	for (int i = 0; i < 25; i += 5) {
		for (int j = 0; j < 3; ++j) {
			polygonMode ?
			glColor3f(wood[j][i / 5][0] * 1 /  2, wood[j][i / 5][1] * 1 /  2, wood[j][i / 5][2] * 1 /  2):
			glColor3f(wood[j][i / 5][0], wood[j][i / 5][1], wood[j][i / 5][2]);
			glBegin(GL_POLYGON);
			glVertex3f(i, 0, point[i/10][j]);
			polygonMode ?
			glColor3f(wood[j][i / 5 + 1][0] * 1 /  2, wood[j][i / 5 + 1][1] * 1 /  2, wood[j][i / 5 + 1][2] * 1 /  2):
			glColor3f(wood[j][i / 5 + 1][0], wood[j][i / 5 + 1][1], wood[j][i / 5 + 1][2]);
			glVertex3f(i + 4.95, 0, point[i / 10][j]);
			polygonMode ?
			glColor3f(wood[j + 1][i / 5 + 1][0] * 1 /  2, wood[j + 1][i / 5 + 1][1] * 1 /  2, wood[j + 1][i / 5 + 1][2] * 1 /  2):
			glColor3f(wood[j + 1][i / 5 + 1][0], wood[j + 1][i / 5 + 1][1], wood[j + 1][i / 5 + 1][2]);
			glVertex3f(i + 4.95, 0, point[i / 10][j + 1]);
			polygonMode ?
			glColor3f(wood[j + 1][i / 5][0] * 1 /  2, wood[j + 1][i / 5][1] * 1 /  2, wood[j + 1][i / 5][2] * 1 /  2):
			glColor3f(wood[j + 1][i / 5][0], wood[j + 1][i / 5][1], wood[j + 1][i / 5][2]);
			glVertex3f(i, 0, point[i / 10][j + 1]);
			glEnd();
		}
	}
	//left (-50 %5)
	for (int i = 30; i <= 50; i += 5) {
		for (int j = 0; j < 3; ++j) {
			polygonMode ?
			glColor3f(wood[j][(50 - i) / 5][0] * 1 /  2, wood[j][(50 - i) / 5][1] * 1 /  2, wood[j][(50 - i) / 5][2] * 1 /  2):
			glColor3f(wood[j][(50 - i) / 5][0], wood[j][(50 - i) / 5][1], wood[j][(50 - i) / 5][2]);
			glBegin(GL_POLYGON);
			glVertex3f(i, 0, point[(50 - i) / 10][j]);
			polygonMode ?
			glColor3f(wood[j][(50 - i) / 5 + 1][0] * 1 /  2, wood[j][(50 - i) / 5 + 1][1] * 1 /  2, wood[j][(50 - i) / 5 + 1][2] * 1 /  2):
			glColor3f(wood[j][(50 - i) / 5 + 1][0], wood[j][(50 - i) / 5 + 1][1], wood[j][(50 - i) / 5 + 1][2]);
			glVertex3f(i - 4.95, 0, point[(50 - i) / 10][j]);
			polygonMode ?
			glColor3f(wood[j + 1][(50 - i) / 5 + 1][0] * 1 /  2, wood[j + 1][(50 - i) / 5 + 1][1] * 1 /  2, wood[j + 1][(50 - i) / 5 + 1][2] * 1 /  2):
			glColor3f(wood[j + 1][(50 - i) / 5 + 1][0], wood[j + 1][(50 - i) / 5 + 1][1], wood[j + 1][(50 - i) / 5 + 1][2]);
			glVertex3f(i - 4.95, 0, point[(50 - i) / 10][j + 1]);
			polygonMode ?
			glColor3f(wood[j + 1][(50 - i) / 5][0] * 1 /  2, wood[j + 1][(50 - i) / 5][1] * 1 /  2, wood[j + 1][(50 - i) / 5][2] * 1 /  2):
			glColor3f(wood[j + 1][(50 - i) / 5][0], wood[j + 1][(50 - i) / 5][1], wood[j + 1][(50 - i) / 5][2]);
			glVertex3f(i, 0, point[(50 - i) / 10][j + 1]);
			glEnd();
		}
	}

	for (int i = 5; i < 50; i += 5) {
		polygonMode ?
			glColor3f(68 / 255.0 * 1 /  2, 44 / 255.0 * 1 /  2, 40 / 255.0 * 1 /  2):
			glColor3f(68 / 255.0, 44 / 255.0, 40 / 255.0);
		glBegin(GL_LINES);
		glVertex3f(i, 0.1, 0);
		glVertex3f(i, 0.1, 50);
		glEnd();
	}
}

/*------------------------------------------------------------------
 * Procedure to draw a cylinder.
 */
void draw_cylinder(void)
{
   if(cylind==NULL){
    cylind = gluNewQuadric();
   }
   /*--- Draw a cylinder ---*/

    gluCylinder(cylind, 0.3, 0.3, /* radius of top and bottom circle */
						4.0,              /* height of the cylinder */
						12,               /* use 12-side polygon approximating circle*/
	                    4);               /* Divide it into 3 sections */

}

/*------------------------------------------------------------------
 * Procedure to draw an arrow .
 */
void draw_arrow(void)
{
   if(cylind==NULL){
    cylind = gluNewQuadric();
   }
   /*--- Draw a cylinder ---*/

    gluCylinder(cylind, 0.6, 0.0, /* radius of top and bottom circle */
						1.0,              /* height of the cylinder */
						12,               /* use 12-side polygon approximating circle*/
	                    4);               /* Divide it into 3 sections */

}

/*----------------------------------------------------------------------
 * Procedure to draw a coordinate system, RGB bars = XYZ axes
 * White ball = origin.
 */
void draw_coord_sys(void)
{
	//draw coord. sys. in line or fill mode.
	if(polygonMode==NORMAL)
	  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Draw a sphere to show the origin
     if(sphere==NULL){
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
     draw_cylinder();
	 glTranslatef(0, 0, 3.5);
	 draw_arrow();
     glPopMatrix();
	 //Draw the Y-axis in GREEN.
	 glColor3f(0.0, 1.0, 0.0);
     glPushMatrix();  //The y-axis
     glRotatef(-90.0, 1, 0, 0);   
     draw_cylinder();
	 glTranslatef(0, 0, 3.5);
	 draw_arrow();
     glPopMatrix();	 
	 //Draw the z-axis in BLUE.
     glColor3f(0.0, 0.0, 1.0);
	 glPushMatrix();
     draw_cylinder();   //The z-axis
	 glTranslatef(0, 0, 3.5);
	 draw_arrow();
	 glPopMatrix();
}

//wall
void draw_bg() {
	polygonMode ?
	glColor3f(110 / 255.0, 13 / 255.0, 32 / 255.0) :
	glColor3f(199 / 255.0, 143 / 255.0, 152 / 255.0);
	glBegin(GL_POLYGON);
	glVertex3f(50, 0, -0.05);
	polygonMode ?
	glColor3f(122 / 255.0, 15 / 255.0, 36 / 255.0) :
	glColor3f(243 / 255.0, 175 / 255.0, 185 / 255.0);
	glVertex3f(50, 50, -0.05);
	polygonMode ?
	glColor3f(110 / 255.0, 13 / 255.0, 32 / 255.0) :
	glColor3f(199 / 255.0, 143 / 255.0, 152 / 255.0);
	glVertex3f(0, 50, -0.05);
	polygonMode ?
	glColor3f(99 / 255.0, 12 / 255.0, 29 / 255.0) :
	glColor3f(158 / 255.0, 144 / 255.0, 120 / 255.0);
	glVertex3f(0, 0, -0.05);
	glEnd();
	polygonMode ?
	glColor3f(99 / 255.0, 12 / 255.0, 29 / 255.0) :
	glColor3f(158 / 255.0, 144 / 255.0, 120 / 255.0);
	glBegin(GL_POLYGON);
	glVertex3f(-0.05, 0, 0);
	polygonMode ?
	glColor3f(110 / 255.0, 13 / 255.0, 32 / 255.0) :
	glColor3f(199 / 255.0, 143 / 255.0, 152 / 255.0);
	glVertex3f(-0.05, 50, 0);
	polygonMode ?
	glColor3f(122 / 255.0, 15 / 255.0, 36 / 255.0) :
	glColor3f(243 / 255.0, 175 / 255.0, 185 / 255.0);
	glVertex3f(-0.05, 50, 50);
	polygonMode ?
	glColor3f(110 / 255.0, 13 / 255.0, 32 / 255.0) :
	glColor3f(199 / 255.0, 143 / 255.0, 152 / 255.0);
	glVertex3f(-0.05, 0, 50);
	glEnd();
	polygonMode ?
	glColor3f(56 / 255.0, 7 / 255.0, 17 / 255.0):
	glColor3f(128 / 255.0, 92 / 255.0, 97 / 255.0);
	glBegin(GL_LINES);
	//glLineWidth(20);
	glVertex3f(0.05, 0, 0.05);
	glVertex3f(0.05, 20, 0.05);
	polygonMode ?
	glColor3f(74 / 255.0, 9 / 255.0, 22 / 255.0) :
	glColor3f(133 / 255.0, 95 / 255.0, 101 / 255.0);
	glVertex3f(0.05, 20, 0.05);
	glVertex3f(0.05, 35, 0.05);
	polygonMode ?
	glColor3f(89 / 255.0, 11 / 255.0, 26 / 255.0) :
	glColor3f(143 / 255.0, 103 / 255.0, 109 / 255.0);
	glVertex3f(0.05, 35, 0.05);
	glVertex3f(0.05, 50, 0.05);
	glEnd();

}

void draw_robot() {robot.draw();}

void draw_handlebar() { handlebar.draw(); }

/*-------------------------------------------------------
 * Display and Idle callback func. This func. draws three
 * cubes at proper places to simulate a solar system.
 */
void display()
{
  /*Clear previous frame and the depth buffer */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  /*----Define the current eye position and the eye-coordinate system---*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //右上前
  gluLookAt(45.0, 30.0, 75.0, 10.0, 10.0, -10.0, 0.0, 10.0, 0.0);

  /*-------Draw the floor & coordinate system----*/
//  draw_coord_sys();
  draw_bg();
  draw_floor();

  glPushMatrix();
  glTranslatef(25, 0, 25);//暫時鎖死
  glScalef(0.8, 0.8, 0.8);
  draw_handlebar();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(pos[0] + 7,pos[1], pos[2] + 7);
  glScalef(0.7, 0.7, 0.7);
  glRotatef(angle, 0, 1, 0);
  draw_robot();
  glPopMatrix();

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

/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-40, 40, -40, 40, 0, 120);
	width = w; height = h;
}
struct Point { double x, y; };

bool cross(Point a, Point b, Point c, Point d) {
	if (!(min(a.x, b.x) <= max(c.x, d.x)
		&& min(c.y, d.y) <= max(a.y, b.y)
		&& min(c.x, d.x) <= max(a.x, b.x)
		&& min(a.y, b.y) <= max(c.y, d.y)))
		return 0;
	double u, v, w, z;
	u = (c.x - a.x) * (b.y - a.y) - (b.x - a.x) * (c.y - a.y);
	v = (d.x - a.x) * (b.y - a.y) - (b.x - a.x) * (d.y - a.y);
	w = (a.x - c.x) * (d.y - c.y) - (d.x - c.x) * (a.y - c.y);
	z = (b.x - c.x) * (d.y - c.y) - (d.x - c.x) * (b.y - c.y);
	return u * v < -(1e-6) && w * z < -(1e-6);
}

bool check, check2;
int cnt;
unsigned char prekey = '\n';
void timerFunc(int Cs) {
	double dy = cosl(angle * acos(-1) / 180.0);
	double dx = sinl(angle * acos(-1) / 180.0);
	if (Cs == RIS) return;
	if (Cs == LWLK) {
		robot.act.inc = (robot.act.inc + 1) % 4; //right:left
		dy = cosl((angle + 90.0) * acos(-1) / 180.0);
		dx = sinl((angle + 90.0) * acos(-1) / 180.0);
		if (robot.act.inc & 1) { dx *= 2; dy *= 2; }
		if (pos[0] + dx < 0.0 || pos[0] + dx > 38.0) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		if (pos[2] + dy < 0.0 || pos[2] + dy > 38.0) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		if (cross({ pos[0] * 1.5 + 7,pos[2] * 1.5 + 7 }, { pos[0] + dx * 1.5 + 7, pos[2] + dy * 1.5 + 7 }, { 25, 15 }, { 25, 35 })) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		if (cross({ pos[0] * 1.5 + 7,pos[2] * 1.5 + 7 }, { pos[0] + dx * 1.5 + 7, pos[2] + dy * 1.5 + 7 }, { 20, 15 }, { 30, 15 })) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		if (cross({ pos[0] * 1.5 + 7,pos[2] * 1.5 + 7 }, { pos[0] + dx * 1.5 + 7, pos[2] + dy * 1.5 + 7 }, { 20, 35 }, { 30, 35 })) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		pos[0] = min(max(pos[0] + dx, 0.0), 38.0);
		pos[2] = min(max(pos[2] + dy, 0.0), 38.0);
		robot.lwalk();
		display();
		cout << pos[0] << " " << pos[2] << "\n";
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (Cs == LWALK) {
		dy = cosl((angle + 90.0) * acos(-1) / 180.0);
		dx = sinl((angle + 90.0) * acos(-1) / 180.0);
		if (pos[0] + dx * 3 < 0.0 || pos[0] + dx * 3 + 9 > 38.0) check = false;
		if (pos[2] + dy * 3 < 0.0 || pos[2] + dy * 3 > 38.0) check = false;
		if (cross({ pos[0] * 4 + 7,pos[2] * 4 + 7 }, { pos[0] + dx * 4 + 7, pos[2] + dy * 4 + 7 }, { 25, 15 }, { 25, 35 })) check = false;
		if (cross({ pos[0] * 4 + 7,pos[2] * 4 + 7 }, { pos[0] + dx * 4 + 7, pos[2] + dy * 4 + 7 }, { 20, 15 }, { 30, 15 })) check = false;
		if (cross({ pos[0] * 4 + 7,pos[2] * 4 + 7 }, { pos[0] + dx * 4 + 7, pos[2] + dy * 4 + 7 }, { 20, 35 }, { 30, 35 })) check = false;
		if (check) check = false;
		else if (robot.act.action == LWALK) {
			robot.act.inc = -1;
			robot.act.action = RIS;
			robot.lwalk(); prekey = '\n';
			display();
		}
		else return;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (Cs == LWALK_JUMP) {
		if (check == false) {
			if ((robot.act.inc + 1) % 4) {
				robot.act.inc = (robot.act.inc + 1) % 4;
				robot.lwalk();
				display();
				glutTimerFunc(150, timerFunc, robot.act.action);
			}
			else check = true;
			return;
		}
		if (cnt == 10) {
			robot.act.inc = -1;
			robot.lwalk();
			display();
			robot.act.action = RIS;
			glutTimerFunc(150, timerFunc, robot.act.action);
		}
		else{
			robot.lwalk_jump(cnt++);
			display();
			glutTimerFunc(100, timerFunc, robot.act.action);
		}
		return;
	}
	if (Cs == RWLK) {
		dy = cosl((angle + 270.0) * acos(-1) / 180.0);
		dx = sinl((angle + 270.0) * acos(-1) / 180.0);
		robot.act.inc = (robot.act.inc + 1) % 4; //right:left
		if (robot.act.inc & 1) { dx *= 2; dy *= 2; }
		if (pos[0] + dx < 0.0 || pos[0] + dx > 38.0) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		if (pos[2] + dy < 0.0 || pos[2] + dy > 38.0) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		if (cross({ pos[0] * 1.5 + 7,pos[2] * 1.5 + 7 }, { pos[0] + dx * 1.5 + 7, pos[2] + dy * 1.5 + 7 }, { 25, 15 }, { 25, 35 })) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		if (cross({ pos[0] * 1.5 + 7,pos[2] * 1.5 + 7 }, { pos[0] + dx * 1.5 + 7, pos[2] + dy * 1.5 + 7 }, { 20, 15 }, { 30, 15 })) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		if (cross({ pos[0] * 1.5 + 7,pos[2] * 1.5 + 7 }, { pos[0] + dx * 1.5 + 7, pos[2] + dy * 1.5 + 7 }, { 20, 35 }, { 30, 35 })) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		pos[0] = min(max(pos[0] + dx, 0.0), 38.0);
		pos[2] = min(max(pos[2] + dy, 0.0), 38.0);
		robot.rwalk();
		display();
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (Cs == RWALK) {
		dy = cosl((angle + 270.0) * acos(-1) / 180.0);
		dx = sinl((angle + 270.0) * acos(-1) / 180.0);
		if (pos[0] + dx * 3 - 9 < 0.0 || pos[0] + dx * 3 > 38.0) check = false;
		else if (pos[2] + dy * 3 < 0.0 || pos[2] + dy * 3 > 38.0) check = false;
		if (cross({ pos[0] * 4 + 7,pos[2] * 4 + 7 }, { pos[0] + dx * 4 + 7, pos[2] + dy * 4 + 7 }, { 25, 15 }, { 25, 35 })) check = false;
		if (cross({ pos[0] * 4 + 7,pos[2] * 4 + 7 }, { pos[0] + dx * 4 + 7, pos[2] + dy * 4 + 7 }, { 20, 15 }, { 30, 15 })) check = false;
		if (cross({ pos[0] * 4 + 7,pos[2] * 4 + 7 }, { pos[0] + dx * 4 + 7, pos[2] + dy * 4 + 7 }, { 20, 35 }, { 30, 35 })) check = false;
		if (check) check = false;
		else if (robot.act.action == LWALK) {
			robot.act.inc = -1;
			robot.act.action = RIS;
			robot.lwalk(); prekey = '\n';
			display();
		}
		else return;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (Cs == RWALK_JUMP) {
		if (check == false) {
			if ((robot.act.inc + 1) % 4) {
				robot.act.inc = (robot.act.inc + 1) % 4;
				robot.rwalk();
				display();
				glutTimerFunc(150, timerFunc, robot.act.action);
			}
			else check = true;
			return;
		}
		if (cnt == 10) {
			robot.act.inc = -1;
			robot.rwalk();
			display();
			robot.act.action = RIS;
			glutTimerFunc(150, timerFunc, robot.act.action);
		}
		else {
			robot.lwalk_jump(cnt++);
			display();
			glutTimerFunc(100, timerFunc, robot.act.action);
		}
		return;
	}
	if (Cs == TURN) {
		//0:right_lift, 1:left_lift 4:right_down 5:left_down else:stand_by
		if (cnt == 15) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			cnt = 0;
		}
		if (robot.act.fir < 2) robot.act.fir += 2;
		else {
			if (cnt < 3) angle += (robot.act.idiomatic ? -40 : 40);
			else if (cnt < 9) angle += (robot.act.idiomatic ? -25 : 25);
			else angle += (robot.act.idiomatic ? -15 : 15);
			cnt++;
		}
		robot.turn();
		display();
		glutTimerFunc(cnt ? 50 : 150, timerFunc, robot.act.action);
	}
	if (Cs == TUN) {
		if (check) check = false;
		else if (robot.act.action == TUN) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
			robot.turn(); prekey = '\n';
			display();
		}
		else return;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (Cs == TUN_JUMP) {
		if (check == false) {
			if (cnt == 15) {
				robot.act.fir = (robot.act.idiomatic & 1);
				cnt = 0; check = true;
			}
			if (robot.act.fir < 2) robot.act.fir += 2;
			else {
				if (cnt < 3) angle += (robot.act.idiomatic ? -40 : 40);
				else if (cnt < 9) angle += (robot.act.idiomatic ? -25 : 25);
				else angle += (robot.act.idiomatic ? -15 : 15);
				cnt++;
			}
			robot.turn();
			display();
			glutTimerFunc(cnt ? 50 : 150, timerFunc, robot.act.action);
			return;
		}
		else {
			if (cnt == 12) {
				robot.act.fir = (robot.act.idiomatic & 1);
				robot.turn();
				display();
				cnt++;
				glutTimerFunc(120, timerFunc, robot.act.action);
				return;
			}
			if (robot.act.fir < 2) robot.act.fir += 2;
			else if (cnt != 13) {
				if (cnt < 6) angle += (robot.act.idiomatic ? -40 : 40);
				else angle += (robot.act.idiomatic ? -20 : 20);
				cnt++;
			}
			else{
				robot.act.fir = -2 + (robot.act.idiomatic & 1);
				robot.act.action = RIS;
				cnt = 0;
			}
			robot.tun_jump(cnt);
			display();
			glutTimerFunc(cnt ? (cnt > 6? 50 : 25) : 120, timerFunc, robot.act.action);
			return;
		}
	}
	if (Cs == ROT) {
		//0:right_lift, 1:left_lift 4:right_down 5:left_down else:stand_by
		if (cnt == 25) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			cnt = 0;
		}
		if (robot.act.fir + 2 < 4) robot.act.fir += 2;
		else {
			if (!(cnt % 6)) robot.act.fir += 2;
			cnt++;
			angle += (robot.act.idiomatic ? -15 : 15);
		}
		robot.rot();
		display();
		if (robot.act.action == 0) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.rot();
			display();
		}
		glutTimerFunc(200, timerFunc, robot.act.action);
	}
	if (Cs == JMP) {
		robot.act.inc = (robot.act.inc + 1); //right:left
		if (robot.act.inc == 7) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		robot.jmp(); display();
		if (robot.act.inc == 3) glutTimerFunc(100, timerFunc, robot.act.action);
		else if (abs(robot.act.inc - 3) & 1) glutTimerFunc(120, timerFunc, robot.act.action);
		else glutTimerFunc(150, timerFunc, robot.act.action);
	}
	if (Cs == RUN) {
		//0:right_lift, 1:left_lift 4:right_down 5:left_down else:stand_by
		robot.act.fir = ((robot.act.fir >= 2) + robot.act.fir + 2) % 4; //right:left
		if (pos[0] + dx < 0.0 || pos[0] + dx > 38.0) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (pos[2] + dy < 0.0 || pos[2] + dy > 38.0) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (cross({ pos[0] * 3 + 7,pos[2] * 3 + 7 }, { pos[0] + dx * 3 + 7, pos[2] + dy * 3 + 7 }, { 25, 15 }, { 25, 35 })) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (cross({ pos[0] * 3 + 7,pos[2] * 3 + 7 }, { pos[0] + dx * 3 + 7, pos[2] + dy * 3 + 7 }, { 20, 15 }, { 30, 15 })) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (cross({ pos[0] * 3 + 7,pos[2] * 3 + 7 }, { pos[0] + dx * 3 + 7, pos[2] + dy * 3 + 7 }, { 20, 35 }, { 30, 35 })) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (robot.act.fir == 2 || robot.act.fir == 3) {
			pos[0] = min(max(pos[0] + dx, 0.0), 38.0);
			pos[2] = min(max(pos[2] + dy, 0.0), 38.0);
		}
		robot.run();
		display();
		glutTimerFunc(150, timerFunc, robot.act.action);
	}
	if (Cs == LEAP) {
		if (pos[0] + dx * 5 < 0.0 || pos[0] + dx * 5 > 38.0) check = false;
		if (pos[2] + dy * 5  < 0.0 || pos[2] + dy * 5 > 38.0) check = false;
		if (cross({ pos[0] * 6 + 7,pos[2] * 6 + 7 }, { pos[0] + dx * 6 + 7, pos[2] + dy * 6 + 7 }, { 25, 15 }, { 25, 35 })) check = false;
		if (cross({ pos[0] * 6 + 7,pos[2] * 6 + 7 }, { pos[0] + dx * 6 + 7, pos[2] + dy * 6 + 7 }, { 20, 15 }, { 30, 15 })) check = false;
		if (cross({ pos[0] * 6 + 7,pos[2] * 6 + 7 }, { pos[0] + dx * 6 + 7, pos[2] + dy * 6 + 7 }, { 20, 35 }, { 30, 35 })) check = false;
		if (check) check = false; 
		else if(robot.act.action == LEAP){
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
			robot.run(); prekey = '\n';
			display();
		}
		else return;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (Cs == LEAP_JUMP) {
		robot.act.fir = (robot.act.fir + 2); //right:left
		if (robot.act.fir >= 14) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		else if (robot.act.fir >= 2) {
			pos[0] = min(max(pos[0] + dx, 0.0), 38.0);
			pos[2] = min(max(pos[2] + dy, 0.0), 38.0);
		}
		if (robot.act.fir >= 0 &&robot.act.fir <= 3) robot.run();
		else robot.leap_jump();
		display();
		if (abs(robot.act.fir / 2 * 2 - 8) == 2) glutTimerFunc(120, timerFunc, robot.act.action);
		else if (robot.act.fir / 2 == 4) glutTimerFunc(100, timerFunc, robot.act.action);
		else glutTimerFunc(150, timerFunc, robot.act.action);
	}
	if (Cs == WALK) {
		dx *= 2; dy *= 2;
		robot.act.fir = ((robot.act.fir >= 4) + robot.act.fir + 2) % 6; //right:left
		cout << robot.act.fir << "\n";
		//0:right_lift, 1:left_lift 4:right_down 5:left_down else:stand_by
		if (pos[0] + dx < 0.0 || pos[0] + dx > 38.0) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (pos[2] + dy < 0.0 || pos[2] + dy > 38.0) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (cross({ pos[0] * 3 + 7,pos[2] * 3 + 7 }, { pos[0] + dx * 3 + 7, pos[2] + dy * 3 + 7 }, { 25, 15 }, { 25, 35 })) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (cross({ pos[0] * 3 + 7,pos[2] * 3 + 7 }, { pos[0] + dx * 3 + 7, pos[2] + dy * 3 + 7 }, { 20, 15 }, { 30, 15 })) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (cross({ pos[0] * 3 + 7,pos[2] * 3 + 7 }, { pos[0] + dx * 3 + 7, pos[2] + dy * 3 + 7 }, { 20, 35 }, { 30, 35 })) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (robot.act.fir == 0 || robot.act.fir == 1 || robot.act.fir == 4 || robot.act.fir == 5) {
			pos[0] = min(max(pos[0] + dx, 0.0), 38.0);
			pos[2] = min(max(pos[2] + dy, 0.0), 38.0);
		}
		robot.walk();
		display();
		glutTimerFunc(300, timerFunc, robot.act.action);

	}
	if (Cs == LIFT) {
		if (pos[0] + dx * 2 < 0.0 || pos[0] + dx * 2 > 38.0) check = false;
		else if (pos[2] + dy * 2 < 0.0 || pos[2] + dy * 2 > 38.0) check = false;
		if (cross({ pos[0] * 1.5 + 7,pos[2] * 3 + 7 }, { pos[0] + dx * 1.5 + 7, pos[2] + dy * 1.5 + 7 }, { 25, 15 }, { 25, 35 })) check = false;
		if (cross({ pos[0] * 1.5 + 7,pos[2] * 1.5 + 7 }, { pos[0] + dx * 1.5 + 7, pos[2] + dy * 1.5 + 7 }, { 20, 15 }, { 30, 15 })) check = false;
		if (cross({ pos[0] * 1.5 + 7,pos[2] * 1.5 + 7 }, { pos[0] + dx * 1.5 + 7, pos[2] + dy * 1.5 + 7 }, { 20, 35 }, { 30, 35 })) check = false;
		if (check) check = false;
		else if (robot.act.action == LIFT) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
			robot.walk(); prekey = '\n';
			display();
		}
		else return;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (Cs == LIFT_JUMP) {
		if (check2 == true) {
			robot.act.fir = (robot.act.fir + 2); //right:left
			if (robot.act.fir >= 8) {
				robot.act.fir = -2 + (robot.act.idiomatic & 1);
				robot.act.action = RIS;
			}
			robot.lift_jump();
			display();
			if(robot.act.fir < 0) glutTimerFunc(280, timerFunc, robot.act.action);
			else if(robot.act.fir / 2 == 2) glutTimerFunc(250, timerFunc, robot.act.action);
			else glutTimerFunc(300, timerFunc, robot.act.action);
		}
		else {
			robot.act.fir = ((robot.act.fir >= 4) + robot.act.fir + 2) % 6; //right:left
			if (robot.act.fir != 0 && robot.act.fir != 1){
				if (robot.act.fir == 0 || robot.act.fir == 1 || robot.act.fir == 4 || robot.act.fir == 5) {
					pos[0] = min(max(pos[0] + dx, 0.0), 38.0);
					pos[2] = min(max(pos[2] + dy, 0.0), 38.0);
				}
				robot.walk();
				display();
				glutTimerFunc(300, timerFunc, robot.act.action);
			}
			else check2 = true;
		}
	}
}

/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
void my_act(unsigned char key, int x, int y){
	if (key == 'Q' || key == 'q') exit(0);
	if ((key == 'D' || key == 'd') && !robot.act.action) {
		robot.act.action = RWLK;
		glutTimerFunc(150, timerFunc, robot.act.action);
		//旁飛燕
		//else if(prekey == ' ' && (key == 'w' || key == 'W'))
	}
	if ((key == 'A' || key == 'a') && !robot.act.action) {
		robot.act.action = LWLK;
		glutTimerFunc(150, timerFunc, robot.act.action);
	  //旁飛燕
	  //else if(prekey == ' ' && (key == 'w' || key == 'W'))
	}
	if (key == 'W' || key == 'w') {
		if (key == 'W' && !robot.act.action) {
			robot.act.action = RUN;
			glutTimerFunc(150, timerFunc, robot.act.action);
		}
		else if (key == 'w' && !robot.act.action) {
			robot.act.action = WALK;
			glutTimerFunc(150, timerFunc, robot.act.action);
		}
		//前飛燕
		//else if(prekey == ' ' && (key == 'w' || key == 'W'))
	}
	if (key == 'S' || key == 's') {
		double dy = cosl((angle + 180) * acos(-1) / 180.0) * 5.0;
		double dx = sinl((angle + 180) * acos(-1) / 180.0) * 5.0;
		cout << pos[0] << " " << pos[2] << " " << dx << " " << dy << "\n";
		if (pos[0] + dx < 0.0 || pos[0] + dx > 38.0) return;
		if (pos[2] + dy < 0.0 || pos[2] + dy > 38.0) return;
		if (cross({ pos[0] * 1.5 + 7,pos[2] * 1.5 + 7 }, { pos[0] + dx * 1.5 + 7, pos[2] + dy * 1.5 + 7}, {25, 15}, {25, 35})) return;
		if (cross({ pos[0] * 1.5 + 7,pos[2] * 1.5 + 7 }, { pos[0] + dx * 1.5 + 7, pos[2] + dy * 1.5 + 7 }, { 20, 15 }, { 30, 15 })) return;
		if (cross({ pos[0] * 1.5 + 7,pos[2] * 1.5 + 7 }, { pos[0] + dx * 1.5 + 7, pos[2] + dy * 1.5 + 7 }, { 20, 35 }, { 30, 35 })) return;
	  pos[0] = min(max(pos[0] + dx, 0.0), 38.0);
	  pos[2] = min(max(pos[2] + dy, 0.0), 38.0);

	  //後飛燕
	  //else if(prekey == ' ' && (key == 'w' || key == 'W'))
  }
	if ((key == 'r' || key == 'R') && !robot.act.action) {
		robot.act.action = TURN;
		glutTimerFunc(150, timerFunc, robot.act.action);
	}
	if (key == 'p' || key == 'P') {
		robot.act.idiomatic = (robot.act.idiomatic + 1) & 1;
		robot.act.fir = -2 + (robot.act.idiomatic & 1);
	}
	if ( (key == 'o' || key == 'O') && !robot.act.action) {
		robot.act.action = ROT; cnt = 0;
		glutTimerFunc(200, timerFunc, robot.act.action);
	}
	if (key == '1' && polygonMode == NORMAL) polygonMode = GLOOMY;
	if ( key == '2' && polygonMode == GLOOMY) polygonMode = NORMAL;
	if (key == ' ' && robot.act.action <= 0) {
		if (prekey == 'W') {
			prekey = '\n';
			robot.act.action = LEAP_JUMP;
			glutTimerFunc(100, timerFunc, robot.act.action);
		}
		else if (prekey == 'w') {
			prekey = '\n';
			robot.act.action = LIFT_JUMP; check2 = false;
			glutTimerFunc(100, timerFunc, robot.act.action);
		}
		else if (prekey == 'r') {
			prekey = '\n';
			robot.act.action = TUN_JUMP; check2 = false;
			glutTimerFunc(100, timerFunc, robot.act.action);
		}
		else if (prekey == 'd') {
			robot.act.action = RWALK_JUMP;
			glutTimerFunc(150, timerFunc, robot.act.action);
		}
		else if (prekey == 'a') {
			robot.act.action = LWALK_JUMP;
			glutTimerFunc(150, timerFunc, robot.act.action);
		}
		else if (prekey == 's') {
			robot.act.action = BWALK_JUMP;
			glutTimerFunc(150, timerFunc, robot.act.action);
		}
		else if (prekey == '\n') {
			robot.act.action = JMP;
			glutTimerFunc(150, timerFunc, robot.act.action);
		}
	}
	display();
}

void my_end(unsigned char key, int x, int y) {
	if (key == 'W') {
		prekey = 'W'; check = true;
		robot.act.action = LEAP;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (key == 'w') {
		prekey = 'w'; check = true;
		robot.act.action = LIFT;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (key == 'r' || key == 'R') {
		prekey = 'r'; check = true;
		robot.act.action = TUN;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (key == 'o') robot.act.action = RIS;
	if (key == 'a' || key == 'A') {
		prekey = 'a'; check = true;
		robot.act.action = LWALK;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (key == 'd' || key == 'D') {
		prekey = 'd'; check = true;
		robot.act.action = RWALK;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (key == 's' || key == 'S') {
		prekey = 's'; check = true;
		robot.act.action = BWALK;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	display(); 
}

//void my_mix(int key, int x, int y) {cout << key << "\n";}

/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char **argv)
{
  /*-----Initialize the GLUT environment-------*/
  glutInit(&argc, argv);

  /*-----Depth buffer is used, be careful !!!----*/
  glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB| GLUT_DEPTH);

  glutInitWindowSize(500, 500);
  glutCreateWindow("cube");

  myinit();      /*---Initialize other state varibales----*/
  
  /*----Associate callback func's whith events------*/
  glutDisplayFunc(display);

  glutReshapeFunc(my_reshape);
  glutKeyboardFunc(my_act);
  glutKeyboardUpFunc(my_end);
  //glutSpecialFunc(my_mix);
  glutTimerFunc(1000, timerFunc, 0);
  glutMainLoop();
}