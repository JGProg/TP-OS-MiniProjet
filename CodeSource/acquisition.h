/**
* \file acquisition.h
* \brief Header of acquisition.c.
* \author jessy giacomoni and Adrien Rogier
* \version 0.1
* \date 01/12/12
* Copyright (c) 2012 jessy giacomoni  and Adrien Rogier. All rights reserved.
*/


#ifndef TP_OS_acquisition_h
#define TP_OS_acquisition_h

#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
/**
* \pre 'nbAcquisition' : Le nombre de séries d'acquisition, il ne peut pas être négatif, 'delaiEntreLesSeries' : le délai entre les séries en secondes, 'nbrAcquisition' : Le nombre d'acquisition >= 0, 'intervalleEntreDeuxAcquisition' : Représente l'intervalle entre deux acquisitions,  tabResultat[] : un pointeur vers un tableau initialisé.
* \post Le tableau 'tabResultat' est complété selon les valeurs des paramètres de la fonction.
*/
void acquisition(unsigned int nbAcquisition,unsigned int delaiEntreLesSeries, unsigned int nbrAcquisition, unsigned int intervalleEntreDeuxAcquisition,int* tabResultat);


/** 
*   \pre None
*   \post On retourne l'heure actuelle en milliseconds.
*/
int recupereHeure(void);

#endif
