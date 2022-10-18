/******************************************************************
 * This program illustrates the fundamental instructions for handling
 * mouse and keyboeard events as well as menu buttons.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include <GL/glut.h>

#define    SIZEX   2000
#define    SIZEY   2000

#define    MY_SAVE   1
#define    MY_LOAD   2
#define    MY_CLEAR  3
#define    MY_QUIT   4
#define    RIS_LOAD  5
#define    RIS_SAVE  6

#define    CURVE     1
#define    LINE      2
#define    POINT     3
#define    SHAPE     4
#define    TEXT      5

#define	   POLY      0
#define    SOLID_R   1
#define    SOLID_T   2
#define    SOLID_C   3
#define    HOLLOW_R  4
#define    HOLLOW_T  5
#define    HOLLOW_C  6


int        height = 649, width = 1280;
unsigned char  image[SIZEX*SIZEY][4], ris[SIZEX*SIZEY][4], color_image[30000][4];
unsigned char  bg_w[SIZEX*SIZEY][4], bg_b[SIZEX*SIZEY][4], l_w[SIZEX*SIZEY][4], l_b[SIZEX*SIZEY][4];

int        pos_x = -1, pos_y = -1;
float      myColor[3] = { 0.0,0.0,0.0 };
int        vertex[128][2], first = 0, side = 0;;
int		   bg = 1, lines = 0, mod = CURVE, pnt_size = 1;
int		   file = 0, view = 0, counter = 0, color = 0, curve = 0, straight = 0, point = 0, shape = 0, text = 0;
int		   file_light = 0, view_light = 0, shape_light = 0, solid_light = 0, hollow_light = 0;
int		   curve_lines = 0, straight_lines = 0, shape_items = 0, text_items = 0;
bool	   anicolor = false, fir_in = true, can_touch = true;

/*---------------------------------------------------------------
 * color_circle(not precision)
 */
float risColor[3], color_x, color_y;
int colorful;
void color_menu() {
	if (!anicolor) {
		can_touch = false;
		//menu_container
		glColor3f(230 / 255.0, 230 / 255.0, 230 / 255.0);
		glBegin(GL_POLYGON);
		glVertex2i(120, height - 120);
		glVertex2i(120, max(0, height - 120 - 150));
		glVertex2i(120 + 200, max(0,height - 120 - 150));
		glVertex2i(120 + 200, height - 120);
		glEnd();
		glFinish();

		//rgbpy(lb)
		double now[3] = { 127.5, 255.0, 0.0 };
		for (double i = 0; fabsl(i - 360.0) > 1e-6; i += 0.25) {
			//printf("%.2lf: %.4lf, %.4lf, %.4lf\n", i, now[0], now[1], now[2]);
			if (i > 0 && i < 30.0) now[0] += 1.0625;
			else if (fabsl(i - 30.0) <= 1e-6) now[0] = 255.0;
			else if (i > 30 && i < 90) now[1] -= 1.0625;
			else if (fabsl(i - 90) <= 1e-6) now[1] = 0.0;
			else if (i > 90 && i < 150) now[2] += 1.0625;
			else if (fabsl(i - 150) <= 1e-6) now[2] = 255.0;
			else if (i > 150 && i < 210) now[0] -= 1.0625;
			else if (fabsl(i - 210) <= 1e-6) now[0] = 0.0;
			else if (i > 210 && i < 270) now[1] += 1.0625;
			else if (fabsl(i - 270) <= 1e-6) now[1] = 255.0;
			else if (i > 270 && i < 330) now[2] -= 1.0625;
			else if (fabsl(i - 270) <= 1e-6) now[2] = 0.0;
			else if (i > 330 && i < 360) now[1] += 1.0625;
			else if (fabsl(i - 360) <= 1e-6) break;
			glColor3f(now[0] / 255.0, now[1] / 255.0, now[2] / 255.0);
			glBegin(GL_LINES);
			glVertex2f(180.0, (double)height - 180.0);
			glVertex2f(180.0 + 50.0*cos(i*0.017453), height - 180.0 + 50.0*sin(i*0.017453));
			glEnd();
			glFinish();
		}

		glColor3f(255 / 255.0, 245 / 255.0, 245 / 255.0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; ++i) {
			glVertex2f(180.0 + 20.0*cos(i*0.017453), height - 180.0 + 20.0*sin(i*0.017453));
		}
		glEnd();
		glFinish();

		//black,white
		for (double i = 0.0; fabsl(i - 255.0) > (1e-6); i += 1.0) {
			glColor3f(i / 255.0, i / 255.0, i / 255.0);
			glBegin(GL_LINES);
			glVertex2f(135.0 + i / 2, (double)height - 260);
			glVertex2f(135.0 + i / 2, height - 260 - 5);
			glEnd();
			glFinish();
		}

		//RGB text
		glColor3f(0 / 255.0, 0 / 255.0, 0 / 255.0);
		glRasterPos2i(250, height - 150);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'R');
		glRasterPos2i(250, height - 170);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'G');
		glRasterPos2i(250, height - 190);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'B');
		glFinish();

		// RGB num box 
		for (int i = 0; i < 3; ++i) {
			glColor3f(250.0/255.0, 250.0/255.0, 250.0/255.0);
			glBegin(GL_POLYGON);
			glVertex2i(265, height - 138 - 20 * i);
			glVertex2i(265, height - 153 - 20 * i);
			glVertex2i(305, height - 153 - 20 * i);
			glVertex2i(305, height - 138 - 20 * i);
			glEnd();
			glFinish();
		}

		// store close buttonc
		for (int i = 0; i < 2; ++i) {
			glColor3f(240/ 255.0, 240 / 255.0, 240 / 255.0);
			glBegin(GL_POLYGON);
			glVertex2i(215 + 50 * i, height - 230);
			glVertex2i(215 + 50 * i, height - 250);
			glVertex2i(260 + 50 * i, height - 250);
			glVertex2i(260 + 50 * i, height - 230);
			glEnd();
			glFinish();
		}

		// store close color
		char sc[2][5] = { "store","close" };
		glColor3f(100 / 255.0, 100 / 255.0, 100 / 255.0);
		for (int i = 0; i < 2; ++i) {
			glRasterPos2i(217 + i * 50, height - 243);
			for(int j =0;j < 5;++j)
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, sc[i][j]);
			glFinish();
		}
		
		anicolor = true;
		glReadPixels(120, height - 120 - 150, 200, 150, GL_RGBA, GL_UNSIGNED_BYTE, color_image);

		can_touch = true;
	}
	else {
		glRasterPos2i(120, height - 120 -150);
		glDrawPixels(200, 150, GL_RGBA, GL_UNSIGNED_BYTE, color_image);
	}

	//draw the touch on color plate
	glColor3f(255 / 255.0, 182 / 255.0, 182 / 255.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; ++i) {
		glVertex2f(color_x + 5.0*cos(i*0.017453), color_y + 5.0*sin(i*0.017453));
	}
	glEnd();
	glFinish();

	//ris color
	glColor3f(risColor[0], risColor[1], risColor[2]);
	glBegin(GL_POLYGON);
	glVertex2i(250, height - 200);
	glVertex2i(250, height - 220);
	glVertex2i(300, height - 220);
	glVertex2i(300, height - 200);
	glEnd();
	glFinish();

	//ris color num
	glColor3f(0 / 255.0, 0 / 255.0, 0 / 255.0);
	for (int i = 0; i < 3; ++i) {
		int c = floor(risColor[i]*255.0);
		char num[4];
		glRasterPos2i(278, height - 150 - 20 * i);
		_itoa(c, num, 10);
		for(int i =0;num[i]!='\0';++i)
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, num[i]);
		glFinish();
	}

	int state = (risColor[0] == risColor[1] && risColor[2] == risColor[2] ? 2 : 1);

}

/*------------------------------------------------------------
 * shape menu
 */
