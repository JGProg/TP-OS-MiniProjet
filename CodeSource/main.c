/***********************************************************************************************|
 |	Entreprise 	|	ESISAR                                                                      |
 |	Matière 	|	CS336                                                                       |
 |	Sujet		|	TP2 ET 3                                                                    |
 |	Auteur 		|	GIACOMONI Jessy et ROGIER Adrien                                            |
 |	Statut 		|	3 Apprenti                                                                  |
 |	Date		|	29/11/2012                                                                  |
 |	Version		|	1.0                                                                         |
 |----------------------------------------------------------------------------------------------|
 |  DESCRIPTION :                                                                               |
 |	Ceci est notre brique général. Elle est testé et opérationnel.                              |
 | 	Il faut complété le code manquant dans chacun des switchs, ainsi                            |
 | 	qu'à la fin pour le code du père.                                                           |
 |                                                                                              |
 ************************************************************************************************/

/**
 * \file main.c
 * \brief Content of main.c
 * \author jessy giacomoni and Adrien Rogier
 * \version 0.1
 * \date 01/12/12
 * Copyright (c) 2012 jessy giacomoni  and Adrien Rogier. All rights reserved.
 */

/************ INCLUDE ET DEFINE ***************/

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
#include "stockage.h"
#include "traitement.h"

/* Pour la mémoire partagée */
#define CLEF 42

/********* FIN INCLUDE ET DEFINE *****************/


