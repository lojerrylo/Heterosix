#include"het_hexapod.h"

// ================= hexapod constant ================
double coxia_dir_angle[6] = { 0 };
double coxia_max_angle;
double coxia_min_angle;
double femur_max_angle;
double femur_min_angle;
double tibia_max_angle;
double tibia_min_angle;
// coxia position angle, angle limit of the three motor
void hexa_const_initialize() {
	coxia_dir_angle[RM] = atan2(0, HEXA_M);
	coxia_dir_angle[RF] = atan2(HEXA_S, HEXA_F);
	coxia_dir_angle[LF] = atan2(HEXA_S, -HEXA_F);
	coxia_dir_angle[LM] = atan2(0, -HEXA_M);
	coxia_dir_angle[LB] = atan2(-HEXA_S, -HEXA_B);
	coxia_dir_angle[RB] = atan2(-HEXA_S, HEXA_B);
	coxia_max_angle = femur_max_angle = dtr(90 - OVERSHOOT_PREVENT_ANGLE_d);
	coxia_min_angle = femur_min_angle = dtr(-90 + OVERSHOOT_PREVENT_ANGLE_d);
	tibia_max_angle = dtr(-OVERSHOOT_PREVENT_ANGLE_d);
	tibia_min_angle = dtr(-180 + OVERSHOOT_PREVENT_ANGLE_d);
}

// =============== hexapods definition ==================
// default constructor (we only need one anyway
hexapods::hexapods() {
	hexa_const_initialize();
	COH_T = COH_T.Tran(0, 0, INI_COH_HEIGHT); // initial hexapod is horizontal, and displace certain amount.
	for (int i = 0; i < 6; i++) {
		coxia_angle[i] = 0;
		femur_angle[i] = dtr(INI_FEMUR_ANGLE_d);
		tibia_angle[i] = dtr(INI_TIBIA_ANGLE_d);
	}
	isMoving = false;
	isWalking = false;
	ignoreCommand = false;
	FK();
	gaitType = GAIT_TRIPLE;
	move.speed = HEXA_SPEED_Regular;
	move.dirChanged = 0;
}
// landing on the floor, ready to move.
void hexapods::loop() {
	if (isMoving) {
		IK();
		FK();
		if (isWalking) {
			gaitTimeStamp += move.speed;
			if (gaitTimeStamp > GAIT_CYCLE_TIME) {
				gaitTimeStamp = 0;
			}
			if (gaitTimeStamp < 0) {
				gaitTimeStamp = GAIT_CYCLE_TIME;
			}
		}
	}
}