void shape_menu() {
	int wid, hei, sx, num;
	wid = 60;
	hei = 45;
	num = 3;
	sx = 298;
	//menu_container
	glColor3f(210 / 255.0, 210 / 255.0, 210 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2i(sx, height - 25);
	glVertex2i(sx, height - 25 - hei);
	glVertex2i(sx + wid, height - 25 - hei);
	glVertex2i(sx + wid, height - 25);
	glEnd();
	glFinish();

	//menu_item_checked
	if (shape_light) {
		glColor3f(225 / 255.0, 225 / 255.0, 225 / 255.0);
		glBegin(GL_POLYGON);
		glVertex2i(sx, height - 25 - (shape_light - 1) * 15);
		glVertex2i(sx, max(0, height - 25 - shape_light * 15));
		glVertex2i(sx + wid, height - 25 - shape_light * 15);
		glVertex2i(sx + wid, height - 25 - (shape_light - 1) * 15);
		glEnd();
		glFinish();
	}

	//cutline
	glColor3f(152 / 255.0, 156 / 255.0, 163 / 255.0);
	glPointSize(1);
	glBegin(GL_POINTS);
	for (int i = 15; i < hei; i += 15) {
		for (int j = sx + 2; j < sx + wid; j += 5) {
			glVertex2i(j, height - 25 - i);
		}
	}
	glEnd();
	glFinish();

	char str1[3][7] = { "poly\n","solid\n","hollow\n" };
	for (int i = 0; i < 3; ++i) {
		glRasterPos2i(sx + 5, height - 20 - 15 * (i + 1));
		glColor3f(138 / 255.0, 142 / 255.0, 148 / 255.0);
		for (int j = 0; str1[i][j] != '\n'; ++j) {
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str1[i][j]);
		}
	}
	glFinish();

	if (shape_light==2) {
		//contianer
		glColor3f(210 / 255.0, 210 / 255.0, 210 / 255.0);
		glBegin(GL_POLYGON);
		glVertex2i(sx + 56, height - 45);
		glVertex2i(sx + 56, height - 45 - hei);
		glVertex2i(sx + 56 + wid + 20, height - 45 - hei);
		glVertex2i(sx + 56 + wid + 20, height - 45);
		glEnd();
		glFinish();

		//menu_item_checked
		if (solid_light) {
			glColor3f(225 / 255.0, 225 / 255.0, 225 / 255.0);
			glBegin(GL_POLYGON);
			glVertex2i(sx + 56, height - 45 - (solid_light - 1) * 15);
			glVertex2i(sx + 56, height - 45 - solid_light * 15);
			glVertex2i(sx + 56 + wid + 20, height - 45 - solid_light * 15);
			glVertex2i(sx + 56 + wid + 20, height - 45 - (solid_light - 1) * 15);
			glEnd();
			glFinish();
		}

		//cutline
		glColor3f(152 / 255.0, 156 / 255.0, 163 / 255.0);
		glPointSize(1);
		glBegin(GL_POINTS);
		for (int i = 15; i < hei; i += 15) {
			for (int j = sx + 56 + 2; j < sx + wid + 20; j += 5) {
				glVertex2i(j, height - 45 - i);
			}
		}
		glEnd();
		glFinish();

		char str2[3][10] = { "rectangle\n","triangle\n","circle\n" };
		for (int i = 0; i < 3; ++i) {
			glRasterPos2i(sx + 56 + 5, height - 40 - 15 * (i + 1));
			glColor3f(138 / 255.0, 142 / 255.0, 148 / 255.0);
			for (int j = 0; str2[i][j] != '\n'; ++j) {
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str2[i][j]);
			}
		}
		glFinish();
	}
	if (shape_light == 3) {
		//contianer
		glColor3f(210 / 255.0, 210 / 255.0, 210 / 255.0);
		glBegin(GL_POLYGON);
		glVertex2i(sx + 56, height - 60);
		glVertex2i(sx + 56, height - 60 - hei);
		glVertex2i(sx + 56 + wid + 20, height - 60 - hei);
		glVertex2i(sx + 56 + wid + 20, height - 60);
		glEnd();
		glFinish();

		//menu_item_checked
		if (hollow_light) {
			glColor3f(225 / 255.0, 225 / 255.0, 225 / 255.0);
			glBegin(GL_POLYGON);
			glVertex2i(sx + 56, height - 60 - (hollow_light - 1) * 15);
			glVertex2i(sx + 56, height - 60 - hollow_light * 15);
			glVertex2i(sx + 56 + wid + 20, height - 60 - hollow_light * 15);
			glVertex2i(sx + 56 + wid + 20, height - 60 - (hollow_light - 1) * 15);
			glEnd();
			glFinish();
		}

		//cutline
		glColor3f(152 / 255.0, 156 / 255.0, 163 / 255.0);
		glPointSize(1);
		glBegin(GL_POINTS);
		for (int i = 15; i < hei; i += 15) {
			for (int j = sx + 56 + 2; j < sx + wid + 20; j += 5) {
				glVertex2i(j, height - 60 - i);
			}
		}
		glEnd();
		glFinish();

		char str2[3][10] = { "rectangle\n","triangle\n","circle\n" };
		for (int i = 0; i < 3; ++i) {
			glRasterPos2i(sx + 56 + 5, height - 55 - 15 * (i + 1));
			glColor3f(138 / 255.0, 142 / 255.0, 148 / 255.0);
			for (int j = 0; str2[i][j] != '\n'; ++j) {
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str2[i][j]);
			}
		}
		glFinish();
	}
}

/*---------------------------------------------------------------
 * size_icon
 */
void size_icon(int pass) {
	//background for this icon
	glColor3f(232 / 255.0, 246 / 255.0, 255 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2i(1, height - 20);
	glVertex2i(1, height - 70);
	glVertex2i(49, height - 70);
	glVertex2i(49, height - 20);
	glEnd();
	glFinish();

	//size text
	char str[4] = "size";
	glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
	glRasterPos2i(8, height - 35);
	for (int i = 0; i < 4; ++i)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	//size num
	char cnt[2];
	_itoa(pnt_size, cnt, 10);
	glRasterPos2i(21, height - 60);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, cnt[0]);
	glFinish();

	if (pass == 2) {
		glColor3f(180 / 255.0, 192 / 255.0, 227 / 255.0);
		glBegin(GL_TRIANGLES);
		glVertex2i(44, height - 55);
		glVertex2i(33, height - 48);
		glVertex2i(33, height - 62);
		glEnd();
		glFinish();
	}

	if (pass == 1) {
		glColor3f(180 / 255.0, 192 / 255.0, 227 / 255.0);
		glBegin(GL_TRIANGLES);
		glVertex2i(3, height - 55);
		glVertex2i(14, height - 48);
		glVertex2i(14, height - 62);
		glEnd();
		glFinish();
	}

	//right
	if (pnt_size < 9) {
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glBegin(GL_TRIANGLES);
		glVertex2i(42, height - 55);
		glVertex2i(35, height - 51);
		glVertex2i(35, height - 59);
		glEnd();
		glFinish();
	}

	//left
	if (pnt_size > 1) {
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glBegin(GL_TRIANGLES);
		glVertex2i(5, height - 55);
		glVertex2i(12, height - 51);
		glVertex2i(12, height - 59);
		glEnd();
		glFinish();
	}
	/*three size of line
	for (int i = 1; i <= 3; ++i) {
		touch ?
			glColor4f(174 / 255.0, 210 / 255.0, 243 / 255.0, 1.0):
			glColor4f(37 / 255.0, 65 / 255.0, 212 / 255.0, 1.0);
		glLineWidth(i);
		glBegin(GL_LINES);
		glVertex2i(10, height - i * 10 - 5);
		glVertex2i(40, height - (i + 1) * 10 - 5);
		glEnd();
		glFinish();
	}*/


}

/*---------------------------------------------------------------
 * color_icon:any curve line
 */
void color_icon(int pass) {
	//background for this icon
	pass?
		glColor3f(180 / 255.0, 192 / 255.0, 227 / 255.0):
		glColor3f(232 / 255.0, 246 / 255.0, 255 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2i(51, height - 20);
	glVertex2i(51, height - 70);
	glVertex2i(99, height - 70);
	glVertex2i(99, height - 20);
	glEnd();
	glFinish();

	//size text
	char str[5] = "color";
	glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
	glRasterPos2i(56, height - 35);
	for (int i = 0; i < 5; ++i)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	glFinish();

	glColor3f(myColor[0], myColor[1], myColor[2]);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; ++i) {
		glVertex2f(75 + 10.0*cos(i*0.017453), height - 55 + 10.0*sin(i*0.017453));
	}
	glEnd();
	glFinish();
}

/*---------------------------------------------------------------
 * curve_icon
 */
