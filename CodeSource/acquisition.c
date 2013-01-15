/**
 * \file acquisition.c
 * \brief Content of acquisition.c.
 * \author jessy giacomoni and Adrien Rogier
 * \version 0.1
 * \date 01/12/12
 * Copyright (c) 2012 jessy giacomoni  and Adrien Rogier. All rights reserved.
 */

#include <stdio.h>
#include "acquisition.h"

/**
* \brief see Header
*/
void acquisition(unsigned int nbAcquisition,unsigned int delaiEntreLesSeries, unsigned int nbSerie, unsigned int delaiEntreDeuxAcquisition,int* ptr_mem_partagee,struct sembuf *sem_P,struct sembuf *sem_V, int mem_ID_Proc_Acquisition, int MUTEX_acquisition_stockage, int semaphore_Proc_Acquisition_Stockage_Mem_plein, int semaphore_Proc_Acquisition_Stockage_Mem_vide)
{
	
	/* Déclaration des variables */
	unsigned int incrementAcquisition = 0;
	int Valretour;
	unsigned int incrementalArray;

	/* Pour chaque série d'échantillonnage )*/
	while(incrementAcquisition < nbSerie)
	{

		/* On prends le sémaphore de la mémoire Vide */
        Valretour = semop(semaphore_Proc_Acquisition_Stockage_Mem_vide,sem_P,1);
		if (Valretour > 0) printf(" Attente Semaphore Acquisition <-> Stockage mémoire vide\n");
		printf(" ACQUISITION : semaphore Acquisition<->Stockage mémoire vide pris \n\n");
		if(Valretour < 0)
		{
			perror("Erreur prendre semaphore\n");
			exit(-3);
		}
		
		
		/* On prends le sémaphore entre les processus Acquisition et Stockage */
        Valretour = semop(MUTEX_acquisition_stockage,sem_P,1);
		if (Valretour > 0) printf(" Attente MUTEX Acquisition<->Stockage \n");
		printf(" ACQUISITION : MUTEX Acquisition<->Stockage pris ");
		if(Valretour < 0)
		{
			perror("Erreur prendre semaphore\n");
			exit(-3);
		}
		
		printf("\n ---------- Série d'échantillonage N° %d -----------\n",incrementAcquisition);
		
		
/* On attache le segment de mémoire partagée identifié par mem_ID au segment de données du processus 'Acquisition' dans une zone libre déterminée par le Système d'exploitation et on s'assure que le segment de mémoire a été correctement attaché à mon processus */
        ptr_mem_partagee = shmat(mem_ID_Proc_Acquisition, NULL, 0);
		if (ptr_mem_partagee == (void*) -1)
		{
			perror("shmat");
			exit(1);
		}

		/* Pour chaque échantillon*/
		for(incrementalArray =0; incrementalArray<nbAcquisition+1;incrementalArray++)
		{
			/* On stocke en mémoire la valeur aléatoire */
			ptr_mem_partagee[incrementalArray]= recupereHeure();
			/* Printf de debuggage */
			printf("Echantillon N° %d \n",incrementalArray);
			/* Puis on s'endort le temps imposé par la spécification */
			sleep(delaiEntreDeuxAcquisition);
		}



		/* On libère la mémoire partagée */
		shmdt(ptr_mem_partagee);
		printf("------ FIN DE LA SERIE D'ACQUISITION -------\n ");
		
		/* On donne la main au processus stockage.c */
		Valretour = semop(MUTEX_acquisition_stockage,sem_V,1);
		printf(" ACQUISITION : MUTEX Acquisition<->Stockage libre \n");
		if(Valretour < 0)
		{
			perror("Erreur rendre semaphore\n");
			exit(-4);
		}
		
		/* On libère le sémaphore de la mémoire pleine */
		Valretour = semop(semaphore_Proc_Acquisition_Stockage_Mem_plein,sem_V,1);
		printf(" ACQUISITION : semaphore Acquisition<->Stockage mémoire pleine libre \n");
		if(Valretour < 0)
		{
			perror("Erreur rendre semaphore\n");
			exit(-4);
		}
		incrementAcquisition++;
		sleep(delaiEntreLesSeries);
	}
	printf(" PID Processus acquisition = [%d] TERMINEE \n",getpid());
}



/**
 * \brief see Header
 */
int recupereHeure(void)
{
    /* Déclaration d'un pointeur sur une structure de type timezone */
    struct timezone * tz;
    struct timeval * tv;
    
    tz = (struct timezone *) malloc (sizeof(struct timezone));
    if(tz == NULL)
    {
        perror("Problème allocation mémoire");
        exit(1);
    }
    
    tz->tz_minuteswest = 0;
    /* tz->tz_dsttime = DST_WET; */
    
    /* Déclaration d'un pointeur sur une structure de type timezone */
    
    tv = (struct timeval *) malloc (sizeof(struct timeval));
    if(tv == NULL)
    {
        perror("Problème allocation mémoire");
        exit(1);
    }
    
    /* Initialisation des structures */
    gettimeofday(tv,tz);
    
    /* Affichage des résultats */
    /*printf("\n\n Seconde : [%d] \n\n µseconde : [%d] \n\n",(int)tv->tv_sec,(int)tv->tv_usec);*/
    return tv->tv_usec*1000000 + tv->tv_sec;
}
