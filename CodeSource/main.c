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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>



int main(int argc, char *argv[])
{
    
	/* Déclaration des variables */
	pid_t pid_acquisition=0, pid_stockage=0, pid_traitement=0;
	
	/* Initialisation du programme et création des processus acquisition, stockage et traitement */
	system("clear");
	printf("\n\n================================ TP 2 ET 3 : SEMAPHORE ET MEMOIRE PARTAGEE ================================\n\n");
	
	
	
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
			// acquisition();
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
			// stockage();
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
			// traitement();
			exit(1);
            break;
            
            /* Code du pere */
		default:
            break;
	}
	/*********************************** Fin du processus traitement ***********************************/
	
	
	
	
	
	/*********************************** Code du pere ***********************************/
	printf("\n\n\t\t\t Code du pere \n\n");
	
	/******************************** Fin de code du pere *******************************/
	
	
	
	return 0;
}