void curve_icon(int state) {
	if (state == 1)//pass
		glColor3f(180 / 255.0, 192 / 255.0, 227 / 255.0);
	else if (mod == 1)//use
		glColor3f(141 / 255.0, 191 / 255.0, 235 / 255.0);
	else if (state == 0)//normal
		glColor3f(232 / 255.0, 246 / 255.0, 255 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2i(101, height - 20);
	glVertex2i(101, height - 70);
	glVertex2i(149, height - 70);
	glVertex2i(149, height - 20);
	glEnd();
	glFinish();

	//size text
	char str[5] = "curve";
	glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
	glRasterPos2i(106, height - 35);
	for (int i = 0; i < 5; ++i)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	glFinish();

	if (!curve_lines) {
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glPointSize(1);
		glBegin(GL_POINTS);
		for (int i = 0; i < 360; ++i) {
			glVertex2f(125 + (3 + 4 / 360.0 * i) *cos(i*0.017453), height - 55 + (3 + 4 / 360.0 * i) * sin(i*0.017453));
		}
		for (int i = 0; i < 360; ++i) {
			glVertex2f(125 + (7 + 5 / 360.0 * i) *cos(i*0.017453), height - 55 + (7 + 5 / 360.0 * i) * sin(i*0.017453));
		}
		glEnd();
		glFinish();
	}
	else {
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glPointSize(1);
		glBegin(GL_POINTS);
		for (int i = 0; i < 360; i+=23) {
			glVertex2f(125 + (3 + 4 / 360.0 * i) *cos(i*0.017453), height - 55 + (3 + 4 / 360.0 * i) * sin(i*0.017453));
		}
		for (int i = 0; i < 360; i+=17) {
			glVertex2f(125 + (7 + 5 / 360.0 * i) *cos(i*0.017453), height - 55 + (7 + 5 / 360.0 * i) * sin(i*0.017453));
		}
		glEnd();
		glFinish();
	}

}

/*---------------------------------------------------------------
 * straight_icon
 */
void straight_icon(int state) {
	if (state == 1)//pass
		glColor3f(180 / 255.0, 192 / 255.0, 227 / 255.0);
	else if (mod == 2)//use
		glColor3f(141 / 255.0, 191 / 255.0, 235 / 255.0);
	else if (state == 0)//normal
		glColor3f(232 / 255.0, 246 / 255.0, 255 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2i(151, height - 20);
	glVertex2i(151, height - 70);
	glVertex2i(199, height - 70);
	glVertex2i(199, height - 20);
	glEnd();
	glFinish();

	//size text
	char str[4] = "line";
	glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
	glRasterPos2i(158, height - 35);
	for (int i = 0; i < 4; ++i)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	glFinish();

	if (straight_lines == 0) {//line
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(160, height - 55);
		glVertex2i(190, height - 55);
		glEnd();
		glFinish();
	}
	else if (straight_lines == 1){//short line line
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		for (int i = 0; i < 3; ++i) {
			glVertex2i(163 + i * 10, height - 55);
			glVertex2i(168 + i * 10, height - 55);
		}
		glEnd();
		glFinish();
	}
	else if (straight_lines == 2) {//point line
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glPointSize(1);
		glBegin(GL_POINTS);
		for (int i = 0; i < 10; ++i) {
			glVertex2i(160 + i * 3, height - 55);
		}
		glEnd();
		glFinish();
	}
}

/*---------------------------------------------------------------
 * straight_icon
 */
void point_icon(int state) {
	//if use no other module can change,so mod -> state 
	if (mod == 3)//use
		glColor3f(141 / 255.0, 191 / 255.0, 235 / 255.0);
	else if (state == 1)//pass
		glColor3f(180 / 255.0, 192 / 255.0, 227 / 255.0);
	else if (state == 0)//normal
		glColor3f(232 / 255.0, 246 / 255.0, 255 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2i(201, height - 20);
	glVertex2i(201, height - 70);
	glVertex2i(249, height - 70);
	glVertex2i(249, height - 20);
	glEnd();
	glFinish();

	//size text
	char str[5] = "point";
	glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
	glRasterPos2i(206, height - 35);
	for (int i = 0; i < 5; ++i)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	glFinish();

	glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
	glPointSize(3);
	glBegin(GL_POINTS);
	glVertex2i(225, height - 55);
	glEnd();
	glFinish();
}

/*---------------------------------------------------------------
 * straight_icon
 */
void shape_icon(int state) {
	if (state == 1)//pass
		glColor3f(180 / 255.0, 192 / 255.0, 227 / 255.0);
	else if (mod == 4)//use
		glColor3f(141 / 255.0, 191 / 255.0, 235 / 255.0);
	else if (state == 0)//normal
	glColor3f(232 / 255.0, 246 / 255.0, 255 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2i(251, height - 20);
	glVertex2i(251, height - 70);
	glVertex2i(299, height - 70);
	glVertex2i(299, height - 20);
	glEnd();
	glFinish();

	//size text
	char str[5] = "shape";
	glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
	glRasterPos2i(256, height - 35);
	for (int i = 0; i < 5; ++i)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	glFinish();

	if (shape_items == POLY) {
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(255, height - 60);
		glVertex2i(265, height - 50);
		glVertex2i(265, height - 50);
		glVertex2i(295, height - 50);
		glVertex2i(295, height - 50);
		glVertex2i(285, height - 60);
		glVertex2i(285, height - 60);
		glVertex2i(255, height - 60);
		glEnd();
		glFinish();

		glColor3f(myColor[0], myColor[1], myColor[2]);
		glBegin(GL_POLYGON);
		glVertex2i(270, height - 50);
		glVertex2i(280, height - 50);
		glVertex2i(275, height - 40);
		glEnd();
		glFinish();

		glColor3f(myColor[0], myColor[1], myColor[2]);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; ++i) {
			glVertex2f(275 + 5*cos(i*0.017453), height - 50 + 5.0*sin(i*0.017453));
		}
		glEnd();
		glFinish();
	}
	if (shape_items == SOLID_R){
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glLineWidth(1);
		glBegin(GL_POLYGON);
		glVertex2i(260, height - 65);
		glVertex2i(260, height - 45);
		glVertex2i(290, height - 45);
		glVertex2i(290, height - 65);
		glEnd();
		glFinish();
	}
	if (shape_items == SOLID_T) {
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glBegin(GL_POLYGON);
		glVertex2i(265, height - 65);
		glVertex2i(285, height - 65);
		glVertex2i(275, height - 45);
		glEnd();
		glFinish();
	}
	if (shape_items == SOLID_C) {
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; ++i) {
			glVertex2f(275 + 10.0 * cos(i*0.017453), height - 55 + 10.0*sin(i*0.017453));
		}
		glEnd();
		glFinish();
	}
	if (shape_items == HOLLOW_R) {
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(260, height - 65);
		glVertex2i(260, height - 45);
		glVertex2i(260, height - 45);
		glVertex2i(290, height - 45);
		glVertex2i(290, height - 45);
		glVertex2i(290, height - 65);
		glVertex2i(290, height - 65);
		glVertex2i(260, height - 65);
		glEnd();
		glFinish();
	}
	if (shape_items == HOLLOW_T) {
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(265, height - 65);
		glVertex2i(285, height - 65);
		glVertex2i(285, height - 65);
		glVertex2i(275, height - 45);
		glVertex2i(275, height - 45);
		glVertex2i(265, height - 65);
		glEnd();
		glFinish();
	}
	if (shape_items == HOLLOW_C) {
		glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
		glPointSize(1);
		glBegin(GL_POINTS);
		for (int i = 0; i < 360; ++i) {
			glVertex2d(275 + 10.0 * cos(i*0.017453), height - 55 + 10.0*sin(i*0.017453));
		}
		glEnd();
		glFinish();
	}
}

/*---------------------------------------------------------------
 * text_icon
 */
void text_icon(int state) {
	if (state == 1)//pass
		glColor3f(180 / 255.0, 192 / 255.0, 227 / 255.0);
	if (mod == TEXT)//use
		glColor3f(141 / 255.0, 191 / 255.0, 235 / 255.0);
	else if (state == 0)//normal
		glColor3f(232 / 255.0, 246 / 255.0, 255 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2i(301, height - 20);
	glVertex2i(301, height - 70);
	glVertex2i(349, height - 70);
	glVertex2i(349, height - 20);
	glEnd();
	glFinish();

	//size text
	char str[4] = "text";
	glColor3f(37 / 255.0, 65 / 255.0, 212 / 255.0);
	glRasterPos2i(310, height - 35);
	for (int i = 0; i < 4; ++i)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	glRasterPos2i(315, height - 60);
	text_items? 
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A'):glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'A');
	glRasterPos2i(325, height - 60);
	text_items ?
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a'):glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'a');
	glFinish();
	glFinish();
}

/*---------------------------------------------------------------
 * any operation for file
 */
void file_icon(int pass) {
	//background for this icon
	pass ?
		glColor3f(180 / 255.0, 192 / 255.0, 227 / 255.0) :
		glColor3f(213 / 255.0, 227 / 255.0, 235 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2i(0, height);
	glVertex2i(0, height - 19);
	glVertex2i(49, height - 19);
	glVertex2i(49, height);
	glEnd();
	glFinish();

	//size text
	char str[4] = "file";
	glColor3f(0 / 255.0, 0 / 255.0, 0 / 255.0);
	glRasterPos2i(8, height - 13);
	for (int i = 0; i < 4; ++i)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	glFinish();
}

/*---------------------------------------------------------------
 * any operation for file
 */
void view_icon(int pass) {
	//background for this icon
	pass ?
		glColor3f(180 / 255.0, 192 / 255.0, 227 / 255.0) :
		glColor3f(213 / 255.0, 227 / 255.0, 235 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2i(50, height);
	glVertex2i(50, height - 19);
	glVertex2i(99, height - 19);
	glVertex2i(99, height);
	glEnd();
	glFinish();

	//size text
	char str[4] = "view";
	glColor3f(0 / 255.0, 0 / 255.0, 0 / 255.0);
	glRasterPos2i(58, height - 13);
	for (int i = 0; i < 4; ++i)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	glFinish();
}

/*------------------------------------------------------------
 * Callback function for view_menu.
 */
void view_func(int value) {
	printf("%d %d %d\n", value, lines, bg);
	if (value == 1) {//modify line
		if (lines) {//noline -> line
			if (bg) {//white
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < SIZEX; j++) {
						for (int k = 0; k < SIZEY; ++k) {
							if (image[j * SIZEX + k][i] == bg_w[j * SIZEX + k][i])
								image[j * SIZEX + k][i] = l_w[j * SIZEX + k][i];
							if (ris[j * SIZEX + k][i] == bg_w[j * SIZEX + k][i])
								ris[j * SIZEX + k][i] = l_w[j * SIZEX + k][i];
						}
					}
				}
			}
			else {//black
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < SIZEX; j++) {
						for (int k = 0; k < SIZEY; ++k) {
							if (image[j * SIZEX + k][i] == bg_b[j * SIZEX + k][i])
								image[j * SIZEX + k][i] = l_b[j * SIZEX + k][i];
							if (ris[j * SIZEX + k][i] == bg_b[j * SIZEX + k][i])
								ris[j * SIZEX + k][i] = l_b[j * SIZEX + k][i];
						}
					}
				}
			}
		}
		else {
			if (bg) {//white
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < SIZEX; j++) {
						for (int k = 0; k < SIZEY; ++k) {
							if (image[j * SIZEX + k][i] == l_w[j * SIZEX + k][i])
								image[j * SIZEX + k][i] = bg_w[j * SIZEX + k][i];
							if (ris[j * SIZEX + k][i] == l_w[j * SIZEX + k][i])
								ris[j * SIZEX + k][i] = bg_w[j * SIZEX + k][i];
						}
					}
				}
			}
			else {
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < SIZEX; j++) {
						for (int k = 0; k < SIZEY; ++k) {
							if (image[j * SIZEX + k][i] == l_b[j * SIZEX + k][i])
								image[j * SIZEX + k][i] = bg_b[j * SIZEX + k][i];
							if (ris[j * SIZEX + k][i] == l_b[j * SIZEX + k][i])
								ris[j * SIZEX + k][i] = bg_b[j * SIZEX + k][i];
						}
					}
				}
			}
		}
	}
	if (value == 2) {//bg
		if (bg) {//white
			if (lines) {//l_b
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < SIZEX; ++j) {
						for (int k = 0; k < SIZEY; ++k) {
							if (image[j * SIZEX + k][i] == l_b[j * SIZEX + k][i])
								image[j * SIZEX + k][i] = l_w[j * SIZEX + k][i];
							if (ris[j * SIZEX + k][i] == l_b[j * SIZEX + k][i])
								ris[j * SIZEX + k][i] = l_w[j * SIZEX + k][i];
						}
					}
				}
			}
			else {
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < SIZEX; ++j) {
						for (int k = 0; k < SIZEY; ++k) {
							if (image[j * SIZEX + k][i] == bg_b[j * SIZEX + k][i])
								image[j * SIZEX + k][i] = bg_w[j * SIZEX + k][i];
							if (ris[j * SIZEX + k][i] == bg_b[j * SIZEX + k][i])
								ris[j * SIZEX + k][i] = bg_w[j * SIZEX + k][i];
						}
					}
				}
			}
		}
		else {//black
			if (lines) {
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < SIZEX; ++j) {
						for (int k = 0; k < SIZEY; ++k) {
							if (image[j * SIZEX + k][i] == l_w[j * SIZEX + k][i])
								image[j * SIZEX + k][i] = l_b[j * SIZEX + k][i];
							if (ris[j * SIZEX + k][i] == l_w[j * SIZEX + k][i])
								ris[j * SIZEX + k][i] = l_b[j * SIZEX + k][i];
						}
					}
				}
			}
			else {
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < SIZEX; ++j) {
						for (int k = 0; k < SIZEY; ++k) {
							if (image[j * SIZEX + k][i] == bg_w[j * SIZEX + k][i])
								image[j * SIZEX + k][i] = bg_b[j * SIZEX + k][i];
							if (ris[j * SIZEX + k][i] == bg_w[j * SIZEX + k][i])
								ris[j * SIZEX + k][i] = bg_b[j * SIZEX + k][i];
						}
					}
				}
			}
		}
	}
}

