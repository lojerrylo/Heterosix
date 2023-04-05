#include"het_main.h"
extern hexapods hexapod;
extern int ManipulatorNow;
extern int CommandID;
extern bool changeManipulator;


// errorHandler will commit command to fix error. maybe
command errorHandler() {
	// not now
	return command{ 0,0,0 };
}

void CommandProcess(int c) {
	CommandProcess(command{ c,0,0 });
}
void CommandProcess(int c, double v) {
	CommandProcess(command{ c,v,0 });
}
void CommandProcess(int c, double x, double y) {
	CommandProcess(command{ c,x,y });
}
// deal with the command, cal the theory value of each angle
void CommandProcess(command c) {
	if (hexapod.ignoreCommand) {
		if (hexapod.gaitTimeStamp == hexapod.lastTimeStamp) {
			hexapod.ignoreCommand = 0;
			hexapod.isMoving = 0;
		}
		else {

		}
	}
	else {
		switch (c.commandID)
		{
		case M_H_STOP_MOVING:
			// HexaStop(); break;
		case M_H_READY:
			HexaReady(); break;
		case M_H_START_MOVING:
			HexaStart();break;
		
			
		case M_H_ADJUST_BODY_TZ:
			HexaBodyTranslationZ(c.val1); break;
		case M_H_ADJUST_BODY_TXY:
			HexaBodyTranslationXY(c.val1, c.val2); break;
		case M_H_ADJUST_BODY_RZ:break;
		case M_H_ADJUST_BODY_RY:break;
		case M_H_ADJUST_BODY_RX:break;
		case M_H_MOVE_WHOLE_STEP:
			if (c.val1 == 0 && c.val2 == 0) { 
				HexaStop();break;
			}
			else {
				HexaMoveWholeStep(c.val1, c.val2);break;
			}
		case M_H_MOVE_CONTINUOUSLY:
			if (c.val1 == 0 && c.val2 == 0) { break; }
			else {
				HexaMoveContinuously(c.val1, c.val2);
				break;
			}
		case M_H_SPEED_UP:
			HexaSpeedUp(); break;
		case M_H_SPEED_DOWN:
			HexaSpeedDown(); break;
		case M_H_GAIT_LOOP: 
			HexaGaitLoop(); break;
		case M_H_GAIT_SELECT:
			HexaGaitSelect(c.val1); break;

		} // switch ending
	}
	hexapod.loop();
}
// this not mean start to run FK or start moving, it just mean our race is start. 
void HexaReady() {
	hexapod.isMoving = 1;
	hexapod.isWalking = 0;
}
// after ready (leg is touching ground), let's go
void HexaStart() {
	hexapod.isMoving = hexapod.isWalking = 1;
}
// but this is mean that hexapod to stop its movement
void HexaStop() {
	hexapod.isMoving = hexapod.isWalking = 0;
}
int HexaBodyTranslationZ(int z) {
	if(hexapod.COH_T[2][3]+z <= hexapod.Pz_max())
		hexapod.COH_T[2][3] += z;
	return 0;
}

int HexaBodyTranslationXY(int x, int y) {
	return 0;
}
void HexaSpeedUp() {
	int &a = (hexapod.move.speed);
	switch(a) {
	case HEXA_SPEED_ExtraSlow: 
		a = HEXA_SPEED_Slow; break;
	case HEXA_SPEED_Slow: 
		a = HEXA_SPEED_Regular; break;
	case HEXA_SPEED_Regular: 
		a = HEXA_SPEED_Fast; break;
	case HEXA_SPEED_Fast:
	case HEXA_SPEED_ExtraFast: 
		a = HEXA_SPEED_ExtraFast;
	}
}
void HexaSpeedDown() {
	int &a = (hexapod.move.speed);
	switch (a) {
	case HEXA_SPEED_ExtraFast:
		a = HEXA_SPEED_Fast; break;	
	case HEXA_SPEED_Regular:
		a = HEXA_SPEED_Slow; break;
	case HEXA_SPEED_Fast:
		a = HEXA_SPEED_Regular; break;
	case HEXA_SPEED_Slow:
	case HEXA_SPEED_ExtraSlow:
		a = HEXA_SPEED_ExtraSlow;
	}
}
void HexaGaitSelect(int gaitID) {
	hexapod.gaitType = gaitID;
}
void HexaGaitLoop() {
	int a = hexapod.gaitType;
	if (a == GAIT_MAX) {
		HexaGaitSelect(GAIT_MIN);
	}
	else {
		HexaGaitSelect(a + 1);
	}
}
void HexaMoveWholeStep(double xdir, double ydir) {
	HexaStart();
	HexaSetDirection(xdir, ydir);
	hexapod.ignoreCommand = 1;
	hexapod.lastTimeStamp = hexapod.gaitTimeStamp;
	hexapod.isMoving = 1;
	hexapod.isWalking = 1;
}
void HexaMoveContinuously(double xdir, double ydir) {
	HexaStart();
	HexaSetDirection(xdir, ydir);
	hexapod.ignoreCommand = 0;
	hexapod.isMoving = 1;
	hexapod.isWalking = 1;

}
void HexaSetDirection(double xdir, double ydir) {
	if (vec4(xdir, ydir, 0).dot(hexapod.move.last_dir) >= 0) {// <90
		hexapod.move.setDirection(xdir, ydir);
		if (hexapod.move.speed<0) {
			hexapod.move.speed *= -1;
			//hexapod.gaitTimeStamp += GAIT_CYCLE_TIME*0.2;
		}
	}
	else {
		hexapod.move.setDirection(-xdir, -ydir);
		if (hexapod.move.speed>0) {
			hexapod.move.speed *= -1;
			//hexapod.gaitTimeStamp += GAIT_CYCLE_TIME*0.2;
		}
	}
}