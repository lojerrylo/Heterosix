#pragma once
//================================================================================
// Environment value 
//================================================================================
// loop delay time in mili second
#define LOOP_TIME 5
// #define SERIAL_RATE 1280000 // ?

// manipulator
#define MANIP_PS2	0
#define MANIP_BT	1
#define MANIP_WIFI	2
#define MANIP_OPENGL 3
// PS2 PIN					* note: just for example, is upon you
#define PS2_PIN_CLOCK 13
#define PS2_PIN_COMMAND 11
#define PS2_PIN_ATTENTION 10
#define PS2_PIN_CS PS2_PIN_ATTENTION
#define PS2_PIN_DATA 12
// motor pin
#define COXIA_RM_PIN 0
//...... and so on
#define TIBIA_LB_PIN 17
#define ARM_M0_PIN 18
#define ARM_M1_PIN 19