#include <stdio.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BAR_WIDTH 50

int main() {
    float num1, num2, result;
    printf("Entrez le premier nombre: ");
    scanf("%f", &num1);
    printf("Entrez le deuxième nombre: ");
    scanf("%f", &num2);
    result = num1 + num2;
    printf("La somme est: %f\n", result);

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("Graphique", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Erreur lors de la création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du rendu
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("Erreur lors de la création du rendu: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Effacer l'écran
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Dessiner le graphique
    SDL_Rect barRect = {SCREEN_WIDTH / 2 - BAR_WIDTH / 2, SCREEN_HEIGHT - result, BAR_WIDTH, result};
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &barRect);

    // Mettre à jour l'affichage
    SDL_RenderPresent(renderer);

    // Attendre un peu avant de quitter
    SDL_Delay(5000);

    // Nettoyer et quitter
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

