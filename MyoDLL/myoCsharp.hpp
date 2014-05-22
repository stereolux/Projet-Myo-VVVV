#pragma once
#include "stdafx.h"
#include <myo.hpp>
#include <string>

using namespace myo;


class myoCsharp : public myo::DeviceListener
{
public :

	int getAcceleration(float *accel); 
	int getGyroscope(float *gyro);
	int getPosition(float *pos);
	void onAccelerometerData(Myo *myo, uint64_t timestamp, const Vector3< float > &accel);
	void onGyroscopeData(Myo *myo, uint64_t timestamp, const Vector3< float > &gyro);
	void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat);
	void onPose(Myo* myo, uint64_t timestamp, Pose pose);
	int initialisation(char *a);
	void activation(unsigned int H);
	void getPose(char *a);
	void vibration(int a);


	Vector3 <float> acceleration;
	Vector3 < float > gyroscope;
	Quaternion < float > position;
	Pose current;
	Hub hub;
	Myo *myMyo;

};