// const, the highest place we can get
double hexapods::Pz_max() {
	static const double Pzmax = TIBIA_LENGTH + FEMUR_LENGTH*cos(opa);
	return Pzmax;
}
// not const, the farthest place we can get
double hexapods::Px_max(double h) {
	return (COXIA_LENGTH + sqrt((TIBIA_LENGTH + FEMUR_LENGTH)*(TIBIA_LENGTH + FEMUR_LENGTH) - h*h));
	/*
						  coxia len
						-----------. <- femur motor
						|           \      
						|			 \     
				Pz_max	|			  \    
						|			   * <- tibia motor
						|			    \  
						|				 \ 
						| px_max	      \
						===================\ <- tiptoe
	*/
}
// const, the nearest place we can get
double hexapods::Px_min(double h) {
	if (h < TIBIA_LENGTH - FEMUR_LENGTH*cos(opa)) {
		return COXIA_LENGTH + FEMUR_LENGTH*cos(opa) + TIBIA_LENGTH*sin(acos((h + FEMUR_LENGTH*cos(opa)) / TIBIA_LENGTH));
	}
	else {
		return COXIA_LENGTH + FEMUR_LENGTH*cos(asin((TIBIA_LENGTH - h) / FEMUR_LENGTH));
	}
}
// only translate the frame to coxia axis, not rotate yet
void hexapods::coxia_axis_pos() {
	coxia_T[RM] = COH_T.Tran(HEXA_M, 0, 0);
	coxia_T[RF] = COH_T.Tran(HEXA_F, HEXA_S, 0);
	coxia_T[LF] = COH_T.Tran(-HEXA_F, HEXA_S, 0);
	coxia_T[LM] = COH_T.Tran(-HEXA_M, 0, 0);
	coxia_T[LB] = COH_T.Tran(-HEXA_B, -HEXA_S, 0);
	coxia_T[RB] = COH_T.Tran(HEXA_B, -HEXA_S, 0);
	// rotation is easy, but translating is boring.
}
// Forward Kinematic. Knowning every angle, find end position.
void hexapods::FK() {
	COH_T = TransMatrix().Tran(0, 0, COH_T.v[2][3]);
	coxia_axis_pos();
	for (int i = 0; i < 6; i++) {
		coxia_T[i] = coxia_T[i].zRot(coxia_dir_angle[i]).zRot(coxia_angle[i]);
		femur_T[i] = coxia_T[i].Tran(COXIA_LENGTH, 0, 0).yRot(femur_angle[i]);
		tibia_T[i] = femur_T[i].Tran(FEMUR_LENGTH, 0, 0).yRot(tibia_angle[i]);
		tiptoe[i] = tibia_T[i].Tran(TIBIA_LENGTH, 0, 0).Origin();
		isOnTheGround[i] = (tiptoe[i].z == 0); 
	}
}
// true if need to turn the gait cycle time
bool hexapods::moves::setDirection(double x, double y) {
	dirChanged = 1;
	last_dir = dir;
	dir = vec4(x, y, 0).unit();
	if (dir.dot(last_dir) < 0) {
		return 1;
	}
	else {
		return 0;
	}
}
// [0~17] get motor angle by loop
double hexapods::MotorAngle(int motorID /*0~17*/) {
	int m = motorID % 3;
	int l = motorID / 3;
	switch (m)
	{
	case 0: return coxia_angle[l];
	case 1: return femur_angle[l];
	case 2: return tibia_angle[l];
	default:return 0;
	}
}
// [0~5, 0~2] get motor angle by legID and motorID 
double hexapods::MotorAngle(int legID /*0~5, reference to het_hexa_cfg.h */
						  , int motor /*0~2, COXIA:0, FEMUR:1, TIBIA:2 */){
	switch (motor)
	{
	case 0: return coxia_angle[legID];
	case 1: return femur_angle[legID];
	case 2: return tibia_angle[legID];
	default:return 0;
	}
}
int hexapods::moves::dir_id() {
	int ID = 0;
	double theta_dir = atan2(dir.y, dir.x);
	if (theta_dir < coxia_dir_angle[5] - PI / 2) ID = 7;
	else if (theta_dir < coxia_dir_angle[4])ID = 8;
	else if (theta_dir < -PI / 2) ID = 9;
	else if (theta_dir < coxia_dir_angle[5])ID = 10;
	else if (theta_dir < coxia_dir_angle[1]-PI/2)ID = 11;
	else if (theta_dir < 0) ID = 12;
	else if (theta_dir < coxia_dir_angle[5]+PI/2)ID = 1;
	else if (theta_dir < coxia_dir_angle[1])ID = 2;
	else if (theta_dir < PI / 2)ID = 3;
	else if (theta_dir < coxia_dir_angle[2])ID = 4;
	else if (theta_dir < coxia_dir_angle[1] + PI / 2)ID = 5;
	else ID = 6;
	return ID;
}
void hexapods::parameter_show() {
	system("cls");
	cout << setw(10) << "leg" << setw(10) << "part" << setw(10) << "angle" << setw(10)
		<< "x" << setw(10) << "y" << setw(10) << "z" << endl << setprecision(5);
	for (int i = 0; i < 6; i++) {
		cout << setw(10) << i
			<< setw(10) << "coxia" << setw(10) << rtd(coxia_angle[i]) << setw(10) << coxia_T[i].Origin() << endl
			<< setw(20) << "femur" << setw(10) << rtd(femur_angle[i]) << setw(10) << femur_T[i].Origin() << endl
			<< setw(20) << "tibia" << setw(10) << rtd(tibia_angle[i]) << setw(10) << tibia_T[i].Origin() << endl
			<< setw(20) << "tiptoe" << setw(10) << "x" << setw(10) << tiptoe[i] << endl << endl;
	}
	cout << "gaittimestamp: " << gaitTimeStamp << " , h: " << COH_T.v[2][3] << endl;
	cout << "Px min: " << Px_min(COH_T[2][3]) << ",  Px man: " << Px_max(COH_T[2][3]) << endl;
	cout << "mov dir:: " << move.dir << endl;
	for (int i = 0; i < 6; i++) {
		cout << "stride start " << i << ":\t" << stride_start[i] << endl;
		cout << "stride end " << i << "  :\t" << stride_end[i] << endl;
		cout << "stride now " << i << "  :\t" << stride_now[i] << "length:  " << stride_now[i].length() << endl;
	}
}