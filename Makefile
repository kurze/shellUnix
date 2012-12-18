CC=gcc
LDFLAGS=-lreadline -lpthread
EXEC=myshell
CFLAGS=-Wall
DEBUG=0

ifeq ($(DEBUG), 1)
	CCFLAGS+=-g -DDEBUG_FLAG
else
	CCFLAGS+=-O3
endif

rebuild : clean all

all:$(EXEC)

$(EXEC): main.o cmd.o shell_fct.o
	$(CC) -o  $(EXEC) $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm -vf *.o

# var qui existe à chaque fois
#	$@  : cible
#	$< : premiere dépendance
#	$^ : tout les pré-requis
#	$+ : la même sans duplicata

# pattern
#	a.o  b.o => %.o
#	[prefixe]%[suffixe]

# conditions
#	ifdef var
#	...
#	endif
#	ifndef
#	ifeq (var1,var2)
#	ifneq
#