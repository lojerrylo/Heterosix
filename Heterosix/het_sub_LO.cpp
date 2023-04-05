#include"het_main.h"
#include"het_Arduino_environment.h"

#ifdef ARDUINO
#include"PS2X_lib.h"
extern PS2X ps2x;
#endif

extern hexapods hexapod;
extern int ManipulatorNow;
extern int CommandID;
extern bool changeManipulator;
//================================================================================
//  sub-function definition
//================================================================================
void ConnectMotor() {
	// pinMode(COXIA_RM_PIN, OUTPUT); ...
}
// literally.
void SetManipulator(int s) {
	ManipulatorNow = s;
	switch (s) {
	case MANIP_PS2:
		ConnectPS2();
		break;
	case MANIP_BT:
		ConnectBluetooth();
		break;
	case MANIP_WIFI:
		ConnectWifi();
		break;
	}
}
// connect PS2. dont return anything, loop itself until connect ;)
void ConnectPS2() { 
#ifdef ARDUINO
	int error;
	do {
	// GamePad(clock, command, attention, data, Pressures, Rumble)
	error = ps2x.config_gamepad(PS2_PIN_CLOCK, PS2_PIN_COMMAND, PS2_PIN_CS, PS2_PIN_DATA, true, true);
	if (error == 0) { break; }
	else { delay(100); }
	} while (1);
#endif
}
// connect BT. dont return anything, loop itself until connect ;)
void ConnectBluetooth(){
	// not now
}
// connect wifi. dont return anything, loop itself until connect ;)
void ConnectWifi(){
	// not now
}

command ReadManipulator() {
	switch (ManipulatorNow) { // no break needed
	case MANIP_PS2:		return ReadPS2();
	case MANIP_BT:		return ReadBluetooth();
	case MANIP_WIFI:	return ReadWifi();
	}
	return command{ DO_NOTHING,0,0 };
}
command ReadPS2() {

	// I'll write it
	CommandID = 0;
	// deal with moving command

	return command{ 0,0 ,0 }; // return command ID
}
command ReadBluetooth() {
	return command{ DO_NOTHING,0,0 };
}
command ReadWifi() {
	return command{ DO_NOTHING,0,0 };
}

// literally. but don't return real data, return the x, y, z unit vector to me. 
IMU_data ReadIMU() {
	// write it
	IMU_data temp;
	temp.xUnit = vec4(1, 0, 0); // temp.xUnit = vec4(1,1,1).unit();
	temp.yUnit = vec4(0, 1, 0);
	temp.zUnit = vec4(0, 0, 1);
	return temp;
}
// true if IMU say the direction vector is in a certain (reasonable) range.
bool RealityMatchTheory(IMU_data IMU) {
	// I'll write it.
	return true;
}
// let motor know what should do
void CommitProcess() {
	// write it
	// and define your function here (not het_main.h)
	// too lazy to use github so u know
}

/*
_command errorHandler()
void CommandProcess(_command c)
is in "het_sub_AY"
*/