/*-------------------------------------------------------------
 * reshape callback function for bar.
 */
void init_bar() {
	//topbar_forsave
	glColor3f(213 / 255.0, 227 / 255.0, 235 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2i(0, height);
	glVertex2i(0, height - 20);
	glVertex2i(width, height - 20);
	glVertex2i(width, height);
	glEnd();
	glFinish();

	//bar cut lines
	glColor3f(180 / 255.0, 204 / 255.0, 212 / 255.0);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2i(0, height - 20);
	glVertex2i(width, height - 20);
	glEnd();
	glFinish();

	//topbar_container
	glColor3f(232 / 255.0, 246 / 255.0, 255 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2i(20, height - 20);
	glVertex2i(20, height - 70);
	glVertex2i(width, height - 70);
	glVertex2i(width, height - 20);
	glEnd();
	glFinish();

	//top item
	file_icon(0);
	view_icon(0);

	//cut_line
	glColor3f(37 / 255.0, 80 / 255.0, 250 / 255.0);
	glPointSize(1);
	glBegin(GL_POINTS);
	for (int j = 1; j <= 7; ++j) {
		for (int i = 3; i < 50; i += 5)
			glVertex2i(50 * j, height - i - 20);
	}
	glEnd();
	glFinish();

	//mid items
	size_icon(0);
	color_icon(0);
	curve_icon(0);
	straight_icon(0);
	point_icon(0);
	shape_icon(0);
	text_icon(0);
}

/*--------------------------------------------------------
 * procedure to clear window
 */
void init_window(int read){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double)width, 0.0, (double)height);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.05, 0.05, 0.08, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	if (read == 2 && !bg && !lines) {
		init_bar();
		glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, ris);
		glFinish();glFlush();
		return;
	}
	glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, bg_b);
	glColor3f(0.95, 0.95, 0.98);
	glBegin(GL_LINES);
	for (int i = 20; i < width; i += 20) {
		glVertex2i(i, 0);
		glVertex2i(i, height - 70);
	}
	for (int i = 20; i < height; i += 20) {
		glVertex2i(0, i);
		glVertex2i(width, i);
	}
	glEnd();
	glFinish();
	if (read == 2 && !bg && lines) {
		init_bar();
		glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, ris);
		glFinish(); glFlush();
		return;
	}
	glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, l_b);
	glClearColor(0.95, 0.95, 0.98, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	if (read == 2 && bg && !lines) {
		init_bar();
		glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, ris);
		glFinish(); glFlush();
		return;
	}
	glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, bg_w);
	glColor3f(0.05, 0.05, 0.08);
	glBegin(GL_LINES);
	for (int i = 20; i < width; i += 20) {
		glVertex2i(i, 0);
		glVertex2i(i, height - 70);
	}
	for (int i = 20; i < height; i += 20) {
		glVertex2i(0, i);
		glVertex2i(width, i);
	}
	glEnd();
	glFinish();
	if (read == 2 && bg && lines) {
		init_bar();
		glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, ris);
		glFinish(); glFlush();
		return;
	}
	glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, l_w);
	if (fir_in) {
		glClearColor(0.95, 0.95, 0.98, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		init_bar();
		glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, ris);
	}
	glFinish();
	glFlush();
}

/*---------------------------------------------------------------
 * reshape the window
 */
void my_reshape(int new_w, int new_h)
{
	int oh = height;
	height = new_h;
	width = new_w;
	init_window(fir_in);
	if(!fir_in){
		glRasterPos2i(0, 0);
		glDrawPixels(SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, ris);
		if (oh < height) {
			printf("\npos%d %d\n", height, oh);
			glColor3f(0.95, 0.95, 0.98);
			glBegin(GL_POLYGON);
			glVertex2i(0, height);
			glVertex2i(0, oh * 2 - height -70);
			glVertex2i(width, oh * 2 - height -70);
			glVertex2i(width, height);
			glEnd();
			glFinish();
		}
		
		init_bar();
		glFinish();
		glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, ris);
	}
	if (color == 2) color_menu(color_x, color_y);
	fir_in = false;
	glutPostRedisplay();   /*---Trigger Display event for redisplay window*/
}

/*------------------------------------------------------------
 * Callback function for file_menu.
 */
