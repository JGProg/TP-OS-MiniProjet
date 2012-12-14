/**
 * \file stockage.c
 * \brief contents of stockage.c.
 * \author jessy giacomoni and Adrien Rogier
 * \version 0.1
 * \date 01/12/12
 * Copyright (c) 2012 jessy giacomoni  and Adrien Rogier. All rights reserved.
 */

#include "stockage.h"

void stockage(int nbAcquisition, int NumeroSeries ,int * tabDonnees)
{
    int incrementalArray;
    char NomFile[FILENAME_MAX] = "data_1_";
    char Line[255];
    FILE * fichier;
    sprintf(NomFile,"Data_1_/data_1_%d.txt",NumeroSeries);
    
    
    fichier = fopen(NomFile,"w");
    /*printf("Le numéro de série est : %d \n",NumeroSeries);*/
    for(incrementalArray =0; incrementalArray<nbAcquisition+1;incrementalArray++)
    {
       /* printf("La valeur du tableau à l'indice %d est : %d \n",incrementalArray, tabDonnees[incrementalArray]);*/
        sprintf(Line,"%d\n",tabDonnees[incrementalArray]);
        fwrite(Line,sizeof(char),strlen(Line),fichier);
    }
    /*printf("_________\n Fin du stockage \n\n");*/
    
    fclose(fichier);
}