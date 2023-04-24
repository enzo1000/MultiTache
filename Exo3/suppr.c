/*
  Description : 
  Le programme dertruit un tableau de sémaphores s'il existe.
  
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>

int main(int argc, char * argv[]){

  if (argc!=3) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
    exit(0);
  }
	  
  int cleSem=ftok(argv[1], atoi(argv[2]));

  int idSem = semget(cleSem, 1, 0600);

  // j'utilise semget de sorte a s'assurer que le tableau a détruire existe.
  if (idSem==-1){
    perror("erreur  semget");
    exit(-1);
  }

  //CleSem est la même que pour le segment de mémoire partagé
  int laMem = shmget(cleSem, 0, 0600);
  if (laMem == -1){
      perror("erreur shmget : ");
      exit(-1);
  }

  printf("sem id : %d \n", idSem);

  // destruction :
  if (semctl(idSem, 0, IPC_RMID, NULL)==-1)
    perror(" erreur semctl : ");

  if (shmctl(laMem, IPC_RMID, NULL) == -1)
    perror(" erreur shmctl : ");

  return 0;
}

