#include <stdlib.h>
#include <sys/types.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
using namespace std;

/*
Ce fichier ne s'incrit pas dans la ligne de l'exercice,
je suis juste content de l'avoir fait.

Je définis (comme un ptit champion)
etiquetteFile = 1 pour les echanges entre Pi

*/


int main(int argc, char * argv[]){

    //Récupère la clé pour nos objets IPC
    key_t clef = ftok("./pourCle.txt", 10);

    //Récup l'objet file ou le créer
    int file_id = msgget(clef, IPC_CREAT|0666);

    //La structure des messages que l'on échange
    struct msgbuf {
        long etiquetteFile;     //Message type (mus be > 0)
        char message[12];      //La data peut être pleins de trucs
    };

    while(1) {
        struct msgbuf monMsg;
        monMsg.etiquetteFile = 1;
        printf("Saisir message\n");
        scanf("%[^\n]", monMsg.message);

        printf("Mon message : %s\n", monMsg.message);

        //On envoi un message dans la file
        int send = msgsnd(
            file_id, &monMsg,
            (size_t)sizeof(monMsg.message), 
            0
        );
        printf("Sendval : %d\n", send);

        struct msgbuf leurMsg;

        //On reçoit un message de notre liste
        int message = msgrcv(
            file_id, &leurMsg, 
            (size_t)sizeof(leurMsg.message), (long) 1, 
            0
        );

        printf("Leurs message : %s\n", leurMsg.message);
    }
}