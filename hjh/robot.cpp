/***************************************************
 * Example program: Draw a trunk.moving cube
 *   Author: S. K. Ueng
 *   National Taiwan Ocean Univ. Comp. Sci. Dept.
 */
#include<bits/stdc++.h>
#include <GL/glut.h>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

#define PI acos(-1)
#define Rad(x) (double)x * PI / 180.0

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

#define DOUBLEDANCE (int)('D')
#define DANCE (int)('O')
#define SUN (int)('S')

float	global_amb[] = { 0.2, 0.2, 0.2, 1.0 };
bool	isopen[8] = { false };
int		opener = 3;

 /* window shape */
int		width = 512, height = 512;

/* view_scope */
float	zoom = 60.0, ratio = (double)width / (double)height, znear = 10.0, zfar = 30.0;
float	pra_zoom = 30.0, pra_znear = 0.0, pra_zfar = 100.0;

int    scene = 0;

/*-----Translation and rotations of eye coordinate system---*/
float   eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
double  Eye[3] = { 0.0, 20.0, 30.0 }, 
		Focus[3] = { 0.0, 20.0, 0.0 },
		Vup[3] = { 0.0, 0.0, 0.0 };

double   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
float   eye[3], eyeMtx[16];
float   cv, sv; /* cos(5.0) and sin(5.0) */

/*-----Drawing stye
 0:4-windows, 1:x direction, 2:y direction, 3:z-dirtection, 4:perspective
 */
int style = 0;

// 函数power_of_two用于判断一个整数是不是2的整数次幂
int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;//这个方法很炫酷啊
}

GLuint textid[30]; 
int doubledance = 4, dance = 20;
int dance_sz[][2] = {   {14, 30}, {22, 29}, {14, 30}, {25, 30},
						{29, 27}, {18, 31}, {21, 28}, {33, 30},
						{21, 28}, {18, 31}, {29, 27}, {24, 28},
						{14, 30}, {22, 29}, {14, 19}, {19, 29} };
struct Texture {
	#define BMP_Header_Length 54  //图像数据在内存块中的偏移量
	/* 函数load_texture
	* 读取一个BMP文件作为纹理
	* 如果失败，返回0，如果成功，返回纹理编号
	*/
	void load_texture(const char* file_name, int id, bool bitmip)
	{
		GLint width, height, total_bytes;
		GLubyte* pixels = 0;

		// 打开文件，如果失败，返回
		FILE* pFile;
		fopen_s(&pFile, file_name, "rb");
		if (pFile == 0) {
			cout << "fail open";
			return;
		}

		// 读取文件中图象的宽度和高度
		fseek(pFile, 0x0012, SEEK_SET);//偏移18字节后是宽高
		fread(&width, 4, 1, pFile);
		fread(&height, 4, 1, pFile);
		fseek(pFile, BMP_Header_Length, SEEK_SET);
		cout << "width=" << width << endl;
		cout << "height=" << height << endl;
		// 计算每行像素所占字节数，并根据此数据计算总像素字节数
		{
			GLint line_bytes = width * 4;
			while (line_bytes % 4 != 0)//要检测图像宽度的位数是否是4的整数倍，如果不是，需要补齐，并且以补齐后的内存带下分配内存。
				++line_bytes;
			total_bytes = line_bytes * height;
		}

		// 根据总像素字节数分配内存
		pixels = (GLubyte*)malloc(total_bytes);
		if (pixels == 0)
		{
			fclose(pFile);
			cout << "fail num";
			return;
		}

		// 读取像素数据
		if (fread(pixels, total_bytes, 1, pFile) <= 0)
		{
			free(pixels);
			fclose(pFile);
			cout << "fail read";
			return;
		}
		// 绑定新的纹理，载入纹理并设置纹理参数
		// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
		glBindTexture(GL_TEXTURE_2D, textid[id]);
		if (bitmip) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);
		}
		free(pixels);
	}


}texture;

struct Billboard {
	float a[3], b[3];
	void init() {
		float  w0, w2;
		double len;
		/*----Get w0 and w2 from the modelview matrix mtx[] ---*/
		w0 = eyeMtx[2]; w2 = eyeMtx[10];
		len = sqrt(w0 * w0 + w2 * w2);
		/*---- Define the a and b axes for billboards ----*/
		b[0] = 0.0; b[1] = 1.0; b[2] = 0.0;
		a[0] = w2 / len; a[1] = 0.0; a[2] = -w0 / len;
	}
	void draw(float x, float z, float w, float h)
	{
		float  v0[3], v1[3], v2[3], v3[3];

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.5);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		/*----Compute the 4 vertices of the billboard ----*/
		v0[0] = x - (w / 2) * a[0]; v0[1] = 0.0; v0[2] = z - (w / 2) * a[2];
		v1[0] = x + (w / 2) * a[0]; v1[1] = 0.0; v1[2] = z + (w / 2) * a[2];
		v2[0] = x + (w / 2) * a[0]; v2[1] = h; v2[2] = z + (w / 2) * a[2];
		v3[0] = x - (w / 2) * a[0]; v3[1] = h; v3[2] = z - (w / 2) * a[2];
		glColor3f(1, 1, 1);
		glNormal3f(0, 0, 1);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex3fv(v0);
		glTexCoord2f(1.0, 1.0); glVertex3fv(v1);
		glTexCoord2f(1.0, 0.0); glVertex3fv(v2);
		glTexCoord2f(0.0, 0.0); glVertex3fv(v3);
		glEnd();
		glDisable(GL_ALPHA_TEST);
	}
}billboard;

struct Polyhedron {	//多面體
	/*-----Define GLU quadric objects, a sphere and a cylinder----*/
	GLUquadricObj *sphere = NULL, *cylind = NULL, *disk = NULL;
	void material(	float sper, float speg, float speb, float spea,		//反射別人的顏色
					float difr, float difg, float difb, float difa,		//我在別人眼中的顏色
					float ambr, float ambg, float ambb, float amba,		//我造成的環境光
					float emir, float emig, float emib, float emia,		//自體發光的顏色
					float shi){											//0(粗糙)-128(光滑)		
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		glColor4f(sper / 255.0, speg / 255.0, speb / 255.0, spea);
		glColorMaterial(GL_FRONT, GL_EMISSION);
		glColor4f(emir / 255.0, emig / 255.0, emib / 255.0, emia);
		glMaterialf(GL_FRONT, GL_SHININESS, shi);
		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glColor4f(ambr / 255.0, ambg / 255.0, ambb / 255.0, amba);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glColor4f(difr / 255.0, difg / 255.0, difb / 255.0, difa);
	}
	void draw_cylind(float top, float bottom, float height){
		gluQuadricTexture(cylind, GL_TRUE);
		gluCylinder(cylind, top, bottom,	/* radius of top and bottom circle */
							height,			/* height of the cylinder */
							12,				/* use 12-side polygon approximating circle*/
							4);				/* Divide it into 3 sections */
	}
	void draw_arrow(float bottom, float height) {
		gluQuadricTexture(cylind, GL_TRUE);
		gluCylinder(cylind, 0.0, bottom,	/* radius of top and bottom circle */
							1.0,            /* height of the cylinder */
							12,             /* use 12-side polygon approximating circle*/
							4);             /* Divide it into 3 sections */
	}
	void draw_sphere(float r) {
		gluQuadricTexture(sphere, GL_TRUE);
		gluSphere(sphere, r,	/* radius */
						  12,	/* composing of 12 slices*/
						  12);	/* composing of 12 stacks */
	}

