#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define CURATA_ECRAN() system("cls")
#else
#define CURATA_ECRAN() system("clear")
#endif

#define MAX_PRODUSE 100
#define NUME_FISIER "produse_farmacie.txt"


#define RESET "\033[0m"
#define VERDE "\033[0;32m"
#define ROSU "\033[0;31m"
#define GALBEN "\033[0;33m"
#define ALBASTRU "\033[0;34m"

typedef struct {
    char nume[50];
    int cantitate;
    int rezervate;
} Produs;

Produs produse[MAX_PRODUSE];
int numar_produse = 0;

void incarca_produse() {
    FILE *fisier = fopen(NUME_FISIER, "r");
    if (fisier == NULL) {
        printf(ROSU "Nu exista un fisier de date. Se va crea unul nou.\n" RESET);
        return;
    }

    while (fscanf(fisier, "%s %d %d", produse[numar_produse].nume, &produse[numar_produse].cantitate, &produse[numar_produse].rezervate) != EOF) {
        numar_produse++;
    }

    fclose(fisier);
}

void salveaza_produse() {
    FILE *fisier = fopen(NUME_FISIER, "w");
    if (fisier == NULL) {
        printf(ROSU "Eroare la deschiderea fisierului pentru scriere.\n" RESET);
        return;
    }

    for (int i = 0; i < numar_produse; i++) {
        fprintf(fisier, "%s %d %d\n", produse[i].nume, produse[i].cantitate, produse[i].rezervate);
    }

    fclose(fisier);
}

void afiseaza_meniu_principal() {
    CURATA_ECRAN();
    printf(ALBASTRU "=== Sistem pentru rezervarea produselor din farmacie ===\n" RESET);
    printf("1. Vizualizare produse disponibile\n");
    printf("2. Adaugare produs\n");
    printf("3. Stergere produs\n");
    printf("4. Cautare produs\n");
    printf("5. Rezervare produs\n");
    printf("6. Afisare produse rezervate\n");
    printf("7. Iesire\n");
}

void afiseaza_produse_disponibile() {
    CURATA_ECRAN();
    printf(GALBEN "=== Produse Disponibile ===\n" RESET);
    for (int i = 0; i < numar_produse; i++) {
        printf("%d. " VERDE "%s" RESET " | Cantitate: %d | Rezervate: %d\n",
               i + 1, produse[i].nume, produse[i].cantitate, produse[i].rezervate);
    }
    printf("\n0. Inapoi la meniul principal\n");
    printf("Apasa orice tasta + Enter pentru a continua...");
    getchar(); getchar();
}

void adauga_produs() {
    CURATA_ECRAN();
    printf(GALBEN "=== Adaugare Produs ===\n" RESET);
    if (numar_produse >= MAX_PRODUSE) {
        printf(ROSU "Limita maxima de produse a fost atinsa.\n" RESET);
        return;
    }

    Produs nou_produs;
    printf("Introduceti " VERDE "numele produsului" RESET ": ");
    scanf("%s", nou_produs.nume);

    printf("Introduceti " VERDE "cantitatea disponibila (numar pozitiv)" RESET ": ");
    while (scanf("%d", &nou_produs.cantitate) != 1 || nou_produs.cantitate < 0) {
        printf(ROSU "Valoare invalida. Introduceti o cantitate pozitiva: " RESET);
        while(getchar() != '\n');
    }

    nou_produs.rezervate = 0;

    produse[numar_produse++] = nou_produs;
    salveaza_produse();
    printf(VERDE "Produs adaugat cu succes.\n" RESET);
}

void sterge_produs() {
    CURATA_ECRAN();
    printf(GALBEN "=== Stergere Produs ===\n" RESET);
    char nume[50];
    printf("Introduceti " VERDE "numele produsului de sters" RESET ": ");
    scanf("%s", nume);

    int gasit = 0;
    for (int i = 0; i < numar_produse; i++) {
        if (strcmp(produse[i].nume, nume) == 0) {
            for (int j = i; j < numar_produse - 1; j++) {
                produse[j] = produse[j + 1];
            }
            numar_produse--;
            gasit = 1;
            printf(VERDE "Produs sters cu succes.\n" RESET);
            break;
        }
    }

    if (!gasit) {
        printf(ROSU "Produsul nu a fost gasit.\n" RESET);
    }

    salveaza_produse();
}

