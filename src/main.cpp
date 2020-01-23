#include <iostream>
#include "detecttennis.h"
#include "dcmotor.h"
#include "US-I2C_servo.h"

using namespace std;

int main()
{
    DetectTennis detection1; // Constructeur
    DcMotor moteurs; // Constructeur
    vector<pair<int, int>> listeBalles;
    unsigned short dist, distTotal(0);
    servoCommute(true); // On verifie que la partie mobile soit en haut
    while(1) // boucle infinie
    {
        debutDetection:
        if(detection1.detect()) // Si une balle est detectee
        {
            listeBalles=detection1.listBalles();
            sort(listeBalles.begin(), listeBalles.end(), DetectTennis::pairCompare); // On trie les balles par ordre décroissant de distance
            moteurs.pivot(listeBalles[0].second); // On s’aligne avec la balle la plus proche
            if((listeBalles[0].first > 150) && (I2Cdetect(3)>100)) // Si la balle est loin
            {
                moteurs.marche(20); // On avance de 20 cm
                distTotal+=20;
                goto debutDetection; // On recommence la detection
            }
            do // tant que la balle est a plus de 10 cm
            {
                dist=I2Cdetect(3)-10;
                cout << dist << endl;
                if(dist > 100) // Si il y a une erreur dans la detection
                {
                    moteurs.marche(-10); // On recule de 10 cm pour recommencer la detection
                    distTotal-=10;
                }
                else
                {
                    moteurs.marche(dist); // On va sur la balle
                    distTotal+=dist;
	    }
            }while(dist>10);
            servoCommute(false); // On attrape la balle
            moteurs.marche(-distTotal); // On recule de la distance totale parcourue
            servoCommute(true); // On lache la balle
            distTotal=0;
            moteurs.marche(-20); // On recule de 20 cm
            moteurs.pivot(30); // On pivote de 30° pour trouver une autre balle
        }
        else
        {
            moteurs.pivot(30); // On pivote de 30° pour trouver une autre balle
        }
    }
    return 0;
}
