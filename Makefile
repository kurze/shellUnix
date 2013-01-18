CC=gcc
LDFLAGS=-lreadline -lpthread
EXEC=myshell
CFLAGS=-Wall -Wextra
DEBUG=0

ifeq ($(DEBUG), 1)
	CFLAGS+= -g -DDEBUG_FLAG -O0
else
	CFLAGS+= -O3
endif

all:$(EXEC) $(EXEC)

rebuild : clean all

$(EXEC): cmd.o shell_fct.o client.o serveur.o main.o
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm -vf *.o

# var qui existe à chaque fois
#	$@	: cible
#	$<	: premiere dépendance
#	$^	: tout les pré-requis
#	$+	: la même sans duplicata

# var classique
#	CC	: Compiler Collection
#	LDFLAGS	: Linker Flags
#	CFLAGS	: Compiler Flags
#	EXEC	: EXECutable name

# pattern
#	% 	: joker
#	a.o b.o	: %.o
#	[prefixe]%[suffixe]

# conditions
#	ifdef var
#	...
#	endif
#	ifndef
#	ifeq (var1,var2)
#	ifneq
#
