#include <stdlib.h>
#include <sys/types.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <stdio.h>
using namespace std;

/*
Je définis (comme un ptit champion)
etiquetteFile = 1 pour les demandes de ressources
              = 2 pour les envois de ressources
              = 3 pour recevoir la ressources à nouveau
*/

int main(int argc, char * argv[]){

    //Récupère la clé pour nos objets IPC
    key_t clef = ftok("./pourCle.txt", 10);

    //Récup l'objet file ou le créer
    int file_id = msgget(clef, IPC_CREAT|0666);

    //La structure des messages que l'on échange
    struct msgbuf {
        long etiquetteFile;     //Message type (must be > 0)
        char message[25];       //La data peut être pleins de trucs
        long pid;               //Le pid du processus pour identifier le message
    };

    while(1) {
        struct msgbuf DemandeRes;

        //A l'état initial, on dispose de la ressource
        //On peut alors la donner au processeur qui la demande

        printf("a la ressource, pret a l envoyer\n");
        //Attente d'un message pour récup la ressource (1)
        int message1 = msgrcv(
            file_id, &DemandeRes,
            (size_t)sizeof(DemandeRes), (long) 1,
            0
        );

        printf("Le processeur %ld demande : ", DemandeRes.pid);
        printf("%s", DemandeRes.message);

        //Des que le message demandant la ressource est reçu
        //On envoi un accusé de réception et on envoi la ressource (en 2)
        struct msgbuf EnvoiRes;
        EnvoiRes.etiquetteFile = DemandeRes.pid;
        strcpy(EnvoiRes.message, "Jenvoi la ressource\n");

        int send = msgsnd(
            file_id, &EnvoiRes,
            (size_t)sizeof(EnvoiRes), 
            0
        );

        printf("Attente de la ressource\n");
        //On passe donc en attente de ressource (en 3)
        struct msgbuf RecupRes;

        int message2 = msgrcv(
            file_id, &RecupRes, 
            (size_t)sizeof(RecupRes.message), (long) 3, 
            0
        );

        printf("Pi renvoi la ressource et ecrit : ");
        printf("%s", RecupRes.message);
        printf("\n");
        //On a alors à nouveau la ressource, super !
        //Un autre Pi peut le récup
    }
}