	/*-----Define a unit box--------*/
	float  points_8[8][3] = {  {-1.0, 0.0, -1.0}, {1.0, 0.0, -1.0},
							{1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0},
							{-1.0, 0.0, 1.0}, {1.0, 0.0, 1.0},
							{1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0} };
	//Define 6 faces using the 8 corners (vertices)
	int    irrpoly_6[6][4] = {  {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5},
								{4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };
	//Define the cube using the 6 faces.
	int    cube_6[6] = { 0, 1, 2, 3, 4, 5 };

	float  normal_6[6][3] = {   {0.0, 0.0, -1.0}, {0.0, -1.0, 0.0}, {1.0, 0.0, 0.0},
								{0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {-1.0, 0.0, 0.0} };
	void draw_hexahedron(float **color, int dir) {	//不規則六面體
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		for (int i = 0; i < 6; ++i) {
			material(color[i][0], color[i][1], color[i][2], color[i][3],		//反射別人的顏色
					 color[i][4], color[i][5], color[i][6], color[i][7],		//我在別人眼中的顏色
					 color[i][8], color[i][9], color[i][10], color[i][11],		//我造成的環境光
					 color[i][12], color[i][13], color[i][14], color[i][15],	//自體發光的顏色
					 color[i][16]);												//0(粗糙)-128(光滑)		
			glNormal3fv(normal_6[i]);
			glBegin(GL_POLYGON);  /* Draw the face */
			glTexCoord2f((dir > 1? 1.0 : 0.0), (dir & 1? 1.0 : 0.0)); glVertex3fv(points_8[irrpoly_6[i][0]]);
			glTexCoord2f((dir > 1 ? 1.0 : 0.0), (dir & 1 ? 0.0 : 1.0)); glVertex3fv(points_8[irrpoly_6[i][1]]);
			glTexCoord2f((dir > 1 ? 0.0 : 1.0), (dir & 1 ? 0.0 : 1.0)); glVertex3fv(points_8[irrpoly_6[i][2]]);
			glTexCoord2f((dir > 1 ? 0.0 : 1.0), (dir & 1 ? 1.0 : 0.0)); glVertex3fv(points_8[irrpoly_6[i][3]]);
			glEnd();
		}
	}
	void draw_cube() {
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glutSolidCube(1);
	}
	void draw_circle(float r) {
		gluDisk(disk,
				0,		//inner radius
				r,		//outer radius
				360,	//360side
				3);
	}
	void draw_disk(float innr, float outr) {
		gluDisk(disk,
				innr,		//inner radius
				outr,		//outer radius
				360,		//360side
				3);
	}
	void init() {
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricNormals(sphere, GLU_SMOOTH);
		cylind = gluNewQuadric();
		gluQuadricDrawStyle(cylind, GLU_FILL);
		gluQuadricNormals(cylind, GLU_SMOOTH);
		disk = gluNewQuadric();
		gluQuadricDrawStyle(disk, GLU_FILL);
		gluQuadricNormals(disk, GLU_SMOOTH);
	}
} polyhedron;

struct Coordinate_System{
	void draw(	float dx, float dy, float dz,
				float sx, float sy, float sz,
				float rx, float ry, float rz) {
		glRotatef(rx, 1.0, 0.0, 0.0);
		glRotatef(ry, 0.0, 1.0, 0.0);
		glRotatef(rz, 0.0, 0.0, 1.0);
		glTranslatef(dx, dy, dz);
		glScalef(sx, sy, sz);
	}
}coo_sys;

struct Robot {
	struct Act {
		int action = 0, fir = -2, idiomatic = 0, inc = -1;
	}act;
	struct Pos {
		float dx = 0, dy = 0, dz = 0;
	}pos;
	float r;
	struct Joint {
		struct Rotate {
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
			polyhedron.material(
				0.0, 0.0, 0.0, 0.0,		//spe
				cr, cg, cb, 1.0,		//mycolor
				cr, cg, cb, 1.0,		//amb
				0.0, 0.0, 0.0, 0.0,		//mylit
				0.3 * 128);				//光滑度
			glPushMatrix();
			glScalef(1, 1, 1);
			glTranslatef(px, py, pz);
			glRotatef(swp.ang, swp.rx, swp.ry, swp.rz);//swap
			glRotatef(ang, rx, ry, rz);//用以控制枝幹方向
			polyhedron.draw_sphere(r);
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
			polyhedron.material(
				40.0, 40.0, 40.0, 0.5,			//spe
				238.0, 202.0, 206.0, 0.9,	//mycolor
				238.0, 202.0, 206.0, 0.9,	//amb
				0.0, 0.0, 0.0, 0.0,			//mylit
				0.4 * 128);						//光滑度
			glPushMatrix();
			glScalef(1, 1, 1);
			glTranslatef(0, 0, 2.5);
			glRotatef(0, 0, 0, 0);
			polyhedron.draw_cylind(cyl.tr - 0.1, cyl.br - 0.1, cyl.h);
			glEnable(GL_TEXTURE_2D);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBindTexture(GL_TEXTURE_2D, textid[1]);
			glPushMatrix();
			polyhedron.draw_cylind(cyl.tr, cyl.br, cyl.h);
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
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
			polyhedron.material(
				0.0, 0.0, 0.0, 0.0,		//spe
				cr, cg, cb, 1.0,	//mycolor
				cr, cg, cb, 1.0,		//amb
				0.0, 0.0, 0.0, 0.0,		//mylit
				0.7);					//光滑度
			glScalef(sx, sy, sz);
			glTranslatef(px, py, pz);
			glRotatef(0, 0, 0, 0);
			polyhedron.draw_cylind(tr, br, h);
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
			robot.hip_l.rot = robot.hip_r.rot = { 0.0, 0.0, 0.0, 0.0 };
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

	void tun_jump(int selfpos) {
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
			robot.belly.mov = { 0, 13.25 + 1.2 * selfpos - 0.1 * selfpos * selfpos, 0 };
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
			robot.belly.mov = { 0, 13.25 + 3.6 * selfpos - 0.3 * selfpos * selfpos, 0 };
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
		robot.shouder_l.swp = { -30.0 - 20 * cnt + 1.67 * cnt * cnt, 1.0, 0.0, 0.0 };
		robot.waist.rot = { 10 * cnt - 0.8 * cnt * cnt, 0, 1, 0 };
		robot.waist.mov = { 0, 0, -0.25 * cnt + 0.042 * cnt * cnt };
		skirt.cyl = { 4.0, 5.0 + 0.3 * cnt - 0.28 * cnt * cnt, 3.5 - 0.84 * cnt + 0.07 * cnt * cnt };
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
		ang = Rad(ang);
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
		//material()
		polyhedron.material(
			40.0, 40.0, 40.0, 0.5,		//spe
			239.0, 222.0, 222.0, 1.0,	//mycolor
			239.0, 222.0, 222.0, 1.0,	//amb
			0.0, 0.0, 0.0, 0.0,			//mylit
			0.4 * 128);					//光滑度
		glPushMatrix();
		glScalef(1, 1, 0.75);
		glTranslatef(belly.mov.dx, belly.mov.dy, belly.mov.dz);//右,前,上
		glRotatef(belly.swp.ang, belly.swp.rx, belly.swp.ry, belly.swp.rz);
		glRotatef(belly.rot.ang, belly.rot.rx, belly.rot.ry, belly.rot.rz);
		polyhedron.draw_cylind(3.0, 4.0, 3.5);
		glEnable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, textid[1]);
		glPushMatrix();
		polyhedron.draw_cylind(3.1, 4.1, 3.5);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		//glPopMatrix();
		

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
		polyhedron.material(
			40.0, 40.0, 40.0, 0.5,		//spe
			239.0, 222.0, 222.0, 1.0,	//mycolor
			239.0, 222.0, 222.0, 1.0,	//amb
			0.0, 0.0, 0.0, 0.0,			//mylit
			0.4 * 128);					//光滑度
		glPushMatrix();
		glScalef(1, 1, 0.75);
		glTranslatef(chest.mov.dx, chest.mov.dy, chest.mov.dz);//右,下,前
		glRotatef(chest.swp.ang, chest.swp.rx, chest.swp.ry, chest.swp.rz);
		glRotatef(chest.rot.ang, chest.rot.rx, chest.rot.ry, chest.rot.rz);
		polyhedron.draw_cylind(3.0, 3.5, 1.5);
		glEnable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, textid[1]);
		glPushMatrix();
		polyhedron.draw_cylind(3.1, 3.6, 1.5);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
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
		knuckle4_r.draw(1.2, knuckle4_r.mov.dx, knuckle4_r.mov.dy, knuckle4_r.mov.dz, knuckle4_r.rot.ang, knuckle4_r.rot.rx, knuckle4_r.rot.ry, knuckle4_r.rot.rz, 237, 225, 211);
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
		polyhedron.material(
			0.0, 0.0, 0.0, 0.0,		//spe
			237, 242, 228, 1.0,	//mycolor
			237, 242, 228, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.7);					//光滑度
		glScalef(0.8, 1, 1);
		glTranslatef(0, 0, 6);
		polyhedron.draw_sphere(5);
		//包包頭
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, textid[2]);
		glPushMatrix();
		glScalef(1, 1, 1);
		glTranslatef(0, 0.5, 0.3);
		polyhedron.draw_sphere(5.2);
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPushMatrix();
		glScalef(1, 1, 1);
		glTranslatef(0, 4.15, 3.25);
		polyhedron.draw_sphere(1.8);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		//雙眼
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		polyhedron.material(
			0.0, 0.0, 0.0, 0.0,		//spe
			0, 0, 0, 1.0,			//mycolor
			0, 0, 0, 1.0,			//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.7);					//光滑度
		glScalef(1, 1, 1);
		glTranslatef(1.75, -3.9, -0.5);
		polyhedron.draw_sphere(0.75);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		polyhedron.material(
			0.0, 0.0, 0.0, 0.0,		//spe
			0, 0, 0, 1.0,	//mycolor
			0, 0, 0, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.7);					//光滑度
		glScalef(1, 1, 1);
		glTranslatef(-3.5, 0, 0);
		polyhedron.draw_sphere(0.75);
		glPopMatrix(); //回肚子
		glPopMatrix(); //return
	}
} robot;			//尚未精簡

struct Stage {
	float **color;
	void init() {
		color = (float **)malloc(sizeof(float *) * 6);
		for (int i = 0; i < 6; ++i)
			color[i] = (float *)malloc(sizeof(float) * 17);
	}
	void change_color(	float spe_r, float spe_g, float spe_b, float spe_a,
						float dif_r, float dif_g, float dif_b, float dif_a,
						float amb_r, float amb_g, float amb_b, float amb_a,
						float emi_r, float emi_g, float emi_b, float emi_a,
						float shi) {
		for (int i = 0; i < 6; ++i) {
			color[i][0] = spe_r; color[i][1] = spe_g; color[i][2] = spe_b; color[i][3] = spe_a;
			color[i][4] = dif_r; color[i][5] = dif_g; color[i][6] = dif_b; color[i][7] = dif_a;
			color[i][8] = amb_r; color[i][9] = amb_g; color[i][10] = amb_b; color[i][11] = amb_a;
			color[i][12] = emi_r; color[i][13] = emi_g; color[i][14] = emi_b; color[i][15] = emi_a;
			color[i][16] = shi;
		}
	}
	void draw() {
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, textid[0]);
		change_color(0.0, 0.0, 0.0, 0.0,//反射別人的顏色
					255.0, 255.0, 255.0, 0.3,	//我在別人眼中的顏色
					255.0, 255.0, 255.0, 0.3,	//我造成的環境光
					0.0, 0.0, 0.0, 1.0,	//自體發光的顏色
					0.0 * 100);					//0(粗糙)-128(光滑)
		for (int i = -70; i <= 70; i += 20)
			for (int j = -40, dir; j <= 40; j += 20) {
				dir = (((i + 70) / 20) & 1) * 2;
				dir += (((j + 40) / 20) & 1);	
				glPushMatrix();
				coo_sys.draw(i, -20, j, 20, 20, 20, 0, 0, 0);	//右上前
				polyhedron.draw_hexahedron(color, dir);
				glPopMatrix();
			}
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, textid[3]);
		change_color(0.0, 0.0, 0.0, 0.0,		//反射別人的顏色
					180.0, 180.0, 180.0, 0.3,	//我在別人眼中的顏色
					180.0, 180.0, 180.0, 0.3,	//我造成的環境光
					0.0, 0.0, 0.0, 1.0,			//自體發光的顏色
					0.0 * 100);					//0(粗糙)-128(光滑)
		glPushMatrix();
		coo_sys.draw(0, -10, 70, 180, 20, 20, 0, 0, 0);	//右上前
		polyhedron.draw_hexahedron(color, 0);
		glPopMatrix();
		glPushMatrix();
		coo_sys.draw(0, -9, 55, 180, 2, 10, 0, 0, 0);	//右上前
		polyhedron.draw_hexahedron(color, 0);
		glPopMatrix();
		glPushMatrix();
		coo_sys.draw(0, 0, 90, 180, 40, 20, 0, 0, 0);	//右上前
		polyhedron.draw_hexahedron(color, 0);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, textid[doubledance]);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		billboard.draw(30, 0, dance_sz[doubledance - 4][0], dance_sz[doubledance - 4][1]);
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, textid[dance]);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		billboard.draw(-20, -25, 30, 30);
		glDisable(GL_TEXTURE_2D);

		glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
		glEnable(GL_TEXTURE_GEN_T);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glAlphaFunc(GL_GREATER, 0.5);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, textid[29]);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glColor3f(1, 1, 1);
		glPushMatrix();
		polyhedron.draw_sphere(80);
		glPopMatrix();
		glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
	}
}stage;

