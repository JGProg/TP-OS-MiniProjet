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
void stockage(int nbrAcquisition, int delaiEntreSerie, unsigned int nbrSerie,int semaphore_Proc_Acquisition_Stockage,int semaphore_Proc_Stockage_Traitement, struct sembuf *sem_P,struct sembuf *sem_V, int mem_ID_Proc_Stockage,int* ptr_mem_partagee)
{
     unsigned int incrementAcquisition = 0;
    int Valretour;
    int incrementalArray;
    char NomFile[FILENAME_MAX] = "data_1_";
    char Line[255];
    FILE * fichier;
       

    
    while(incrementAcquisition < nbrSerie)
    {
        
        Valretour = semop(semaphore_Proc_Stockage_Traitement,sem_P,1);
        if(Valretour < 0)
        {
            perror("Erreur prendre semaphore\n");
            exit(-3);
        }
        printf("SEMAPHORE 2 ATTENTE PROCESSUS TRAITEMENT\n");
        Valretour = semop(semaphore_Proc_Acquisition_Stockage,sem_P,1);
        if(Valretour < 0)
        {
            perror("Erreur prendre semaphore\n");
            exit(-3);
        }
        
        /* On attache le segment de mémoire partagée identifié par mem_ID au segment de données du processus 'Acquisition' dans une zone libre déterminée par le Système d'exploitation
         et je m'assure que le segment de mémoire a été correctement attaché à mon processus
         */
        if ((ptr_mem_partagee = shmat(mem_ID_Proc_Stockage, NULL, 0)) == (void*) -1)	{
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
        Valretour = semop(semaphore_Proc_Acquisition_Stockage,sem_V,1);
        if(Valretour < 0)
        {
            perror("Erreur rendre semaphore\n");
            exit(-4);
        }
        Valretour = semop(semaphore_Proc_Stockage_Traitement,sem_V,1);
        if(Valretour < 0)
        {
            perror("Erreur rendre semaphore\n");
            exit(-4);
        }
        
        printf("SEMAPHORE 2 REND\n\n");
        incrementAcquisition++;
        sleep(delaiEntreSerie+1);
    }
    
    
    
     
    
}