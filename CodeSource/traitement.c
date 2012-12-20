/**
 * \file traitement.c
 * \brief header of traitement.c
 * \author jessy giacomoni and Adrien Rogier
 * \version 0.1
 * \date 01/12/12
 * Copyright (c) 2012 jessy giacomoni  and Adrien Rogier. All rights reserved.
 */
#include "traitement.h"


/**
 * \brief see Header
 */
void traitement(unsigned int nbrSerie, int semaphore_Proc_Stockage_Traitement,struct sembuf *sem_P,struct sembuf *sem_V, unsigned int delaiEntreSerie, unsigned int delaiAcquisition, unsigned int nbrAcquisition)
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
        Valretour = semop(semaphore_Proc_Stockage_Traitement,sem_P,1);
        if(Valretour < 0)
        {
            perror("Erreur prendre semaphore\n");
            exit(-3);
        }
        
        printf("TROISIEME SEMAPHORE\n");
        
        sprintf(NomFichierData1,"Data_1_/data_1_%d.txt",incrementeSerie+1);
        sprintf(NomFichierData2,"Data_2_/data_2_%d.txt",incrementeSerie+1);
        
        
        Fichier_Data_1_ = fopen(NomFichierData1,"r");
        if(Fichier_Data_1_ == NULL)
        {
            printf("Probleme Ouverture du fichier\n");
            exit(3);
        }
        
        Fichier_Data_2_ = fopen(NomFichierData2,"w+");
        if(Fichier_Data_2_ == NULL)
        {
            printf("Probleme Ouverture du fichier\n");
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
        
        
        printf("TRAITEMENT\n");
        printf("FIN TRAITEMENT\n");
        Valretour = semop(semaphore_Proc_Stockage_Traitement,sem_V,1);
        if(Valretour < 0)
        {
            perror("Erreur rendre semaphore\n");
            exit(-4);
        }
        printf("SEMAPHORE 3 REND\n\n");
        incrementeSerie++;
        sleep(delaiEntreSerie+nbrAcquisition*delaiAcquisition+3);
    }

    DessinerGraphe(nbrSerie);
    
    /* printf("_________\n Fin du traitement \n\n"); */
}


void DessinerGraphe(int nbrSerie)
{
    int incrementalNumeroSerie;
    char FinFichier[1000] = "plot ";
    char NomFile[FILENAME_MAX] = "data_1_";
    FILE * f;
    
    for (incrementalNumeroSerie=1; incrementalNumeroSerie < nbrSerie; incrementalNumeroSerie++)
    {
        sprintf(NomFile,"\"data_2_/data_2_%d.txt\" with lines , ",incrementalNumeroSerie);
        strcat(FinFichier,NomFile);
    }
    sprintf(NomFile,"\"data_2_/data_2_%d.txt\" with lines\r\n ",nbrSerie);
    strcat(FinFichier,NomFile);


    f = popen("gnuplot","w");
    fprintf(f,"set xlabel \"Comparaison\r\n");
    fprintf(f,"set ylabel \"Numero Acquisition \r\n");
    fprintf(f,"set multiplot\r\n");
    fprintf(f,"%s",FinFichier);
    
    fflush(f);
    sleep(100);
    
    pclose(f);

}

/*
int main(void)
{
    DessinerGraphe(2);
    return 0;
}
 */
 
