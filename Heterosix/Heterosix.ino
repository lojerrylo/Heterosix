#include"het_main.h"
#include"het_math.h"
#include"het_hexapod.h"
#include"PS2X_lib.h"
/*=======================================================
	Heterosix.ino  ver0.0.1
	Just paste in .ino file
	And I wouldn't touch it anymore.
	Any further revision, like declare bluetooth object,
	should be done in here (your kingdom)
	

	2023/04/06	Aiyung Chen
	
=========================================================*/
//================================================================================
// Global variable
//================================================================================
hexapods hexapod;
int ManipulatorNow = 0;
int CommandID = 0;
bool changeManipulator = 0;
IMU_data IMUdata;
command commands;
PS2X ps2x;


//================================================================================
//  arduino main function
//================================================================================
void setup() { // for arduino
	SetManipulator(INITIAL_MANIPULATOR);
	ConnectMotor();
}

void loop() {// arduino loop function
	if(changeManipulator){
		SetManipulator(ManipulatorNow);
	}
	IMUdata = ReadIMU();
	if(!RealityMatchTheory(IMUdata)){
		commands = errorHandler();	// errorHandler will commit command to fix error. maybe
	}
	else{
		commands = ReadManipulator();	// commanded by Manipulator.
	}	
	CommandProcess(commands);	// pure math calculation
	CommitProcess();			// set signal to motor
	delay(LOOP_TIME);	// wait
}