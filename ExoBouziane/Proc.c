#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
    
    if (argc!=5) {
        printf("Nbre d'args invalide, utilisation :\n");
        printf("%s nombre-zones fichier-pour-cle-ipc entier-pour-cle-ipc id\n", argv[0]);
        exit(0);
    }

    struct msgbuf {
        long etiquetteFile;
        int zone;
    };

    int cle = ftok(argv[2], atoi(argv[3]));
    int nbZone = atoi(argv[1]);
    int id = atoi(argv[4]);

    int file_id = msgget(cle, IPC_CREAT | 0666);
    int laMem = shmget(cle, nbZone*sizeof(int), IPC_CREAT | 0666);
    if (laMem == -1){
        perror("erreur shmget : ");
        exit(-1);
    }

    for (int i = 0; i < nbZone ; i++) {
        struct msgbuf TraitementZone;
        if (id != 1) {
            int acceptationZone = msgrcv(
                file_id, &TraitementZone,
                (size_t)sizeof(TraitementZone), (long) id,
                0
            );
        }

        printf("Je suis %d je traite %d\n", id, i);

        calcul(1);

        //Mem
        //attachement au segment pour pouvoir y accéder
        int * p_att = (int *)shmat(laMem, NULL, 0);
        if (p_att == (int *) - 1){
            perror("erreur shmat : ");
            exit(-1);
        }

        // j'ai un pointeur sur le segment j'incrémente le tableau et l'affiche
        p_att[i] += 1;
        printf("Notre segment de mem partage : [");
        for(int z=0; z < nbZone - 1; z++){
            printf("%d, ",p_att[z]);
        }
        printf("%d ]\n\n", p_att[nbZone - 1]);

        //détachement pour signaler au système la fin de l'utilisation du segment
        if (shmdt(p_att) == -1){
            perror("erreur shmdt : ");
            exit(-1);
        }
        //Fin Mem

        struct msgbuf FinTraitementZone;
        FinTraitementZone.etiquetteFile = id + 1;
        FinTraitementZone.zone = i + 1;

        int send = msgsnd(
            file_id, &FinTraitementZone,
            (size_t)sizeof(FinTraitementZone),
            0
        );
    }
}