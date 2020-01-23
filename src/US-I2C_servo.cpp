#include "US-I2C_servo.h"

using namespace std;

unsigned short I2Cdetect(unsigned short nbMesures)
{
    unsigned short adress=0x70, bus=1; // L’adresse constante du SRF08 et le numero du bus I2C
    int fd;
    vector<int> dist(nbMesures);
    fd=wiringPiI2CSetup(adress); // On initialise la bibliotheque WiringPi
    if(fd == -1) // Si erreur I2C (pas branche, mauvaise adresse...)
    {
        cout << "Erreur initialisation I2C" << endl;
        return -1;
    }
    unsigned short i;
    for(i=0; i<nbMesures; i++) // On peut faire plusieurs mesures pour plus de precision
    {
        wiringPiI2CWriteReg8(fd, 0, 0x51); // On donne l’unite avec laquelle encoder la reponse (cm), l’actualise
        delay(500); // On patiente pour que le SRF08 effectue la mesure
        dist[i]=((wiringPiI2CReadReg8(fd, 2)*256) + wiringPiI2CReadReg8(fd, 3)); // On lit la distance (2 octets)
    }
    sort(dist.begin(), dist.end()); // On trie les distances par ordre croissant
    return dist[dist.size()/2]; // On recupere la valeur mediane
}

void servoCommute(bool haut)
{
    wiringPiSetup(); // On initialise la bibliotheque WiringPi
    unsigned short pinServo(22), temps; // Pin de contrôle du servomoteur
    if(haut) // Si on doit lever
    {
        temps=1000;
    }
    else // Si on doit baisser
    {
        temps=2500;
    }
    pinMode(pinServo, OUTPUT); // Pin en mode sortie
    for(unsigned short i(0); i<35; i++) // Signal carre « en creneaux », le servomoteur fait 180° en 20*35ms=0,7s
    {
        digitalWrite(pinServo, 1);
        delayMicroseconds(temps);
        digitalWrite(pinServo, 0);
        delayMicroseconds(20000-temps); // Periode : 20 ms c-a-d 20 000µs
    }
}