void file_func(int value)
{
	int i, j;
	//for image 0 ,0 is the left-top pos
	if (value == MY_QUIT) exit(0);
	else if (value == MY_CLEAR) init_window(2);
	else if (value == MY_SAVE) { /* Save current window */
		glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	else if (value == MY_LOAD) { /* Restore the saved image */
		glRasterPos2i(0, 0);
		glDrawPixels(SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, image);
		view_func(0);view_func(1);
		glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, ris);
	}
	else if (value == RIS_SAVE) {
		glReadPixels(0, 0, SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, ris);
	}
	else if (value == RIS_LOAD) {
		glRasterPos2i(0, 0);
		glDrawPixels(SIZEX, SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, ris);
	}
	glFlush();
}

/*------------------------------------------------------------
 * Callback function for display, redisplay, expose events
 * Just clear the window again
 */
void display_func(void)
{
	glFlush();
}

/*--------------------------------------------------------------
 * Callback function for keyboard event.
 * key = the key pressed,
 * (x,y)= position in the window, where the key is pressed.
 */
void keyboard(unsigned char key, int x, int y)
{
	//esc
	if (key == 27) exit(0);
	if (mod == TEXT && pos_x != -1 && pos_y != -1) {
		if (isprint(key)) {
			glColor3f(myColor[0],myColor[1],myColor[2]);
			glRasterPos2i(pos_x, height - pos_y);
			text_items?
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, key) : glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, key);
			glFinish();
			file_func(RIS_SAVE);
			pos_x += 8;
		}
	}
}

/*---------------------------------------------------------
 * Procedure to draw a polygon
 */
void draw_polygon()
{
	int  i;
	glColor3f(myColor[0], myColor[1], myColor[2]);
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
	static GLUquadricObj *mycircle = NULL;
	glColor3f(myColor[0], myColor[1], myColor[2]);
	if (mycircle == NULL) {
		mycircle = gluNewQuadric();
		gluQuadricDrawStyle(mycircle, GLU_FILL);
	}
	glPushMatrix();
	glTranslatef(pos_x, height - pos_y, 0.0);
	gluDisk(mycircle,
		0.0,           /* inner radius=0.0 */
		10.0,          /* outer radius=10.0 */
		16,            /* 16-side polygon */
		3);
	glPopMatrix();
}

/*------------------------------------------------------------
 * top_bar_menu:to call the any size of menu in mid bar
 */
void top_bar_menu(int choose, int state) {
	if (state == 0) {
		file_func(RIS_LOAD);
		if(choose == 1)file_icon(file_light = file = 0);
		if(choose == 2)view_icon(view_light = view = 0);
		return;
	}
	if (state == 1) {
		file_func(RIS_SAVE);
	}
	if(choose == 1)file_icon(file = 1);
	if(choose == 2)view_icon(view = 1);

	int wid, hei, sx, num;
	if (choose == 1) {
		wid = 100;
		hei = 60;
		num = 4;
		sx = 48;
	}
	else if (choose == 2) {
		wid = 100;
		hei = 30;
		num = 2;
		sx = 98;
	}
	//menu_container
	glColor3f(210 / 255.0, 210 / 255.0, 210 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2i(sx, height - 5);
	glVertex2i(sx, max(0, height - 5 - hei));
	glVertex2i(sx + wid, height - 5 - hei);
	glVertex2i(sx + wid, height - 5);
	glEnd();
	glFinish();

	//menu_item_checked
	if (file_light) {
		glColor3f(225 / 255.0, 225 / 255.0, 225 / 255.0);
		glBegin(GL_POLYGON);
		glVertex2i(sx, height - 5 - (file_light - 1) * 15);
		glVertex2i(sx, max(0, height - 5 - file_light * 15));
		glVertex2i(sx + wid, height - 5 - file_light * 15);
		glVertex2i(sx + wid, height - 5 - (file_light - 1) * 15);
		glEnd();
		glFinish();
	}

	if (view_light) {
		glColor3f(225 / 255.0, 225 / 255.0, 225 / 255.0);
		glBegin(GL_POLYGON);
		glVertex2i(sx, height - 5 - (view_light - 1) * 15);
		glVertex2i(sx, max(0, height - 5 - view_light * 15));
		glVertex2i(sx + wid, height - 5 - view_light * 15);
		glVertex2i(sx + wid, height - 5 - (view_light - 1) * 15);
		glEnd();
		glFinish();
	}

	//menu_cutline
	glColor3f(152 / 255.0, 156 / 255.0, 163 / 255.0);
	glPointSize(1);
	glBegin(GL_POINTS);
	for (int i = 15; i < hei; i += 15) {
		for (int j = sx + 2; j < sx + wid; j += 5) {
			glVertex2i(j, height - 5 - i);
		}
	}
	glEnd();
	glFinish();

	//file
	if (choose == 1) {
		char str[4][15] = {"save\n","load\n","clear\n","quit   (Esc)\n"};
		for (int i = 0; i < 4; ++i) {
			glRasterPos2i(sx + 5, height - 15 * (i + 1));
			glColor3f(138 / 255.0, 142 / 255.0, 148 / 255.0);
			for (int j = 0; str[i][j] != '\n'; ++j) {
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i][j]);
			}
		}
	}
	else if (choose == 2) {
		//char str[4][2][8] = { { "no line\n","black\n" }, { "line\n","black\n" }, { "no line\n","white\n" }, { "line\n","white\n" } };
		char str[4][8] = {"no line\n","line\n","black\n","white\n"};

		glColor3f(138 / 255.0, 142 / 255.0, 148 / 255.0);
		glRasterPos2i(sx + 5, height - 15);
		for (int j = 0; str[lines][j] != '\n'; ++j) {
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[lines][j]);
		}
		glRasterPos2i(sx + 5, height - 30);
		for (int j = 0; str[2 + bg][j] != '\n'; ++j) {
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[2 + bg][j]);
		}
	}
	glFinish();
}

/*------------------------------------------------------------
 * mid_bar_menu:to call the any size of menu in mid bar
 */
void mid_bar_menu(int choose, int state) {
	if (state == 0) {
		file_func(RIS_LOAD);
		if (choose == 2) {
			color_icon(0);
			return;
		}
		else if (choose == 1) {
			size_icon(0);
			return;
		}
		else if (choose == 3) {
			curve_icon(0);
			return;
		}
		else if (choose == 4) {
			straight_icon(0);
			return;
		}
		else if (choose == 5) {
			point_icon(0);
			return;
		}
		else if (choose == 6) {
			shape_icon(0);
			return;
		}
		else if (choose == 7) {
			text_icon(0);
			return;
		}
	}
	else if (state == 1) {
		if (choose == 2) {
			file_func(RIS_SAVE);
			color_icon(1);
			return;
		}
		else if (choose == 1) {
			file_func(RIS_SAVE);
			size_icon(1);
			return;
		}
		else if (choose == 3) {
			file_func(RIS_SAVE);
			curve_icon(1);
			return;
		}
		else if (choose == 4) {
			file_func(RIS_SAVE);
			straight_icon(1);
			return;
		}
		else if (choose == 5) {
			file_func(RIS_SAVE);
			point_icon(1);
			return;
		}
		else if (choose == 6) {
			file_func(RIS_SAVE);
			shape_icon(1);
		}
		else if (choose == 7) {
			file_func(RIS_SAVE);
			text_icon(1);
			return;
		}
	}
	else if (state == 2) {
		if (choose == 1) {
			file_func(RIS_SAVE);
			size_icon(2);
			return;
		}
	}
	if (choose == 2){
		color_icon(0);
		if (colorful) {
			double angle;
			if (fabsl(risColor[1] - 1.0) <= 1e-6 && fabsl(risColor[2] - 0.0) <= 1e-6) {
				angle = risColor[0] * 255.0 / 4.25 + 330;
				if (angle - 360.0 >= 0) angle -= 360;
			}
			else if (fabsl(risColor[0] - 1.0) <= 1e-6 && fabsl(risColor[2] - 0.0) <= 1e-6) {
				angle = 90 - risColor[1] * 255.0 / 4.25;
			}
			else if (fabsl(risColor[0] - 1.0) <= 1e-6 && fabsl(risColor[1] - 0.0) <= 1e-6) {
				angle = risColor[2] * 255.0 / 4.25 + 90;
			}
			else if (fabsl(risColor[2] - 1.0) <= 1e-6 && fabsl(risColor[1] - 0.0) <= 1e-6) {
				angle = 210 - risColor[0] * 255.0 / 4.25;
			}
			else if (fabsl(risColor[2] - 1.0) <= 1e-6 && fabsl(risColor[0] - 0.0) <= 1e-6) {
				angle = risColor[1] * 255.0 / 4.25 + 210;
			}
			else if (fabsl(risColor[1] - 1.0) <= 1e-6 && fabsl(risColor[0] - 0.0) <= 1e-6) {
				angle = 330 - risColor[2] * 255.0 / 4.25;
			}
			//printf("angle %f", angle);
			angle *= 3.1415926 / 180.0;
			color_menu(color_x = (30 * cos(angle) + 180), color_y = height - (180 - 30 *sin(angle)));
		}
		else {
			color_menu(color_x = 135 + (floor)(risColor[0] * 255.0)/2, color_y = height - 263);
		}
	}
	if (choose == 6 && mod == 4) shape_menu();
}

