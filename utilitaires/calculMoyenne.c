#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, i;
    float sum = 0, num;
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    
    if (gnuplotPipe == NULL) {
        printf("Erreur lors de l'ouverture de Gnuplot.\n");
        return 1;
    }

    printf("Combien de nombres voulez-vous entrer ? ");
    scanf("%d", &n);
    
    fprintf(gnuplotPipe, "set term wxt title 'Histogramme des nombres entrés' persist\n");
    fprintf(gnuplotPipe, "set style data histograms\n");
    fprintf(gnuplotPipe, "set style fill solid\n");
    fprintf(gnuplotPipe, "set boxwidth 0.5 relative\n");

    for (i = 0; i < n; i++) {
        printf("Entrez le nombre %d: ", i + 1);
        scanf("%f", &num);
        sum += num;
        fprintf(gnuplotPipe, "%d %f\n", i + 1, num);
    }

    // Calcul de la moyenne
    float moyenne = sum / n;
    

    // Tracer la moyenne comme une ligne horizontale
    printf("La moyenne est: %.2f\n", moyenne);

    // Écriture des commandes Gnuplot dans le script
    fprintf(gnuplotPipe, "plot '-' using 2:xtic(1) lc rgb 'blue' title 'La moyenne = %.2f', '' using 0:1:2 with labels offset -0.5,-1 rotate by 90 tc lt 1\n", moyenne);
    fprintf(gnuplotPipe, "%d %f\n", n + 1, moyenne);
    fprintf(gnuplotPipe, "e\n");

    pclose(gnuplotPipe);
    
    return 0;
}
