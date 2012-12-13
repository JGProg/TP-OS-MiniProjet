/***********************************************************************************************|
 |	Entreprise 	|	ESISAR								|
 |	Matière 	|	CS336								|
 |	Sujet		|	TP2 ET 3							|
 |	Auteur 		|	GIACOMONI Jessy et ROGIER Adrien				|
 |	Statut 		|	3 Apprenti							|
 |	Date		|	29/11/2012							|
 |	Version		|	1.0								|
 |---------------------------------------------------------------------------------------------- |
 |  DESCRIPTION :										|
 |	Ceci est notre brique général. Elle est testé et opérationnel.				|
 | 	Il faut complété le code manquant dans chacun des switchs, ainsi			|
 | 	qu'à la fin pour le code du père.							|
 |												|
 ************************************************************************************************/

/**
 * \file acquisition.h
 * \brief Header of acquisition.c.
 * \author jessy giacomoni and Adrien Rogier
 * \version 0.1
 * \date 01/12/12
 * Copyright (c) 2012 jessy giacomoni  and Adrien Rogier. All rights reserved.
 */

/* include global */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/sem.h>

/* include source code */
#include "acquisition.h"

/* Pour la mémoire partagée */
#define CLEF 42


/**
 * \todo Le père doit attendre que ses fils soit terminer avant qu'il se termine.
 */
