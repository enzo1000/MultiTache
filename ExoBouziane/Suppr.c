#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>

int main(int argc, char * argv[]){

    if (argc!=3) {
        printf("Nbre d'args invalide, utilisation :\n");
        printf("%s fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
        exit(0);
    }
	  
    int cle = ftok(argv[1], atoi(argv[2]));
    if (cle == -1) {
        perror("Erreur ftok : ");
        exit(2);
    }

    int msgid = msgget(cle, 0666);
    if(msgid==-1) {
        perror("erreur msgget : ");
        exit(2);
    }

    //CleSem est la même que pour le segment de mémoire partagé
    int laMem = shmget(cle, 0, 0600);
    if (laMem == -1){
        perror("erreur shmget : ");
        exit(-1);
    }

    // destruction :
    if (shmctl(laMem, IPC_RMID, NULL) == -1)
        perror(" erreur shmctl : ");
    
    if (msgctl(msgid, IPC_RMID, NULL) == -1)
        perror("erreur suppression file de message :");

    return 0;
}

