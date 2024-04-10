#include <stdio.h>
#include <stdlib.h>

// Fonction pour calculer le PGCD (Algorithme d'Euclide)
int pgcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Fonction pour calculer le PPCM
int ppcm(int a, int b) {
    return (a * b) / pgcd(a, b);
}

int main() {
    // Entrée des nombres
    int num1, num2;
    printf("Entrez le premier nombre : ");
    scanf("%d", &num1);
    printf("Entrez le deuxième nombre : ");
    scanf("%d", &num2);

    // Calcul du PGCD et du PPCM
    int gcd = pgcd(num1, num2);
    int lcm = ppcm(num1, num2);

    // Création du fichier de données pour Gnuplot
    FILE *dataFile = fopen("pgcd_ppcm.dat", "w");
    if (dataFile == NULL) {
        printf("Erreur lors de la création du fichier de données.\n");
        return 1;
    }

    // Écriture des données dans le fichier
    fprintf(dataFile, "PPCM\t%d\nPGCD\t%d", lcm, gcd);

    // Fermeture du fichier de données
    fclose(dataFile);

    // Création du script Gnuplot pour le graphique
    FILE *gnuplotScript = fopen("pgcd_ppcm_plot.gnu", "w");
    if (gnuplotScript == NULL) {
        printf("Erreur lors de la création du script Gnuplot.\n");
        return 1;
    }

    // Écriture des commandes Gnuplot dans le script
    fprintf(gnuplotScript, "set style data histograms\n");
    fprintf(gnuplotScript, "set style fill solid\n");
    fprintf(gnuplotScript, "set boxwidth 0.5 relative\n");
    fprintf(gnuplotScript, "plot 'pgcd_ppcm.dat' using 2:xtic(1) lc rgb 'blue' title 'PPCM = %d et PGCD = %d', '' using 0:1:2 with labels offset -0.5,-1 rotate by 90 tc lt 1\n", lcm, gcd);

    // Fermeture du fichier de script Gnuplot
    fclose(gnuplotScript);

    // Appel de Gnuplot pour afficher le graphique
    system("gnuplot -persist pgcd_ppcm_plot.gnu");

    return 0;
}
