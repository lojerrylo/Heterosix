#include<Windows.h>
#include<gl\GLU.H>
#include<gl\GLAUX.H>
#include<gl\GLUT.H>
#include"render-hexa.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "glut32.lib")

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Show stl");//視窗標題而已
	glutReshapeFunc(ChangeSize);//視窗大小變化時做些什麼
	glutSpecialFunc(SpecialKeys);	//按鍵盤時做些什麼 (上下左右)
	glutKeyboardFunc(NormalKeys);	//按鍵盤時做些什麼 (字母)
	glutMouseFunc(Mouse);		//滑鼠點擊時做些什麼
	glutMotionFunc(MouseMove);	//滑鼠移動時做些什麼
	glutDisplayFunc(RenderScene);//畫圖
	SetupRC();	//背景
	glutTimerFunc(30, update, 0);//等個30毫秒更新螢幕-->主要是滑鼠滑動時用
	glutMainLoop();//無限重複

	return 0;
}