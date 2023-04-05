#include"het_hexa_cfg.h"
#include"het_hexapod.h"
#include"het_math.h"
// Inverse Kinematic
void hexapods::IK() {
	bool invaildIKsol = 1;
	double True_Pz = COH_T.v[2][3];	// for now(the hexapod doesn't incline) Pz is same for all 6 legs
	double True_Px;					// this would be different through
	double stride_length = decideStrideLength(True_Pz);
	while (invaildIKsol) {
		for (int i = 0; i < 6; i++) {
			setStrideVec(i, stride_length);
			_stride_par h_and_percent = decideStrideHeight(i);
			True_Px = setStrideVecNow(i, stride_length, h_and_percent.v2);
			leg_IK(True_Pz - h_and_percent.v1, True_Px, i);
		}
		move.dirChanged = 0;
		invaildIKsol = 0;	// assume all vaild in this version
	}

}
int WAVE_42_D3(int k) {
	switch (k) {
	case 0: return 0;
	case 1:return 3;
	case 2:return 4;
	case 3:return 1;
	case 4:return 2;
	case 5:return 5;
	}
}
int WAVE_42_D2(int k) {
	switch (k) {
	case 0: return 0;
	case 1:return 3;
	case 2:return 5;
	case 3:return 1;
	case 4:return 4;
	case 5:return 2;
	}
}
int WAVE_33_C(int k) {
	switch (k) {
	case 0: return 0;
	case 1:return 1;
	case 2:return 4;
	case 3:return 3;
	case 4:return 2;
	case 5:return 5;
	}
}
int WAVE_33_T(int k) {
	switch (k) {
	case 0: return 0;
	case 1:return 4;
	case 2:return 1;
	case 3:return 3;
	case 4:return 2;
	case 5:return 5;
	}
}
// according to timestamp to decide the leg is in the air or on the ground ;)
_stride_par hexapods::decideStrideHeight(int legID) {
	int groupID; double t;
	_stride_par temp;
	switch (gaitType) {
	case GAIT_WAVE_51:
		groupID = legID;
		t = gaitTimeStamp - groupID*GAIT_CYCLE_TIME / 6.0;
		break;
	case GAIT_WAVE_42_D3:
		groupID = WAVE_42_D3(legID);
		t = gaitTimeStamp - groupID*GAIT_CYCLE_TIME / 6.0;
		break;
	case GAIT_WAVE_42_D2:
		groupID = WAVE_42_D2(legID);
		t = gaitTimeStamp - groupID*GAIT_CYCLE_TIME / 6.0;
		break;
	case GAIT_WAVE_33_C:
		groupID = WAVE_33_C(legID);
		t = gaitTimeStamp - groupID*GAIT_CYCLE_TIME / 6.0;
		break;
	case GAIT_WAVE_33_T:
		groupID = WAVE_33_T(legID);
		t = gaitTimeStamp - groupID*GAIT_CYCLE_TIME / 6.0;
		break;
	case GAIT_RIPPLE:
		groupID = legID % 3; // group 0, 1, 2
		t = gaitTimeStamp - groupID*GAIT_CYCLE_TIME / 3.0;
		break;
	case GAIT_TRIPLE:
		groupID = legID % 2; // group 0, 1
		t = gaitTimeStamp - groupID*GAIT_CYCLE_TIME / 2.0;
		break;
	}

	t += GAIT_CYCLE_TIME*(t < 0); 
	t /= GAIT_CYCLE_TIME;
	_gait_state gaitState; // val1 for height, val2 for stride percentage
	if (t < 3.0 / 15.0) {
		gaitState.ID = SWING_FORWARD;
		gaitState.x.v1 = t*15.0 / 3.0;
		gaitState.x.v2 = 1 - t*15.0 / 3.0;
	}
	else if (t < 4.0 / 15.0) {
		gaitState.ID = SWING_BACKWARD_LANDING;
		gaitState.x.v1 = (t - 3.0 / 15.0)*15.0 / (4.0 - 3.0);
		gaitState.x.v2 = (t - 3.0 / 15.0)*15.0 / (15.0 - 3.0);
	}
	else if (t < 14.0 / 15.0) {
		gaitState.ID = SUPPORTING;
		gaitState.x.v1 = (t - 4.0 / 15.0)*15.0 / (14.0 - 4.0);
		gaitState.x.v2 = (t - 3.0 / 15.0)*15.0 / (15.0 - 3.0);
	}
	else {
		gaitState.ID = SWING_BACKWARD_LIFTING;
		gaitState.x.v1 = (t - 14.0 / 15.0)*15.0 / (15.0 - 14.0);
		gaitState.x.v2 = (t - 3.0 / 15.0)*15.0 / (15.0 - 3.0);
	}
	temp = decideHeight_x(gaitState);
	return temp;
}
_stride_par hexapods::decideHeight_x(_gait_state c) {
	_stride_par temp;
	temp.v2 = c.x.v2;
	if (isWalking) {
		switch (c.ID) {
		case SWING_FORWARD:
			temp.v1 = LIFTING_HEIGHT;	// remember to revise
			break;
		case SWING_BACKWARD_LANDING:
			temp.v1 = 0;// LIFTING_HEIGHT * (1 - sqrt(1 - (c.x.v1 - 1)*(c.x.v1 - 1)));
			break;
		case SUPPORTING:
			temp.v1 = 0;
			break;
		case SWING_BACKWARD_LIFTING:
			temp.v1 = 0;//  LIFTING_HEIGHT * (1 - sqrt(1 - (c.x.v1 *c.x.v1)));
			break;
		}
	}
	else {
		temp.v1 = 0;
	}
	return temp;
}
// decide stride, included
double hexapods::decideStrideLength(double true_Pz) {
	return  Px_max(true_Pz) - Px_min(true_Pz);
}
// find the stride start, end, and now
void hexapods::setStrideVec(int ledID, double l) {
	stride_start[ledID] = coxia_T[ledID].Origin().XYplane();// stride_start[ledID].z = 0;
	stride_start[ledID] = stride_start[ledID].extend(Px_min(coxia_T[ledID].Origin().z) + l / 2) + (l / 2)*move.dir;
	stride_end[ledID] = stride_start[ledID] + (-l)*move.dir;

}

