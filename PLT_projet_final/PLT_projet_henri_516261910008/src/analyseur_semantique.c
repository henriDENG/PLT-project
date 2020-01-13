#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"analyseur_lexical.h"
#include"analyseur_syntaxique.h"
#include"analyseur_semantique.h"

#define FILE_NAME_R "Dpile.txt"             // fichier original
#define FILE_NAME_W1 "nocomment_pile.txt"   // fichier sans commentaire
#define FILE_NAME_W2 "lexeme.txt"           // fichier sans commentaire sans caracteres inutiles
#define FILE_NAME_W3 "arbre_syntaxique.txt" // fichier pour dessiner l'arbre syntaxique
#define MAX_LINE 1024                       // limite de la maximum longueur de ligne
#define MAX_ETAT 20                         // limite de la maximum longueur de l'etat

int main(void)
{   
    char etats[10][10] = {0};       // etats: 1, aa, bb, 二三 
    int initial = 0;                // initial: 1
    int final[10] = {0};            // final: 3, 9, 16
    int transitions[10][2] = {0};   // transitions: 0 1 | 2 3 | 11 26 | 8 322 |
    char caractere[10] = {0};       // caractere: a | b | 1 | 9 |
    char pile1[10][2] = {0};        // pile1: + a | - a | + c | - d |
    char pile2[10][2] = {0};        // pile2: + d | - e | + a | - b |
    int num_etats = 0;              // nombre d'etat
    int num_final = 0;              // nombre de final
    int num_transitions = 0;        // nombre de transitions
    int num_pile = 0;               // nombre de piles

    // lexical
    char Input_file[100] = {0};
    printf("Donner le nom de fichier d'entree : ");
    scanf("%s", Input_file);
    char *str = ".txt";
    strcat(Input_file, str);
    supprimer_commentaire(Input_file, FILE_NAME_W1);
    traiter_file(FILE_NAME_W1, FILE_NAME_W2);
    examiner_caracteres(FILE_NAME_W2);
    examiner_mots(FILE_NAME_W2);

    // syntaxique
    examiner_syntaxique(FILE_NAME_W2, FILE_NAME_W3);

    // semantique
    examiner_arbre_syntaxique(FILE_NAME_W3, etats, &initial, final, transitions, caractere, pile1, pile2, &num_etats, &num_final, &num_transitions, &num_pile);

	return 0;
}