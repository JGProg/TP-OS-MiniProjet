#Makefile Générique 
#Nom binome : Jessy Giacomoni & Adrien Rogier
#Promo : 3APP P2015



#SRC = $(wildcard *.c) :  Il s'en sert pour récupérer tous les fichiers finissant par .c du répétoire dans lequel se situe le Makefile.
#OBJS = $(SRC:.c=.o) : permet de préciser les nom de chaque fichier .o à partir du fichier .c.
#> Le nom de la cible est « @ »
#> Le nom de la première dépendance est « < »
#> La liste des dépendances est « ^ »
#%.o : %.c : règle générique pour la construction d'un fichier .o.

#Option GCC :
#pedantic : les avertissements requis par les standards ISO C et ISO C++
#o2 : Le compilateur tente de réduire la taille du code et le temps d'exécution tout en limitant le temps de compilation
#Wall : activer les messages d'avertissement

CC = gcc
DEBUG=yes
ifeq ($(DEBUG),yes)
	CFLAGS= -W -Wall -g -O2
	LDFLAGS=
else
	CFLAGS=-W -Wall
	LDFLAGS=
endif
 
SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)
EXEC=prog

all : $(EXEC) 
 
$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean :
	@rm -rf *.o $(EXEC)

mrproper : clean
	@rm -rf $(EXEC)
