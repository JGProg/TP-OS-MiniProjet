//
//  File.c
//  TP-OS
//
//  Created by jessy giacomoni  and Adrien Rogier on 01/12/12.
//  Copyright (c) 2012 jessy giacomoni. All rights reserved.
//

#include <stdio.h>


void traitement(int nbAcquisition, int NumeroSeries)
{
    int incrementalArray;
    char NomFile[FILENAME_MAX] = "data_1_";
    char Line[255];
    FILE * fichier;
    sprintf(NomFile,"Data_2_/data_2_%d.txt",NumeroSeries);
    
    
    fichier = fopen(NomFile,"w");

    printf("_________\n Fin du traitement \n\n");
    
    fclose(fichier);
}