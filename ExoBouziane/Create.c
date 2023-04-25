#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
    
    if (argc!=4) {
        printf("Nbre d'args invalide, utilisation :\n");
        printf("%s nombre-zones fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
        exit(0);
    }

    int cle = ftok(argv[2], atoi(argv[3]));
    int nbZone = atoi(argv[1]);

    int file_id = msgget(cle, IPC_CREAT | 0666);
    int laMem = shmget(cle, nbZone*sizeof(int), IPC_CREAT | 0666);
    if (laMem == -1){
        perror("erreur shmget : ");
        exit(-1);
    }

    //attachement au segment pour pouvoir y accéder
    int * p_att = (int *)shmat(laMem, NULL, 0);
    if (p_att== (int *)-1){
        perror("erreur shmat : ");
        exit(-1);
    }

    //j'ai un pointeur sur le segment, j'initialise le tableau 
    for(int i=0; i < atoi(argv[2]); i++){
        p_att[i] = 0;
    }

    // détachement pour signaler au système la fin de l'utilisation du segment
    if (shmdt(p_att) == -1){
        perror("erreur shmdt : ");
        exit(-1);
    }
}