/*------------------------------------------------------------
 * Callback function handling mouse-press events
 */
void mouse_func(int button, int state, int x, int y)
{
	//printf("%d %d\n", x, y);//lsft-button
	if (!can_touch) return;
	if (color == 2) {
		if (state == GLUT_UP) {
			//in colorful
			double r = ceil(sqrt((double)((x - 180) * (x - 180) + (y - 180) * (y - 180))));
			if ( r <= 45.0 && r >= 25.0) {
				if (!colorful) colorful = 1;
				//angle;
				double angle = acos((double)(x - 180) / r) * 180 / 3.1415926;
				if (y - 180 > 0) angle = 360 - angle;
				printf("%f", angle);
				//printf("angle : %lf\n", angle);
				if (angle > 330.0 || angle < 30.0) {
					risColor[1] = 1.0;
					risColor[2] = 0.0;
					risColor[0] = (angle + 30 - (angle < 30 ? 0 : 360)) * 4.25 / 255.0;
				}
				else if (angle > 30.0 && angle < 90.0) {
					risColor[0] = 1.0;
					risColor[2] = 0.0;
					risColor[1] = (90 - angle) * 4.25 / 255.0;
				}
				else if (angle > 90.0 && angle < 150.0) {
					risColor[0] = 1.0;
					risColor[1] = 0.0;
					risColor[2] = (angle - 90) * 4.25 / 255.0;
				}
				else if (angle > 150.0 && angle < 210.0) {
					risColor[2] = 1.0;
					risColor[1] = 0.0;
					risColor[0] = (210 - angle) * 4.25 / 255.0;
				}
				else if (angle > 210.0 && angle < 270.0) {
					risColor[2] = 1.0;
					risColor[0] = 0.0;
					risColor[1] = (angle - 210) * 4.25 / 255.0;
				}
				else if (angle > 270.0 && angle < 330.0) {
					risColor[1] = 1.0;
					risColor[0] = 0.0;
					risColor[2] = (330 - angle) * 4.25 / 255.0;
				}
				else if (abs(angle - 30) <= (1e-6)) {
					risColor[0] = risColor[1] = 1.0;
					risColor[2] = 0.0;
				}
				else if (abs(angle - 90) <= (1e-6)) {
					risColor[0] = 1.0;
					risColor[1] = risColor[2] = 0.0;
				}
				else if (abs(angle - 150) <= (1e-6)) {
					risColor[0] = risColor[2] = 1.0;
					risColor[1] = 0.0;
				}
				else if (abs(angle - 210) <= (1e-6)) {
					risColor[2] = 1.0;
					risColor[0] = risColor[1] = 0.0;
				}
				else if (abs(angle - 270) <= (1e-6)) {
					risColor[2] = risColor[1] = 1.0;
					risColor[0] = 0.0;
				}
				else if (abs(angle - 330) <= (1e-6)) {
					risColor[1] = 1.0;
					risColor[0] = risColor[2] = 0.0;
				}
				color_menu(color_x = x, color_y = (height - y));
			}
			else if (y >= 260 && y <= 265 && x >= 135 && x <= 262.5) {
				if (colorful) colorful = 0;
				risColor[0] = risColor[1] = risColor[2] = (x - 135) * 2/255.0;
				color_menu(color_x = x, color_y = (height - y));
			}
			else if (x > 255 && x < 310 && y > 230 && y < 250) {
				mid_bar_menu(2, color = 0);
			}
			else if (x > 215 && x < 260 && y > 230 && y < 250) {
				myColor[0] = risColor[0];
				myColor[1] = risColor[1];
				myColor[2] = risColor[2];
				mid_bar_menu(2, color = 0);
				shape_icon(0);
			}
			else return;
		}
		else return;
	}
	if (button != GLUT_LEFT_BUTTON) return;
	//if()
	if (color == 1 && state == GLUT_UP) {
		for (int i = 0; i < 3; ++i) risColor[i] = myColor[i];
		colorful = (risColor[0] == risColor[1] && risColor[1] == risColor[2]) ? 0 : 1;
		mid_bar_menu(2, color = 2);
	}
	if (counter == 2) {
		if (state == GLUT_UP) size_icon(2);
		else pnt_size++;
	}
	if (counter == 1) {
		if (state == GLUT_UP) size_icon(1);
		else pnt_size--;
	}
	if (file && state == GLUT_UP) {
		if (x > 0 && x < 49 && y>0 && y < 20) return;
		file_func((y - 5) / 15 + 1);
		top_bar_menu(1, 0);
	}
	if (view && state == GLUT_UP) {
		if (x > 50 && x < 99 && y > 0 && y < 20) return;
		if ((y - 5) / 15 + 1 == 1) lines = 1 - lines;
		else if ((y - 5) / 15 + 1 == 2) bg = 1 - bg;
		view_func((y - 5) / 15 + 1);
		top_bar_menu(1, 0);
		file_func(RIS_LOAD);
	}
	if (curve) {//pass
		if (state == GLUT_UP) curve_icon(1);
		else if (mod == CURVE) curve_lines = 1 - curve_lines;
		else if (mod != CURVE) {
			mod = CURVE;
			straight_lines = 0;straight_icon(0);
			point_icon(0);
			shape_items = 0; shape_icon(0);
			text_items = 0; text_icon(0);
		}
	}
	if (straight) {//pass
		if (state == GLUT_UP) straight_icon(1);
		else if (mod == LINE) straight_lines = 1 - straight_lines;
		else if (mod != LINE) {
			mod = LINE;
			curve_lines = 0; curve_icon(0);
			point_icon(0);
			shape_items = 0; shape_icon(0);
			text_items = 0; text_icon(0);
		}
	}
	if (point) {//pass
		if (state == GLUT_UP) point_icon(1);
		if (mod != POINT) {
			mod = POINT;
			curve_lines = 0; curve_icon(0);
			straight_lines = 0; straight_icon(0);
			shape_items = 0; shape_icon(0);
			text_items = 0; text_icon(0);
		}
	}
	if (shape) {
		if (state == GLUT_UP) shape_icon(1);
		if (mod != SHAPE) {
			mod = SHAPE;
			curve_lines = 0; curve_icon(0);
			straight_lines = 0; straight_icon(0);
			point_icon(0);
			text_items = 0; text_icon(0);
			file_func(RIS_SAVE);
			side = 0;
			mid_bar_menu(6, -1);
		}
	}
	if (shape_light == 1) {
		if (state == GLUT_UP) {
			shape_light = 0;
			shape_icon(0);
		}
		else {
			side = 0;
			shape_items = 0;
			top_bar_menu(6, shape = 0);
		}
	}
	if (solid_light) {
		if (state == GLUT_UP) {
			shape_light = solid_light = 0;
			shape_icon(0);
		}
		else {
			shape_items = solid_light;
			top_bar_menu(6, shape = 0);
		}
	}
	if (hollow_light) {
		if (state == GLUT_UP) {
			shape_light = hollow_light = 0;
			shape_icon(0);
		}
		else {
			shape_items = 3 + hollow_light;
			top_bar_menu(6, shape = 0);
		}
	}
	if (text) {
		if (state == GLUT_UP) text_icon(1);
		else if (mod == TEXT) text_items = 1 - text_items;
		else if (mod != TEXT) {
			mod = TEXT;
			pos_x = pos_y = -1;
			curve_lines = 0; curve_icon(0);
			straight_lines = 0; straight_icon(0);
			shape_items = 0; shape_icon(0);
			point_icon(0);
		}
	}
	else if(y > 70 && !shape){
		if (state != GLUT_DOWN) return;
		if (mod == POINT) {
			glColor3f(myColor[0], myColor[1], myColor[2]);
			glPointSize(pnt_size);   
			glBegin(GL_POINTS);  
			glVertex2f(x, height - y);
			glEnd();
		}
		else if (mod == SHAPE && shape_items == POLY) {
			if (side == 0) {
				vertex[side][0] = x; vertex[side][1] = y;
				side++;
			}
			else {
				if (fabs(vertex[0][0] - x) + fabs(vertex[0][1] - y) < 5) draw_polygon();
				else if (fabs(vertex[side - 1][0] - x) + fabs(vertex[side -1][1] - y) < 5) draw_polygon();
				else {
					vertex[side][0] = x;
					vertex[side][1] = y;
					glColor3f(myColor[0], myColor[1], myColor[2]);
					glBegin(GL_LINES);
					glVertex2f(vertex[side - 1][0], height - vertex[side - 1][1]);
					glVertex2f(vertex[side][0], height - vertex[side][1]);
					glFinish();
					glEnd();
					side++;
				}
			}
		}
		else if (mod == TEXT) {
			pos_x = x;
			pos_y = y;
		}
	}
}

