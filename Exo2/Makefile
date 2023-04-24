########################################
#~ définitions
########################################

BIN=bin/init  bin/suppr bin/test

# liste des fichiers sources
SRCS0=semInit.c
SRCS1=supprimeSem.c

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	g++ -Wall -Iinclude -c $< -o $@

bin/init: $(SRCS0:%.c=obj/%.o)
	g++  calculCC.o $@ -o $@ $+

bin/suppr: $(SRCS1:%.c=obj/%.o)
	g++ -o $@ $+

bin/test: $(SRCS2:%.c=obj/%.o)
	g++ -o $@ $+

clean:
	rm -f $(BIN) obj/*.o *~