int main(int argc, char *argv[])
{
    
    /* DECLARATIONS  */
    unsigned int NombreDeFilsTermine;
    unsigned int NbrFils = 3;
    
    /* Ceux passer en paramètres */
    unsigned int nbrSerie;
    unsigned int delaiEntreSerie;
    unsigned int nbrAcquisition;
    unsigned int delaiAcquisition;
    
    /* Il fera la taille du nombre d'acquisition. */
    int * tabResultat;
    
    /* Pour le nombre de fois que l'on doit appeler la fonction acquisition */
    unsigned int incrementAcquisition;
    
    /* Semaphore */
    int semaphore;
    struct sembuf *sem_P = (struct sembuf *) malloc(2*sizeof(struct sembuf));
    struct sembuf *sem_V = (struct sembuf *) malloc(2*sizeof(struct sembuf));
    int Valretour;
    union semun {
        int val;
        struct semid_ds *buf;
        ushort * array;
    } argument;
    key_t key;
    
    
    /* Creation des pid pour les forks */
    pid_t pid_acquisition;
    pid_t pid_stockage;
    pid_t pid_traitement;
    
    /* identificateur du segment de mémoire partagée associé à CLEF */
    int mem_ID;
    /* pointeur sur l'adresse d'attachement du segment de mémoire partagée */
	int* ptr_mem_partagee;
    
    
    /* INITIALISATIONS */
    
    /* Initialisation de la taille du nombre d'acquisition */
    tabResultat = (int *) malloc(nbrAcquisition * sizeof(int));
    /* Test memoire partagée */
    tabResultat[0] =5;
    
    /* Pour le nombre de fois que l'on doit appeler la fonction acquisition */
    incrementAcquisition = 0;
    
    /* Des pid pour les forks */
    pid_acquisition=0;
    pid_stockage=0;
    pid_traitement=0;
    
    /* Le clé pour la mémoire partagée */
    key = 1234;
    
    /* CREATION du sémpahore */
    
    /*
     struct sembuf {
     ushort_t        sem_num;        semaphore number
     short           sem_op;          semaphore operation
     short           sem_flg;         operation flags
     };
     */
    sem_P[0].sem_num =  0;
    sem_P[0].sem_op  = -1;
    sem_P[0].sem_flg =  0;
    
    sem_V[0].sem_num = 0;
    sem_V[0].sem_op  = 1;
    sem_V[0].sem_flg = 0;
    
    /* Creation du sémaphore */
    semaphore = semget(key ,1,IPC_CREAT | 0666);
    if(semaphore < 0)
    {
        perror("Probleme de creation de sémaphore\n");
        exit(-2);
    }
    
    /* Changement d'état du sémaphore */
    argument.val = 1;
    semctl(semaphore,0,SETVAL,argument);
    
	
    system("clear");
	printf("\n\n================================ TP 2 ET 3 : SEMAPHORE ET MEMOIRE PARTAGEE ================================\n\n");
    
    /* On test les paramètres */
    if(argc != 5)
    {
        printf("%d",argc);
        printf("Le nombre de paramètre n'est pas correct\n");
        printf("\t-> n : le nombre de séries d'acuqisition qui seront réalisés\n");
        printf("\t-> delai : le temps en seconde entre 2 série d'acquisition de données\n");
        printf("\t-> nbAcquisition : Le nombre d'acquisition faites pour une série\n");
        printf("\t-> delaiAcqusition : le temps en seconde entre 2 acquisitions d'une même série\n");
        exit(-1);
        
    }
    else
    {
        nbrSerie        =  atoi(argv[1]);
        delaiEntreSerie =  atoi(argv[2]);
        nbrAcquisition  =  atoi(argv[3]);
        delaiAcquisition=  atoi(argv[4]);
    }
    

	
	/*********************************** Création du processus acquisition ***********************************/
    
	pid_acquisition = fork();
	switch(pid_acquisition)
	{
            
            /* Gestion des erreurs du fork */
		case -1:
			perror(" Erreur fork \n");
			return EXIT_FAILURE;
            break;
            
            /* Code du la fonction acquisition.c */
		case 0:
			while(incrementAcquisition < nbrSerie)
            {
                
                Valretour = semop(semaphore,sem_P,1);
                if(Valretour < 0)
                {
                    perror("Erreur prendre semaphore\n");
                    exit(-3);
                }
                /* Nouveau segment mémoire de taille "nbAquisition" octets, avec des droits d'écriture et de lecture 
                   et on s'assure que l'espace mémoire a été correctement créé
                 */
                if ((mem_ID = shmget(CLEF, sizeof(tabResultat), 0666 | IPC_CREAT)) < 0)
                {
                    perror("shmget");
                    exit(-5);
                }
                /* On attache le segment de mémoire partagée identifié par mem_ID au segment de données du processus 'Acquisition' dans une zone libre déterminée par le Système d'exploitation
                 et on s'assure que le segment de mémoire a été correctement attaché à mon processus
                 */
                if ((ptr_mem_partagee = shmat(mem_ID, NULL, 0)) == (void*) -1)
                {
                    perror("shmat");
                    exit(1);
                }
                acquisition(nbrSerie,delaiEntreSerie,nbrAcquisition,delaiAcquisition,ptr_mem_partagee);
                
                /* On libère la mémoire partagée */
                shmdt(ptr_mem_partagee);
                
                Valretour = semop(semaphore,sem_V,1);
                if(Valretour < 0)
                {
                    perror("Erreur rendre semaphore\n");
                    exit(-4);
                }
                incrementAcquisition++;
            }
			exit(1);
            break;
            
            /* Code du pere */
		default:
            break;
            
	}
	/*********************************** Fin code processus acquisition ***********************************/
    
	
	
	
	/*********************************** Création du processus stockage ***********************************/
 	pid_stockage = fork();
 	
	switch(pid_stockage)
	{
            /* Gestion des erreurs du fork */
		case -1:
			perror(" Erreur fork \n");
			return EXIT_FAILURE;
            break;
            
            /* Code de la fonction stockage */
		case 0:
            Valretour = semop(semaphore,sem_P,1);
            if(Valretour < 0)
            {
                perror("Erreur prendre semaphore\n");
                exit(-3);
            }
            printf("Deuxième sémaphore\n");
            
            /* On cherche lesegment mémoire associé à CLEF et je récupère l'identificateur de ce segment mémoire. On attribue des droits de lecture uniquement
             et on s''assure que l'espace mémoire a été correctement créé
             */
            if ((mem_ID = shmget(CLEF, sizeof(tabResultat), 0444)) < 0)
            {
                perror("shmget");
                exit(1);
            }
            
            /* On attache le segment de mémoire partagée identifié par mem_ID au segment de données du processus 'Acquisition' dans une zone libre déterminée par le Système d'exploitation
             et je m'assure que le segment de mémoire a été correctement attaché à mon processus
             */
            if ((ptr_mem_partagee = shmat(mem_ID, NULL, 0)) == (void*) -1)	{
                perror("shmat");
                exit(1);
            }
            tabResultat[0] = (ptr_mem_partagee)[0];
            printf("%d\n",tabResultat[0]);
            /* On libère la mémoire partagée */
            shmdt(ptr_mem_partagee);
            
            Valretour = semop(semaphore,sem_V,1);
            if(Valretour < 0)
            {
                perror("Erreur rendre semaphore\n");
                exit(-4);
            }
            exit(1);
            break;
            
            /* Code du pere */
        default:
            break;
            
    }
    /*********************************** Fin du processus stockage ***********************************/
    
    
    
	/*********************************** Création du processus traitement ***********************************/
 	pid_traitement = fork();
 	
	switch(pid_traitement)
	{
            /* Gestion des erreurs du fork */
		case -1:
			perror(" Erreur fork \n");
			return EXIT_FAILURE;
            break;
            
            /* Code du la fonction stockage.c */
		case 0:
			printf("\n\n\t\t\t Partie traitement \n\n");
			/* traitement();*/
			exit(1);
            break;
            
            /* Code du pere */
		default:
            break;
	}
	/*********************************** Fin du processus traitement ***********************************/
	
    
    for(NombreDeFilsTermine=0;NombreDeFilsTermine<NbrFils;wait(NULL),NombreDeFilsTermine++);
	/*********************************** Code du pere ***********************************/
	printf("\n\n\t\t\t Fin du code du pere \n\n");
  	/******************************** Fin de code du pere *******************************/
	
	
	
	return 0;
}

