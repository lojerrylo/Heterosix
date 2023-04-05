#pragma once
#include"het_hexa_cfg.h"
#include"het_hexapod.h"
#include"het_math.h"
#include"commandList.h"
#include"het_Arduino_environment.h"
#ifdef ARDUINO
#include"PS2X_lib.h"
#endif
//================================================================================
//  default setting 
//================================================================================
#define INITIAL_MANIPULATOR MANIP_OPENGL // modify it.
#define INITIAL_GAIT_MODE GAIT_TRIPLE
typedef struct _command{
	int commandID;
	double val1;	// digital
	double val2;	// analog
}command;			// "_" is for... in case... any possible trouble. i.e. same name
typedef struct _coor3d{
	double x;
	double y;
	double z;
}coor3d;
typedef struct _IMU_DATA{
	vec4 xUnit;
	vec4 yUnit;
	vec4 zUnit;
}IMU_data;
void setup();
void loop();
//================================================================================
//  sub-function declare;
//================================================================================
void ConnectMotor();
void SetManipulator(int);
void ConnectPS2();
void ConnectBluetooth();
void ConnectWifi();
command ReadManipulator();
command ReadPS2();
command ReadBluetooth();
command ReadWifi();
IMU_data ReadIMU();
bool RealityMatchTheory(IMU_data IMU);
void CommandProcess(command c);
void CommandProcess(int c);
void CommandProcess(int c, double v);
void CommandProcess(int c, double x, double y);
void CommitProcess();
// Hexapod function
int HexaBodyTranslationZ(int z);
int HexaBodyTranslationXY(int x, int y);
void HexaReady();
void HexaStart();
void HexaStop();
void HexaSpeedUp();
void HexaSpeedDown();
void HexaGaitSelect(int gaitID);
void HexaGaitLoop();
void HexaMoveWholeStep(double xdir, double ydir);
void HexaMoveContinuously(double xdir, double ydir);
void HexaSetDirection(double xdir, double ydir);
// Arm function
//int Arm_EEF_Translation(int, int, int);
//int Arm_EEF_AimingAdjust(int, int);
//int Arm_EEF_AlignAiming(int);
//int Arm_R1_Rotation(int);
//int Arm_R2_Rotation(int);
//int Arm_R3_Rotation(int);
//int Arm_R4_Rotation(int);
//int Arm_R5_Rotation(int);
//int Arm_IK_HexaRotated();
command errorHandler();