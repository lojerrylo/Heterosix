#pragma once
#include"het_math.h"
#include"het_main.h"
#define GAIT_CYCLE_TIME 360.0
#define LIFTING_HEIGHT 5.0
#define opa dtr(OVERSHOOT_PREVENT_ANGLE_d) // overshoot prevention angle literally
typedef struct _stride_par {
	double v1;
	double v2;
}stride_par;
typedef struct _gait_state {
	int ID;
	_stride_par x;
}gait_state;
class hexapods {
public:
	// member
	TransMatrix COH_T;		// center of hexapod
	TransMatrix coxia_T[6];	// coxia motor frame position. note that x-unit is parallel to the coxia itself.
	TransMatrix femur_T[6]; // femur motor frame position. note that x-unit is parallel to the femur itself.
	TransMatrix tibia_T[6]; // tibia motor frame position. note that x-unit is parallel to the tibia itself.
	vec4 tiptoe[6];			// point that touch the ground
	double coxia_angle[6];	// the amount of coxia matrix rotation /alpha
	double femur_angle[6];	// the amount of femur motor rotation /beta
	double tibia_angle[6];	// the amount of tibia motor rotation /gamma

	bool isOnTheGround[6];  // where is my itchy buttom

	// short gaitID;
	int gaitType;
	double gaitTimeStamp;	//	from 0 to GAIT_CYCLE_MAX 
	double lastTimeStamp;
	bool isMoving;	// do we need to solve IK. (for example, while robotic arm is moving, hexapod is not moving at all.
	bool isWalking;	// is leg need to depart the ground
	bool ignoreCommand;

	// total stride and mov dir: <------------------
	vec4 stride_start[6];	//   ^						* cus when the leg is touching ground, the trajectory is actually backward...
	vec4 stride_end[6];		//                     ^	  (is not definitely not an excuss for my bad planning)
	vec4 stride_now[6];		// depend on time

// action member class
	class moves {
	public:
		bool dirChanged;
		vec4 dir;
		vec4 last_dir;
		bool ing;
		int speed;
		moves() :dir(0, 1, 0) {}
		bool setDirection(double x, double y);
		int dir_id();
		// void translate(double dx, double dy, double dz);
		// void rotate(vec4 V, double dtheta);
		// void rotate(double x_theta, double y_theta, double z_theta);
	}move;

	hexapods();// default constructor

	void loop();	// when comtroller press "start"


	double Pz_max();		// physic constant
	double Px_max(double h);	// depend on body height
	double Px_min(double h);	// depend on body height
	void coxia_axis_pos();
	void FK();
	void IK();
	void setStrideVec(int ledID, double l);
	double setStrideVecNow(int legID, double stride_l, double stride_p);
	double decideStrideLength(double true_Pz);
	_stride_par decideStrideHeight(int);
	_stride_par decideHeight_x(_gait_state c);
	void leg_IK(double Pz, double Px, int ID);
	double MotorAngle(int motorID);
	double MotorAngle(int legID, int motor);

// testing only

#ifndef ARDUINO
	void parameter_show();
#endif // !ARDUINO

};

extern double coxia_dir_angle[6];
extern double coxia_max_angle;
extern double coxia_min_angle;
extern double femur_max_angle;
extern double femur_min_angle;
extern double tibia_max_angle;
extern double tibia_min_angle;