struct BaseLight
{
	float color[4];
	float amb[4],dif[4],spe[4];
	float intensity;

	void init() {
		for (int i = 0; i < 4; ++i) {
			dif[i] = color[i] * intensity;
			spe[i] = color[i] * intensity;
		}
	}
};
struct PointLit{
	BaseLight baselit;
	float pos[4] = { 25, 40, 25, 1.0 };
	
	/*
	衰減參數
	float constant = 1.0;	//常量參數(設1可濾掉強的不合理的光)
	float linear = 0.09;	//線性參數(控制緩慢的衰減效果)
	float quadratic = 0.032;		//指數參數(控制迅速的衰減效果)
	float dis(float *_pos) {
		float sum = 0;
		sum += (_pos[0] - pos[0]) * (_pos[0] - pos[0]);
		sum += (_pos[1] - pos[1]) * (_pos[1] - pos[1]);
		sum += (_pos[2] - pos[2]) * (_pos[2] - pos[2]);
		return sqrt(sum);
	}
	*/
	void init(){
		baselit.color[0] = baselit.color[1] = baselit.color[2] = 30.0;
		baselit.dif[0] = baselit.dif[1] = baselit.dif[2] = 30.0;
		baselit.amb[0] = baselit.amb[1] = baselit.amb[2] = 30.0;
		baselit.spe[0] = baselit.spe[1] = baselit.spe[2] = 30.0;
		baselit.color[3] = baselit.dif[3] = baselit.spe[3] = baselit.amb[3] = 0.01;
		baselit.intensity = 0.05;
	}
	void draw() {
		if(isopen[3])polyhedron.material(				//玻璃(仿塑膠)
					180.0, 180.0, 180.0, 0.7,			//反射別人的顏色
					200.0, 200.0, 200.0, 0.5,			//我在別人眼中的顏色
					200.0, 200.0, 200.0, 0.2,			//我造成的環境光
					baselit.color[0],baselit.color[1],baselit.color[2],baselit.color[3],			//自體發光的顏色
					0.8 * 128);							//0(粗糙)-128(光滑)	
		else	polyhedron.material(					//玻璃(仿塑膠)
				180.0, 180.0, 180.0, 0.7,	//反射別人的顏色
				200.0, 200.0, 200.0, 0.5,	//我在別人眼中的顏色
				200.0, 200.0, 200.0, 0.2,	//我造成的環境光
				0.0, 0.0, 0.0, 0.0,			//自體發光的顏色
				0.8 * 128);					//0(粗糙)-128(光滑)	
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫燈
		glPushMatrix();
		polyhedron.draw_sphere(1.0);
		glPopMatrix();
		baselit.init();
	}
} pointlit;
struct SpotLit {
	BaseLight baselit;
	float pos[4];
	float len = 20.0, ang = 0;
	float dir[4] = { 0.0, 0.0, 20.0, 0 };		//平行眼睛-腳正前方
	float CutoffAng = 45;
	void init() {
		pos[4] = 1.0;
		baselit.color[0] = baselit.color[1] = baselit.color[2] = 30.0;
		baselit.dif[0] = baselit.dif[1] = baselit.dif[2] = 30.0;
		baselit.amb[0] = baselit.amb[1] = baselit.amb[2] = 30.0;
		baselit.spe[0] = baselit.spe[1] = baselit.spe[2] = 30.0;
		baselit.color[3] = baselit.dif[3] = baselit.spe[3] = baselit.amb[3] = 0.01;
		baselit.intensity = 0.5;
	}
	void draw() {
		if (isopen[5])polyhedron.material(				//玻璃(仿塑膠)
			180.0, 180.0, 180.0, 0.7,			//反射別人的顏色
			200.0, 200.0, 200.0, 0.5,			//我在別人眼中的顏色
			200.0, 200.0, 200.0, 0.2,			//我造成的環境光
			baselit.color[0], baselit.color[1], baselit.color[2], baselit.color[3],			//自體發光的顏色
			0.8 * 128);							//0(粗糙)-128(光滑)	
		else	polyhedron.material(					//玻璃(仿塑膠)
			180.0, 180.0, 180.0, 0.7,	//反射別人的顏色
			200.0, 200.0, 200.0, 0.5,	//我在別人眼中的顏色
			200.0, 200.0, 200.0, 0.2,	//我造成的環境光
			0.0, 0.0, 0.0, 0.0,			//自體發光的顏色
			0.8 * 128);					//0(粗糙)-128(光滑)	
		pos[0] = robot.pos.dx; pos[1] = robot.pos.dy + 21; pos[2] = robot.pos.dz + 12.5;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫燈
		glPushMatrix();
		glRotatef(robot.r, 0, 1, 0);
		glTranslatef(pos[0] * 0.7 + 7, pos[1] * 0.7, pos[2] * 0.7);
		polyhedron.draw_sphere(0.6);
		glPopMatrix();
		baselit.init();
	}

}spotlit;
struct DirLit {
	BaseLight baselit;
	float	len = 90.0;
	float	ang = 0;						//跟右地平線夾角()
	float	pos[4] = { 90.0, 0, 0.0, 0.0 };
	float	dir[4] = { 0, 0, 0, 0 };		//都照舞台中心
	void init() {
		baselit.amb[0] = baselit.amb[1] = 255.0;	//#FFFFCE(中心)
		baselit.amb[2] = 206.0; baselit.amb[3] = 0.5;
		baselit.dif[0] = baselit.dif[1] = baselit.dif[2] = 255.0;
		baselit.spe[0] = baselit.spe[1] = baselit.spe[2] = 255.0;
		baselit.color[0] = baselit.color[1] = 255.0;	//#FFFFB9暈光
		baselit.color[2] = 185.0; baselit.color[3] = 1.0;
		baselit.dif[3] = baselit.spe[3] = 0.7;
		baselit.intensity = 0.3;
	}
	void run() {
		ang += 5.0;
		if (ang >= 360.0) ang -= 360.0;
		pos[0] = cos(Rad(ang)) * len; dir[0] = pos[0];
		pos[1] = sin(Rad(ang)) * len; dir[1] = pos[1];
		if (ang == 0) {
			baselit.intensity = 0.3;
			baselit.color[0] = 255.0;
			baselit.color[1] = 255.0;
			baselit.color[2] = 185.0;
		}
		else if (ang < 90.0) {
			baselit.intensity = (0.3 * (90 - ang) + 0.8 * ang) / 90;
			baselit.color[2] = (185 * (90 - ang) + 55 * ang) / 90;
		}
		else if (ang < 180.0) {
			baselit.intensity = (0.8 * (180 - ang) + 0.25 * (ang - 90)) / 90;
			baselit.color[0] = (255 * (180 - ang) + 247 * (ang - 90)) / 90;
			baselit.color[1] = (255 * (180 - ang) + 80 * (ang - 90)) / 90;
			baselit.color[2] = (185 * (180 - ang)) / 90;
		}
		else {
			baselit.intensity = 0.0;
		}
		glFlush();
	}
	void draw() {
		if (isopen[4])
			polyhedron.material(
			baselit.spe[0], baselit.spe[1], baselit.spe[2], baselit.spe[3],					//反射別人的顏色
			baselit.color[0], baselit.color[1], baselit.color[2], baselit.color[3],			//我在別人眼中的顏色
			baselit.amb[0], baselit.amb[1], baselit.amb[2], baselit.amb[3],					//我造成的環境光
			baselit.dif[0], baselit.dif[1], baselit.dif[2], baselit.dif[3],					//自體發光的顏色
			0.0 * 128);																		//0(粗糙)-128(光滑)	
		glPushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		polyhedron.draw_sphere(3.0);
		glPopMatrix();
		baselit.init();
	}

}dirlit;
struct Camera {
	float scope[2][8][3], ratio = (double)width / (double)height;
	float **color;
	double mtx[16];
	/*----Define normals of camera ----*/
	bool drawmoniter;
	void init() {
		color = (float **)malloc(sizeof(float *) * 6);
		for(int i = 0; i < 6; ++i)
			color[i] = (float *)malloc(sizeof(float) * 4);

		scope[0][0][0] = 0.0; scope[0][0][1] = 0.0; scope[0][0][2] = 0.0;
		scope[0][4][0] = 0.0; scope[0][4][1] = 0.1; scope[0][4][2] = 0.0;
		scope[0][5][0] = 0.1 * ratio; scope[0][5][1] = 0.1; scope[0][5][2] = 0;
		scope[0][1][0] = 0.1 * ratio; scope[0][1][1] = 0; scope[0][1][2] = 0;
		double h1 = znear * tan(Rad(90 - (180.0 - zoom) / 2.0));
		scope[0][3][0] = (-h1 + 0.05) * ratio; scope[0][3][1] = -h1; scope[0][3][2] = znear;
		scope[0][7][0] = (-h1 + 0.05) * ratio; scope[0][7][1] = 0.1 + h1; scope[0][7][2] = znear;
		scope[0][6][0] = (h1 + 0.05) * ratio; scope[0][6][1] = 0.1 + h1; scope[0][6][2] = znear;
		scope[0][2][0] = (h1 + 0.05) * ratio; scope[0][2][1] = -h1; scope[0][2][2] = znear;

		scope[1][0][0] = (-h1 + 0.05) * ratio; scope[1][0][1] = -h1; scope[1][0][2] = 0;
		scope[1][4][0] = (-h1 + 0.05) * ratio; scope[1][4][1] = 0.1 + h1; scope[1][4][2] = 0;
		scope[1][5][0] = (h1 + 0.05) * ratio; scope[1][5][1] = 0.1 + h1; scope[1][5][2] = 0;
		scope[1][1][0] = (h1 + 0.05) * ratio; scope[1][1][1] = -h1; scope[1][1][2] = 0;
		double h2 = zfar * tan(Rad(90 - (180.0 - zoom) / 2.0));
		scope[1][3][0] = (-h2 + 0.05) * ratio; scope[1][3][1] = -h2; scope[1][3][2] = zfar - znear;
		scope[1][7][0] = (-h2 + 0.05) * ratio; scope[1][7][1] = 0.1 + h2; scope[1][7][2] = zfar - znear;
		scope[1][6][0] = (h2 + 0.05) * ratio; scope[1][6][1] = 0.1 + h2; scope[1][6][2] = zfar - znear;
		scope[1][2][0] = (h2 + 0.05) * ratio; scope[1][2][1] = -h2; scope[1][2][2] = zfar - znear;

		mtx[0] = u[0][0]; mtx[1] = u[0][1]; mtx[2] = u[0][2]; mtx[3] = 0.0;
		mtx[4] = u[1][0]; mtx[5] = u[1][1]; mtx[6] = u[1][2]; mtx[7] = 0.0;
		mtx[8] = u[2][0]; mtx[9] = u[2][1]; mtx[10] = u[2][2]; mtx[11] = 0.0;
		mtx[12] = 0.0; mtx[13] = 0.0; mtx[14] = 0.0; mtx[15] = 1.0;
		drawmoniter = true;
	}
	void moniter() {
		for (int i = 0; i < 6; ++i) {
			color[i][0] = 249.0 / 255.0;
			color[i][0] = 245.0 / 255.0;
			color[i][0] = 184.0 / 255.0;
			color[i][0] = 0.3;
		}
		glPolygonMode(GL_FRONT, GL_FILL);
		glPushMatrix();
		polyhedron.draw_hexahedron(color, 0);
		glPopMatrix();
		for (int i = 0; i < 6; ++i) {
			color[i][0] = 99.0 / 255.0;
			color[i][0] = 133.0 / 255.0;
			color[i][0] = 109.0 / 255.0;
			color[i][0] = 0.3;
		}
		glPolygonMode(GL_FRONT, GL_FILL);
		glPushMatrix();
		coo_sys.draw(0, 0, znear, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0);
		polyhedron.draw_hexahedron(color, 0);
		glPopMatrix();
	}
	struct Wing {
		void feather(float ang, float r, float len) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			polyhedron.material(		//玉
				76, 76, 76, 1.0,		//spe
				137, 226, 160, 1.0,		//mycolor
				34, 58, 40, 1.0,		//amb
				0.0, 0.0, 0.0, 0.0,		//mylit
				0.2 * 128);				//光滑度
			glPushMatrix();												//畫翅頭
			coo_sys.draw(0.0, 0.0, 0.5, 0.8, 1.0, 1.0, ang, 0, 0);		//後上右
			glScalef(0.8, 1, 1);
			polyhedron.draw_cylind(0.1, r, len);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			polyhedron.material(		//玉
				76, 76, 76, 1.0,		//spe
				137, 226, 160, 1.0,		//mycolor
				34, 58, 40, 1.0,		//amb
				0.0, 0.0, 0.0, 0.0,		//mylit
				0.2 * 128);				//光滑度
			glPushMatrix();												//畫翅尾
			coo_sys.draw(0.0, 0.0, len, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0);	//後上右
			polyhedron.draw_sphere(r);
			glPopMatrix();
			glPopMatrix();
		}
		void draw() {
			feather(-30.0, 1.5, 6.0);
			feather(-10.0, 2, 5.5);
			feather(10.0, 1.5, 4.5);
		}
	}wing_r, wing_l;
	void draw() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫鏡頭
		polyhedron.material(		//黑玻璃
			110, 110, 110, 1.0,		//spe
			3, 3, 3, 1.0,			//mycolor
			3, 3, 3, 1.0,			//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.7 * 128);				//光滑度
		glPushMatrix();
		polyhedron.draw_cylind(1.5, 1.5, 5.0);
		glPopMatrix();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫裝飾(圓)
		polyhedron.material(		//jade
			185, 185, 185, 1.0,		//spe
			243.0, 241, 120, 1.0,	//mycolor
			200, 200, 100, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.3 * 128);				//光滑度
		glPushMatrix();
		coo_sys.draw(0.0, 0.0, 0.5, 1.0, 1.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cylind(1.6, 1.6, 5.0);
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫裝飾(圓)
		polyhedron.material(		//jade
			185, 185, 185, 1.0,		//spe
			243.0, 241, 120, 1.0,	//mycolor
			200, 200, 100, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.3 * 128);				//光滑度
		glPushMatrix();
		coo_sys.draw(1.7, -2.5, 0.5, 1.0, 1.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cylind(0.5, 0.5, 5.0);
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫裝飾物(方)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			243.0, 241, 120, 1.0,	//mycolor
			200, 200, 100, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);				//光滑度
		glPushMatrix();
		coo_sys.draw(0.0, 3.0, 3.0, 4.0, 1.0, 5.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體(前)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			96, 115, 96, 1.0,		//mycolor
			90, 100, 90, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);				//光滑度
		glPushMatrix();
		coo_sys.draw(0.0, 0.5, 3.5, 5.0, 8.0, 5.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		polyhedron.material(		//jade
			250, 250, 250, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);				//光滑度
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(前_左)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);				//光滑度
		glPushMatrix();
		coo_sys.draw(-2.5, 0.5, 1.0, 1.0, 9.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(前_右)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);				//光滑度
		glPushMatrix();
		coo_sys.draw(2.5, 0.5, 1.0, 1.0, 9.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(前_下)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);					//光滑度
		glPushMatrix();
		coo_sys.draw(0.0, -3.5, 1.0, 6.0, 1.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(前_上)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);					//光滑度
		glPushMatrix();
		coo_sys.draw(0.0, 4.5, 1.0, 6.0, 1.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體(後)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			99, 133, 109., 1.0,		//mycolor
			90, 110, 100, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);					//光滑度
		glPushMatrix();
		coo_sys.draw(0.0, 1.0, 5.5, 6.0, 10.0, 5.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(後_前左)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);					//光滑度
		glPushMatrix();
		coo_sys.draw(-3.0, 1.0, 3.0, 1.0, 11.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(後_前右)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);					//光滑度
		glPushMatrix();
		coo_sys.draw(3.0, 1.0, 3.0, 1.0, 11.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(後_後右)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);					//光滑度
		glPushMatrix();
		coo_sys.draw(-3.0, 1.0, 8.0, 1.0, 11.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(後_後左)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);					//光滑度
		glPushMatrix();
		coo_sys.draw(3.0, 1.0, 8.0, 1.0, 11.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(後_下左)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);					//光滑度
		glPushMatrix();
		coo_sys.draw(-3.0, -4.0, 5.5, 1.0, 1.0, 6.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(後_下右)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);					//光滑度
		glPushMatrix();
		coo_sys.draw(3.0, -4.0, 5.5, 1.0, 1.0, 6.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(後_上左)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);				//光滑度
		glPushMatrix();
		coo_sys.draw(-3.0, 6.0, 5.5, 1.0, 1.0, 6.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(後_上右)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);				//光滑度
		glPushMatrix();
		coo_sys.draw(3.0, 6.0, 5.5, 1.0, 1.0, 6.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(後_前下)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);				//光滑度
		glPushMatrix();
		coo_sys.draw(0.0, -4.0, 3.0, 7.0, 1.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(後_前上)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);				//光滑度
		glPushMatrix();
		coo_sys.draw(0.0, 6.0, 3.0, 7.0, 1.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(後_後下)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);				//光滑度
		glPushMatrix();
		coo_sys.draw(0.0, -4.0, 8.0, 7.0, 1.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//畫相機體裝飾(後_後上)
		polyhedron.material(		//jade
			197, 197, 197, 1.0,		//spe
			249, 245, 184, 1.0,		//mycolor
			220, 220, 160, 1.0,		//amb
			0.0, 0.0, 0.0, 0.0,		//mylit
			0.6 * 128);				//光滑度
		glPushMatrix();
		coo_sys.draw(0.0, 6.0, 8.0, 7.0, 1.0, 1.0, 0, 0, 0);	//右上前
		polyhedron.draw_cube();
		glPopMatrix();

		glPushMatrix();
		coo_sys.draw(0.0, 1.0, 5.5, 1.0, 1.0, 1.0, 0, 90.0, 0);
		wing_r.draw();//畫右翅
		glPopMatrix();
		glPushMatrix();
		coo_sys.draw(0.0, 1.0, 5.5, 1.0, 1.0, 1.0, 0, -90.0, 0);
		wing_l.draw();//畫左翅
		glPopMatrix();

		if (drawmoniter) {	//畫視角
			glPushMatrix();
			coo_sys.draw(0.0, 1.0, 5.5, 1.0, 1.0, 1.0, 0, 180.0, 0);
			moniter();
			glPopMatrix();
		}
		glPopMatrix();
	}
} camera;
struct Fog {
	float color[4] = { 0.1, 0.1, 0.1, 0.1 };
	int level[3] = { GL_LINEAR , GL_EXP , GL_EXP2 };
	int now = 1;
}fog;
/*------------------------------------------
 * Procedure to compute norm of vector v[]
 */
 float norm2(float v[])
 {
	 double  sum;
	 float   temp;

	 sum = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	 temp = (float)sqrt(sum);
	 return temp;
 }

void draw_robot() { 
	glPushMatrix();
	coo_sys.draw(robot.pos.dx, robot.pos.dy, robot.pos.dz, 1.0, 1.0, 1.0, 0.0, robot.r, 0.0);
	robot.draw();
	glPopMatrix();
}

void draw_stage() {
	stage.draw();
}

/*------------------------------------------------------
 * Procedure to make projection matrix
 */
void make_projection(int x){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (x == 4) {
		gluPerspective(zoom, ratio, znear, zfar);
	}
	else {
		//glOrtho(左,右,下,上,near,far)
		if (width > height)//左右下上
			 glOrtho(-pra_zoom, pra_zoom,
					-pra_zoom * (float)height / (float)width, pra_zoom * (float)height / (float)width,
					pra_znear, pra_zfar);
		else glOrtho(-pra_zoom * (float)width / (float)height, pra_zoom * (float)width / (float)height,
					-pra_zoom, pra_zoom,
					pra_znear, pra_zfar);
	}
	glMatrixMode(GL_MODELVIEW);
}

/*-------------------------------------------------------
 * Make viewing matrix
 */
void make_view(int x){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	switch (x) {
	case 4:       /* Perspective */

	  /* In this sample program, eye position and Xe, Ye, Ze are computed
		 by ourselves. Therefore, use them directly; no trabsform is
		 applied upon eye coordinate system
		 */
		gluLookAt(	eye[0], eye[1], eye[2],
					eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2],
					u[1][0], u[1][1], u[1][2]);
		glGetFloatv(GL_MODELVIEW_MATRIX, eyeMtx);
		billboard.init();
		break;

	case 1:       /* X direction parallel viewing */
		gluLookAt(robot.pos.dx + 40, robot.pos.dy + 20, robot.pos.dz, robot.pos.dx - 40, robot.pos.dy + 20, robot.pos.dz, 0.0, 1.0, 0.0);
		break;
	case 2:       /* Y direction parallel viewing */
		gluLookAt(robot.pos.dx, robot.pos.dy + 40, robot.pos.dz, robot.pos.dx, robot.pos.dy - 0, robot.pos.dz, 1.0, 0.0, 0.0);
		break;
	case 3:		/* Z direction parallel viewing */
		gluLookAt(robot.pos.dx, robot.pos.dy + 20, robot.pos.dz + 40, robot.pos.dx, robot.pos.dy + 20, robot.pos.dz - 100, 0.0, 1.0, 0.0);
		break;
	}
}

