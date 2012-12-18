/**
 * \file stockage.h
 * \brief header of stockage.c.
 * \author jessy giacomoni and Adrien Rogier
 * \version 0.1
 * \date 01/12/12
 * Copyright (c) 2012 jessy giacomoni  and Adrien Rogier. All rights reserved.
 */

#ifndef TP_OS_stockage_h
#define TP_OS_stockage_h
#include <string.h>
#include <stdio.h>

/**
 * \pre 'nbAcquisition' : Le nombre de séries d'acquisition, il ne peut pas être négatif, 'NumeroSeries' est le numéro de la série courante., 'tabDonne' c'est le tableau qui comporte les données
 * \post Le tableau est copié dans un fichier txt. sous la fome data_1_X.txt ou X est le numéro de la série.
 */
void stockage(int nbAcquisition, int NumeroSeries,int * tabDonnees);

#endif
