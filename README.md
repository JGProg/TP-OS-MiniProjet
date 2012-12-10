TP-OS-MiniProjet
================
A SUPPRIMER
===============
    J'écris ici parce que je ne peux pas créer de nouveau fichier -_- enfin je n'y arrive pas. Je n'ai pas compris
    pourquoi tu as changé le nom des variables ? Dans le TP la prof a indiqué le nom des variables, je ne crois pas
    qu'elle appréciera. Ou j'ai peut-être mal compris.
    De plus je comprends vraiment pas ton code. Je te propose ce code ci-dessous qui pour moi est plus simple :
    
    
    #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/time.h>

void acquisition()
{        

        /* Déclaration d'un pointeur sur une structure de type timezone */
        struct timezone * tz;
        tz = (struct timezone *) malloc (sizeof(struct timezone));
        if(tz == NULL)
        {
                perror("Problème allocation mémoire");
                exit(1);
        }
        
        /* Déclaration d'un pointeur sur une structure de type timezone */
        struct timeval * tv;
        tv = (struct timeval *) malloc (sizeof(struct timeval));
        if(tv == NULL)
        {
                perror("Problème allocation mémoire");
                exit(1);
        }
        
        /* Initialisation des structures */
        gettimeofday(tv,tz);
        
        /* Affichage des résultats */
        printf("\n\n Seconde : [%d] \n\n µseconde : [%d] \n\n",tv->tv_sec,tv->tv_usec);
}

L'architecture logiciel va être composé de :

- Un programme main.c
- Un programme acquisition.c/h
- Un programme stockage.c/h
- Un programme traitement.c/h
- Et un programme bonus Courbe.c/h
- Un Makefile



Programme main.c
================

  DESCRIPTION :
    
    Le main.c va être le processus père. Son objectifs sera principalement d'initialiser et 
    de créer les 3 processus fils (4 avec le bonus).
    
  SPECIFICATION :
    
    - Il est le seul à pouvoir créer des processus.
    - Il ne peut pas mourir tant que tous ses processus fils ne sont pas mort
    - Il supprime l'intégralité des données qui ont été générées par lui ou un de ses fils juste avant de mourir.
    - Il gère les retours d'erreurs.
    - Ce programme doit contenir les variables :
        -> n : le nombre de séries d'acuqisition qui seront réalisés
        -> delai : le temps en seconde entre 2 série d'acquisition de données
        -> nbAcquisition : Le nombre d'acquisition faites pour une série
        -> delaiAcqusition : le temps en seconde entre 2 acquisitions d'une même série
        -> Mesure[] : Ce tableau est un segment de mémoire partagé alloué avec la fonction malloc  


Programme acquisition.c/h
=========================

  DESCRIPTION :
    
    L'acquisition des données va être simulée : les données seront l'heure actuelle du système.
    La fonction gettimeofday() permet de récupérer cette information par l'intermédiaire d'une structure
    de donnée timeval :
    
    #include <sys/types.h>
    #include <unistd.h>
    
    struct timeval {
      int tv_sec; // seconde
      int tv_usec; // Microsecondes
    };
    
    Les données générées sont décrites par cette formule :
    
              valeur = µsecode*10e6 + seconde
                                  
  SPECIFICATION :
  
    - Les résutats des relevés de l'heure seront stockés dans un tableau passé en paramètre par le main.c.
    - La fonction stockera les résultats des tests dans un tableau.
    - La fonction prend en paramètre :
        -> n : le nombre de séries d'acuqisition qui seront réalisés.
        -> delai : le temps en seconde entre 2 série d'acquisition de données.
        -> nbAcquisition : Le nombre d'acquisition faites pour une série.
        -> delaiAcqusition : le temps en seconde entre 2 acquisitions d'une même série.
        -> Mesure[] : Un tableau alloué dynamiquement qui stockera la valeur des résultats générés par la fonction.


Programme stockage.c/h
======================

  DESCRIPTION :

      Cette fonction prends les données passées en paramètre par la variable Mesure[] de type tableau. Elle 
      copie ensuite les valeurs du tableau dans un fichier et supprime les valeurs du tableau copié dans le fichier.
    
  SPECIFICATION :
  
    - La fonction acquistion.c devra générée un fichier data_1_X.txt avec X correspondant au nombre de série.
    - Dans le fichier data_1_X.txt on aura strictement une valeur par ligne.
    - La fonction supprimera les valeurs du tableau que celle-ci à copier dans le fichier data_1_X.txt


Programme traitement.c/h
========================

  DESCRIPTION :

      Cette fonction récupère les valeurs présente dans le fichier data_1_X.txt. Puis traite les données 
      du fichier et stocke le résultat dans un fichier data_2_X.txt. X étant le numéro de la série.
      
  SPECIFICATION :
  
    - La fonction acquistion.c devra générée un fichier data_1_X.txt avec X correspondant au nombre de série.
    - Dans le fichier data_1_X.txt on aura strictement une valeur par ligne.
    - La fonction supprimera les valeurs du tableau que celle-ci à copier dans le fichier data_1_X.txt
    - 





Programme courbe.c/h
====================

  DESCRIPTION :

      Fonction bonus. Cette fonction necessite l'installation  du logiciel Gnuplot. Ce logiciel 
      permet de tracer des courbes. Cette fonction s'integrera dans la fonction traitement.c. Elle dessinera les
      courbes en fonction des valeurs de data_2_X.txt.
      
  SPECIFICATION :
  

    A definir
    
Fin
===





