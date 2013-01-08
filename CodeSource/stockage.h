/**
 * \file stockage.h
 * \brief header of stockage.c.
 * \author jessy giacomoni and Adrien Rogier
 * \version 0.1
 * \date 01/12/12
 * Copyright (c) 2012 jessy giacomoni  and Adrien Rogier. All rights reserved.
 */

#ifndef TP_OS_stockage_h
#define TP_OS_stockage_h
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
 * \post Le tableau est copié dans un fichier txt. sous la fome data_1_X.txt ou X est le numéro de la série.
 */
void stockage(int nbrAcquisition, int delaiEntreSerie, unsigned int nbrSerie,int MUTEX, struct sembuf *sem_P,struct sembuf *sem_V, int mem_ID_Proc_Stockage,int* ptr_mem_partagee, int sempahore_Proc_Acquisition_Stockage_Mem_plein, int sempahore_Proc_Acquisition_Stockage_Mem_vide,int sempahore_Proc_Stockage_Traitement_Mem_plein, int sempahore_Proc_Stockage_Traitement_Mem_vide);

#endif
