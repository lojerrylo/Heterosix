#define _HEXAPOD_

#include<Windows.h>
#include<gl\GLU.H>
#include<gl\GLAUX.H>
#include<gl\GLUT.H>
#include<iostream>
#include"render-hexa.h"
#include"../Heterosix/het_hexa_cfg.h"
#include"../Heterosix/het_hexapod.h"
#include"../Heterosix/het_math.h"
#include"../Heterosix/het_main.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "glut32.lib")

using std::cout; using std::endl;

// ============= For openGL =================
GLfloat xRot = -45.0f;		// 繞著x軸轉
GLfloat zRot = 30.0f;		// 繞著y軸轉	

// q	start simulation

// ==============  HEXAPOD start ============

extern hexapods hexapod;

void SpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:		xRot += 5.0f; break;
	case GLUT_KEY_DOWN:		xRot -= 5.0f; break;
	case GLUT_KEY_RIGHT:	zRot += 5.0f; break;
	case GLUT_KEY_LEFT:		zRot -= 5.0f; break;;
	case GLUT_KEY_F1:CommandProcess(M_H_READY); break;
	case GLUT_KEY_F2:CommandProcess(M_H_START_MOVING); break;
	case GLUT_KEY_F3:CommandProcess(M_H_STOP_MOVING); break;
	}
}
void NormalKeys(unsigned char key, int x, int y) {
	switch (key) {

	case ' ': break;
	case 'q': CommandProcess(M_H_MOVE_WHOLE_STEP, -1, 1);	break;	// left  up
	case 'w': CommandProcess(M_H_MOVE_WHOLE_STEP, 0, 1);	break;	//       up
	case 'e': CommandProcess(M_H_MOVE_WHOLE_STEP, 1, 1);	break;	// right up
	case 'a': CommandProcess(M_H_MOVE_WHOLE_STEP, -1, 0);	break;	// left
	case 's': CommandProcess(M_H_MOVE_WHOLE_STEP, 0, 0);	break;	// stop.
	case 'd': CommandProcess(M_H_MOVE_WHOLE_STEP, 1, 0);	break;	// right
	case 'z': CommandProcess(M_H_MOVE_WHOLE_STEP, -1, -1);	break;	// left  down
	case 'x': CommandProcess(M_H_MOVE_WHOLE_STEP, 0, -1);	break;	//       down
	case 'c': CommandProcess(M_H_MOVE_WHOLE_STEP, 1, -1);	break;	// right down

	case 'Q': CommandProcess(M_H_MOVE_CONTINUOUSLY, -1, 1);	break;	// left  up
	case 'W': CommandProcess(M_H_MOVE_CONTINUOUSLY, 0, 1);	break;	//       up
	case 'E': CommandProcess(M_H_MOVE_CONTINUOUSLY, 1, 1);	break;	// right up
	case 'A': CommandProcess(M_H_MOVE_CONTINUOUSLY, -1, 0);	break;	// left
	case 'S': CommandProcess(M_H_MOVE_CONTINUOUSLY, 0, 0);	break;	// stop.
	case 'D': CommandProcess(M_H_MOVE_CONTINUOUSLY, 1, 0);	break;	// right
	case 'Z': CommandProcess(M_H_MOVE_CONTINUOUSLY, -1, -1);break;	// left  down
	case 'X': CommandProcess(M_H_MOVE_CONTINUOUSLY, 0, -1);	break;	//       down
	case 'C': CommandProcess(M_H_MOVE_CONTINUOUSLY, 1, -1);	break;	// right down

	case 'r': CommandProcess(M_H_GAIT_LOOP); break;
	case 't': CommandProcess(M_H_GAIT_SELECT, GAIT_TRIPLE); break;
	case 'y': CommandProcess(M_H_GAIT_SELECT, GAIT_RIPPLE);  break;

	case 'f': break;
	case 'g': break;
	case 'h': break;

	case 'v': CommandProcess(M_H_ADJUST_BODY_TZ, 1); break;
	case 'b': CommandProcess(M_H_ADJUST_BODY_TZ, -1); break;
	case 'n': break;
	case 'm': break;

	case 'u': break;
	case 'i': break;
	case 'o': break;
	case 'p': break;
	case 'j': break;
	case 'k': CommandProcess(M_H_SPEED_DOWN);	break;
	case 'l': CommandProcess(M_H_SPEED_UP);	break;
	

	default:break;
	}
}

void draw_floor(int range) {
	glColor3ub(50, 50, 50);
	glBegin(GL_QUADS);
	glVertex3d(range, range, -1);
	glVertex3d(-range, range, -1);
	glVertex3d(-range, -range, -1);
	glVertex3d(range, -range, -1);
	glEnd();
}

