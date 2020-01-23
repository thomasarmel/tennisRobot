#ifndef DCMOTOR_H_INCLUDED
#define DCMOTOR_H_INCLUDED

#include <iostream>
#include <wiringPi.h>

/** Compiler avec l'option -lwiringPi */

class DcMotor
{
public:
    DcMotor();
    ~DcMotor();
    void marche(short dist); // On avance / recule
    void pivot(short deg); // On pivote

private:
    void prochainStep(unsigned short numero, unsigned short pin[]); // Les impulsions donnees aus moteurs
    void reglage_pins(unsigned short p1, unsigned short p2, unsigned short p3, unsigned short p4, unsigned short pin[]); // 1/4 d’impulsion

    unsigned short pin1[4]={7, 0, 1, 2}; // Les pins GPIO des 2 moteurs
    unsigned short pin2[4]={3, 4, 5, 6};
    unsigned short unable; // Pin GPIO pour arreter l’alimentation des moteurs
};

#endif // DCMOTOR_H_INCLUDED