/*-------------------------------------------------------------
 * motion callback function. The mouse is pressed and moved.
 */
void motion_func(int  x, int y)
{
	if (mod == POINT) return;
	if (mod == SHAPE && shape_items == POLY) return;
	if (y < 70) {
		first = 0;
		return;
	}
	if (first == 0) {
		first = 1;
		pos_x = x; pos_y = y;
	}
	else if(mod == CURVE){
		glColor3f(myColor[0], myColor[1], myColor[2]);
		glLineWidth(pnt_size);
		glBegin(GL_LINES);
		glVertex2i(pos_x, height - pos_y);
		glVertex2i(x, height - y);
		if(curve_lines)first = 0;
		glEnd();
		pos_x = x; pos_y = y;
	}
	else if (mod == LINE) {
		int w = (pnt_size / 3 + 1) * 3;
		file_func(RIS_LOAD);
		glColor3f(myColor[0], myColor[1], myColor[2]);
		glLineWidth(pnt_size);
		glBegin(GL_LINES);
		int dis = floor(sqrt((x - pos_x)*(x - pos_x) + (y - pos_y)*(y - pos_y)));
		double angle = atan((double)(pos_y - y) / (x- pos_x)) * 57.29578;
		//printf("%d %d %d %d\n", pos_x, pos_y, x, y);
		if ((pos_y - y) <= 0 && (x - pos_x) >= 0) angle = abs(angle);
		if ((pos_y - y) <= 0 && (x - pos_x) <= 0) angle = 180 - angle;
		if ((pos_y - y) >= 0 && (x - pos_x) <= 0) angle = 180 - angle;
		if ((pos_y - y) >= 0 && (x - pos_x) >= 0) angle = 360 - angle;
		//printf("%lf\n", angle);
		angle *= 0.017453;
		if (!straight_lines) {
			glVertex2i(pos_x, height - pos_y);
			glVertex2i(x, height - y);
		}
		else{
			for (int i = 0; i < dis - (w + 1); i += (2 * w - 1)) {
				glVertex2d(pos_x + i * cos(angle), 1.0 * height - (1.0 * pos_y + i * sin(angle)));
				glVertex2d(pos_x + (i + w) * cos(angle), 1.0 * height - (1.0 * pos_y + (i + w)*sin(angle)));
			}
		}
		glEnd();
	}
	else if (mod == SHAPE) {
		if (shape_items == SOLID_R) {
			file_func(RIS_LOAD);
			glColor3f(myColor[0], myColor[1], myColor[2]);
			glBegin(GL_POLYGON);
			glVertex2i(pos_x, height - pos_y);
			glVertex2i(pos_x, height - y);
			glVertex2i(x, height - y);
			glVertex2i(x, height - pos_y);
		}
		else if (shape_items == SOLID_T) {
			file_func(RIS_LOAD);
			glColor3f(myColor[0], myColor[1], myColor[2]);
			glBegin(GL_POLYGON);
			glVertex2i(pos_x, height - y);
			glVertex2i(x, height - y);
			glVertex2i((pos_x + x) / 2, height - pos_y);
		}
		else if (shape_items == SOLID_C) {
			file_func(RIS_LOAD);
			glColor3f(myColor[0], myColor[1], myColor[2]);
			glBegin(GL_POLYGON);
			double r = sqrt(1.0 * (pos_x - x) * (pos_x - x) + 1.0 * (pos_y - y) * (pos_y - y)) / 2.0;
			for (int i = 0; i < 360; ++i) {
				glVertex2d((pos_x + x) / 2 + r * cos(i*0.017453), height - (pos_y + y) / 2 + r*sin(i*0.017453));
			}
		}
		if (shape_items == HOLLOW_R) {
			file_func(RIS_LOAD);
			glLineWidth(pnt_size);
			glColor3f(myColor[0], myColor[1], myColor[2]);
			glBegin(GL_LINES);
			glVertex2i(pos_x, height - pos_y);
			glVertex2i(pos_x, height - y);
			glVertex2i(pos_x, height - y);
			glVertex2i(x, height - y);
			glVertex2i(x, height - y);
			glVertex2i(x, height - pos_y);
			glVertex2i(x, height - pos_y);
			glVertex2i(pos_x, height - pos_y);
		}
		else if (shape_items == HOLLOW_T) {
			file_func(RIS_LOAD);
			glLineWidth(pnt_size);
			glColor3f(myColor[0], myColor[1], myColor[2]);
			glBegin(GL_LINES);
			glVertex2i(pos_x, height - y);
			glVertex2i(x, height - y);
			glVertex2i(x, height - y);
			glVertex2i((pos_x + x) / 2, height - pos_y);
			glVertex2i((pos_x + x) / 2, height - pos_y);
			glVertex2i(pos_x, height - y);
		}
		else if (shape_items == HOLLOW_C) {
			file_func(RIS_LOAD);
			glPointSize(pnt_size);
			glColor3f(myColor[0], myColor[1], myColor[2]);
			glBegin(GL_POINTS);
			double r = sqrt(1.0 * (pos_x - x) * (pos_x - x) + 1.0 * (pos_y - y) * (pos_y - y)) / 2.0;
			for (int i = 0; i < 360; ++i) {
				glVertex2d((pos_x + x) / 2 + r * cos(i*0.017453), height - (pos_y + y) / 2 + r * sin(i*0.017453));
			}
		}
		glEnd();
	}
	glFinish();
}

/*-------------------------------------------------------------
 * passive motion callback function. The mouse is unpressed and moved.
 */
