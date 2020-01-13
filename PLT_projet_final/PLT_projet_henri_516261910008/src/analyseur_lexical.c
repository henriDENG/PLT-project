#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"analyseur_lexical.h"

#define FILE_NAME_R "Dpile.txt"             // fichier original
#define FILE_NAME_W1 "nocomment_pile.txt"   // fichier sans commentaire
#define FILE_NAME_W2 "lexeme.txt"           // fichier sans commentaire sans caracteres inutiles
#define MAX_LINE 1024                       // limite de la maximum longueur de ligne
#define MAX_ETAT 20                         // limite de la maximum longueur de l'etat

int main(void)
{   
    char Input_file[100] = {0};
    printf("Donner le nom de fichier d'entree : ");
    scanf("%s", Input_file);
    char *str = ".txt";
    strcat(Input_file, str);
    supprimer_commentaire(Input_file, FILE_NAME_W1);
    traiter_file(FILE_NAME_W1, FILE_NAME_W2);
    examiner_caracteres(FILE_NAME_W2);
    examiner_mots(FILE_NAME_W2);
	return 0;
}