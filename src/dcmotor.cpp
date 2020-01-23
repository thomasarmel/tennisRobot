#include "dcmotor.h"

using namespace std;

DcMotor::DcMotor()
{
    unable=21;
    wiringPiSetup(); // On initialise la bibliotheque WiringPi
    pinMode(pin1[0], OUTPUT); // On regle les pins en mode sortie
    pinMode(pin1[1], OUTPUT);
    pinMode(pin1[2], OUTPUT);
    pinMode(pin1[3], OUTPUT);
    pinMode(pin2[0], OUTPUT);
    pinMode(pin2[1], OUTPUT);
    pinMode(pin2[2], OUTPUT);
    pinMode(pin2[3], OUTPUT);
    pinMode(unable, OUTPUT);
}

DcMotor::~DcMotor()
{
}

void DcMotor::marche(short dist)
{
    // 1 tour = 200 pas = 18,85 cm
    // delai 5 ms -> 18,85 cm/s
    // 1 pas=0.0943 cm
    int nbPas;
    unsigned int delai=20; // Delai entre chaque pas : 20 ms (change la vitesse)
    digitalWrite(unable, 1); // On alimente les moteurs
    if(dist >= 0) // On avance
    {
        nbPas=dist/0.0943;
        for(unsigned int i=0; i<nbPas; i++)
        {
            prochainStep(3 - (i % 4), pin1); // les moteurs 1 et 2 sont symetriques, donc inverses
            prochainStep(i % 4, pin2);
            delay(delai);
        }
    }
    else // On recule
    {
        nbPas=(-dist)/0.0943;
        for(unsigned int i=0; i<nbPas; i++)
        {
            prochainStep(i % 4, pin1); // les moteurs 1 et 2 sont symetriques, donc inverses
            prochainStep(3 - (i % 4), pin2);
            delay(delai);
        }
    }
    digitalWrite(unable, 0); // On enleve l’alimentation des moteurs
}

void DcMotor::pivot(short deg)
{
    // dist 360°: 17 cm * pi = 53,407075111 cm
    int nbPas;
    unsigned int delai=20;
    digitalWrite(unable, 1); // On alimente les moteurs
    if(deg >= 0)
    {
        nbPas=deg*1.5732;
        for(unsigned int i=0; i<nbPas; i++)
        {
            prochainStep(i % 4, pin1);
            prochainStep(i % 4, pin2);
            delay(delai);
        }
    }
    else
    {
        nbPas=(-deg)*1.5732;
        for(unsigned int i=0; i<nbPas; i++)
        {
            prochainStep(3 - (i % 4), pin1);
            prochainStep(3 - (i % 4), pin2);
            delay(delai);
        }
    }
    digitalWrite(unable, 0); // On enleve l’alimentation des moteurs
}

void DcMotor::prochainStep(unsigned short numero, unsigned short pin[])
{
switch(numero)  // fonctionnement « pas complets »
    {
    case 0:
        reglage_pins(1, 0, 0, 0, pin);
        break;
    case 1:
        reglage_pins(0, 0, 1, 0, pin);
        break;
    case 2:
        reglage_pins(0, 1, 0, 0, pin);
        break;
    case 3:
        reglage_pins(0, 0, 0, 1, pin);
        break;
    }
}

void DcMotor::reglage_pins(unsigned short p1, unsigned short p2, unsigned short p3, unsigned short p4, unsigned short pin[])
{
    digitalWrite(pin[0], p1);
    digitalWrite(pin[1], p2);
    digitalWrite(pin[2], p3);
    digitalWrite(pin[3], p4);
}
