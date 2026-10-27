#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 100

typedef struct Node {
    char player[MAX_CHAR];
    int punti;
    struct Node *next;
} Node;

void aggiungiPlayer(Node **head, char *player, int punti) {
    Node *nuovo = malloc(sizeof(Node));
    if (nuovo == NULL) {
        return;
    }

    strcpy(nuovo->player, player);
    nuovo->punti = punti;
    nuovo->next = *head;
    *head = nuovo;
}

void eliminaPlayer(Node **head, char *player) {
    Node *curr = *head, *prev = NULL;

    while (curr != NULL && strcmp(curr->player, player) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        return;
    }

    if (prev == NULL) {
        *head = curr->next;
    } else {
        prev->next = curr->next;
    }

    free(curr);
}

void stampaClassifica(Node *head) {
    while (head != NULL) {
        printf("%s %d punti\n", head->player, head->punti);
        head = head->next;
    }
}

int main(void) {
    Node *head = NULL;
    int scelta;
    char player[MAX_CHAR];
    int punti;

    do {
        printf("\n1) Inserisci un player\n");
        printf("2) Elimina un player\n");
        printf("3) Stampa classifica\n");
        printf("0) Esci\n");
        printf("Scelta: ");
        scanf("%d", &scelta);

        while (scelta < 0 || scelta > 3) {
            printf("Scelta non valida, riprova\n");
            scanf("%d", &scelta);
        }

        switch (scelta) {
            case 1:
                printf("Inserisci il nome del player: \n");
                scanf("%s", player);
                printf("Inserisci i punti: \n");
                scanf("%d", &punti);
                aggiungiPlayer(&head, player, punti);
                break;

            case 2:
                printf("Inserisci il nome del player da eliminare: \n");
                scanf("%s", player);
                eliminaPlayer(&head, player);
                break;

            case 3:
                printf("CLASSIFICA:\n");
                stampaClassifica(head);
                break;
        }
    } while (scelta != 0);

    while (head != NULL) {
        Node *curr = head;
        head = head->next;
        free(curr);
    }

    return 0;
}
