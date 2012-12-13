TP-OS-MiniProjet
================

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

      Cette fonction prends les données passées en paramètre par la variable Mesure[ ] de type tableau. Elle 
      copie ensuite les valeurs du tableau dans un fichier et supprime les valeurs du tableau copié dans le fichier.
    
  SPECIFICATION :
  
    - La fonction acquistion.c devra générée un fichier data_1_X.txt avec X correspondant au nombre de série.
    - Dans le fichier data_1_X.txt on aura strictement une valeur par ligne.
    - La fonction supprimera les valeurs du tableau que celle-ci à copier dans le fichier data_1_X.txt

Transition Acqusition/Stockage
==============================

Nous allons tout d'abord récupérer une série d'acquisition. Lorsque celui-ci est fait alors nous allons stocker. 
Durant, le délai entre les séries nous allons donc en profiter pour donner la main au stockage. Lors de la terminaison de celui-ci nous allons redonner la main à l'acquisition pour la prochaine série.

- Remarque: Cependant, un problème peut persister. En effet, si le temps de stockage est supérieur au délai entre les séries alors le délai de sera donc pas respecté.



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





