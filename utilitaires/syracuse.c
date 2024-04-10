#include <stdio.h>
#include <stdlib.h>

// Fonction pour calculer la suite de Syracuse
int syracuse(int n) {
    if (n % 2 == 0)
        return n / 2;
    else
        return 3 * n + 1;
}

int main() {
    int n, i, term;
    printf("Entrez le premier terme de la suite de Syracuse: ");
    scanf("%d", &n);
    printf("Entrez le nombre de termes à générer: ");
    scanf("%d", &term);

    // Création du fichier de données pour Gnuplot
    FILE *dataFile = fopen("syracuse.txt", "w");
    if (dataFile == NULL) {
        printf("Erreur lors de la création du fichier de données.\n");
        return 1;
    }

    // Génération des termes de la suite de Syracuse et écriture dans le fichier
    for (i = 0; i < term; i++) {
        fprintf(dataFile, "%d %d\n", i, n);
        n = syracuse(n);
    }

    // Fermeture du fichier de données
    fclose(dataFile);

    // Création du script Gnuplot pour le graphique
    FILE *gnuplotScript = fopen("syracuse_plot.gnu", "w");
    if (gnuplotScript == NULL) {
        printf("Erreur lors de la création du script Gnuplot.\n");
        return 1;
    }

    // Écriture des commandes Gnuplot dans le script
    fprintf(gnuplotScript, "plot 'syracuse.txt' with linespoints title 'Suite de Syracuse'\n");
    fprintf(gnuplotScript, "set xlabel 'N'\n");
    fprintf(gnuplotScript, "set ylabel 'Valeur'\n");

    // Fermeture du fichier de script Gnuplot
    fclose(gnuplotScript);

    // Appel de Gnuplot pour afficher le graphique
    system("gnuplot -persist syracuse_plot.gnu");

    return 0;
}