void hexa_hexagon(unsigned r, unsigned g, unsigned b, bool line = 1) {
	glColor3ub(r, g, b);
	if (line) {	glBegin(GL_LINE_LOOP);	}
	else { glBegin(GL_POLYGON); }
	for (int i = 0; i < 6; i++) {
		glVertex4dv(hexapod.coxia_T[i].Origin().v);
	}
	glEnd();
}
void hexa_coxia(unsigned r, unsigned g, unsigned b) {
	glColor3ub(r, g, b);
	glBegin(GL_LINES);
	for (int i = 0; i < 6; i++) {
		glVertex4dv(hexapod.coxia_T[i].Origin().v);
		glVertex4dv(hexapod.femur_T[i].Origin().v);
	}
	glEnd();
}
void hexa_femur(unsigned r, unsigned g, unsigned b) {
	glColor3ub(r, g, b);
	glBegin(GL_LINES);
	for (int i = 0; i < 6; i++) {
		glVertex4dv(hexapod.femur_T[i].Origin().v);
		glVertex4dv(hexapod.tibia_T[i].Origin().v);
	}
	glEnd();
}
void hexa_tibia(unsigned r, unsigned g, unsigned b) {
	glColor3ub(r, g, b);
	glBegin(GL_LINES);
	for (int i = 0; i < 6; i++) {
		glVertex4dv(hexapod.tibia_T[i].Origin().v);
		glVertex4dv(hexapod.tiptoe[i].v);
	}
	glEnd();
}
void hexa_stride_circle(unsigned r, unsigned g, unsigned b) {
	glColor3ub(r, g, b);
	glBegin(GL_LINES);
	for (int i = 0; i < 6; i++) {
		glVertex4dv(hexapod.stride_end[i].v);
		glVertex4dv(hexapod.stride_start[i].v);
	}
	glEnd();
}
void hexa_stride_now(unsigned r, unsigned g, unsigned b) {
	glColor3ub(r, g, b);
	glBegin(GL_LINES);
	for (int i = 0; i < 6; i++) {
		glVertex4dv(hexapod.coxia_T[i].Origin().v);
		glVertex4dv(hexapod.stride_now[i].v);
	}
	glEnd();
}
void hexa_support_polygon(unsigned r, unsigned g, unsigned b) {
	glColor3ub(r, g, b);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 6; i++) {
		if (hexapod.isOnTheGround[i]) {
			glVertex4dv(hexapod.tiptoe[i].v);
		}
	}
	glEnd();
}
void draw_hexa() {
	glLineWidth(2.0);
	hexa_hexagon(255, 255, 255, 1);
	hexa_coxia(255, 0, 0);
	hexa_femur(255, 150, 0);
	hexa_tibia(0, 200, 0);
	glLineWidth(0.5);
	hexa_stride_circle(255, 255, 255);
	hexa_stride_now(255, 255, 0);
}
void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
	glMatrixMode(GL_PROJECTION); glPushMatrix();
	glRotatef(xRot, 1.0f, 0, 0);
	glRotatef(zRot, 0, 0,1.0f);
	glMatrixMode(GL_MODELVIEW); glPushMatrix();
	draw_floor(90);
	draw_hexa();
	glMatrixMode(GL_MODELVIEW); glPopMatrix();
	glMatrixMode(GL_PROJECTION); glPopMatrix();
	glutSwapBuffers();
}
void SetupRC() {
	setup();// 我就象徵性地放這裡
	glShadeModel(GL_SMOOTH);
	// glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);	// 灰背景
}
GLint w_static, h_static;

void ChangeSize(int w, int h) {
	 w_static = w; h_static = h;
	GLfloat nRange = 100;
	if (h == 0) { h = 1; }			//避免被縮為0
	glViewport(0, 0, w, h);		//設定投影位置
	glMatrixMode(GL_PROJECTION);//接下來要設定的是投影矩陣
	glLoadIdentity();			//引入單位矩陣
	
	if (w <= h) {
	glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -2 * nRange, 2 * nRange);
	}
	else {
	glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -2 * nRange, 2 * nRange);
	}
	
	
	glMatrixMode(GL_MODELVIEW);//讓rendersence作用的矩陣
	glLoadIdentity();
}

void Mouse(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON || button == GLUT_MIDDLE_BUTTON) && state == GLUT_DOWN) {
		cout << x-w_static/2 << "\t" << y-h_static/2 << endl;
	}
}
void MouseMove(int x, int y) {

}
void update(int value) {
	glutPostRedisplay();
	loop();
	hexapod.parameter_show();
	glutTimerFunc(LOOP_TIME, update, 0);
}