/*---------------------------------------------------------
 * Procedure to draw view volume, eye position, focus ,...
 * for perspective projection
 */
void draw_camera(){
	glPushMatrix();
	coo_sys.draw(eye[0], eye[1], eye[2], 1.0, 1.0, 1.0, 0.0, 0.0, 0.0);
	glMultMatrixd(camera.mtx);
	camera.draw();
	glPopMatrix();
}

void draw_pointlit() {
	glPushMatrix();
	glTranslatef(pointlit.pos[0], pointlit.pos[1], pointlit.pos[2]);
	pointlit.draw();
	glPopMatrix();
	if (!isopen[3])return;
	glLightfv(GL_LIGHT3, GL_POSITION, pointlit.pos);  /*fixed position in eye space---*/
	glLightfv(GL_LIGHT3, GL_DIFFUSE, pointlit.baselit.dif);
	glLightfv(GL_LIGHT3, GL_SPECULAR, pointlit.baselit.spe);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHTING);

}

void draw_dirlit() {
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(dirlit.pos[0], dirlit.pos[1], dirlit.pos[2]);
	dirlit.draw();
	glPopMatrix();
	if (!isopen[4])return;
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT4);
	glLightfv(GL_LIGHT4, GL_POSITION, dirlit.dir);  /*fixed position in eye space---*/
	glLightfv(GL_LIGHT4, GL_DIFFUSE, dirlit.baselit.dif);
	glLightfv(GL_LIGHT4, GL_SPECULAR, dirlit.baselit.spe);
}

