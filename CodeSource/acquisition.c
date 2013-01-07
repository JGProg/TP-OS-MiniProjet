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
void acquisition(unsigned int nbAcquisition,unsigned int delaiEntreLesSeries, unsigned int nbSerie, unsigned int delaiEntreDeuxAcquisition,int* ptr_mem_partagee, int semaphore_Proc_Acquisition_Stockage,struct sembuf *sem_P,struct sembuf *sem_V, int mem_ID_Proc_Acquisition, int semaphore_Proc_Acquisition_Traitement)
{
    unsigned int incrementAcquisition = 0;
    int Valretour;
    unsigned int incrementalArray;

    while(incrementAcquisition < nbSerie)
    {
        Valretour = semop(semaphore_Proc_Acquisition_Stockage,sem_P,1);
        if(Valretour < 0)
        {
            perror("Erreur prendre semaphore\n");
            exit(-3);
        }
        
        Valretour = semop(semaphore_Proc_Acquisition_Traitement,sem_P,1);
        if(Valretour < 0)
        {
            perror("Erreur prendre semaphore\n");
            exit(-3);
        }
        
        
        printf("PREMIER SEMAPHORE \n");
        printf("ACQUISITION\n");
        /* On attache le segment de mémoire partagée identifié par mem_ID au segment de données du processus 'Acquisition' dans une zone libre déterminée par le Système d'exploitation
         et on s'assure que le segment de mémoire a été correctement attaché à mon processus
         */
        if ((ptr_mem_partagee = shmat(mem_ID_Proc_Acquisition, NULL, 0)) == (void*) -1)
        {
            perror("shmat");
            exit(1);
        }
        
        for(incrementalArray =0; incrementalArray<nbAcquisition+1;incrementalArray++)
        {
            ptr_mem_partagee[incrementalArray]= recupereHeure();
            sleep(delaiEntreDeuxAcquisition);
        }
        
        
        /* On libère la mémoire partagée */
        shmdt(ptr_mem_partagee);
        printf("FIN ACQUISITION\n");
        Valretour = semop(semaphore_Proc_Acquisition_Traitement,sem_V,1);
        if(Valretour < 0)
        {
            perror("Erreur rendre semaphore\n");
            exit(-4);
        }
        
        Valretour = semop(semaphore_Proc_Acquisition_Stockage,sem_V,1);
        if(Valretour < 0)
        {
            perror("Erreur rendre semaphore\n");
            exit(-4);
        }
        printf("SEMAPHORE 1 REND\n\n");
        incrementAcquisition++;
        sleep(delaiEntreLesSeries);
    }
    /*printf("_________\n Fin de l'acquisition \n\n");*/
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
	tz->tz_dsttime = DST_WET;
    
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
