#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include "calcul.h"
/*
 Exemple de création d'un tableau de sémaphores, dont le nombre
 d'éléments et la valeur initiale est passée en paramètre du
 programme (dans cet exemple, les éléments sont initialisés à la même valeur)
 */

int main(int argc, char * argv[]){
  
  //Nb semaphores
  //Valeur initiale (et nb processus en parallèle)
  //Fichier pour cle ipc
  //Entier pour cle ipc
  if (argc!=5) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s nombre-semaphores valeur-initiale fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
    exit(0);
  }

  int cleSem = ftok(argv[3], atoi(argv[4]));  //Créer une cle ipc à partir du chemin du fichier et d'une valeur fournit en param
  int nbSem = atoi(argv[1]);

  //Création du tableau de sémaphore de nbSem
  int idSem = semget(cleSem, nbSem, IPC_CREAT | 0666);
  
  if(idSem == -1){
    perror("erreur semget : ");
    exit(-1);
  }

  printf("sem id : %d \n", idSem);
  
  // initialisation des sémaphores a la valeur passée en parametre (nbSem) (faire autrement pour des valeurs différentes ):
  unsigned short tabinit[nbSem];
  for (int i = 0; i < nbSem; i++) tabinit[i] = atoi(argv[2]);;

  //Structure pour stocker le tableau de sémaphore
  union semun{
    int val;
    struct semid_ds * buf;
    unsigned short * array;
  };
  
  union semun valinit;

  valinit.array = tabinit;

  //Initialisation d'un ensemble de sémaphores et autre SETALL
  if (semctl(idSem, nbSem, SETALL, valinit) == -1){ //semctl pour initialiser
    perror("erreur initialisation sem : ");
    exit(1);
  }

  /* test affichage valeurs des sémaphores du tableau */
  valinit.array = (unsigned short*)malloc(nbSem * sizeof(unsigned short)); // pour montrer qu'on récupère bien un nouveau tableau dans la suite

  //On récupère tout les sémaphores GETALL dans valinit
  //Si un processus tier à modifier un semaphore ou autre
  if (semctl(idSem, nbSem, GETALL, valinit) == -1){
    perror("erreur initialisation sem : ");
    exit(1);
  }
   
  printf("Valeurs des sempahores apres initialisation [ ");
  for(int i=0; i < nbSem-1; i++){
    printf("%d, ", valinit.array[i]);
  }
  printf("%d ] \n", valinit.array[nbSem-1]);

  free(valinit.array);
  return 0;
}
