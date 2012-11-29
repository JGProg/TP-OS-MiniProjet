Allocation mémoire avec un malloc
=================================

Création d'un segment de mémoire partagée
-----------------------------------------

	`shmget` : création du segment de mémoire partagée
	`shmat` : attachement du segment de mémoire partagée // Donne autorisation 

La fonction `shmat` attache le segment de mémoire partagée identifié par shmid au segment de données du processus appelant. L'adresse d'attachement est indiquée par shmaddr avec les critères suivants :

 - Si `shmaddr` vaut 0, le système essaye de trouver une zone libre dans l'intervalle 1 - 1.5Go en commençant par l'adresse la plus haute et en descendant jusqu'à trouver l'emplacement adéquat.
 - Si `shmaddr` n'est pas nulle et si `SHM_RND` est indiqué dans `shmflg`, l'attachement a lieu à l'adresse `shmaddr` arrondie au multiple inférieur de `SHMLBA`. Si `SHM_RND` n'est pas indiqué shmaddr doit être alignée sur une frontière de page, et l'attachement a lieu à cette adresse.
 - Si `SHM_RDONLY` est indiqué dans `shmflg`, le segment est attaché en lecture seulement, et le processus doit disposer de la permission de lecture dessus. Sinon le segment est attaché en lecture et écriture et le processus doit disposer des deux permissions d'accès. Il n'y a pas de notion d'écriture seule pour les segments de mémoire partagée.
	
	
Valeur de retour
----------------
	Reussite
	========
	Si `shmat` réussit, les membres de la structure shmid_ds associée au segment de mémoire partagée sont mis à jour ainsi :

	`shm_atime` : correspond à l'heure actuelle.
	`shm_lpid` :contient le PID de l'appelant.
	`shm_nattch` : est incrémenté de 1
	
	Erreur
	======
	`EACCES` : L'appelant n'a pas les permissions d'accès nécessaires pour l'attachement.
	`EINVAL` : `shmid` est invalide, shmaddr est mal alignée ou l'attachement a échoué sur `brk`.
	`ENOMEM` : Pas assez de mémoire pour le système.
	

Destruction d'un segment de mémoire partagée
--------------------------------------------

	La fonction `shmdt` détache le segment de mémoire partagée situe à l'adresse indiquée par `shmaddr`. Le segment doit être effectivement attaché, et l'adresse shmaddr doit être celle renvoyée précédement par shmat.
	
	Reussite
	========
	
		Quand shmdt réussit, les membres de la structure shmid_ds associée au segment de mémoire partagée sont mis à jour ainsi :
		`shm_dtime` : correspond à l'heure actuelle.
		`shm_lpid` : contient le PID de l'appelant.
		`shm_nattch` : est décrémenté de 1. S'il devient nul, et si le segment est marqué pour destruction, il est effectivement détruit.


		La région occupée de l'espace d'adressage du processus est libérée.
		
	Erreur
	======
	
		`EINVAL.` : Pas de segment attaché à l'adresse `shmaddr`.