#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOG_ENTRIES 1000
#define MAX_IP_LENGTH 16

// Structure pour stocker les données extraites des logs
typedef struct {
    char ip[MAX_IP_LENGTH];
    int accessCount;
} LogEntry;

// Fonction pour lire les logs à partir d'un fichier
int readLogs(const char *filename, LogEntry *logs) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier de logs.\n");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    while (fscanf(file, "%s", logs[count].ip) != EOF && count < MAX_LOG_ENTRIES) {
        logs[count].accessCount++;
        count++;
    }

    fclose(file);
    return count;
}

// Fonction pour générer un fichier de données pour gnuplot
void generateDataFile(const char *filename, LogEntry *logs, int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de la création du fichier de données.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d\n", logs[i].ip, logs[i].accessCount);
    }

    fclose(file);
}

int main() {
    LogEntry logs[MAX_LOG_ENTRIES];
    int logCount = readLogs("server_logs.txt", logs);

    // Générer un fichier de données pour gnuplot
    generateDataFile("data.txt", logs, logCount);

    // Appeler gnuplot pour tracer le graphique
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe == NULL) {
        fprintf(stderr, "Erreur lors de l'exécution de gnuplot.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(gnuplotPipe, "set title 'Accès par IP'\n");
    fprintf(gnuplotPipe, "set xlabel 'Adresse IP'\n");
    fprintf(gnuplotPipe, "set ylabel 'Nombre d''accès'\n");
    fprintf(gnuplotPipe, "set style data histograms\n");
    fprintf(gnuplotPipe, "plot 'data.txt' using 2:xtic(1) title ''\n");

    pclose(gnuplotPipe);

    return 0;
}
