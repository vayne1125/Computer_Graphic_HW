/******************************************************************
 * This program illustrates the fundamental instructions for handling 
 * mouse and keyboeard events as well as menu buttons.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>

#define    SIZEX   1000
#define    SIZEY   1000

#define    MY_QUIT -1
#define    MY_CLEAR -2
#define    MY_SAVE  -3
#define    MY_BLEND -4
#define    MY_LOAD  -5

#define    WHITE   1
#define    RED     2
#define    GREEN   3
#define    BLUE    4

#define    POINT   1
#define    LINE    2
#define    POLYGON 3
#define    CIRCLE  4
#define    CURVE   5
#define    TRIANGLE   6
#define    RECTANGLE   7
#define    TEXT   8
#define    PAINT 9
//text mode
#define RANDOM 1
#define FIXED 2
typedef    int   menu_t;
menu_t     top_m, text_mode_m, file_m;

int        height=800, width=1000;
unsigned char  image[SIZEX*SIZEY][4];  /* Image data in main memory */

int        pos_x=-1, pos_y=-1;
float      myColor[3]={0.0,0.0,0.0};
int        obj_type = -1;
int        first=0;      /* flag of initial points for lines and curve,..*/
int        vertex[128][2]; /*coords of vertices */
int        side=0;         /*num of sides of polygon */
float      pnt_size=1.0;
int text_mode = RANDOM;

int red_btn[4] = { 65, 13 ,100, 25 }; //35*10
int orange_btn[4] = {105,13,140,25};
int yellow_btn[4] = { 145,13,180,25 };
int green_btn[4] = { 185,13,220,25 };
int blue_btn[4] = { 225,13,260,25 };
int dblue_btn[4] = { 265,13,300,25 };
int purple_btn[4] = { 305,13,340,25 };
int black_btn[4] = { 345,13,380,25 };

int point_btn[4] = { 65, 36 ,100, 48};
int line_btn[4] = { 105, 36 ,140, 48 };
int poly_btn[4] = { 145, 36 ,180, 48 };
int curve_btn[4] = { 185, 36 ,224, 48 };
int circle_btn[4] = { 229, 36 ,269, 48 };
int triangle_btn[4] = { 274, 36 ,322, 48 };
int rectangle_btn[4] = { 327, 36 ,385, 48 };
int text_btn[4] = { 390, 36 ,425, 48 };
int paint_btn[4] = { 430, 36 ,465, 48 };

