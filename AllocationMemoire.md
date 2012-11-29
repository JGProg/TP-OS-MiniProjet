Allocation mémoire avec un malloc
=================================

Création d'un segment de mémoire partagée
-----------------------------------------

###Description
- - - - - - - - 

`shmget()` renvoie l'identificateur du segment de mémoire partagée associé à la valeur de l'argument clé. Un nouveau segment mémoire, de taille size arrondie au multiple supérieur de `PAGE_SIZE`, est créé si clé a la valeur `IPC_PRIVATE` ou si aucun segment de mémoire partagée n'est associé à clé, et `IPC_CREAT` est présent dans `shmflg`.
`shmflg` est composé de :

- `IPC_CREAT` : pour créer un nouveau segment. Sinon shmget() recherchera le segment associé à clé, vérifiera que l'appelant a la permission de recevoir l'identifiant shmid associé au segment, et contrôlera que le segment n'est pas détruit.
- `IPC_EXCL` : est utilisé avec `IPC_CREAT` pour garantir l'échec si le segment existe déjà.
mode d'accès (les 9 bits de poids faibles) indiquant les permissions pour le propriétaire, le groupe et les autres. Actuellement la permission d'exécution n'est pas utilisée par le système.

Si un nouveau segment est créé, les permissions d'accès de `shmflg` sont copiées dans le membre `shm_perm` de la structure `shmid_ds` décrivant le segment. Cette structure est définie ainsi :

	struct shmid_ds 
	{
  		struct          ipc_perm shm_perm; /* Permissions d'accès       */
  		int             shm_segsz;         /* Taille segment en octets  */
  		time_t          shm_atime;         /* Heure dernier attachement */
  		time_t          shm_dtime;         /* Heure dernier détachement */
  		time_t          shm_ctime;         /* Heure dernier changement  */
	  	unsigned short  shm_cpid;          /* PID du créateur           */
		unsigned short  shm_lpid;          /* PID du dernier opérateur  */
  		short           shm_nattch;        /* Nombre d'attachements     */
  		/* ------------- Les champs suivants sont prives -------------- */
  		unsigned short  shm_npages;        /* Taille segment en pages   */
  		unsigned long   *shm_pages;        /* Taille d'une page (?)     */
  		struct shm_desc *attaches;         /* Descript. attachements    */
	};
	
	
	struct ipc_perm
	{
  		key_t  key;
  		ushort uid;   /* UID et GID effectifs du propriétaire    */
  		ushort gid;
  		ushort cuid;  /* UID et GID effectif du créateur         */
  		ushort cgid;
  		ushort mode;  /* Mode d'accès sur 9 bits de poids faible */
  		ushort seq;   /* Numéro de séquence                      */
	};
	
	

###Valeur de retour
- - - - - - - - 

Réussite

Un identificateur de segment shmid valide est renvoyé en cas de réussite, sinon -1 est renvoyé et errno contient le code d'erreur.

Erreur

- `EINVAL` `SHMMIN` > size ou size > `SHMMAX`, ou size plus grand que la taille du segment.
- `EEXIST` On a indiqué `IPC_CREAT | IPC_EXCL` et le segment existe déjà.
- `EIDRM` Le segment est détruit.
- `ENOSPC` Tous les ID de mémoire partagée sont utilisés, ou l'allocation d'un segment partagé de taille size dépasserait les limites de mémoire partagée du système.
- `ENOENT` Aucun segment n'est associé à clé, et IPC_CREAT n'etait pas indiqué.
- `EACCES` L'appelant n'a pas les autorisations d'accès au segment.
- `ENOMEM` Pas assez de mémoire.



Attachement d'un segment de mémoire partagée
-----------------------------------------

###Description
- - - - - - - - 

`shmget`: création du segment de mémoire partagée.
`shmat`: attachement du segment de mémoire partagée // Donne autorisation 

La fonction `shmat` attache le segment de mémoire partagée identifié par shmid au segment de données du processus appelant. L'adresse d'attachement est indiquée par shmaddr avec les critères suivants :

 - Si `shmaddr` vaut 0, le système essaye de trouver une zone libre dans l'intervalle 1 - 1.5Go en commençant par l'adresse la plus haute et en descendant jusqu'à trouver l'emplacement adéquat.
 - Si `shmaddr` n'est pas nulle et si `SHM_RND` est indiqué dans `shmflg`, l'attachement a lieu à l'adresse `shmaddr` arrondie au multiple inférieur de `SHMLBA`. Si `SHM_RND` n'est pas indiqué shmaddr doit être alignée sur une frontière de page, et l'attachement a lieu à cette adresse.
 - Si `SHM_RDONLY` est indiqué dans `shmflg`, le segment est attaché en lecture seulement, et le processus doit disposer de la permission de lecture dessus. Sinon le segment est attaché en lecture et écriture et le processus doit disposer des deux permissions d'accès. Il n'y a pas de notion d'écriture seule pour les segments de mémoire partagée.
	
	
###Valeur de retour
- - - - - - - - 

Réussite
	
Si `shmat` réussit, les membres de la structure shmid_ds associée au segment de mémoire partagée sont mis à jour ainsi :

- `shm_atime` : correspond à l'heure actuelle.
- `shm_lpid` :contient le PID de l'appelant.
- `shm_nattch` : est incrémenté de 1
	
Erreur

- `EACCES` : L'appelant n'a pas les permissions d'accès nécessaires pour l'attachement.
- `EINVAL` : `shmid` est invalide, shmaddr est mal alignée ou l'attachement a échoué sur `brk`.
- `ENOMEM` : Pas assez de mémoire pour le système.
	

Destruction d'un segment de mémoire partagée
--------------------------------------------

###Description
- - - - - - - - 

La fonction `shmdt` détache le segment de mémoire partagée situe à l'adresse indiquée par `shmaddr`. Le segment doit être effectivement attaché, et l'adresse shmaddr doit être celle renvoyée précédement par shmat.


###Valeur de retour
- - - - - - - - 

Réussite
	
Quand shmdt réussit, les membres de la structure shmid_ds associée au segment de mémoire partagée sont mis à jour ainsi :
- `shm_dtime` : correspond à l'heure actuelle.
- `shm_lpid` : contient le PID de l'appelant.
- `shm_nattch` : est décrémenté de 1. S'il devient nul, et si le segment est marqué pour destruction, il est effectivement détruit.


La région occupée de l'espace d'adressage du processus est libérée.
		
Erreur	
- `EINVAL.` : Pas de segment attaché à l'adresse `shmaddr`.