//Nom : Program.cs
// Utilisation : definit le point d'entree pour l'application console.
// Date de creation : 17/05/2014
// Auteurs : Thomas BLAVETTE, Sofiane BOURAHLA, Guillaume BOULIN, Matthieu MIGEON
// et Mathieu VAISSIERES
//
// Librairie creee par des étudiants de Polytech Nantes en partenariat
// avec Stéréolux. Il s'agit de développer un librairie permettant de faire le lien entre
// le SDK myo et le SDK VVVV

//compile with: /unsafe
#region usings
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.IO;
using System.Text;
using System.Linq;

using VVVV.Utils.Streams;

using VVVV.PluginInterfaces.V1;
using VVVV.PluginInterfaces.V2;
using VVVV.Utils.Algorithm;
using VVVV.Utils.VColor;
using VVVV.Utils.VMath;
using VVVV.PluginInterfaces.V2.NonGeneric;

#endregion usings

namespace MyoNode
{
    [PluginInfo(Name = "Myo", Category = "Devices", Help = "Myo node test")]//Information sur le bloc, vu par les utilisateurs de VVVV


    public unsafe class Myo : IPluginEvaluate, IDisposable
    {
	//Prototypes des méthodes contenu dans Myo.dll
        [System.Runtime.InteropServices.DllImport("Myo.dll", EntryPoint = "CreateInstance")]
        private static extern void* CreateInstance(StringBuilder a);

        [System.Runtime.InteropServices.DllImport("Myo.dll", EntryPoint = "releaseInstance")]
        private static extern void releaseInstance(void* myo);

        [System.Runtime.InteropServices.DllImport("Myo.dll", EntryPoint = "cycle")]
        private static extern void cycle(void* myo, int H);
        [System.Runtime.InteropServices.DllImport("Myo.dll", EntryPoint = "getAccelerometer")]
        private static extern void getAccelerometer(void* myo, float* a);
        [System.Runtime.InteropServices.DllImport("Myo.dll", EntryPoint = "getGyroscope")]
        private static extern void getGyroscope(void* myo, float* a);
        [System.Runtime.InteropServices.DllImport("Myo.dll", EntryPoint = "getPosition")]
        private static extern void getPosition(void* myo, float* a);
        [System.Runtime.InteropServices.DllImport("Myo.dll", EntryPoint = "getPose")]
        private static extern void getPose(void* myo, StringBuilder a);
        [System.Runtime.InteropServices.DllImport("Myo.dll", EntryPoint = "setVibration")]
        private static extern void setVibration(void* myo, int a);


		//Déclaration des pins d'entrées et de sorties
        [Output("Output Accelerometer", AutoFlush = false)]//Pin Accelerometre
        IValueOut FoutAcc;
        [Output("Output Gyroscope", AutoFlush = false)]//Pin Gyroscope
        IValueOut FoutGyro;
        [Output("Output Quaternion", AutoFlush = false)]//Pin Quaternion
        IValueOut FoutQuat;
        [Output("Output Gesture", AutoFlush = false)]//Pin Gesture
        IStringOut Gesture;
        [Output("Output Debug", AutoFlush = false)]//Pin debogage
        IStringOut Debug;
        [Input("Input Vibration, AutoFlush =false")]//Pin pour faire vibrer le myo
        IValueIn Vibration;

		void* myo; //Pointeur contenant l'adresse de l'objet Myo créer dans la librairie externe. Est utilisé a chaque appelle d'une fonction externe 
       
	   float[] accelero;
        float[] gyro;
        float[] quat;
        StringBuilder chaine;
        StringBuilder debugg;
        static int H = 5; //Mettre à jours les données du myo toutes les 5ms
        double vibrationP=0;//Variable contenant la puissance de vibration
       
	   public void MyoNode()
        {
        }
        //Méthode appelée par le logiciel VVVV
		public void Evaluate(int spreadMax)
        {
            if (debugg == null) debugg = new StringBuilder(100);//Instanciation d'une chaine de caracteres
            //Phase d'initialisation des objets et configuration des pins de sortie
			if(myo==null || accelero==null || gyro==null || quat==null|| chaine==null){
                myo = CreateInstance(debugg);
                accelero = new float[3];
                gyro = new float[3];
                quat = new float[4];
                chaine = new StringBuilder(100);
                FoutAcc.SliceCount=3;
                FoutGyro.SliceCount = 3;
                FoutQuat.SliceCount = 4;
                Debug.SetString(0, debugg.ToString());
            }
			
            else
            {
                cycle(myo, H);//Mettre à jours les donnees du Myo
                fixed (float* ptr_accelero = accelero)
                    getAccelerometer(myo, ptr_accelero);//Récuperation des valeurs d'accelérations
                fixed (float* ptr_gyro = gyro)
                    getGyroscope(myo, ptr_gyro);//Récuperation des valeurs de gyroscope
                fixed (float* ptr_quat = quat)
                    getPosition(myo, ptr_quat);//Récuperation des valeurs de position
                getPose(myo, chaine); //Récuperation de la chaine de caractères
				//Affectation des sortiesq
                FoutAcc.SetValue3D(0, accelero[0], accelero[1], accelero[2]);
                FoutGyro.SetValue3D(0, gyro[0], gyro[1], gyro[2]);
                FoutQuat.SetValue4D(0, quat[0], quat[1], quat[2], quat[3]);
                Gesture.SetString(0, chaine.ToString());
				
                if(Vibration.PinIsChanged)Vibration.GetValue(0,out vibrationP); //Lecture du pin de vibration
               
			   setVibration(myo, (int) vibrationP);	//Faire vibrer le myo
            }

            
        }
        public void Dispose()
         {
             releaseInstance(myo);//Libérer la mémoire
         }
    }
}
