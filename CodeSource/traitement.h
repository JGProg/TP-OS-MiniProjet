/**
 * \file traitement.h
 * \brief header of traitement.c
 * \author jessy giacomoni and Adrien Rogier
 * \version 0.1
 * \date 01/12/12
 * Copyright (c) 2012 jessy giacomoni  and Adrien Rogier. All rights reserved.
 */


#ifndef TP_OS_traitement_h
#define TP_OS_traitement_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <string.h>

/**
 * \pre
 * \post Comparaison des valeurs depuis le fichier généré par le processus stockage.
 */
void traitement(unsigned int nbrSerie,struct sembuf *sem_P,struct sembuf *sem_V, int semaphore_Proc_Stockage_Traitement_fichier_plein, int semaphore_Proc_Stockage_Traitement_fichier_vide, int MUTEX_stockage_traitement);


void DessinerGraphe(int nbrSerie);
#endif
