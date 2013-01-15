#include "courbe.h"


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
    fprintf(f,"set xlabel \"Comparaison\" \r\n");
    fprintf(f,"set ylabel \"Numero Acquisition\" \r\n");
    fprintf(f,"set multiplot\r\n");
    fprintf(f,"%s",FinFichier);
    
    fflush(f);
    sleep(10);
    
    pclose(f);
    
}