void draw_spotlit() {
	glPushMatrix();
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spotlit.dir);
	spotlit.draw();
	glPopMatrix();
	if (!isopen[5])return;
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT5);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, spotlit.CutoffAng);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, spotlit.baselit.dif);
	glLightfv(GL_LIGHT5, GL_SPECULAR, spotlit.baselit.spe);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 0);
}

 /*-------------------------------------------------------
  * This procedure draw the scene
  */
void draw_scene(int style) {
	glDisable(GL_TEXTURE_2D);
	draw_dirlit();
	draw_pointlit();
	//draw_spotlit();
	draw_stage();
	draw_robot();
	if(style && style != 4) draw_camera();
}

void display()
{

  /*Clear previous frame and the depth buffer */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (fog.now) {
	  glEnable(GL_FOG);
	  glFogi(GL_FOG_MODE, fog.level[fog.now - 1]);
	  glFogfv(GL_FOG_COLOR, fog.color);/*set the fog color */

  }
  else glDisable(GL_FOG);

  /*----Define the current eye position and the eye-coordinate system---*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  switch (style) {
  case 0:
	  make_projection(4);
	  glViewport(width / 2, 0, width / 2, height / 2);
	  make_view(4);
	  draw_scene(0);

	  make_projection(1);
	  glViewport(0, height / 2, width / 2, height / 2);
	  make_view(1);
	  draw_scene(1);

	  make_projection(2);
	  glViewport(width / 2, height / 2, width / 2, height / 2);
	  make_view(2);
	  draw_scene(2);

	  make_projection(3);
	  glViewport(0, 0, width / 2, height / 2);
	  make_view(3);
	  draw_scene(3);
	  break;

  case 1:case 2:case 3:case 4:
	  make_projection(style);
	  glViewport(0, 0, width, height);
	  make_view(style);//now_view
	  draw_scene(4);
	  break;
  }

  /*-------Swap the back buffer to the front --------*/
	glutSwapBuffers();
	glFlush(); /*--Display the results----*/
 }

