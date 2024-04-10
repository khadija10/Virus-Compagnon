
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_LOG_ENTRIES 1000

// Fonction pour générer un nombre aléatoire entre min et max
int randomInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

int main() {
    FILE *file = fopen("server_logs.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de la création du fichier de logs.\n");
        exit(EXIT_FAILURE);
    }

    // Initialiser le générateur de nombres aléatoires avec une graine basée sur l'heure actuelle
    srand(time(NULL));

    // Générer des données de logs simulées et les écrire dans le fichier
    for (int i = 0; i < NUM_LOG_ENTRIES; i++) {
        fprintf(file, "192.168.%d.%d\n", randomInRange(0, 255), randomInRange(0, 255));
    }

    fclose(file);
    printf("Fichier de logs généré avec succès.\n");

    return 0;
}