// dealing with alpha(coxia angle) and return Px
double hexapods::setStrideVecNow(int legID, double stride_l, double stride_p) {
	stride_now[legID] = stride_end[legID] + (stride_l * stride_p) * move.dir;
	vec4 coxia_origin_xy = coxia_T[legID].Origin().XYplane();// coxia_origin_xy.z = 0;
	vec4 coxia_to_stride_now(stride_now[legID] - coxia_origin_xy);
	int mov_dirID = move.dir_id();
	bool half_way = stride_p >= 0.5;
	bool plus_it = 1;
	switch (mov_dirID) {
	case 1:	case 2:
		if (legID == 1 || legID == 2 || legID == 3) { plus_it = 0; }
		break;
	case 3:	case 4:
		if (legID == 2 || legID == 3 || legID == 4) { plus_it = 0; }
		break;
	case 5:	case 6:
		if (legID == 3 || legID == 4 || legID == 5) { plus_it = 0; }
		break;
	case 7:	case 8:
		if (legID == 0 || legID == 4 || legID == 5) { plus_it = 0; }
		break;
	case 9:	case 10:
		if (legID == 0 || legID == 1 || legID == 5) { plus_it = 0; }
		break;
	case 11: case 12:
		if (legID == 0 || legID == 1 || legID == 2) { plus_it = 0; }
		break;
	}

	if ((plus_it && half_way)||(!plus_it && !half_way)) {
		coxia_angle[legID] = coxia_to_stride_now.angle(coxia_origin_xy);
	}
	else {
		coxia_angle[legID] = -coxia_to_stride_now.angle(coxia_origin_xy);
	}

	return coxia_to_stride_now.length();
}
// leg inverse kinematic
void  hexapods::leg_IK(double Pz, double Px, int legID) { // must put legel Px and Pz, use other function to check eligibility
	double a = COXIA_LENGTH;
	double b = FEMUR_LENGTH;
	double c = TIBIA_LENGTH;
	double e = sqrt(Pz*Pz + Px*Px);
	double d = sqrt(Pz*Pz + (Px - a)*(Px - a));
	double theta1 = atan(Pz / Px); // Px is never zero anyway
	double theta2 = atan(Pz / (Px - a)); // same reason above
	double theta3 = acos((d*d + c*c - b*b) / (2 * d*c));
	double theta4 = theta2 + theta3;
	double theta5 = acos((d*d + b*b - c*c) / (2 * b*d));
	double beta = theta5 - theta2;
	double gamma = -(theta3 + theta5);
	femur_angle[legID] = -beta; //2.
	tibia_angle[legID] = -gamma; //3.
}