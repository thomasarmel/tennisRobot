#include <iostream>
#include <pthread.h>
#include <wiringPi.h>

using namespace std;

void marche_avant(unsigned int delai, int nbPas, unsigned short pin[]);
void marche_arriere(unsigned int delai, int nbPas, unsigned short pin[]);
void prochainStep(unsigned short numero, unsigned short pin[]);
void reglage_pins(unsigned short p1, unsigned short p2, unsigned short p3, unsigned short p4, unsigned short pin[]);
void *fThreadRoue1(void *arg);
void *fThreadRoue2(void *arg);

namespace
{
    unsigned short pin1[]={7, 0, 1, 2}, pin2[]={3, 4, 5, 6}, unable=21;
    unsigned int delaiPrompt;
    int nbPasPrompt;
}

int main()
{
    pthread_t threadRoue1, threadRoue2;
    wiringPiSetup();
    pinMode(pin1[0], OUTPUT);
    pinMode(pin1[1], OUTPUT);
    pinMode(pin1[2], OUTPUT);
    pinMode(pin1[3], OUTPUT);
    pinMode(pin2[0], OUTPUT);
    pinMode(pin2[1], OUTPUT);
    pinMode(pin2[2], OUTPUT);
    pinMode(pin2[3], OUTPUT);
    pinMode(unable, OUTPUT);
    digitalWrite(unable, 1);
    cout << "Le délai entre chaque pas permet de contrôler la vitesse du moteur." << endl;
    cout << "Entrez une valeur d'au moins 5 millisecondes (rotation très rapide)." << endl;
    cout << "Délai désiré: ";
    cin >> delaiPrompt;
    cout << "Choisissez le nombre de pas accomplis par le moteur." << endl;
    cout << "(Une valeur négative fera tourner le moteur en sens inverse.)" << endl;
    cout << "Nombre de pas désiré: ";
    cin >> nbPasPrompt;

    if(pthread_create(&threadRoue1, NULL, fThreadRoue1, NULL))
    {
	    cout << "Erreur lors de la creation du thread (roue 1)" << endl;
	    return EXIT_FAILURE;
    }
    if(pthread_create(&threadRoue2, NULL, fThreadRoue2, NULL))
    {
	    cout << "Erreur lors de la creation du thread (roue 2)" << endl;
	    return EXIT_FAILURE;
    }

    if(pthread_join(threadRoue1, NULL))
    {
	    cout << "Erreur pthread_join (roue 1)" << endl;
	    return EXIT_FAILURE;
    }
    if(pthread_join(threadRoue2, NULL))
    {
	    cout << "Erreur pthread_join (roue 2)" << endl;
	    return EXIT_FAILURE;
    }

    digitalWrite(unable, 0);
    return 0;
}

void marche_avant(unsigned int delai, int nbPas, unsigned short pin[])
{
    for(unsigned int i=0; i<nbPas; i++)
    {
        prochainStep(i % 4, pin);
        delay(delai);
    }
}

void marche_arriere(unsigned int delai, int nbPas, unsigned short pin[])
{
    for(unsigned int i=0; i<nbPas; i++)
    {
        prochainStep(3 - (i % 4), pin);
        delay(delai);
    }
}

void prochainStep(unsigned short numero, unsigned short pin[])
{
    /*switch(numero)  // Couple maximal
    {
    case 0:
        reglage_pins(1, 0, 1, 0, pin);
        break;
    case 1:
        reglage_pins(0, 1, 1, 0, pin);
        break;
    case 2:
        reglage_pins(0, 1, 0, 1, pin);
        break;
    case 3:
        reglage_pins(1, 0, 0, 1, pin);
        break;
    }*/
switch(numero)  // pas complets
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

void reglage_pins(unsigned short p1, unsigned short p2, unsigned short p3, unsigned short p4, unsigned short pin[])
{
    digitalWrite(pin[0], p1);
    digitalWrite(pin[1], p2);
    digitalWrite(pin[2], p3);
    digitalWrite(pin[3], p4);
}

void *fThreadRoue1(void *arg)
{
    if (nbPasPrompt < 0)
    {
        marche_arriere(delaiPrompt, -nbPasPrompt, pin1);
    }
    else
    {
        marche_avant(delaiPrompt, nbPasPrompt, pin1);
    }
    (void)arg;
    pthread_exit(NULL);
}

void *fThreadRoue2(void *arg) // Attention: tourne à l'envers du fait des engrenages
{
    if (nbPasPrompt < 0)
    {
        marche_avant(delaiPrompt, -nbPasPrompt, pin2);
    }
    else
    {
        marche_arriere(delaiPrompt, nbPasPrompt, pin2);
    }
    (void)arg;
    pthread_exit(NULL);
}


/**
HIGH=1
LOW=0
*/
