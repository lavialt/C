#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRLEN 50
#define MAX_GENERI 50

typedef struct
{
    char nome_traccia[MAX_STRLEN];
    char genere[MAX_STRLEN];
    char nome_artista[MAX_STRLEN];
    int lunghezza; // Lunghezza in secondi
} Payload;

typedef struct node
{
    Payload payload;
    struct node *next;
} Node;

// Rimuove i \n dal payload di un nodo
void remove_newline(Node *pn)
{
    int count = 0;
    while (pn->payload.nome_traccia[count] != '\n')
    {
        count++;
    }
    pn->payload.nome_traccia[count] = '\0';

    count = 0;
    while (pn->payload.genere[count] != '\n')
    {
        count++;
    }
    pn->payload.genere[count] = '\0';

    count = 0;
    while (pn->payload.nome_artista[count] != '\n')
    {
        count++;
    }
    pn->payload.nome_artista[count] = '\0';
}

void inserisci_in_testa(Node **head, Node *item)
{
    if (*head == NULL)
    {
        *head = item;
        item->next = NULL;
        return;
    }

    item->next = *head;
    *head = item;
}

// Trasforma la durata in secondi in mm:ss o tiene in secondi se minore di 60
char *formatta_durata(int secondi, char *output_buf)
{
    if (secondi < 60)
    {
        sprintf(output_buf, "%ds", secondi);
    }
    else
    {
        int minuti = secondi / 60;
        secondi = secondi % 60;
        sprintf(output_buf, "%dm:%ds", minuti, secondi);
    }

    return output_buf;
}

void get_lista_generi(char lista_generi[MAX_GENERI][MAX_STRLEN])
{
    for (int i = 0; i < MAX_GENERI; i++)
    {
        strcpy(lista_generi[i], "");
    }
    strcpy(lista_generi[0], "Rock");
    strcpy(lista_generi[1], "Pop");
    strcpy(lista_generi[2], "Jazz");
    strcpy(lista_generi[3], "Swing");
    strcpy(lista_generi[4], "Rap");
}

// Scrive il payload dei nodi sul file in formato csv, ritorna quanti nodi ha scritto
int salva_nodi(Node *head, FILE *fp)
{
    Node *curr = head;
    int count = 0;
    while (curr != NULL)
    {
        fprintf(fp,
                "%s;%s;%s;%d\n",
                curr->payload.nome_traccia,
                curr->payload.genere,
                curr->payload.nome_artista,
                curr->payload.lunghezza);
                
        curr = curr->next;
        count++;
    }

    return count;
}

int leggi_nodi(Node **head, FILE *fp)
{
    Node *tmp;
    int count = 0;
    tmp = (Node *)malloc(sizeof(Node));
    if (tmp == NULL)
    {
        return -1;
    }
    while (fscanf(fp,
                  "%[^;];%[^;];%[^;];%d\n",
                  tmp->payload.nome_traccia,
                  tmp->payload.genere,
                  tmp->payload.nome_artista,
                  &tmp->payload.lunghezza) == 4)
    {
        inserisci_in_testa(head, tmp);
        count++;
    }

    free(tmp);
    return count;
}

int main()
{
    FILE *fp;
    fp = fopen("data.csv", "r");
    Node *head = NULL;
    int n_nodi;
    if (fp == NULL)
        printf("\nIl file non esiste");
    else if (feof(fp))
    {
        printf("\nIl file è vuoto");
        fclose(fp);
    }
    else
    {
        n_nodi = leggi_nodi(&head, fp);
        printf("Lette %d canzoni", n_nodi);
        fclose(fp);
    }

    int scelta;
    int count = 0;
    Node *buf_nodo = (Node *)malloc(sizeof(Node));
    Node *curr;
    char str_buf[MAX_STRLEN];
    char lista_generi[MAX_GENERI][MAX_STRLEN];
    get_lista_generi(lista_generi);
    int minuti;
    int secondi;
    int check_formato;
    int temp_genere;
    do
    {
        printf("\n0. Esci");
        printf("\n1. Inserire traccia");
        printf("\n2. Visualizza tutte le tracce\n");
        scanf("%d", &scelta);
        getchar();

        switch (scelta)
        {
        case 0:
            fp = fopen("data.csv", "w");
            if (fp == NULL)
            {
                printf("\nImpossibile aprire il file, la playlist non verrà salvata");
            }
            else
            {
                n_nodi = salva_nodi(head, fp);
                printf("\nSalvate %d canzoni nel file", n_nodi);
                fclose(fp);
            }
            break;
        case 1:
            printf("\nInserire il nome della traccia: ");
            fgets(buf_nodo->payload.nome_traccia, MAX_STRLEN, stdin);
            printf("\nSelezionare il genere: ");
            while (strcmp(lista_generi[count], ""))
            {
                printf("\n%d: %s", count + 1, lista_generi[count]);
                count++;
            };
            printf("\n");
            scanf("%d", &temp_genere);
            getchar();
            // TODO: Aggiungere check input
            strcpy(buf_nodo->payload.genere, lista_generi[temp_genere - 1]);
            printf("\nInserire il nome dell'artista/band: ");
            fgets(buf_nodo->payload.nome_artista, MAX_STRLEN, stdin);

            check_formato = 0;
            do
            {
                printf("\nInserire la lunghezza della canzone nel formato XXm:XXs : ");
                int item_letti = scanf("%dm:%ds", &minuti, &secondi);
                getchar();
                if (item_letti != 2)
                {
                    printf("\nErrore nell'inserimento, utilizzare il formato XXm:XXs");
                    printf("\nEsempio: 03m:12s");
                }
                else
                    check_formato = 1;
            } while (!check_formato);
            buf_nodo->payload.lunghezza = minuti * 60 + secondi;

            remove_newline(buf_nodo);
            inserisci_in_testa(&head, buf_nodo);
            break;
        case 2:
            if (head == NULL)
            {
                printf("\nLa playlist è vuota");
            }
            else
            {
                curr = head;
                while (curr != NULL)
                {
                    printf("\nNome traccia: %s", curr->payload.nome_traccia);
                    printf("\nArtista: %s", curr->payload.nome_artista);
                    printf("\nGenere: %s", curr->payload.genere);
                    printf("\nLunghezza: %s", formatta_durata(curr->payload.lunghezza, str_buf));

                    curr = curr->next;
                }
            }
            break;
        default:
            printf("\nInput errato");
            break;
        }
    } while (scelta != 0);
}