/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (double)w / (double)h, 1.5, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
	double dy = cosl(Rad(robot.r));
	double dx = sinl(Rad(robot.r));
	if (Cs == RIS) return;
	if (Cs == LWLK) {
		robot.act.inc = (robot.act.inc + 1) % 4; //right:left
		dy = cosl(Rad(robot.r + 90.0));
		dx = sinl(Rad(robot.r + 90.0));
		if (robot.act.inc & 1) { dx *= 2; dy *= 2; }
		if (robot.pos.dx + dx < -75.0 || robot.pos.dx + dx > 75.0) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		if (robot.pos.dz + dy < -45.0 || robot.pos.dz + dy > 45.0) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		/*
		if (cross({ robot.pos.dx * 1.5 + 7,robot.pos.dz * 1.5 + 7 }, { robot.pos.dx + dx * 1.5 + 7, robot.pos.dz + dy * 1.5 + 7 }, { 25, 15 }, { 25, 35 })) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		if (cross({ robot.pos.dx * 1.5 + 7,robot.pos.dz * 1.5 + 7 }, { robot.pos.dx + dx * 1.5 + 7, robot.pos.dz + dy * 1.5 + 7 }, { 20, 15 }, { 30, 15 })) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		if (cross({ robot.pos.dx * 1.5 + 7,robot.pos.dz * 1.5 + 7 }, { robot.pos.dx + dx * 1.5 + 7, robot.pos.dz + dy * 1.5 + 7 }, { 20, 35 }, { 30, 35 })) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		*/
		robot.pos.dx = min(max(robot.pos.dx + dx, 0.0), 38.0);
		robot.pos.dz = min(max(robot.pos.dz + dy, 0.0), 38.0);
		robot.lwalk();
		display();
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (Cs == LWALK) {
		dy = cosl(Rad(robot.r + 90.0));
		dx = sinl(Rad(robot.r + 90.0));
		if (robot.pos.dx + dx * 3 < -75.0 || robot.pos.dx + dx * 3 + 9 > 75.0) check = false;
		if (robot.pos.dz + dy * 3 < -45.0 || robot.pos.dz + dy * 3 > 45.0) check = false;
		/*
		if (cross({ robot.pos.dx * 4 + 7,robot.pos.dz * 4 + 7 }, { robot.pos.dx + dx * 4 + 7, robot.pos.dz + dy * 4 + 7 }, { 25, 15 }, { 25, 35 })) check = false;
		if (cross({ robot.pos.dx * 4 + 7,robot.pos.dz * 4 + 7 }, { robot.pos.dx + dx * 4 + 7, robot.pos.dz + dy * 4 + 7 }, { 20, 15 }, { 30, 15 })) check = false;
		if (cross({ robot.pos.dx * 4 + 7,robot.pos.dz * 4 + 7 }, { robot.pos.dx + dx * 4 + 7, robot.pos.dz + dy * 4 + 7 }, { 20, 35 }, { 30, 35 })) check = false;
		*/
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
		dy = cosl(Rad(robot.r + 270.0));
		dx = sinl(Rad(robot.r + 270.0));
		robot.act.inc = (robot.act.inc + 1) % 4; //right:left
		if (robot.act.inc & 1) { dx *= 2; dy *= 2; }
		if (robot.pos.dx + dx < -75.0 || robot.pos.dx + dx > 75.0) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		else if (robot.pos.dz + dy < -45.0 || robot.pos.dz + dy > 45.0) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		/*
		else if (cross({ robot.pos.dx * 1.5 + 7,robot.pos.dz * 1.5 + 7 }, { robot.pos.dx + dx * 1.5 + 7, robot.pos.dz + dy * 1.5 + 7 }, { 25, 15 }, { 25, 35 })) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		else if (cross({ robot.pos.dx * 1.5 + 7,robot.pos.dz * 1.5 + 7 }, { robot.pos.dx + dx * 1.5 + 7, robot.pos.dz + dy * 1.5 + 7 }, { 20, 15 }, { 30, 15 })) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		else if (cross({ robot.pos.dx * 1.5 + 7,robot.pos.dz * 1.5 + 7 }, { robot.pos.dx + dx * 1.5 + 7, robot.pos.dz + dy * 1.5 + 7 }, { 20, 35 }, { 30, 35 })) {
			robot.act.inc = -1;
			robot.act.action = RIS;
		}
		*/
		if (robot.act.inc != -1) {
			robot.pos.dx = min(max(robot.pos.dx + dx, 0.0), 38.0);
			robot.pos.dz = min(max(robot.pos.dz + dy, 0.0), 38.0);
		}
		robot.rwalk();
		display();
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	if (Cs == RWALK) {
		dy = cosl(Rad(robot.r + 270.0));
		dx = sinl(Rad(robot.r + 270.0));
		if (robot.pos.dx + dx * 3 - 9 < -75.0 || robot.pos.dx + dx * 3 > 75.0) check = false;
		else if (robot.pos.dz + dy * 3 < -45.0 || robot.pos.dz + dy * 3 > 45.0) check = false;
		/*
		if (cross({ robot.pos.dx * 4 + 7,robot.pos.dz * 4 + 7 }, { robot.pos.dx + dx * 4 + 7, robot.pos.dz + dy * 4 + 7 }, { 25, 15 }, { 25, 35 })) check = false;
		if (cross({ robot.pos.dx * 4 + 7,robot.pos.dz * 4 + 7 }, { robot.pos.dx + dx * 4 + 7, robot.pos.dz + dy * 4 + 7 }, { 20, 15 }, { 30, 15 })) check = false;
		if (cross({ robot.pos.dx * 4 + 7,robot.pos.dz * 4 + 7 }, { robot.pos.dx + dx * 4 + 7, robot.pos.dz + dy * 4 + 7 }, { 20, 35 }, { 30, 35 })) check = false;
		*/
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
			if (cnt < 3) robot.r += (robot.act.idiomatic ? -40 : 40);
			else if (cnt < 9) robot.r += (robot.act.idiomatic ? -25 : 25);
			else robot.r += (robot.act.idiomatic ? -15 : 15);
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
				if (cnt < 3) robot.r += (robot.act.idiomatic ? -40 : 40);
				else if (cnt < 9) robot.r += (robot.act.idiomatic ? -25 : 25);
				else robot.r += (robot.act.idiomatic ? -15 : 15);
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
				if (cnt < 6) robot.r += (robot.act.idiomatic ? -40 : 40);
				else robot.r += (robot.act.idiomatic ? -20 : 20);
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
			robot.r += (robot.act.idiomatic ? -15 : 15);
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
		if (robot.pos.dx + dx < -75.0 || robot.pos.dx + dx > 75.0) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (robot.pos.dz + dy < -45.0 || robot.pos.dz + dy > 45.0) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		/*
		if (cross({ robot.pos.dx * 3 + 7,robot.pos.dz * 3 + 7 }, { robot.pos.dx + dx * 3 + 7, robot.pos.dz + dy * 3 + 7 }, { 25, 15 }, { 25, 35 })) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (cross({ robot.pos.dx * 3 + 7,robot.pos.dz * 3 + 7 }, { robot.pos.dx + dx * 3 + 7, robot.pos.dz + dy * 3 + 7 }, { 20, 15 }, { 30, 15 })) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (cross({ robot.pos.dx * 3 + 7,robot.pos.dz * 3 + 7 }, { robot.pos.dx + dx * 3 + 7, robot.pos.dz + dy * 3 + 7 }, { 20, 35 }, { 30, 35 })) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		*/
		if (robot.act.fir == 2 || robot.act.fir == 3) {
			robot.pos.dx = min(max(robot.pos.dx + dx, 0.0), 38.0);
			robot.pos.dz = min(max(robot.pos.dz + dy, 0.0), 38.0);
		}
		robot.run();
		display();
		glutTimerFunc(150, timerFunc, robot.act.action);
	}
	if (Cs == LEAP) {
		if (robot.pos.dx + dx * 5 < -75.0 || robot.pos.dx + dx * 5 > 75.0) check = false;
		if (robot.pos.dz + dy * 5  < -45.0 || robot.pos.dz + dy * 5 > 45.0) check = false;
		/*
		if (cross({ robot.pos.dx * 6 + 7,robot.pos.dz * 6 + 7 }, { robot.pos.dx + dx * 6 + 7, robot.pos.dz + dy * 6 + 7 }, { 25, 15 }, { 25, 35 })) check = false;
		if (cross({ robot.pos.dx * 6 + 7,robot.pos.dz * 6 + 7 }, { robot.pos.dx + dx * 6 + 7, robot.pos.dz + dy * 6 + 7 }, { 20, 15 }, { 30, 15 })) check = false;
		if (cross({ robot.pos.dx * 6 + 7,robot.pos.dz * 6 + 7 }, { robot.pos.dx + dx * 6 + 7, robot.pos.dz + dy * 6 + 7 }, { 20, 35 }, { 30, 35 })) check = false;
		*/
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
			robot.pos.dx = min(max(robot.pos.dx + dx, 0.0), 38.0);
			robot.pos.dz = min(max(robot.pos.dz + dy, 0.0), 38.0);
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
		//0:right_lift, 1:left_lift 4:right_down 5:left_down else:stand_by

		if (robot.pos.dx + dx < -75.0 || robot.pos.dx + dx > 75.0) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (robot.pos.dz + dy < -45.0 || robot.pos.dz + dy > 45.0) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		/*
		if (cross({ robot.pos.dx * 3 + 7,robot.pos.dz * 3 + 7 }, { robot.pos.dx + dx * 3 + 7, robot.pos.dz + dy * 3 + 7 }, { 25, 15 }, { 25, 35 })) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (cross({ robot.pos.dx * 3 + 7,robot.pos.dz * 3 + 7 }, { robot.pos.dx + dx * 3 + 7, robot.pos.dz + dy * 3 + 7 }, { 20, 15 }, { 30, 15 })) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		if (cross({ robot.pos.dx * 3 + 7,robot.pos.dz * 3 + 7 }, { robot.pos.dx + dx * 3 + 7, robot.pos.dz + dy * 3 + 7 }, { 20, 35 }, { 30, 35 })) {
			robot.act.fir = -2 + (robot.act.idiomatic & 1);
			robot.act.action = RIS;
		}
		*/
		if (robot.act.fir == 0 || robot.act.fir == 1 || robot.act.fir == 4 || robot.act.fir == 5) {
			robot.pos.dx = min(max(robot.pos.dx + dx, 0.0), 38.0);
			robot.pos.dz = min(max(robot.pos.dz + dy, 0.0), 38.0);
		}
		robot.walk();
		display();
		glutTimerFunc(300, timerFunc, robot.act.action);

	}
	if (Cs == LIFT) {
		if (robot.pos.dx + dx * 2 < -75.0 || robot.pos.dx + dx * 2 > 75.0) check = false;
		else if (robot.pos.dz + dy * 2 < -45.0 || robot.pos.dz + dy * 2 > 45.0) check = false;
		/*
		if (cross({ robot.pos.dx * 1.5 + 7,robot.pos.dz * 3 + 7 }, { robot.pos.dx + dx * 1.5 + 7, robot.pos.dz + dy * 1.5 + 7 }, { 25, 15 }, { 25, 35 })) check = false;
		if (cross({ robot.pos.dx * 1.5 + 7,robot.pos.dz * 1.5 + 7 }, { robot.pos.dx + dx * 1.5 + 7, robot.pos.dz + dy * 1.5 + 7 }, { 20, 15 }, { 30, 15 })) check = false;
		if (cross({ robot.pos.dx * 1.5 + 7,robot.pos.dz * 1.5 + 7 }, { robot.pos.dx + dx * 1.5 + 7, robot.pos.dz + dy * 1.5 + 7 }, { 20, 35 }, { 30, 35 })) check = false;
		*/
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
					robot.pos.dx = min(max(robot.pos.dx + dx, 0.0), 38.0);
					robot.pos.dz = min(max(robot.pos.dz + dy, 0.0), 38.0);
				}
				robot.walk();
				display();
				glutTimerFunc(300, timerFunc, robot.act.action);
			}
			else check2 = true;
		}
	}
	if (Cs == DOUBLEDANCE) {
		doubledance++;
		if (doubledance >= 20) doubledance = 4;
		display();
		glutTimerFunc(300, timerFunc, DOUBLEDANCE);
	}
	if (Cs == DANCE) {
		dance++;
		if (dance >= 29) dance = 20;
		display();
		glutTimerFunc(300, timerFunc, DANCE);
	}
	if (Cs == SUN) {
		dirlit.run();
		display();
		glutTimerFunc(50, timerFunc, SUN);
	}
}