int main(int argc, char *argv[])
{
    
    
/***************************** DECLARATIONS DES VARIABLES + INITIALISATION *****************************/
    
    int status;
    
    /* Variable passer en paramètres du programme */
    unsigned int nbrSerie;
    unsigned int delaiEntreSerie;
    unsigned int nbrAcquisition;
    unsigned int delaiAcquisition;
    
    /* Déclaration d'un pointeur qui pointera sur une zone mémoire */
    int * tabResultat;
    
    /* Pointeur sur l'adresse d'attachement du segment de mémoire partagée */
    int* ptr_mem_partagee;
    
    /* Des que les deux premiers processus on terminé on termine la mémoire partagée */
    int rtrn;
    
    /* Identificateur du segment de mémoire partagée associé à CLEF */
    int mem_ID_Proc_Acquisition;
    int mem_ID_Proc_Stockage;
    
    /* --------------- DECLARATIONS + INITIALISATIONS DES SEMAPHORES ------------*/
    
    int MUTEX;
    int sempahore_Proc_Acquisition_Stockage_Mem_plein;
    int sempahore_Proc_Acquisition_Stockage_Mem_vide;
    int semaphore_Proc_Stockage_Traitement;
    int sempahore_Proc_Stockage_Traitement_Mem_plein;
    int sempahore_Proc_Stockage_Traitement_Mem_vide;
    
    /* Semaphore à enlever */
    int semaphore_Proc_Acquisition_Traitement;
    
    struct sembuf *sem_P = (struct sembuf *) malloc(2*sizeof(struct sembuf));
    struct sembuf *sem_V = (struct sembuf *) malloc(2*sizeof(struct sembuf));
    
    union semun {
        int val;
        struct semid_ds *buf;
        ushort * array;
    } argument;
    
    key_t key;
    
    /* ----------------------------- FIN SEMAPHORE ------------------------------*/
    
    
    /* creation des PID pour les forks */
    pid_t pid_acquisition;
    pid_t pid_stockage;
    pid_t pid_traitement;
    
    /********************************** FIN DECLARATION **************************************/
    
    
    
    /********************************** INITIALISATIONS **************************************/
    
    /* Initialisation de la taille du nombre d'acquisition */
    tabResultat = (int *) malloc(nbrAcquisition * sizeof(int));
    
    /* Initialisations es pid pour les forks */
    pid_acquisition=0;
    pid_stockage=0;
    pid_traitement=0;
    
    
    /*-------------------------- MEMOIRE PARTAGEE -----------------------------*/
    
    /* Nouveau segment mémoire de taille "nbAquisition" octets, avec des droits d'écriture et de lecture
     et on s'assure que l'espace mémoire a été correctement créé
     */
    mem_ID_Proc_Acquisition = shmget(CLEF, sizeof(tabResultat), 0666 | IPC_CREAT);
    if (mem_ID_Proc_Acquisition < 0)
    {
        perror("shmget");
        exit(-5);
    }
    
    /* On cherche le segment mémoire associé à CLEF et je récupère l'identificateur de ce segment mémoire. On attribue des droits de lecture uniquement
     et on s''assure que l'espace mémoire a été correctement créé */
    mem_ID_Proc_Stockage = shmget(CLEF, sizeof(tabResultat), 0444);
    if (mem_ID_Proc_Stockage < 0)
    {
        perror("shmget");
        exit(1);
    }
    
    /* Creation du sémpahore */
    
    /*
     struct sembuf {
     ushort_t        sem_num;         nombre semaphore
     short           sem_op;          Opération sur le semaphore
     short           sem_flg;         Drapeau opération
     };
     */
    sem_P[0].sem_num =  0; /* Numéro de notre sémaphore: le premier et le seul */
    sem_P[0].sem_op  = -1; /* Pour un P() on décrémente */
    sem_P[0].sem_flg =  0;
    
    sem_V[0].sem_num = 0;
    sem_V[0].sem_op  = 1;
    sem_V[0].sem_flg = 0;
    
    
    key = 666;
    key = 42;
    
    /* Creation du sémaphore entre le processus Acquisition et Stockage */
    MUTEX = semget(345 ,1,IPC_CREAT | 0666);
    if(MUTEX < 0)
    {
        perror("Probleme de creation de sémaphore\n");
        exit(-2);
    }
    
    /* Creation du sémaphore entre le processus Stockage et Traitement */
    semaphore_Proc_Stockage_Traitement = semget(42 ,1,IPC_CREAT | 0666);
    if(semaphore_Proc_Stockage_Traitement < 0)
    {
        perror("Probleme de creation de sémaphore\n");
        exit(-2);
    }
    
    /* Creation du sémaphore entre le processus Stockage et Traitement */
    semaphore_Proc_Acquisition_Traitement = semget(43 ,1,IPC_CREAT | 0666);
    if(semaphore_Proc_Acquisition_Traitement < 0)
    {
        perror("Probleme de creation de sémaphore\n");
        exit(-2);
    }
    
    /* Creation du sémaphore entre le processus Acquisition et Stockage pour mémoire pleine */
    sempahore_Proc_Acquisition_Stockage_Mem_plein = semget(44 ,1,IPC_CREAT | 0666);
    if(sempahore_Proc_Acquisition_Stockage_Mem_plein < 0)
    {
        perror("Probleme de creation de sémaphore\n");
        exit(-2);
    }
    
    /* Creation du sémaphore entre le processus Acquisition et Stockage pour mémoire vide */
    sempahore_Proc_Acquisition_Stockage_Mem_vide = semget(45 ,1,IPC_CREAT | 0666);
    if(sempahore_Proc_Acquisition_Stockage_Mem_vide < 0)
    {
        perror("Probleme de creation de sémaphore\n");
        exit(-2);
    }
    
    /* Creation du sémaphore entre le processus Acquisition et Stockage pour mémoire vide */
    sempahore_Proc_Stockage_Traitement_Mem_plein = semget(46 ,1,IPC_CREAT | 0666);
    if(sempahore_Proc_Stockage_Traitement_Mem_plein < 0)
    {
        perror("Probleme de creation de sémaphore\n");
        exit(-2);
    }
    
    /* Creation du sémaphore entre le processus Acquisition et Stockage pour mémoire vide */
    sempahore_Proc_Stockage_Traitement_Mem_vide = semget(47 ,1,IPC_CREAT | 0666);
    if(sempahore_Proc_Stockage_Traitement_Mem_vide < 0)
    {
        perror("Probleme de creation de sémaphore\n");
        exit(-2);
    }
    
    /* Changement d'état des sémaphores */
    argument.val = 1;
    semctl(MUTEX,0,SETVAL,argument);
    argument.val = 2;
    semctl(semaphore_Proc_Stockage_Traitement,0,SETVAL,argument);
    argument.val = 3;
    semctl(semaphore_Proc_Acquisition_Traitement,0,SETVAL,argument);
    argument.val = 4;
    semctl(sempahore_Proc_Acquisition_Stockage_Mem_plein,0,SETVAL,argument);
    argument.val = 5;
    semctl(sempahore_Proc_Acquisition_Stockage_Mem_vide,0,SETVAL,argument);
    argument.val = 6;
    semctl(sempahore_Proc_Stockage_Traitement_Mem_plein,0,SETVAL,argument);
    argument.val = 7;
    semctl(sempahore_Proc_Stockage_Traitement_Mem_vide,0,SETVAL,argument);
        
    /****************************************** FIN INITIALISATION *********************************************/
	
    system("clear");
    printf("\n\n================================ TP 2 ET 3 : SEMAPHORE ET MEMOIRE PARTAGEE ================================\n\n");
    
    /* On test les paramètres */
    if(argc != 5)
    {
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
                acquisition(nbrAcquisition,delaiEntreSerie, nbrSerie, delaiAcquisition,ptr_mem_partagee, MUTEX,sem_P,sem_V,mem_ID_Proc_Acquisition,sempahore_Proc_Acquisition_Stockage_Mem_plein,sempahore_Proc_Acquisition_Stockage_Mem_vide);
                                
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
            stockage( nbrAcquisition, delaiEntreSerie, nbrSerie, MUTEX, sem_P,sem_V,  mem_ID_Proc_Stockage, ptr_mem_partagee, sempahore_Proc_Acquisition_Stockage_Mem_plein, sempahore_Proc_Acquisition_Stockage_Mem_vide,sempahore_Proc_Stockage_Traitement_Mem_plein,sempahore_Proc_Stockage_Traitement_Mem_vide);
            
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
            
            /* Code de la fonction stockage.c */
		case 0:
           traitement(nbrSerie, MUTEX,sem_P,sem_V, delaiEntreSerie,delaiAcquisition,nbrAcquisition,semaphore_Proc_Acquisition_Traitement,sempahore_Proc_Stockage_Traitement_Mem_vide,sempahore_Proc_Stockage_Traitement_Mem_plein);
            exit(1);
            break;
            

		default:
            break;
	}
	
	/*********************************** Fin du processus traitement ***********************************/
	
	
	/********************************* Code du pere ***********************************/
	
    while (  wait(&status) > 0 )
    {
        printf("Un fils est mort, paix à son âme\n");
    }
    semctl(semaphore_Proc_Stockage_Traitement, 0, IPC_RMID, 0);
    semctl(MUTEX, 0, IPC_RMID, 0);
    
    rtrn = shmctl(mem_ID_Proc_Acquisition, IPC_RMID, 0);
	if(rtrn == -1)
    {
        perror("Ici Error\n");exit(-6);
    }
	printf("\n\n\t\t\t Fin du code du pere \n\n");
	
  	/******************************** Fin de code du pere *******************************/
    return 0;
}
