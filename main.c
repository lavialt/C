#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR 100

typedef struct node {
    char item[MAX_CHAR];
    int quantita;
    struct node *next;
} Node;

void inserisciItem(Node **head, char *item, int quantita) {
    Node *nuovo = malloc(sizeof(Node));
    strcpy(nuovo->item, item);
    nuovo->quantita = quantita;
    nuovo->next = *head;
    *head = nuovo;
}

void eliminaItem(Node **head, char *item) {
    Node *curr = *head, *prev = NULL;
    while (curr != NULL && strcmp(curr->item, item) != 0) {
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

void stampaLista(Node *head) {
     if (head == NULL) {
        printf("La lista della spesa e' vuota.\n");
    }
    else {
    while (head != NULL) {
        printf("%s %d\n", head->item, head->quantita);
        head = head->next;
    }
    }  
}

int leggi_nodi(Node **head, FILE *fp) {
    Node *tmp;
    int count = 0;
    tmp = (Node *)malloc(sizeof(Node));
    if (tmp == NULL) {
        return -1;
    }
    while (fscanf(fp, " %99[^;];%d", tmp->item, &tmp->quantita) == 2) {
        inserisciItem(head, tmp->item, tmp->quantita);
        count++;
    }
    free(tmp);
    return count;
}

int salva_nodi(Node *head) {
    FILE *fp = fopen("spesa.txt", "w");
    if (fp == NULL) {
        perror("Impossibile aprire spesa.txt per il salvataggio");
        return -1;
    }

    while (head != NULL) {
        if (fprintf(fp, "%s;%d\n", head->item, head->quantita) < 0) {
            perror("Errore durante la scrittura di spesa.txt");
            fclose(fp);
            return -1;
        }
        head = head->next;
    }
    printf("Lista salvata in spesa.txt\n");
    return 0;
}

int main() {
    Node *head = NULL;
    FILE *fp;
    fp = fopen("spesa.txt", "r");
    int n_nodi;
    if (fp == NULL) {
        printf("spesa.txt non trovato \n");
    } else {
        n_nodi = leggi_nodi(&head, fp);
        if (n_nodi < 0) {
            fprintf(stderr, "Memoria insufficiente durante la lettura.\n");
            fclose(fp);
            return 1;
        }
        printf("Lette %d items\n", n_nodi);
        fclose(fp);
    }
    int scelta;
    char item[MAX_CHAR];
    int quantita;

    do {
        printf("1. Inserisci Item\n");
        printf("2. Elimina Item\n");
        printf("3. Stampa Lista\n");
        printf("0. Esci\n");
        printf("Scelta: ");
        scanf("%d", &scelta);
        while (scelta < 0 || scelta > 3) {
            printf("Scelta non valida. Riprova: ");
            scanf("%d", &scelta);
        }
        switch (scelta) {
            case 1:
                printf("Inserisci il nome dell'item da aggiungere nella lista della spesa: ");
                scanf("%99s", item);
                printf("Inserisci la quantita': ");
                scanf("%d", &quantita);
                inserisciItem(&head, item, quantita);
                break;
            case 2:
                printf("Inserisci il nome dell'item da eliminare dalla lista della spesa: ");
                scanf("%99s", item);
                eliminaItem(&head, item);
                break;
            case 3:
                printf("Lista della spesa:\n");
                printf("-------------------\n");
                stampaLista(head);
                printf("-------------------\n");
                break;
            case 0:
                printf("Uscita dal programma.\n");
                break;
            default:
                printf("Scelta non valida. Riprova.\n");
        }
    } while (scelta != 0);

    salva_nodi(head);

    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}