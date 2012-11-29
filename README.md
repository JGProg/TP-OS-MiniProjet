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
    - Il gère les retours d'erreurs.
    - Ce programme doit contenir les variables :
        -> n : le nombre de séries d'acuqisition qui seront réalisés
        -> delai : le temps en seconde entre 2 série d'acquisition de données
        -> nbAcquisition : Le nombre d'acquisition faites pour une série
        -> delaiAcqusition : le temps en seconde entre 2 acquisitions d'une même série
    
  ALGORITHME :
  
  

=================================================================================================================================================
                 Programme acquisition.c/h
=================================================================================================================================================

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

                 La fonction




=================================================================================================================================================
                 Programme stockage.c/h
=================================================================================================================================================






=================================================================================================================================================
                 Programme traitement.c/h
=================================================================================================================================================







=================================================================================================================================================
                 Programme courbe.c/h
=================================================================================================================================================