void cauta_produs() {
    CURATA_ECRAN();
    printf(GALBEN "=== Cautare Produs ===\n" RESET);
    char nume[50];
    printf("Introduceti " VERDE "numele produsului de cautat" RESET ": ");
    scanf("%s", nume);

    int gasit = 0;
    for (int i = 0; i < numar_produse; i++) {
        if (strcmp(produse[i].nume, nume) == 0) {
            printf("Produs: " VERDE "%s" RESET " | Cantitate: %d | Rezervate: %d\n", produse[i].nume, produse[i].cantitate, produse[i].rezervate);
            gasit = 1;
            break;
        }
    }

    if (!gasit) {
        printf(ROSU "Produsul nu a fost gasit.\n" RESET);
    }
}

void rezervare_produs() {
    CURATA_ECRAN();
    printf(GALBEN "=== Rezervare Produs ===\n" RESET);
    char nume[50];
    int cantitate_rezervata;

    printf("Introduceti " VERDE "numele produsului" RESET ": ");
    scanf("%s", nume);

    int gasit = 0;
    for (int i = 0; i < numar_produse; i++) {
        if (strcmp(produse[i].nume, nume) == 0) {
            printf("Produs gasit: " VERDE "%s" RESET " | Cantitate disponibila: %d\n", produse[i].nume, produse[i].cantitate);
            printf("Introduceti " VERDE "cantitatea de rezervat" RESET " (maxim %d): ", produse[i].cantitate);
            while (scanf("%d", &cantitate_rezervata) != 1 || cantitate_rezervata < 1 || cantitate_rezervata > produse[i].cantitate) {
                printf(ROSU "Valoare invalida. Introduceti o valoare intre 1 si %d: " RESET, produse[i].cantitate);
                while(getchar() != '\n');
            }

            produse[i].cantitate -= cantitate_rezervata;
            produse[i].rezervate += cantitate_rezervata;
            printf(VERDE "Rezervare efectuata cu succes.\n" RESET);
            salveaza_produse();
            gasit = 1;
            break;
        }
    }

    if (!gasit) {
        printf(ROSU "Produsul nu a fost gasit.\n" RESET);
    }
}

void afisare_produse_rezervate() {
    CURATA_ECRAN();
    printf(GALBEN "=== Produse Rezervate ===\n" RESET);
    int exista = 0;
    for (int i = 0; i < numar_produse; i++) {
        if (produse[i].rezervate > 0) {
            printf("Produs: " VERDE "%s" RESET " | Rezervate: %d\n", produse[i].nume, produse[i].rezervate);
            exista = 1;
        }
    }
    if (!exista) {
        printf(ROSU "Nu exista produse rezervate.\n" RESET);
    }

    printf("\n0. Inapoi la meniul principal\n");
    printf("Apasa orice tasta + Enter pentru a continua...");
    getchar(); getchar();
}

int main() {
    incarca_produse();
    int optiune;
    do {
        afiseaza_meniu_principal();
        printf("Alegeti o optiune: ");
        if (scanf("%d", &optiune) != 1) {
            printf(ROSU "Optiune invalida.\n" RESET);
            while (getchar() != '\n');
            continue;
        }

        switch (optiune) {
            case 1: afiseaza_produse_disponibile(); break;
            case 2: adauga_produs(); break;
            case 3: sterge_produs(); break;
            case 4: cauta_produs(); break;
            case 5: rezervare_produs(); break;
            case 6: afisare_produse_rezervate(); break;
            case 7: printf(VERDE "Iesire din sistem.\n" RESET); break;
            default: printf(ROSU "Optiune invalida. Incercati din nou.\n" RESET);
        }
    } while (optiune != 7);
    return 0;
}