void red_color() { glColor3f(1.0, 0.0, 0.0); }
void orange_color() { glColor3f(1.0, 129 / 255.0, 66 / 255.0); }
void yellow_color() { glColor3f(1.0, 231 / 255.0, 112 / 255.0); }
void green_color() { glColor3f(0.0, 1.0, 0.0); }
void blue_color() { glColor3f(77 / 255.0, 1.0, 1.0); }
void dblue_color() { glColor3f(41 / 255.0, 41 / 255.0, 1.0); }
void purple_color() { glColor3f(156 / 255.0, 51 / 255.0, 1.0); };
void black_color() { glColor3f(0, 0, 0.0); }
void color_bar(void) {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "color\0";
    glRasterPos2i(15, height - 25);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }
    //red
    red_color();
    glBegin(GL_QUADS);
    glVertex2i(red_btn[0], height - red_btn[1]);
    glVertex2i(red_btn[2], height - red_btn[1]);
    glVertex2i(red_btn[2], height - red_btn[3]);
    glVertex2i(red_btn[0], height - red_btn[3]);
    glEnd();

    //orenge:rgb(255, 129, 66)
    orange_color();
    glBegin(GL_QUADS);
    glVertex2i(orange_btn[0], height - orange_btn[1]);
    glVertex2i(orange_btn[2], height - orange_btn[1]);
    glVertex2i(orange_btn[2], height - orange_btn[3]);
    glVertex2i(orange_btn[0], height - orange_btn[3]);
    glEnd();

    //yellow:rgb(255, 231, 112)
    yellow_color();
    glBegin(GL_QUADS);
    glVertex2i(yellow_btn[0], height - yellow_btn[1]);
    glVertex2i(yellow_btn[2], height - yellow_btn[1]);
    glVertex2i(yellow_btn[2], height - yellow_btn[3]);
    glVertex2i(yellow_btn[0], height - yellow_btn[3]);
    glEnd();

    //green:
    green_color();
    glBegin(GL_QUADS);
    glVertex2i(green_btn[0], height - green_btn[1]);
    glVertex2i(green_btn[2], height - green_btn[1]);
    glVertex2i(green_btn[2], height - green_btn[3]);
    glVertex2i(green_btn[0], height - green_btn[3]);
    glEnd();

    //blue:rgb(77, 255, 255)
    blue_color();
    glBegin(GL_QUADS);
    glVertex2i(blue_btn[0], height - blue_btn[1]);
    glVertex2i(blue_btn[2], height - blue_btn[1]);
    glVertex2i(blue_btn[2], height - blue_btn[3]);
    glVertex2i(blue_btn[0], height - blue_btn[3]);
    glEnd();

    //dark blue:rgb(41, 41, 255)
    dblue_color();
    glBegin(GL_QUADS);
    glVertex2i(dblue_btn[0], height - dblue_btn[1]);
    glVertex2i(dblue_btn[2], height - dblue_btn[1]);
    glVertex2i(dblue_btn[2], height - dblue_btn[3]);
    glVertex2i(dblue_btn[0], height - dblue_btn[3]);
    glEnd();

    //purple:rgb(156, 51, 255)
    purple_color();
    glBegin(GL_QUADS);
    glVertex2i(purple_btn[0], height - purple_btn[1]);
    glVertex2i(purple_btn[2], height - purple_btn[1]);
    glVertex2i(purple_btn[2], height - purple_btn[3]);
    glVertex2i(purple_btn[0], height - purple_btn[3]);
    glEnd();

    //black
    black_color();
    glBegin(GL_QUADS);
    glVertex2i(black_btn[0], height - black_btn[1]);
    glVertex2i(black_btn[2], height - black_btn[1]);
    glVertex2i(black_btn[2], height - black_btn[3]);
    glVertex2i(black_btn[0], height - black_btn[3]);
    glEnd();

}

