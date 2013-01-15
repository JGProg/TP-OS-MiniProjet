/**
 * \file stockage.c
 * \brief contents of stockage.c.
 * \author jessy giacomoni and Adrien Rogier
 * \version 0.1
 * \date 01/12/12
 * Copyright (c) 2012 jessy giacomoni  and Adrien Rogier. All rights reserved.
 */

#include "stockage.h"

/**
 * \brief see Header
 */
 
    int semaphore_Proc_Acquisition_Stockage_Mem_plein;
    int semaphore_Proc_Acquisition_Stockage_Mem_vide;
 
void stockage(int nbrAcquisition, unsigned int nbrSerie, struct sembuf *sem_P,struct sembuf *sem_V, int mem_ID_Proc_Stockage,int* ptr_mem_partagee, int MUTEX_acquisition_stockage, int MUTEX_stockage_traitement, int semaphore_Proc_Acquisition_Stockage_Mem_plein, int semaphore_Proc_Acquisition_Stockage_Mem_vide, int semaphore_Proc_Stockage_Traitement_fichier_plein, int semaphore_Proc_Stockage_Traitement_fichier_vide)
{

	/* Déclaration des variables */
	unsigned int incrementAcquisition = 0;
	int Valretour;
	int incrementalArray;
	char NomFile[FILENAME_MAX] = "data_1_";
	char Line[255];
	FILE * fichier;

	/* Pour chaque série d'échantillonage */
	while(incrementAcquisition < nbrSerie)
	{

	        /* On prends le sémaphore du fichier libre entre les processus stockages et traitements */
        Valretour = semop(semaphore_Proc_Stockage_Traitement_fichier_vide,sem_P,1);
		if (Valretour > 0) printf(" Attente SEMAPHORE fichier libre Stockage<->Traitement \n");
		printf(" STOCKAGE : Semaphore fichier vide Stockage<->Traitement pris \n");
		if(Valretour < 0)
		{
			perror("Erreur rendre semaphore\n");
			exit(-4);
		}
		/* On prends le sémaphore de la mémoire pleine entre les processus acquisitions et stockages */
        Valretour = semop(semaphore_Proc_Acquisition_Stockage_Mem_plein,sem_P,1) ;
		if (Valretour> 0)
        {
            printf(" Attente SEMAPHORE acquisition<->stockage mémoire pleine \n");
            exit(-4);
        }
        printf(" STOCKAGE : Semaphore  Stockage<->Acquisition mémoire pleine pris \n");
		
        if(Valretour < 0)
		{
			perror("Erreur rendre semaphore\n");
			exit(-4);
		}
		
		/* On prends le MUTEX entre acquisition et stockage */
        Valretour = semop(MUTEX_acquisition_stockage,sem_P,1);
		if (Valretour > 0)
        {
            printf(" Attente MUTEX Acquisition<->Stockage \n");
            exit(-4);
        }
		printf(" STOCKAGE : MUTEX Acquisition<->Stockage pris \n");
		if(Valretour < 0)
		{
		    perror("Erreur prendre semaphore\n");
		    exit(-3);
		}
		
		/* On prends le MUTEX entre stockage et traitement */
        Valretour = semop(MUTEX_stockage_traitement,sem_P,1);
		if ( Valretour == 1)
        {
            printf(" Attente MUTEX Stockage<->Traitement \n");
            exit(-3);

        }
        printf(" STOCKAGE : MUTEX Stockage<->Traitement pris \n");
		if(Valretour < 0)
		{
		    perror("Erreur prendre semaphore\n");
		    exit(-3);
		}

		/* On attache le segment de mémoire partagée identifié par mem_ID au segment de données du processus 'Acquisition' dans une zone libre déterminée par le Système d'exploitation
		 et je m'assure que le segment de mémoire a été correctement attaché à mon processus
		 */
        ptr_mem_partagee = shmat(mem_ID_Proc_Stockage, NULL, 0);
		if (ptr_mem_partagee == (void*) -1)
		{
		    perror("shmat");
		    exit(1);
		}
		sprintf(NomFile,"Data_1_/data_1_%d.txt",incrementAcquisition+1);
		fichier = fopen(NomFile,"w+");
		/*printf("Le numéro de série est : %d \n",NumeroSeries);*/
		for(incrementalArray =0; incrementalArray<nbrAcquisition+1;incrementalArray++)
		{
		    /* printf("La valeur du tableau à l'indice %d est : %d \n",incrementalArray, tabDonnees[incrementalArray]);*/
		    sprintf(Line,"%d\n",ptr_mem_partagee[incrementalArray]);
		    fwrite(Line,sizeof(char),strlen(Line),fichier);
		}
		/*printf("_________\n Fin du stockage \n\n");*/
		fclose(fichier);
		/* On libère la mémoire partagée */
		shmdt(ptr_mem_partagee);

		printf("FIN STOCKAGE\n");
		
		Valretour = semop(MUTEX_stockage_traitement,sem_V,1);
		printf(" STOCKAGE : MUTEX Stockage<->traitement libre \n");
		if(Valretour < 0)
		{
		    perror("Erreur rendre semaphore\n");
		    exit(-4);
		}
		
		Valretour = semop(MUTEX_acquisition_stockage,sem_V,1);
		printf(" STOCKAGE : MUTEX Acquisition<->Stockage libre \n");
		if(Valretour < 0)
		{
		    perror("Erreur rendre semaphore\n");
		    exit(-4);
		}
		incrementAcquisition++;
		/* On libère le sémaphore de la mémoire vide entre le processus acquisition et stockage */
		Valretour = semop(semaphore_Proc_Acquisition_Stockage_Mem_vide,sem_V,1);
		printf(" STOCKAGE : Semaphore Stockage<->Acquisition mémoire vide libre\n");
		if(Valretour < 0)
		{
			perror("Erreur rendre semaphore\n");
			exit(-4);
		}
		/* On libère le sémaphore du fichier entre le processus stockage et traitement */
		printf(" STOCKAGE : Semaphore fichier plein Stockage<->Traitement libre\n");
		Valretour = semop(semaphore_Proc_Stockage_Traitement_fichier_plein,sem_V,1);
		if(Valretour < 0)
		{
			perror("Erreur rendre semaphore\n");
			exit(-4);
		}
	}
	printf(" PID Processus stockage = [%d] TERMINEE \n",getpid());
}