/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
bool view = true, eye_mode = false, camera_zoom = true;
void my_act(unsigned char key, int x, int y){
	float  X[3], Y[3], Z[3];

	//end
	if (key == 'Q' || key == 'q') exit(0);

	//bg mode
	if (key == '0') { scene = (scene + 1) % 2; cout << scene; }

	//vision mode
	else if (key == '1') style = (style ? 0 : 4);
	else if (style && key == '2') style = style % 4 + 1;

	//direction
	else if (key == 't' || key == 'T') eye_mode = !eye_mode;	//0:1 = t:r
	else if (key == 's' || key == 'S') camera_zoom = !camera_zoom; // which zoom
	else if (key - '3' >= 0 && key - '3' <= 2) opener = key - '3' + 3;	//對燈做操作
	else if (key == '9') {	//對霧做操作
		if (opener == 9) fog.now = (fog.now + 1) % 4;
		else opener = 9;	
	}
	else if (key == 'c' || key == 'C')  isopen[opener] = !isopen[opener];

	//drew eyes views
	else if (key == '!') camera.drawmoniter = !camera.drawmoniter;
	


	//eye_mode 0 : 1 => translate : rotate
	if ((!style || style == 1) && (key == 'x' || key == 'X')) {
		if (!eye_mode) {//translate at x
			double cmpeye[3], cmprobot[3] = {robot.pos.dx, robot.pos.dy, robot.pos.dz };
			bool caneye = true;
			for (int i = 0; i < 3; i++) {
				cmpeye[i] = eye[i] + (key == 'X' ? -0.5 : 0.5) * u[0][i];
				if (fabs(cmpeye[i] - cmpeye[i]) > 50.0) caneye = false;
			}
			if (caneye) {
				eyeDx = eyeDx + (key == 'X' ? 0.5 : -0.5);
				for (int i = 0; i < 3; i++) eye[i] = cmpeye[i];
			}
		}
		else {//rotate at x
			eyeAngx += (key == 'X' ? 5.0 : -5.0);
			if (eyeAngx > 360.0) eyeAngx -= 360.0;
			else if (eyeAngx < 0.0) eyeAngx += 360.0;
			for (int i = 0; i < 3; ++i) {
				Y[i] = u[1][i] * cv + (key == 'X' ? -1 : 1) * u[2][i] * sv;
				Z[i] = u[2][i] * cv + (key == 'X' ? 1 : -1) * u[1][i] * sv;
			}
			for (int i = 0; i < 3; i++) {
				u[1][i] = Y[i];
				u[2][i] = Z[i];
			}
			camera.init();
		}
	}
	else if ((!style || style == 2) && (key == 'y' || key == 'Y')) {
		if (!eye_mode) {//translate at y
			double cmpeye[3], cmprobot[3] = { robot.pos.dx, robot.pos.dy, robot.pos.dz };;
			bool caneye = true;
			for (int i = 0; i < 3; i++) {
				cmpeye[i] = eye[i] + (key == 'Y' ? -0.5 : 0.5) * u[1][i];
				if (fabs(cmpeye[i] - cmpeye[i]) > 50.0) caneye = false;
			}
			if (caneye) {
				eyeDy = eyeDy + (key == 'Y' ? 0.5 : -0.5);
				for (int i = 0; i < 3; i++) eye[i] = cmpeye[i];
			}
		}
		else {//rotate at y
			eyeAngy += (key == 'Y' ? 5.0 : -5.0);
			if (eyeAngy > 360.0) eyeAngy -= 360.0;
			else if (eyeAngy < 0.0) eyeAngy += 360.0;
			for (int i = 0; i < 3; i++) {
				X[i] = cv * u[0][i] + (key == 'Y' ? -1 : 1) * sv * u[2][i];
				Z[i] = cv * u[2][i] + (key == 'Y' ? 1 : -1) * sv * u[0][i];
			}
			for (int i = 0; i < 3; i++) {
				u[0][i] = X[i];
				u[2][i] = Z[i];
			}
			camera.init();
		}
	}
	else if ((!style || style == 3) && (key == 'z' || key == 'Z')) {
		if (!eye_mode) {//translate at z
			double cmpeye[3], cmprobot[3] = { robot.pos.dx, robot.pos.dy, robot.pos.dz };
			bool caneye = true;
			for (int i = 0; i < 3; i++) {
				cmpeye[i] = eye[i] + (key == 'Z' ? -0.5 : 0.5) * u[2][i];
				if (fabs(cmpeye[i] - cmpeye[i]) > 50.0) caneye = false;
			}
			if (caneye) {
				eyeDz = eyeDz + (key == 'Z' ? 0.5 : -0.5);
				for (int i = 0; i < 3; i++) eye[i] = cmpeye[i];
			}
		}
		else {//rotate at z
			eyeAngz += (key == 'Z' ? 5.0 : -5.0);
			if (eyeAngz > 360.0) eyeAngz -= 360.0;
			if (eyeAngz < 0.0) eyeAngz += 360.0;
			for (int i = 0; i < 3; i++) {
				X[i] = cv * u[0][i] + (key == 'Z' ? -1 : 1) * sv * u[1][i];
				Y[i] = cv * u[1][i] + (key == 'Z' ? 1 : -1) * sv * u[0][i];
			}
			for (int i = 0; i < 3; i++) {
				u[0][i] = X[i];
				u[1][i] = Y[i];
			}
			camera.init();
		}
	}

	//lit color & intensity

	else if (key == 'r' || key == 'R') {
		if (opener == 3) {
			if (key == 'r') pointlit.baselit.color[0] = min((float)255.0, pointlit.baselit.color[0] + (float)2.0);
			else pointlit.baselit.color[0] = max((float)0.0, pointlit.baselit.color[0] - (float)2.0);
		}
		else if (opener == 5) {
			if (key == 'r') spotlit.baselit.color[0] = min((float)255.0, spotlit.baselit.color[0] + (float)2.0);
			else spotlit.baselit.color[0] = max((float)0.0, spotlit.baselit.color[0] - (float)2.0);
		}
		else if (opener == 9 && fog.now) {
			if (key == 'r') fog.color[0] = min((float)1.0, fog.color[0] + (float)0.1);
			else fog.color[0] = max((float)0.0, fog.color[0] - (float)0.1);
		}
	}
	else if (key == 'g' || key == 'G') {
		if (opener == 3) {
			if (key == 'g') pointlit.baselit.color[1] = min((float)255.0, pointlit.baselit.color[1] + (float)2.0);
			else pointlit.baselit.color[1] = max((float)0.0, pointlit.baselit.color[1] - (float)2.0);
		}
		else if (opener == 5) {
			if (key == 'g') spotlit.baselit.color[1] = min((float)255.0, spotlit.baselit.color[1] + (float)2.0);
			else spotlit.baselit.color[1] = max((float)0.0, spotlit.baselit.color[1] - (float)2.0);
		}
		else if (opener == 9 && fog.now) {
			if (key == 'g') fog.color[1] = min((float)1.0, fog.color[1] + (float)0.1);
			else fog.color[1] = max((float)0.0, fog.color[1] - (float)0.1);
		}
	}
	else if (key == 'b' || key == 'B') {
		if (opener == 3) {
			if (key == 'b') pointlit.baselit.color[2] = min((float)255.0, pointlit.baselit.color[2] + (float)2.0);
			else pointlit.baselit.color[2] = max((float)0.0, pointlit.baselit.color[2] - (float)2.0);
		}
		else if (opener == 5) {
			if (key == 'b') spotlit.baselit.color[2] = min((float)255.0, spotlit.baselit.color[2] + (float)2.0);
			else spotlit.baselit.color[2] = max((float)0.0, spotlit.baselit.color[2] - (float)2.0);
		}
		else if (opener == 9 && fog.now) {
			if (key == 'b') fog.color[2] = min((float)1.0, fog.color[2] + (float)0.1);
			else fog.color[2] = max((float)0.0, fog.color[2] - (float)0.1);
		}
	}
	else if (key == 'i' || key == 'I') {
		if (opener == 3) {
			if (key == 'i') pointlit.baselit.intensity = min((float)1.0, pointlit.baselit.intensity + (float)0.005);
			else pointlit.baselit.intensity = max((float)0.0, pointlit.baselit.intensity - (float)0.005);
			cout <<key <<pointlit.baselit.intensity << "\n";
		}
		if (opener == 5) {
			if (key == 'i') spotlit.baselit.intensity = min((float)1.0, spotlit.baselit.intensity + (float)0.005);
			else spotlit.baselit.intensity = max((float)0.0, spotlit.baselit.intensity - (float)0.005);
			cout << key << spotlit.baselit.intensity << "\n";
		}
	}

	//eye zoom
	else if (key == 'v') {//zoom_ang_add
		if ((!style || style == 4) && camera_zoom && zoom + 5.0 <= 90.0) {
			zoom += 5.0; camera.init();
		}
		else if ((!style && style != 4) && !camera_zoom && pra_zoom + 5.0 <= 40.0)
			pra_zoom += 5.0;
	}
	else if (key == 'V') {//zoom_ang_sub
		if ((!style || style == 4) && camera_zoom && zoom - 5.0 >= 40.0) {
			zoom -= 5.0; camera.init();
		}
		else if ((!style && style != 4) && !camera_zoom && pra_zoom - 5.0 >= 5.0)
			pra_zoom -= 5.0;
	}
	else if (key == 'n') {
		if ((!style || style == 4) && camera_zoom && znear + 10.0 <= zfar - 20.0) {
			znear += 10.0; camera.init();
		}
		else if ((!style && style != 4) && !camera_zoom && pra_znear + 10.0 >= pra_zfar - 20.0)
			pra_znear += 10.0;
	}
	else if (key == 'N') {
		if ((!style || style == 4) && camera_zoom && znear - 10.0 > 0.0) {
			znear -= 10.0; camera.init();
		}
		else if ((!style && style != 4) && !camera_zoom && pra_znear - 10.0 > 0)
			pra_znear -= 10.0;
	}
	else if (key == 'f') {//zoom_far_add
		if ((!style || style == 4) && camera_zoom && zfar + 10.0 <= 200.0) {
			zfar += 10.0; camera.init();
		}
		else if ((!style && style != 4) && !camera_zoom && pra_zfar + 10.0 <= 200.0)
			pra_zfar += 10.0;
	}
	else if (key == 'F') {//zoom_far_sub
		if ((!style || style == 4) && camera_zoom && zfar - 10.0 >= znear + 20.0) {
			zfar -= 10.0; camera.init();
		}
		else if ((!style && style != 4) && !camera_zoom && pra_zfar - 10.0 >= znear + 20.0)
			pra_zfar -= 10.0;
	}
	
	//robot translate
	else if (robot.act.action <= 0) {
		if (key == 'p' || key == 'P') {//parall dir
		//if(prekey == ' ') //旁飛燕
			robot.act.fir = (key == 'p' ? -2 : -1);//right:left
			robot.act.action = (key == 'p' ? RWLK : LWLK);
			glutTimerFunc(150, timerFunc, robot.act.action);
		}
		else if (key == 'h' || key == 'w') {//forward dir
			//if(prekey == ' ') //前飛燕
			robot.act.action = (key == 'h' ? RUN : WALK);
			glutTimerFunc(150, timerFunc, robot.act.action);
		}
		//else if (key == 'H' || key == 'W')//backward 踢臀跑&小碎步
		else if (key == 'H' || key == 'W') {//無姿勢後退
			//if(prekey == ' ') //後飛燕
			double dy = cosl(Rad(robot.r + 180)) * 5.0;
			double dx = sinl(Rad(robot.r + 180)) * 5.0;
			if (robot.pos.dx + dx < 0.0 || robot.pos.dx + dx > 38.0) return;
			if (robot.pos.dz + dy < 0.0 || robot.pos.dz + dy > 38.0) return;
			/*	把桿被我收起來了
			if (cross({robot.pos.dx * 1.5 + 7,robot.pos.dz * 1.5 + 7}, {robot.pos.dx + dx * 1.5 + 7, robot.pos.dz + dy * 1.5 + 7}, {25, 15}, {25, 35})) return;
			if (cross({ robot.pos.dx * 1.5 + 7,robot.pos.dz * 1.5 + 7 }, { robot.pos.dx + dx * 1.5 + 7, robot.pos.dz + dy * 1.5 + 7 }, { 20, 15 }, { 30, 15 })) return;
			if (cross({ robot.pos.dx * 1.5 + 7,robot.pos.dz * 1.5 + 7 }, { robot.pos.dx + dx * 1.5 + 7, robot.pos.dz + dy * 1.5 + 7 }, { 20, 35 }, { 30, 35 })) return;
			*/
			robot.pos.dx = min(max(robot.pos.dx + dx, 0.0), 38.0);
			robot.pos.dz = min(max(robot.pos.dz + dy, 0.0), 38.0);
		}
		else if (key == 'u' || key == 'U') {//slow rot
			robot.act.idiomatic = (key == 'u' ? 0 : 1);//right:left
			robot.act.action = TURN;
			glutTimerFunc(150, timerFunc, robot.act.action);
		}
		else if (key == 'o' || key == 'O') {//quick rot
			robot.act.idiomatic = (key == 'o' ? 0 : 1);//right:left
			robot.act.action = ROT; cnt = 0;
			glutTimerFunc(200, timerFunc, robot.act.action);
		}
		else if (key == ' ') {
			if (prekey == '\n') {
				//prekey = ' ';
				robot.act.action = JMP;
				glutTimerFunc(150, timerFunc, robot.act.action);
			}
			else {
				if (prekey == 'h') {
					robot.act.action = LEAP_JUMP;
					glutTimerFunc(100, timerFunc, robot.act.action);
				}
				else if (prekey == 'w') {
					robot.act.action = LIFT_JUMP; check2 = false;
					glutTimerFunc(100, timerFunc, robot.act.action);
				}
				else if (prekey == 'W') {
					robot.act.action = BWALK_JUMP;
					glutTimerFunc(150, timerFunc, robot.act.action);
				}
				else if (prekey == 'u') {
					robot.act.action = TUN_JUMP; check2 = false;
					glutTimerFunc(100, timerFunc, robot.act.action);
				}
				else if (prekey == 'p') {
					robot.act.action = RWALK_JUMP;
					glutTimerFunc(150, timerFunc, robot.act.action);
				}
				else if (prekey == 'P') {
					robot.act.action = LWALK_JUMP;
					glutTimerFunc(150, timerFunc, robot.act.action);
				}
				prekey = '\n';
			}
		}
	}
	display();
}

