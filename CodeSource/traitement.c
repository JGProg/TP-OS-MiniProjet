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
void traitement(int NumeroSerie)
{
    char NomFichierData1[FILENAME_MAX] = "data_1_";
    char NomFichierData2[FILENAME_MAX] = "data_2_";
    char Line[255];
    char line_lues[512];
    int premiereValeur;
    int deuxiemeValeur;
    
    FILE * Fichier_Data_2_;
    FILE * Fichier_Data_1_;
    
    sprintf(NomFichierData1,"Data_1_/data_1_%d.txt",NumeroSerie);
    sprintf(NomFichierData2,"Data_2_/data_2_%d.txt",NumeroSerie);
    
    
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
    
    
    /* printf("_________\n Fin du traitement \n\n"); */
}

/*
int main(void)
{
 FILE * f;
 f = popen("gnuplot","w");
 fprintf(f,"plot \"toto.dat\" with lines \n");

    fflush(f);
 sleep(100);
 pclose(f);

    return 0;
}
 */
