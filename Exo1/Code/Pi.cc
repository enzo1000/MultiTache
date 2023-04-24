#include <stdlib.h>
#include <sys/types.h>
#include <iostream>
#include <cstring>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;

key_t clef = ftok("./pourCle.txt", 10);

int main(int argc, char * argv[]){

    //Même début de fichier

    //Récupère la clé pour nos objets IPC
    key_t clef = ftok("./pourCle.txt", 10);

    //Récup l'objet file ou le créer
    int file_id = msgget(clef, IPC_CREAT|0666);

    //La structure des messages que l'on échange
    struct msgbuf {
        long etiquetteFile;    //Message type (mus be > 0)
        char message[25];      //La data peut être pleins de trucs
        long pid;              //Le pid du processus pour identifier le message
    };

    //Lets go
    //On demande la ressource (en 1)
    printf("On demande la ressource\n");
    struct msgbuf DemandeRes;
    DemandeRes.etiquetteFile = 1;
    DemandeRes.pid = getpid();
    strcpy(DemandeRes.message, "Besoin dla ressource\n");

    int send1 = msgsnd(
        file_id, &DemandeRes,
        (size_t)sizeof(DemandeRes),
        0
    );

    //Attente de la res (en 2)
    struct msgbuf RecupRes;

    int message = msgrcv(
        file_id, &RecupRes, 
        (size_t)sizeof(RecupRes), (long) getpid(), 
        0
    );
    printf("Message Pctrl : ");
    printf("%s", RecupRes.message);

    //On renvoi la res (en 3)
    struct msgbuf EnvoiRes;
    EnvoiRes.etiquetteFile = 3;
    strcpy(EnvoiRes.message, "Jenvoi la ressource\n");

    int send2 = msgsnd(
        file_id, &EnvoiRes,
        (size_t)sizeof(EnvoiRes.message), 
        0
    );
    printf("On renvoi la ressource\n");

}