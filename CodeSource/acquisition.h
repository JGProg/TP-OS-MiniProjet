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
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/sem.h>

/**
* \pre 'nbAcquisition' : Le nombre de séries d'acquisition, il ne peut pas être négatif, 'delaiEntreLesSeries' : le délai entre les séries en secondes, 'nbrAcquisition' : Le nombre d'acquisition >= 0, 'intervalleEntreDeuxAcquisition' : Représente l'intervalle entre deux acquisitions,  tabResultat[] : un pointeur vers un tableau initialisé.
* \post Le tableau 'tabResultat' est complété selon les valeurs des paramètres de la fonction.
*/
void acquisition(unsigned int nbAcquisition,unsigned int delaiEntreLesSeries, unsigned int nbSerie, unsigned int delaiEntreDeuxAcquisition,int* ptr_mem_partagee,struct sembuf *sem_P,struct sembuf *sem_V, int mem_ID_Proc_Acquisition, int MUTEX_acquisition_stockage, int semaphore_Proc_Acquisition_Stockage_Mem_plein, int semaphore_Proc_Acquisition_Stockage_Mem_vide);


/** 
*   \pre None
*   \post On retourne l'heure actuelle en milliseconds.
*/
int recupereHeure(void);

#endif
