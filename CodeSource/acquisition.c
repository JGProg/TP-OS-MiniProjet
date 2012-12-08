#include <stdio.h>
#include "acquisition.h"



/**
 * \brief see Header
*/
void acquisition(unsigned int nbAcquisition,unsigned int delaiEntreLesSeries, unsigned int nbrAcquisitionSerie, unsigned int delaiEntreDeuxAcquisition,int* tabResultat)
{
    unsigned int i = 0;
    unsigned int j = 0;
    for(j=0;j<nbrAcquisitionSerie;j++)
    {
        for(i=0; i< nbAcquisition;i++)
        {
            tabResultat[j +i + nbAcquisition] = recupereHeure();
            printf("Valeur du tableau inséré %d\n",tabResultat[j + i + nbAcquisition]);
            sleep(delaiEntreDeuxAcquisition);
        }
        sleep(delaiEntreLesSeries);
    }
    printf("Helloe\n");
}


/**
 * \brief see Header
 */
int recupereHeure(void)
{
    struct timezone * tv;
    tv = (struct timezone *) malloc (sizeof(struct timezone));
    if(tv == NULL)
    {
        perror("Problème allocation mémoire");
        exit(1);
    }
    return tv->tz_minuteswest;
}


/**
 * \brief see Header
 */
void afficher(int* Matrice, size_t M, size_t N)
{
    size_t j, i;
    printf("Affichage de la matrice est : \n");
    for(j = 0; j < N; j++)
    {
        for(i = 0; i < M; i++)
        {
            printf("%3d\t",Matrice[M * j + i] );
        }
        printf("\n");
    }
}