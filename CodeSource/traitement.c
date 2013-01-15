/**
 * \file traitement.c
 * \brief header of traitement.c
 * \author jessy giacomoni and Adrien Rogier
 * \version 0.1
 * \date 01/12/12
 * Copyright (c) 2012 jessy giacomoni  and Adrien Rogier. All rights reserved.
 */
#include "traitement.h"
#include "courbe.h"

/**
 * \brief see Header
 */
void traitement(unsigned int nbrSerie,struct sembuf *sem_P,struct sembuf *sem_V, int semaphore_Proc_Stockage_Traitement_fichier_plein, int semaphore_Proc_Stockage_Traitement_fichier_vide, int MUTEX_stockage_traitement)
{
    char NomFichierData1[FILENAME_MAX] = "data_1_";
    char NomFichierData2[FILENAME_MAX] = "data_2_";
    char Line[255];
    char line_lues[512];
    int premiereValeur;
    int deuxiemeValeur;
    unsigned int incrementeSerie=0;
    int Valretour;
    
    FILE * Fichier_Data_2_;
    FILE * Fichier_Data_1_;
    
    while(incrementeSerie < nbrSerie)
    {
        /* On prends le sémaphore du fichier pleine entre les processus stockage et traitement */
        Valretour = semop(semaphore_Proc_Stockage_Traitement_fichier_plein,sem_P,1);
    	if (Valretour == 0)
            printf(" Attente SEMAPHORE Stockage<->Traitement fichier plein \n");
    	printf(" TRAITEMENT : SEMAPHORE Stockage<->Traitement fichier plein pris \n");
        if(Valretour < 0)
        {
            perror("Erreur prendre semaphore\n");
            exit(-3);
        }
        Valretour = semop(MUTEX_stockage_traitement,sem_P,1);
        if (Valretour == 0) printf(" Attente SEMAPHORE MUTEX Stockage<->Traitement \n");
        printf(" TRAITEMENT : MUTEX Stockage<->Traitement pris \n");
        if(Valretour < 0)
        {
            perror("Erreur prendre semaphore \n");
            exit(-3);
        }
        
        
        sprintf(NomFichierData1,"Data_1_/data_1_%d.txt",incrementeSerie+1);
        sprintf(NomFichierData2,"Data_2_/data_2_%d.txt",incrementeSerie+1);
        
        
        Fichier_Data_1_ = fopen(NomFichierData1,"r");
        if(Fichier_Data_1_ == NULL)
        {
            printf("Probleme Ouverture du fichier 1 traitement \n");
            exit(3);
        }
        
        Fichier_Data_2_ = fopen(NomFichierData2,"w+");
        if(Fichier_Data_2_ == NULL)
        {
            printf("Probleme Ouverture du fichier 2 traitement \n");
            exit(3);
        }
        
        while(fgets(line_lues,512,Fichier_Data_1_) != NULL)
        {
            premiereValeur = atoi(line_lues);
            /* printf("premiere valeur %d \n",premiereValeur); */
            if(fgets(line_lues,512,Fichier_Data_1_)!= NULL)
            {
                deuxiemeValeur = atoi(line_lues);
                /* printf("Deuxieme valeur %d \n",deuxiemeValeur); */
                sprintf(Line,"%d\n",deuxiemeValeur-premiereValeur);
                fwrite(Line,sizeof(char),strlen(Line),Fichier_Data_2_);
            }
            else
            {
                sprintf(Line,"%d\n",premiereValeur);
                fwrite(Line,sizeof(char),strlen(Line),Fichier_Data_2_);
            }
            
        }
        
        fclose(Fichier_Data_1_);
        fclose(Fichier_Data_2_);
        
        printf("FIN TRAITEMENT\n");
        
        
        Valretour = semop(MUTEX_stockage_traitement,sem_V,1);
        printf(" TRAITEMENT : MUTEX Stockage<->Traitement libre \n");
        if(Valretour < 0)
        {
            perror("Erreur rendre semaphore\n");
            exit(-4);
        }
        incrementeSerie++;
        /* On libère le sémaphore de la mémoire vide entre les processus stockage et traitement */
        Valretour = semop(semaphore_Proc_Stockage_Traitement_fichier_vide,sem_V,1);
        printf(" TRAITEMENT : SEMAPHORE Stockage<->Traitement fichier vide libre \n");
        if(Valretour < 0)
		{
			perror("Erreur rendre semaphore \n");
			exit(-4);
		}
    }
    DessinerGraphe(nbrSerie);
    printf(" PID Processus traitement = [%d] TERMINEE \n",getpid());    
    /* printf("_________\n Fin du traitement \n\n"); */
}


/*
 int main(void)
 {
 DessinerGraphe(2);
 return 0;
 }
 */

