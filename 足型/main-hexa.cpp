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
	glutCreateWindow("Show stl");//�������D�Ӥw
	glutReshapeFunc(ChangeSize);//�����j�p�ܤƮɰ��Ǥ���
	glutSpecialFunc(SpecialKeys);	//����L�ɰ��Ǥ��� (�W�U���k)
	glutKeyboardFunc(NormalKeys);	//����L�ɰ��Ǥ��� (�r��)
	glutMouseFunc(Mouse);		//�ƹ��I���ɰ��Ǥ���
	glutMotionFunc(MouseMove);	//�ƹ����ʮɰ��Ǥ���
	glutDisplayFunc(RenderScene);//�e��
	SetupRC();	//�I��
	glutTimerFunc(30, update, 0);//����30�@���s�ù�-->�D�n�O�ƹ��ưʮɥ�
	glutMainLoop();//�L������

	return 0;
}