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
 
    int sempahore_Proc_Acquisition_Stockage_Mem_plein;
    int sempahore_Proc_Acquisition_Stockage_Mem_vide;
 
void stockage(int nbrAcquisition, int delaiEntreSerie, unsigned int nbrSerie,int MUTEX,struct sembuf *sem_P,struct sembuf *sem_V, int mem_ID_Proc_Stockage,int* ptr_mem_partagee, int sempahore_Proc_Acquisition_Stockage_Mem_plein, int sempahore_Proc_Acquisition_Stockage_Mem_vide, int sempahore_Proc_Stockage_Traitement_Mem_plein, int sempahore_Proc_Stockage_Traitement_Mem_vide)
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
       
	/* On prends le sémaphore de la mémoire vide entre les processus stockage et traitement */
		Valretour = semop(sempahore_Proc_Stockage_Traitement_Mem_vide,sem_P,1);
		if(Valretour < 0)
		{
			perror("Erreur rendre semaphore\n");
			exit(-4);
		}
		
		 printf("ATTENTE SEMAPHORE\n");
        
		/* On prends le sémaphore de la mémoire pleine entre les processus acquisitions et stockages */
		Valretour = semop(sempahore_Proc_Acquisition_Stockage_Mem_plein,sem_P,1);
		if(Valretour < 0)
		{
			perror("Erreur rendre semaphore\n");
			exit(-4);
		}
		
        printf("ATTENTE SEMAPHORE 2 \n");
		Valretour = semop( MUTEX,sem_P,1);
		if(Valretour < 0)
		{
		    perror("Erreur prendre semaphore\n");
		    exit(-3);
		}
			
		
		
	
		printf("SEMAPHORE 2 ATTENTE PROCESSUS TRAITEMENT\n");

		
		
		
		

		/* On attache le segment de mémoire partagée identifié par mem_ID au segment de données du processus 'Acquisition' dans une zone libre déterminée par le Système d'exploitation
		 et je m'assure que le segment de mémoire a été correctement attaché à mon processus
		 */
		if ((ptr_mem_partagee = shmat(mem_ID_Proc_Stockage, NULL, 0)) == (void*) -1)
		{
		    perror("shmat");
		    exit(1);
		}

		printf("DEUXIEME SEMAPHORE\n");
		printf("STOCKAGE\n");

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
		Valretour = semop(MUTEX,sem_V,1);
		if(Valretour < 0)
		{
		    perror("Erreur rendre semaphore\n");
		    exit(-4);
		}

		printf("SEMAPHORE 2 REND\n\n");
		incrementAcquisition++;
		/* On libère le sémaphore de la mémoire vide entre le processus acquisition et stockage */
		Valretour = semop(sempahore_Proc_Acquisition_Stockage_Mem_vide,sem_V,1);
		if(Valretour < 0)
		{
			perror("Erreur rendre semaphore\n");
			exit(-4);
		}
		/* On libère le sémaphore de la mémoire pleine entre le processus stockage et traitement */
		Valretour = semop(sempahore_Proc_Stockage_Traitement_Mem_plein,sem_V,1);
		if(Valretour < 0)
		{
			perror("Erreur rendre semaphore\n");
			exit(-4);
		}
	}
}