void type_bar(void) {
    glColor3f(0.0, 0.0, 0.0);
    char* c = "type\0";
    glRasterPos2i(15, height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }

    //rgb(255, 219, 237)
    //point
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    glBegin(GL_QUADS);
    glVertex2i(point_btn[0], height - point_btn[1]);
    glVertex2i(point_btn[2], height - point_btn[1]);
    glVertex2i(point_btn[2], height - point_btn[3]);
    glVertex2i(point_btn[0], height - point_btn[3]);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    c = " point\0";
    glRasterPos2i(65, height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    //line
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    glBegin(GL_QUADS);
    glVertex2i(line_btn[0], height - line_btn[1]);
    glVertex2i(line_btn[2], height - line_btn[1]);
    glVertex2i(line_btn[2], height - line_btn[3]);
    glVertex2i(line_btn[0], height - line_btn[3]);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    c = "  line\0";
    glRasterPos2i(105, height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    //poly
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    glBegin(GL_QUADS);
    glVertex2i(poly_btn[0], height - poly_btn[1]);
    glVertex2i(poly_btn[2], height - poly_btn[1]);
    glVertex2i(poly_btn[2], height - poly_btn[3]);
    glVertex2i(poly_btn[0], height - poly_btn[3]);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    c = " poly\0";
    glRasterPos2i(145, height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    //curve
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    glBegin(GL_QUADS);
    glVertex2i(curve_btn[0], height - curve_btn[1]);
    glVertex2i(curve_btn[2], height - curve_btn[1]);
    glVertex2i(curve_btn[2], height - curve_btn[3]);
    glVertex2i(curve_btn[0], height - curve_btn[3]);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    c = " curve\0";
    glRasterPos2i(185, height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    //circle
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    glBegin(GL_QUADS);
    glVertex2i(circle_btn[0], height - circle_btn[1]);
    glVertex2i(circle_btn[2], height - circle_btn[1]);
    glVertex2i(circle_btn[2], height - circle_btn[3]);
    glVertex2i(circle_btn[0], height - circle_btn[3]);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    c = " circle\0";
    glRasterPos2i(229, height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    //triangle
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    glBegin(GL_QUADS);
    glVertex2i(triangle_btn[0], height - triangle_btn[1]);
    glVertex2i(triangle_btn[2], height - triangle_btn[1]);
    glVertex2i(triangle_btn[2], height - triangle_btn[3]);
    glVertex2i(triangle_btn[0], height - triangle_btn[3]);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    c = " triangle\0";
    glRasterPos2i(274, height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    //rectangle
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    glBegin(GL_QUADS);
    glVertex2i(rectangle_btn[0], height - rectangle_btn[1]);
    glVertex2i(rectangle_btn[2], height - rectangle_btn[1]);
    glVertex2i(rectangle_btn[2], height - rectangle_btn[3]);
    glVertex2i(rectangle_btn[0], height - rectangle_btn[3]);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    c = " rectangle\0";
    glRasterPos2i(327, height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    //text
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    glBegin(GL_QUADS);
    glVertex2i(text_btn[0], height - text_btn[1]);
    glVertex2i(text_btn[2], height - text_btn[1]);
    glVertex2i(text_btn[2], height - text_btn[3]);
    glVertex2i(text_btn[0], height - text_btn[3]);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    c = "  text\0";
    glRasterPos2i(390, height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }

    //paint
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    glBegin(GL_QUADS);
    glVertex2i(paint_btn[0], height - paint_btn[1]);
    glVertex2i(paint_btn[2], height - paint_btn[1]);
    glVertex2i(paint_btn[2], height - paint_btn[3]);
    glVertex2i(paint_btn[0], height - paint_btn[3]);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    c = " paint\0";
    glRasterPos2i(430, height - 47);
    for (char* i = c; *i != '\0'; i++) {
        printf("%c", *i);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i);
    }
}

void myMenu(void) {
    //glPolygonMode(GL_FRONT, GL_FILL);
    glColor3f(1.0, 194 / 255.0, 222 / 255.0);
    glBegin(GL_POLYGON);
        glVertex2i(0, height);
        glVertex2i(width, height);
        glVertex2i(width, height-100);
        glVertex2i(0, height - 100);
    glEnd();

    //rgb(255, 235, 244)
    glColor3f(1.0, 240/255.0, 250/255.0);
    glBegin(GL_POLYGON);
    glVertex2i(5, height-5);
    glVertex2i(width-5, height-5);
    glVertex2i(width-5, height - 95);
    glVertex2i(5, height - 95);
    glEnd();

    color_bar();
    type_bar();
}



/*------------------------------------------------------------
 * Callback function for display, redisplay, expose events
 * Just clear the window again
 */
void display_func(void)
{
  /* define window background color */
  //glClear(GL_COLOR_BUFFER_BIT);
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
  gluOrtho2D(0.0, (double) width, 0.0, (double) height);
  glViewport(0,0,width,height);
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
            glRasterPos2i(x, height -y);        
        }
        else if (text_mode == FIXED) {
            glRasterPos2i(pos_x, height - pos_y);
            pos_x += 10;
        }
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, key);
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
    for(i=0;i<side;i++)
      glVertex2f(vertex[i][0], height-vertex[i][1]);
  glEnd();
  glFinish();
  side = 0;    /* set side=0 for next polygon */
}

/*------------------------------------------------------------
 * Procedure to draw a circle
 */
void draw_circle()
{
  static GLUquadricObj *mycircle=NULL;

  if(mycircle==NULL){
    mycircle = gluNewQuadric();
    gluQuadricDrawStyle(mycircle,GLU_FILL);
  }
  glPushMatrix();
  glTranslatef(pos_x, height-pos_y, 0.0);
  gluDisk(mycircle,
       0.0,           /* inner radius=0.0 */
	  10.0,          /* outer radius=10.0 */
	  16,            /* 16-side polygon */
	   3);
  glPopMatrix();
}

/*------------------------------------------------------------
 * Callback function handling mouse-press events
 */
void mouse_func(int button, int state, int x, int y)
{
    //printf("%d %d:::\n", GLUT_RIGHT_BUTTON, GLUT_UP);
    if (button == 0 && state == 1 && obj_type == PAINT) { //右鍵放開 (代碼是錯的...)
        first = 0;
    }
  if(button!=GLUT_LEFT_BUTTON||state!=GLUT_DOWN)
    return;
  printf("%d %d\n",x,y);
  //按鈕區 -> 不能畫圖
  if (y <= 100) {
      //color
      if (x >= red_btn[0] && x <= red_btn[2] && y >= red_btn[1] && y <= red_btn[3]) {
          printf("red\n");
          red_color();
      }
      else if (x >= orange_btn[0] && x <= orange_btn[2] && y >= orange_btn[1] && y <= orange_btn[3]) {
          printf("orange\n");
          orange_color();
      }
      else if (x >= yellow_btn[0] && x <= yellow_btn[2] && y >= yellow_btn[1] && y <= yellow_btn[3]) {
          printf("yellow\n");
          yellow_color();
      }
      else if (x >= green_btn[0] && x <= green_btn[2] && y >= green_btn[1] && y <= green_btn[3]) {
          printf("green\n");
          green_color();
      }
      else if (x >= blue_btn[0] && x <= blue_btn[2] && y >= blue_btn[1] && y <= blue_btn[3]) {
          printf("blue\n");
          blue_color();
      }
      else if (x >= dblue_btn[0] && x <= dblue_btn[2] && y >= dblue_btn[1] && y <= dblue_btn[3]) {
          printf("dblue\n");
          dblue_color();
      }
      else if (x >= purple_btn[0] && x <= purple_btn[2] && y >= purple_btn[1] && y <= purple_btn[3]) {
          printf("purple\n");
          purple_color();
      }
      else if (x >=black_btn[0] && x <= black_btn[2] && y >= black_btn[1] && y <= black_btn[3]) {
          printf("black\n");
          black_color();
      }
      else if (x >= point_btn[0] && x <= point_btn[2] && y >= point_btn[1] && y <= point_btn[3]) {
          printf("point\n");
          obj_type = POINT;
      }
      else if (x >= line_btn[0] && x <= line_btn[2] && y >= line_btn[1] && y <= line_btn[3]) {
          printf("line\n");
          first = 0;
          obj_type = LINE;
      }
      else if (x >= poly_btn[0] && x <= poly_btn[2] && y >= poly_btn[1] && y <= poly_btn[3]) {
          printf("poly\n");
          side = 0;
          obj_type = POLYGON;
      }
      else if (x >= curve_btn[0] && x <= curve_btn[2] && y >= curve_btn[1] && y <= curve_btn[3]) {
          printf("curve\n");
          first = 0;
          obj_type = CURVE;
      }
      else if (x >= circle_btn[0] && x <= circle_btn[2] && y >= circle_btn[1] && y <= circle_btn[3]) {
          printf("circle\n");
          obj_type = CIRCLE;
      }
      else if (x >= triangle_btn[0] && x <= triangle_btn[2] && y >= triangle_btn[1] && y <= triangle_btn[3]) {
          printf("triangle\n");
          obj_type = TRIANGLE;
      }
      else if (x >= rectangle_btn[0] && x <= rectangle_btn[2] && y >= rectangle_btn[1] && y <= rectangle_btn[3]) {
          printf("rectangle\n");
          obj_type = RECTANGLE;
      }
      else if (x >= text_btn[0] && x <= text_btn[2] && y >= text_btn[1] && y <= text_btn[3]) {
          printf("text\n");
          obj_type = TEXT;
      }
      else if (x >= paint_btn[0] && x <= paint_btn[2] && y >= paint_btn[1] && y <= paint_btn[3]) {
          printf("paint\n");
          first = 0;
          obj_type = PAINT;
      }
      return;
  }
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
          glPointSize(pnt_size);
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
      break;
  case TEXT:
      pos_x = x; pos_y = y;
      break;
  default:
      break;
  }
  glFinish();
}


    
/*-------------------------------------------------------------
 * motion callback function. The mouse is pressed and moved.
 */
void motion_func(int  x, int y)
{
  if(obj_type != CURVE && obj_type != PAINT) return;
  if(first==0 && y >= 100){
    first = 1;
    pos_x = x; pos_y = y;
  }else if(y>=100) {
    glBegin(GL_LINES);
      glVertex3f(pos_x, height-pos_y, 0.0);
      glVertex3f(x, height - y, 0.0);
    glEnd();
    pos_x = x; pos_y = y;
  }
  glFinish();
}

/*--------------------------------------------------------
 * procedure to clear window
 */
void init_window(void)
{
  /*Do nothing else but clear window to black*/

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (double) width, 0.0, (double) height);
  glViewport(0,0,width, height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(1.0, 1.0, 1.0, 1.0); //背景
  glClear(GL_COLOR_BUFFER_BIT);//清除畫面

  my_reshape(width,height);
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

  if(value==MY_QUIT) exit(0);
  else if(value==MY_CLEAR) init_window();
  else if(value==MY_SAVE){ /* Save current window */
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
                 image);
	for(i=0;i<width;i++)   /* Assign 0 opacity to black pixels */
	  for(j=0;j<height;j++)
		if(image[i*width+j][0]==0 &&
           image[i*width+j][1]==0 &&
           image[i*width+j][2]==0) image[i*width+j][3] = 0;
		else image[i*width+j][3] = 127; /* Other pixels have A=127*/
  }else if(value==MY_LOAD){ /* Restore the saved image */
	 glRasterPos2i(0, 0);
     glDrawPixels(width, height, 
	       GL_RGBA, GL_UNSIGNED_BYTE, 
	       image);
  }else if(value==MY_BLEND){ /* Blending current image with the saved image */	
	  glEnable(GL_BLEND); 
	  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	if(value==1){
	  if(pnt_size<10.0) pnt_size += 1.0;
	}else{
	  if(pnt_size>1.0) pnt_size = pnt_size -1.0;
	}
}

void text_mode_func(int value) {
    text_mode = value;
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
void main(int argc, char **argv)
{
  int  size_menu;

  glutInit(&argc, argv);    /*---Make connection with server---*/

  glutInitWindowPosition(0,0);  /*---Specify window position ---*/
  glutInitWindowSize(width, height); /*--Define window's height and width--*/

  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA); /*---set display mode---*/
  init_func();
   /* Create parent window */
  glutCreateWindow("paint"); ;

  glutDisplayFunc(display_func); /* Associate display event callback func */
  glutReshapeFunc(my_reshape);  /* Associate reshape event callback func */

  glutKeyboardFunc(keyboard); /* Callback func for keyboard event */

  glutMouseFunc(mouse_func);  /* Mouse Button Callback func */
  glutMotionFunc(motion_func);/* Mouse motion event callback func */

  text_mode_m = glutCreateMenu(text_mode_func);
  glutAddMenuEntry("random", RANDOM);
  glutAddMenuEntry("fixed", FIXED);


  file_m = glutCreateMenu(file_func);   /* Create another menu, file-menu */
  glutAddMenuEntry("save", MY_SAVE);
  glutAddMenuEntry("load", MY_LOAD);
  glutAddMenuEntry("blend", MY_BLEND);
  glutAddMenuEntry("clear", MY_CLEAR);
  glutAddMenuEntry("quit", MY_QUIT);

  size_menu = glutCreateMenu(size_func);
  glutAddMenuEntry("Bigger", 1);
  glutAddMenuEntry("Smaller",2);

  top_m = glutCreateMenu(top_menu_func);/* Create top menu */
  glutAddSubMenu("Text mode", text_mode_m);
  glutAddSubMenu("Size", size_menu);
  glutAddSubMenu("file", file_m);       /* add file-menu as a sub-menu */
  glutAttachMenu(GLUT_RIGHT_BUTTON);    /* associate top-menu with right but*/

  /*---Test whether overlay support is available --*/
  if(glutLayerGet(GLUT_OVERLAY_POSSIBLE)){
    fprintf(stderr,"Overlay is available\n");
  }else{
    fprintf(stderr,"Overlay is NOT available, May encounter problems for menu\n");
  }
  myMenu();
  /*---Enter the event loop ----*/
  glutMainLoop();            
}
