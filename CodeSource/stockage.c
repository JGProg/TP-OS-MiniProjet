/**
 * \file stockage.c
 * \brief contents of stockage.c.
 * \author jessy giacomoni and Adrien Rogier
 * \version 0.1
 * \date 01/12/12
 * Copyright (c) 2012 jessy giacomoni  and Adrien Rogier. All rights reserved.
 */

#include <stdio.h>
#include "stockage.h"

void stockage(int nbAcquisition, int NumeroSeries ,int * tabDonnees)
{
    int incrementalArray;
    printf("Le numéro de série est : %d \n",NumeroSeries);
    for(incrementalArray =0; incrementalArray<nbAcquisition+1;incrementalArray++)
    {
        printf("La valeur du tableau à l'indice %d est : %d \n",incrementalArray, tabDonnees[incrementalArray]);
    }
    printf("_________\n Fin de la lecture \n\n");
}