void passive_func(int x, int y) {//0,0 if left-top pos
	if (color == 2) {
		double r = ceil(sqrt((double)((x - 180) * (x - 180) + (y - 180) * (y - 180))));
		if(r < 50&& r > 20)  glutSetCursor(GLUT_CURSOR_DESTROY);
		else if(y >= 260 && y <= 265 && x >= 135 && x <= 262.5)
			glutSetCursor(GLUT_CURSOR_DESTROY);
		else if (y > 120 && y < 270 && x > 120 && x < 320) {
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		}
		else glutSetCursor(GLUT_CURSOR_NONE);
	}
	else {
		if (first) first = 0;
		if (x == 0 || x == width || y == 0 || y == height) {
			file_func(RIS_SAVE);
		}
		if (y > 70 && !shape) {
			file_func(RIS_SAVE);
			if (mod == TEXT) glutSetCursor(GLUT_CURSOR_TEXT);
			else glutSetCursor(GLUT_CURSOR_DESTROY);
		}
		else glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		if (!file) {
			if (!view && !color) {
				if (x > 1 && x < 50 && y > 1 && y < 20) {
					top_bar_menu(1, 1);
				}
			}
		}
		else {
			if ((x > 1 && x < 49 && y > 1 && y < 19) || (x >= 48 && x < 149 && y > 5 && y < 65)) {
				if (file_light){
					if (x > 1 && x < 49 && y > 1 && y < 19) {
						file_light = 0;
						top_bar_menu(1, -1);
					}
					else {
						int num = (y - 5) / 15 + 1;
						if (file_light != num) {
							file_light = num;
							top_bar_menu(1, -1);
						}
					}
				}
				else {
					if (x >= 48 && x < 149 && y > 5 && y < 65) {
						int num = (y - 5) / 15 + 1;
						file_light = num;
						top_bar_menu(1, -1);
					}
				}
			}
			//original 3 -> 2 warning
			else top_bar_menu(1, 0);
		}
		if (!view) {
			if (!file && !color && !curve) {
				if (x > 50 && x < 100 && y > 1 && y < 19) {
					top_bar_menu(2, 1);
				}
			}
		}
		else {
			if ((x > 50 && x < 100 && y > 1 && y < 19) || (x > 98 && x < 199 && y > 5 && y < 35)) {
				if (view_light) {
					if (x > 50 && x < 100 && y > 1 && y < 19) {
						view_light = 0;
						printf("%d", view_light);
						top_bar_menu(2, -1);
					}
					else {
						int num = (y - 5) / 15 + 1;
						if (view_light != num) {
							view_light = num;
							printf("%d", view_light);
							top_bar_menu(2, -1);
						}
					}
				}
				else {
					if (x > 98 && x < 199 && y > 1 && y < 19){
						int num = (y - 5) / 15 + 1;
						view_light = num;
						printf("%d", view_light);
						top_bar_menu(2, -1);
					}
				}
			}
			else top_bar_menu(2, 0);
		}
		if (counter) {
			if (x > 35 && x < 42 && y > 51 && y < 59) {
				if (pnt_size == 9) {
					mid_bar_menu(1, counter = 0);
				}
			}
			else if (x > 5 && x < 12 && y>51 && y < 59) {
				if (pnt_size == 1) {
					mid_bar_menu(1, counter = 0);
				}
			}
			else {
				mid_bar_menu(1, counter = 0);
			}
		}
		else{
			if (x > 35 && x < 42 && y > 51 && y < 59 && pnt_size != 9) {
				mid_bar_menu(1, counter = 2);
			}
			else if (x > 5 && x < 12 && y > 51 && y < 59 && pnt_size != 1) {
				mid_bar_menu(1, counter = 1);
			}
		}
		if (color) {
			if (x > 50 && x < 100 && y > 20 && y < 70) printf("");
			else if (color != 2)mid_bar_menu(2, color = 0);
		}
		else{
			if (!file && !view && !curve) {
				if (x > 50 && x < 100 && y > 20 && y < 70) {
					mid_bar_menu(2, color = 1);
				}
			}
		}
		if (curve) {
			if (x > 100 && x < 150 && y > 20 && y < 70) printf("");
			else {
				mid_bar_menu(3, curve = 0);
				//curve_icon(0);
				straight_icon(0);
				point_icon(0);
				shape_icon(0);
				text_icon(0);
			}
		}
		else {
			if (!view && !color && !file && !straight) {
				if (x > 100 && x < 150 && y > 20 && y < 70) {
					mid_bar_menu(3, curve = 1);
				}
			}
		}
		if (straight) {
			if (x > 150 && x < 200 && y > 20 && y < 70) printf("");
			else {
				mid_bar_menu(4, straight = 0);
				curve_icon(0);
				//straight_icon(0);
				point_icon(0);
				shape_icon(0);
				text_icon(0);
			}
		}
		else {
			if (!view && !color && !curve) {
				if (x > 150 && x < 200 && y > 20 && y < 70) {
					mid_bar_menu(4, straight = 1);
				}
			}
		}
		if (point) {
			if (x > 200 && x < 250 && y > 20 && y < 70) printf("");
			else {
				mid_bar_menu(5, point = 0);
				curve_icon(0);
				straight_icon(0);
				//point_icon(0);
				shape_icon(0);
				text_icon(0);
			}
		}
		else {
			if (!file && !straight) {
				if (x > 200 && x < 250 && y > 20 && y < 70) {
					mid_bar_menu(5, point = 1);
				}
			}
		}
		if (!shape) {
			if (!point) {
				if (x > 250 && x < 300 && y > 20 && y < 70) {
					glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
					mid_bar_menu(6, shape = 1);
				}
			}
		}
		else {
			if (shape_light == 2) {//solid
				if (solid_light) {
					if (x >= 298 && x < 354 && y > 25 && y < 70) {//move to the shape menu
						solid_light = 0;
						int num = (y - 25) / 15 + 1;
						shape_light = num;
						file_func(RIS_LOAD);
						mid_bar_menu(6, -1);
						shape_icon(1);
					}
					else if (x > 251 && x < 298 && y > 20 && y < 70) {//move to the shape icon
						solid_light = shape_light = 0;
						file_func(RIS_LOAD);
						mid_bar_menu(6, -1);
						shape_icon(1);
					}
					else if (x >= 354 && x < 434 && y > 45 && y < 90) {//change in solid menu
						int num = (y - 45) / 15 + 1;
						if (solid_light != num) {
							solid_light = num;
							mid_bar_menu(6, -1);
						}
					}
					else {
						shape_light = solid_light = 0;
						mid_bar_menu(6, shape = 0);
						curve_icon(0);
						straight_icon(0);
						point_icon(0);
						//shape_icon(0);
						text_icon(0);
					}
				}
				else {
					if (x >= 298 && x < 354 && y > 25 && y < 70) {//move to the shape menu
						int num = (y - 25) / 15 + 1;
						if (shape_light != num) {
							shape_light = num;
							file_func(RIS_LOAD);
							mid_bar_menu(6, -1);
							shape_icon(1);
						}
					}
					else if (x > 251 && x < 298 && y > 20 && y < 70) {//move to the shape icon
							shape_light = 0;
							file_func(RIS_LOAD);
							mid_bar_menu(6, -1);
							shape_icon(1);
					}
					else if (x >= 354 && x < 434 && y > 45 && y < 90) {//change in solid menu
						int num = (y - 45) / 15 + 1;
						solid_light = num;
						mid_bar_menu(6, -1);
					}
					else {
						shape_light = 0;
						mid_bar_menu(6, shape = 0);
						curve_icon(0);
						straight_icon(0);
						point_icon(0);
						//shape_icon(0);
						text_icon(0);
					}
				}
			}
			else if (shape_light == 3) {//hollow
				if (hollow_light) {
					if (x >= 298 && x < 354 && y > 25 && y < 70) {//move to the shape menu
						hollow_light = 0;
						int num = (y - 25) / 15 + 1;
						shape_light = num;
						file_func(RIS_LOAD);
						mid_bar_menu(6, -1);
						shape_icon(1);
					}
					else if (x > 251 && x < 298 && y > 20 && y < 70) {//move to the shape icon
						hollow_light = shape_light = 0;
						file_func(RIS_LOAD);
						mid_bar_menu(6, -1);
						shape_icon(1);
					}
					else if (x >= 354 && x < 434 && y > 60 && y < 105) {//change in hollow menu
						int num = (y - 60) / 15 + 1;
						if (hollow_light != num) {
							hollow_light = num;
							mid_bar_menu(6, -1);
						}
					}
					else {
						shape_light = hollow_light = 0;
						mid_bar_menu(6, shape = 0);
						curve_icon(0);
						straight_icon(0);
						point_icon(0);
						//shape_icon(0);
						text_icon(0);
					}
				}
				else {
					if (x >= 298 && x < 354 && y > 25 && y < 70) {//move to the shape menu
						int num = (y - 25) / 15 + 1;
						if (shape_light != num) {
							shape_light = num;
							file_func(RIS_LOAD);
							mid_bar_menu(6, -1);
							shape_icon(1);
						}
					}
					else if (x > 251 && x < 298 && y > 20 && y < 70) {//move to the shape icon
						shape_light = 0;
						file_func(RIS_LOAD);
						mid_bar_menu(6, -1);
						shape_icon(1);
					}
					else if (x >= 354 && x < 434 && y > 60 && y < 105) {//change in hollow menu
						int num = (y - 45) / 15 + 1;
						hollow_light = num;
						mid_bar_menu(6, -1);
					}
					else {
						shape_light = 0;
						mid_bar_menu(6, shape = 0);
						curve_icon(0);
						straight_icon(0);
						point_icon(0);
						//shape_icon(0);
						text_icon(0);
					}
				}
			}
			else if (shape_light == 1) {
				if (x >= 298 && x < 354 && y > 25 && y < 70) {
					int num = (y - 25) / 15 + 1;
					if (shape_light != num) {
						shape_light = num;
						mid_bar_menu(6, -1);
					}
				}
				else if (x > 251 && x < 298 && y > 20 && y < 70) {//move to the shape icon
					shape_light = 0;
					mid_bar_menu(6, -1);
				}
				else {
					shape_light = 0;
					mid_bar_menu(6, shape = 0);
				}
			}
			else if(shape_light == 0){
				if (x >= 298 && x < 354 && y > 25 && y < 70) {
					int num = (y - 25) / 15 + 1;
					shape_light = num;
					mid_bar_menu(6, -1);
				}
				else if (x > 251 && x < 298 && y > 20 && y < 70) printf("");
				else {
					mid_bar_menu(6, shape = 0);
					curve_icon(0);
					straight_icon(0);
					point_icon(0);
					//shape_icon(0);
					text_icon(0);
				}
			}
		}
		if (text) {
			if (x > 300 && x < 350 && y > 20 && y < 70) printf("");
			else {
				mid_bar_menu(7, text = 0);
				curve_icon(0);
				straight_icon(0);
				point_icon(0);
				shape_icon(0);
				//text_icon(0);
			}
		}
		else {
			if (!shape) {
				if (x > 300 && x < 350 && y > 20 && y < 70) {
					mid_bar_menu(7, text = 1);
				}
			}
		}
	}
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

/*---------------------------------------------------------------
 * Main procedure sets up the window environment.
 */
void main(int argc, char **argv)
{

	glutInit(&argc, argv);    /*---Make connection with server---*/
	glutInitWindowPosition(0, 0);  /*---Specify window position ---*/
	glutInitWindowSize(width, height); /*--Define window's height and width--*/
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); /*---set display mode---*/
	init_func();
	init_window(0);
	/* Create parent window */
	glutCreateWindow("Draw");

	glutDisplayFunc(display_func); /* Associate display event callback func */
	glutReshapeFunc(my_reshape);  /* Associate reshape event callback func */

	glutKeyboardFunc(keyboard); /* Callback func for keyboard event */

	glutMouseFunc(mouse_func);  /* Mouse Button Callback func */
	glutMotionFunc(motion_func);/* Mouse motion event callback func */
	glutPassiveMotionFunc(passive_func);
	//glutEntryFunc(entry_func);

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
