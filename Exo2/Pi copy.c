//Vieille version avec fork

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

int main(int argc, char * argv[]){  
  if (argc!=5) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s nbSemaphores  nbProcessus  fichier-pour-cle-ipc  entier-pour-cle-ipc\n", argv[0]);
    exit(0);
  }

  union semun{  //Structure concernant la récupération du sémaphore et de son affichage.
    int val;
    struct semid_ds * buf;
    unsigned short * array;
  };

  int cle_sem = ftok(argv[3], atoi(argv[4]));
  int nbSem = atoi(argv[1]);
  int idSem = semget(cle_sem, 0, 0666); //semget pour récup le sémaphore
  int nbProc = atoi(argv[2]);

  //Si on arrive pas à récupérer le tableau de sémaphore
  if(idSem == -1) {
    perror("erreur semget : ");
    exit(-1);
  }
  printf("sem id : %d \n", idSem);

  //Le programme :  On lance nbProcessus concurents qui vont calculer nbSemaphores tout en s'attendans à chaques étapes

  // Le programme est inutilement compliqué, il nous n'est pas demandé de réaliser des fork()
  // On peut donc supprimer 1 boucle
  for (int i = 0; i < nbProc ; i++) {       //On lance i processeurs (nbProc)
    if (fork() == 0) {                      //Pas à faire
      for (int j = 0; j < nbSem ; j++) {    //Qui vont réaliser j étapes (nbSem)
        calcul(i%2);                        //Ils effectuent un calcul

        struct sembuf op[] = {{j, -1, 0}, {j, 0, 0}};   //Structure d'instruction pour :
        //Décrémenter (Opération P)
        if (semop(idSem, op, 1) < 0) {
          perror("erreur semop :");
          exit(-1);
        }

        //On réalise l'affichage du tableau de sémaphore pour la visualisation
        union semun val;
        val.array = (unsigned short *) malloc (nbSem * sizeof(unsigned short));
        if (semctl(idSem, nbSem, GETALL, val) == -1){
          perror("erreur recuperation sem : ");
          exit(1);
        }

        printf("Valeurs des semaphores apres operation [ "); 
        for(int k = 0; k < nbSem-1; k++){
          printf("%d, ", val.array[k]);
        }
        printf("%d ] \n", val.array[nbSem-1]);

        //Attente
        printf("Mise en attente du proc %d \n", i);
        if(semop(idSem, op+1, 1) < 0) {
          perror("erreur semop :");
          exit(-1);
        }
        printf("Rdv n°%d effectué par %d ! \n", j, i);
        free(val.array);
      }
      printf("Fin du programme pour %d\n", i);
      return 0;
    }
  }

  return 0;
}