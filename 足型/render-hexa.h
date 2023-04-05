#pragma once


// glut callback function
void SpecialKeys(int key, int x, int y);
void NormalKeys(unsigned char key, int x, int y);
void RenderScene(void);
void SetupRC();
void ChangeSize(int w, int h);
void Mouse(int button, int state, int x, int y);
void MouseMove(int x, int y);
void update(int value);