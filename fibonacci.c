#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, first = 0, second = 1, next, i;
    printf("Entrez le nombre de termes de la suite de Fibonacci: ");
    scanf("%d", &n);
    printf("Les %d premiers termes de la suite de Fibonacci sont: ", n);

    // Création du fichier de données pour Gnuplot
    FILE *dataFile = fopen("fibonacci.txt", "w");
    if (dataFile == NULL) {
        printf("Erreur lors de la création du fichier de données.\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        if (i <= 1)
            next = i;
        else {
            next = first + second;
            first = second;
            second = next;
        }
        printf("%d ", next);

        // Écriture des données dans le fichier
        fprintf(dataFile, "%d %d\n", i, next);
    }
    printf("\n");

    // Fermeture du fichier de données
    fclose(dataFile);

    // Création du script Gnuplot pour le graphique
    FILE *gnuplotScript = fopen("fibonacci_plot.gnu", "w");
    if (gnuplotScript == NULL) {
        printf("Erreur lors de la création du script Gnuplot.\n");
        return 1;
    }

    // Écriture des commandes Gnuplot dans le script
    fprintf(gnuplotScript, "plot 'fibonacci.txt' with linespoints title 'Suite de Fibonacci'\n");
    fprintf(gnuplotScript, "set xlabel 'N'\n");
    fprintf(gnuplotScript, "set ylabel 'Valeur'\n");

    // Fermeture du fichier de script Gnuplot
    fclose(gnuplotScript);

    // Appel de Gnuplot pour afficher le graphique
    system("gnuplot -persist fibonacci_plot.gnu");

    return 0;
}

