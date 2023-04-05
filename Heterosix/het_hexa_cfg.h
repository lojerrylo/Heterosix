#pragma once
// === constant ===
#define HEXA_F 20.0
#define HEXA_B HEXA_F	// I hope they're the same, but different is still okey.
#define HEXA_M 30.0
#define HEXA_S 40.0
#define COXIA_LENGTH  15.0
#define FEMUR_LENGTH  17.0
#define TIBIA_LENGTH  25.0
#define COXIA_CAPABLE_RANGE_d 40.0
#define OVERSHOOT_PREVENT_ANGLE_d 10.0 
// === part name ===
// leg ID
#define RM 0
#define RF 1
#define LF 2
#define LM 3
#define LB 4
#define RB 5
#define RightMiddle	RM
#define RightFront	RF
#define LeftFront	LF
#define LeftMiddle	LM
#define LeftBack	LB
#define RightBack	RB
// leg part ID/ motor ID
#define COXIA 0
#define FEMUR 1
#define TIBIA 2
#define TARSUS 3
#define FOOTTIP TARSUS
// === initiallize ===
#define INI_COH_HEIGHT 30.0
#define INI_PZ INI_COH_HEIGHT
#define INI_PX 35.0
#define INI_FEMUR_ANGLE_d -45.0
#define INI_TIBIA_ANGLE_d 135.0
// === gait selection === // see gsheet for detail.
#define GAIT_WAVE_51	1	// clockwise
#define GAIT_WAVE_42_D3	2	// D3 group sequence
#define GAIT_WAVE_42_D2	3	// D2 group sequence
#define GAIT_WAVE_33_C	4	// centerline symmetry, move all right leg, then left leg
#define GAIT_WAVE_33_T	5	// triple gait group but wave gait
#define GAIT_RIPPLE		6
#define GAIT_TRIPLE		7	
#define GAIT_MIN GAIT_WAVE_51
#define GAIT_MAX GAIT_TRIPLE

// maybe there is more...

// === movement speed ===: howmany loop to complete a move. ===
#define HEXA_SPEED_REST			0
#define HEXA_SPEED_ExtraSlow	4						
#define HEXA_SPEED_Slow			(2*HEXA_SPEED_ExtraSlow)
#define HEXA_SPEED_Regular		(3*HEXA_SPEED_ExtraSlow)	
#define HEXA_SPEED_Fast			(4*HEXA_SPEED_ExtraSlow)	
#define HEXA_SPEED_ExtraFast	(5*HEXA_SPEED_ExtraSlow)	
// === movement speed ===
#define HEXA_STRIDE_CONSERVATIVE	0
#define HEXA_STRIDE_AGGRESSIVE		1
// === gait state ===
#define SWING_FORWARD			0
#define SWING_BACKWARD_LANDING	1
#define SUPPORTING				2
#define SWING_BACKWARD_LIFTING	3