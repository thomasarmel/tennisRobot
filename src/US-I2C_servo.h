#ifndef USI2CSERVO_H_INCLUDED
#define USI2CSERVO_H_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>
#include <wiringPi.h>
#include <wiringPiI2C.h>

/** Compiler avec l'option -lwiringPi */

unsigned short I2Cdetect(unsigned short nbMesures); // Detection ultrasons
void servoCommute(bool haut); // Changement de position du servomoteur

#endif // USI2CSERVO_H_INCLUDED
