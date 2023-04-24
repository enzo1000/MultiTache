########################################
#~ définitions
########################################

BIN=bin/init  bin/suppr bin/test

# liste des fichiers sources
SRCS0=init.c
SRCS1=suppr.c
SRCS2=test.c

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	g++ -Wall -Iinclude -c $< -o $@

bin/init: $(SRCS0:%.c=obj/%.o)
	g++ -o $@ $+

bin/suppr: $(SRCS1:%.c=obj/%.o)
	g++ -o $@ $+

bin/test: $(SRCS2:%.c=obj/%.o)
	g++ -o $@ $+

clean:
	rm -f $(BIN) obj/*.o *~