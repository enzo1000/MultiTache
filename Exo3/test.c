#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
  
    if (argc!=6) {
        printf("Nbre d'args invalide, utilisation :\n");
        printf("%s nombre-traitements nombre-zones fichier-pour-cle-ipc entier-pour-cle-ipc id\n", argv[0]);
        exit(0);
    }

    int cle = ftok(argv[3], atoi(argv[4]));
    int nbSem = atoi(argv[1]) - 1;          //Nombre de segment à traiter
    int nbProc = atoi(argv[2]);
    int id = atoi(argv[5]);

    int idSem = semget(cle, nbSem, 0600);
    if(idSem == -1){
        perror("erreur semget : ");
        exit(-1);
    }

    union semun{  //Structure concernant la récupération du sémaphore et de son affichage.
        int val;
        struct semid_ds * buf;
        unsigned short * array;
    };

    /*
    On récup le tableau de mémoire partagé.
    on lance i processus demandant chacun i ressources (processus 1, 1ressource, 2, 2ressource ...)
    On initialise notre tableau de sémaphore à 1, de cette manière, le processus 1 consomme 1 res puis en donne 2
    pour le processus 2 et ainsi de suite
    */

    /*
    for (int i = 0; i <= nbSem ; i++) {
        if (fork() == 0) {      //Pas à faire
            int id = i + 1; */
            printf("Je suis le proc : %d, init traitement\n", id);
            for (int j = 0; j < nbProc ; j++) {
                //Le processus i fait "id" opérations sur la case j
                struct sembuf op1[] = {j, -id, 0};
                if(semop(idSem, op1, 1) < 0) {
                    perror("erreur semop :");
                    exit(-1);
                }

                printf(" %d : Je traite ma zone %d \n", id, j+1);

                //Mémoire partagé piou piou
                int laMem = shmget(cle, atoi(argv[2])*sizeof(int), 0600);
                if (laMem == -1){
                    perror("erreur shmget : ");
                    exit(-1);
                }

                //attachement au segment pour pouvoir y accéder
                int * p_att = (int *)shmat(laMem, NULL, 0);
                if (p_att == (int *) - 1){
                    perror("erreur shmat : ");
                    exit(-1);
                }

                // j'ai un pointeur sur le segment j'incrémente le tableau et l'affiche
                p_att[j] += 1;
                printf("Notre segment de mem partage : [");
                for(int z=0; z < nbProc - 1; z++){
                    printf("%d, ",p_att[z]);
                }
                printf("%d ]\n\n", p_att[nbProc - 1]);

                //détachement pour signaler au système la fin de l'utilisation du segment
                if (shmdt(p_att) == -1){
                    perror("erreur shmdt : ");
                    exit(-1);
                }

                //On rend alors id+1 ressources pour le processus suivant
                struct sembuf op2[] = {j, id+1, 0};
                if(semop(idSem, op2, 1) < 0) {
                    perror("erreur semop :");
                    exit(-1);
                }
            }
            printf("%d : Je rend la main au prochain proc : %d\n", id, id+1);
            return 0;
}    
        /*
    }
}
*/