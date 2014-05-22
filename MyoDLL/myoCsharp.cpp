//Nom : myoCsharp.cpp
// Utilisation : Librairie faisant le lien avec le sdk Myo et des applications écrites dans d’autres langages
// Date de création : 17/05/2014
// Auteurs : Thomas BLAVETTE, Sofiane BOURAHLA, Guillaume BOULIN, Matthieu MIGEON
// et Mathieu VAISSIERES
//
// Librairie créée par des étudiants de Polytech Nantes en partenariat
// avec Stéréolux. Il s'agit de développer un librairie permettant de faire le lien entre
// le SDK myo et le SDK VVVV

#include "stdafx.h"
#include "myoCsharp.hpp"
#include <myo.hpp>
#include <string>
using namespace myo;

//Appeller par le sdk pour actualiser les valeurs d'accélérations
void myoCsharp::onAccelerometerData(Myo *myo, uint64_t timestamp, const Vector3< float > &accel)
{
	acceleration = accel;
}

//Appeller par le sdk pour actualiser les valeurs de gyroscope
void myoCsharp::onGyroscopeData(Myo *myo, uint64_t timestamp, const Vector3< float > &gyro)
{
	gyroscope = gyro;
}

)//Appeller par le sdk pour actualiser les valeurs de position
void myoCsharp::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
{
	position = quat;
}

//Appeller par le sdk pour actualiser le mouvement effectués par l'utilisateur
void myoCsharp::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
{
	current = pose;
}
//Transformer les vectors 3d en tableau de float
int myoCsharp::getAcceleration(float *accel)
{
	accel[0] = acceleration.x();
	accel[1] = acceleration.y();
	accel[2] = acceleration.z();
	return 1;
}

//Transformer les vectors 3d en tableau de float
int myoCsharp::getGyroscope(float *gyro)
{
	gyro[0] = gyroscope.x();
	gyro[1] = gyroscope.y();
	gyro[2] = gyroscope.z();
	return 0;

}

//Transformer les vectors 3d en tableau de float
int myoCsharp::getPosition(float *pos)
{
	pos[0] = position.x();
	pos[1] = position.y();
	pos[2] = position.z();
	pos[3] = position.w();
	return 0;
}

//Tentative de connexion au myo
int myoCsharp::initialisation(char *a)
{
		myMyo = hub.waitForAnyMyo(10000);
		if (myMyo == NULL)
		{
		strcpy(a, "Pas de connection au myo");
			return -1;
		}
		else {
			myMyo->vibrate(myo::Myo::VibrationMedium);
			hub.addListener(this);
			strcpy(a, "ok");
			return 0;
		}
	
	return -1;

}

//Mettre à jours les données du myo
void myoCsharp::activation(unsigned int H)
{
	hub.run(H);
}

//Récupérer la gesture courrantes dans une chaine de caractères
void myoCsharp::getPose(char *a)

{
	strcpy(a, current.toString().c_str());
}

//Faire vibrer le myo en fontion de a, trois modes possible short, medium et long
void myoCsharp::vibration(int a)
{
	switch (a){
	case 1: myMyo->vibrate(myo::Myo::VibrationShort);
		break;
	case 2: myMyo->vibrate(myo::Myo::VibrationMedium);
		break;
	case 3:myMyo->vibrate(myo::Myo::VibrationLong);
	break;
	default: break;
	}
}