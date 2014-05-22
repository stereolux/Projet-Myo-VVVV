//Nom : dllmain.cpp
// Utilisation : Librairie faisant le lien avec le sdk Myo et des applications écrites dans d’autres langages
// Date de creation : 17/05/2014
// Auteurs : Thomas BLAVETTE, Sofiane BOURAHLA, Guillaume BOULIN, Matthieu MIGEON
// et Mathieu VAISSIERES
//
// Librairie creee par des étudiants de Polytech Nantes en partenariat
// avec Stéréolux. Il s'agit de développer un librairie permettant de faire le lien entre
// le SDK myo et le SDK VVVV

#include "stdafx.h"




BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
//Créer une instance d'un objet myoCsharp pour utiliser le sdk myo
//Prend un parametre un pointeur de chaine de caracteres pour retourner les erreurs eventuelle
extern "C" __declspec(dllexport) void* CreateInstance(char *a)
{
	try{
		myoCsharp* p = new myoCsharp;
		if (p->initialisation(a) < 0) return NULL; //Si initialisation retourne un nombre inférieur à 0, signal qu'il y a un problème
		return p; //Retourne pointeur d'objet
	}
	catch (std::exception e){
		strcpy(a, e.what());//Copie de la chaine de caractères d'erreur pour remonter dans la lib C#
		return NULL;
	}
}



//Raffraichissement des donnees
extern "C" __declspec(dllexport) void cycle(void *myo, int H)
{
	myoCsharp *p = (myoCsharp*) myo;
	p->activation(H);
}
//Detruit l'instance, libere la mémoire
extern "C" __declspec(dllexport) void releaseInstance(void *myo)
{
	myoCsharp *p = (myoCsharp*)myo;
	delete p;
}

//Recuperer les donnees de l'accelerometre
extern "C" __declspec(dllexport) void getAccelerometer(void* myo,float *a)
{
	myoCsharp* p = (myoCsharp*)myo;
	p->getAcceleration(a);
}

//Recuperer les donnees du Gyroscope
extern "C" __declspec(dllexport) void getGyroscope(void* myo, float *a)
{
	myoCsharp* p = (myoCsharp*)myo;
	p->getGyroscope(a);
}

//Recuperer les donnees de la position
extern "C" __declspec(dllexport) void getPosition(void* myo, float *a)
{
	myoCsharp* p = (myoCsharp*)myo;
	p->getPosition(a);
}

//Recuperer la gesture
extern "C" __declspec(dllexport) void getPose(void* myo, char *a)
{
	myoCsharp* p = (myoCsharp*)myo;
	 p->getPose(a);
}

//Envoyer un signal au myo pour le faire vibrer
extern "C" __declspec(dllexport) void setVibration(void* myo, int a)
{
	myoCsharp* p = (myoCsharp*)myo;
	p->vibration(a);
}