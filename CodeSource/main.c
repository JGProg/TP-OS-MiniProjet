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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "acquisition.h"
#include <signal.h>
#include <sys/wait.h> 


/**
 * \todo Le père doit attendre que ses fils soit terminer avant qu'il se termine.
 */
int main(int argc, char *argv[])
{
    /* Déclaration des variables */
    
    unsigned int nbrSerie;
    unsigned int delaiEntreSerie;
    unsigned int nbrAcquisition;
    unsigned int delaiAcquisition;
    
	
	pid_t pid_acquisition=0, pid_stockage=0, pid_traitement=0;
    int * tabResultat;
    int status;
	/* Initialisation du programme et création des processus acquisition, stockage et traitement */
	
    system("clear");
    
	printf("\n\n================================ TP 2 ET 3 : SEMAPHORE ET MEMOIRE PARTAGEE ================================\n\n");

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
			printf("\n\n\t\t\t Partie acquisition \n\n");
            /* Il fera la taille du nombre d'acquisition. */

            tabResultat = (int *) malloc(nbrAcquisition * sizeof(int));
            acquisition(nbrSerie,delaiEntreSerie,nbrAcquisition, delaiAcquisition, tabResultat);
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
			printf("\n\n\t\t\t Partie stockage \n\n");
			/* stockage(); */
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
	
	

    wait (&status) ;
    if (WIFEXITED (status))
        printf ("fils termine normalement: status = %d\n", WEXITSTATUS (status)) ;
	/*********************************** Code du pere ***********************************/
	printf("\n\n\t\t\t Code du pere \n\n");
  	/******************************** Fin de code du pere *******************************/
	
	
	
	return 0;
}