/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is end pressed,
 * the program is aborted.
 */
void my_end(unsigned char key, int x, int y) {
	if (key == 'h') {
		prekey = 'h'; check = true;
		robot.act.action = LEAP;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	else if (key == 'w') {
		prekey = 'w'; check = true;
		robot.act.action = LIFT;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	else if (key == 'u' || key == 'U') {
		prekey = 'u'; check = true;
		robot.act.action = TUN;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	else if (key == 'o' || key == 'O') robot.act.action = RIS;
	else if (key == 'P') {
		prekey = 'P'; check = true;
		robot.act.action = LWALK;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	else if (key == 'p') {
		prekey = 'p'; check = true;
		robot.act.action = RWALK;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	else if (key == 'W' || key == 'H') {
		prekey = 'W'; check = true;
		robot.act.action = BWALK;
		glutTimerFunc(300, timerFunc, robot.act.action);
	}
	display(); 
}

/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
	robot.init();
	stage.init();
	camera.init();
	pointlit.init();
	spotlit.init();
	dirlit.init();
	glClearColor(23 / 255.0, 17 / 255.0, 30 / 255.0, 1.0);      /*set the background color BLACK */

	/*Clear the Depth & Color Buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */

	glEnable(GL_NORMALIZE);   /*Enable mornalization  */
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_BLEND);		/* Enable blending */


	/* color = src_alpha * src_color + (1-src_alpha) * dst_color */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);

	polyhedron.init();
	pointlit.init();
	/*---- Compute cos(5.0) and sin(5.0) ----*/
	cv = cos(Rad(5.0));
	sv = sin(Rad(5.0));
	/*---- Copy eye position ---*/
	eye[0] = Eye[0];
	eye[1] = Eye[1];
	eye[2] = Eye[2];

	/*-----Define some global lighting status -----*/
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_amb);

	isopen[3] = true;	//turn on the point light
	isopen[4] = true;	//turn on the diraction light
	isopen[5] = true;	//turn on the spot light

	 /*----Set up fog conditions ---*/
	glEnable(GL_FOG);                /*enable fog fade */
	glFogi(GL_FOG_MODE, GL_LINEAR);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
	glFogf(GL_FOG_DENSITY, 0.05);    /*fog opacity(density)= 0.25*/
	glFogf(GL_FOG_START, 30.0);       /*Setup two ends for GL_LINEAR*/
	glFogf(GL_FOG_END, 150.0);
	glFogfv(GL_FOG_COLOR, fog.color);/*set the fog color */


	/*-----Enable face culling -----*/
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);


	glGenTextures(30, textid);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	texture.load_texture("./floor.bmp", 0, 1);
	texture.load_texture("./flower.bmp", 1, 0);
	texture.load_texture("./hair.bmp", 2, 1);
	texture.load_texture("./mat.bmp", 3, 1);
	string path = "./doubledance/", data = ".bmp";
	for (int i = 1; i <= 16; ++i) {
		string id = to_string(i);
		string now = path + id + data;
		const char* str = now.c_str();
		texture.load_texture(str, i + 3, 1);
	}
	path = "./dance/", data = ".bmp";
	for (int i = 1; i <= 9; ++i) {
		string id = to_string(i);
		string now = path + id + data;
		const char* str = now.c_str();
		texture.load_texture(str, i + 19, 1);
	}
	texture.load_texture("./sky.bmp", 29, 1);

	glutTimerFunc(100, timerFunc, DOUBLEDANCE);
	glutTimerFunc(100, timerFunc, DANCE);
	glutTimerFunc(100, timerFunc, SUN);

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

  glutInitWindowSize(width, height);
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