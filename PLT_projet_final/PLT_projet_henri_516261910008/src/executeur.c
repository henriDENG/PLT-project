#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"analyseur_lexical.h"
#include"analyseur_syntaxique.h"
#include"analyseur_semantique.h"
#include"compile_automate.h"
#include"executeur.h"

#define FILE_NAME_R "Dpile.txt"             // fichier original
#define FILE_NAME_W1 "nocomment_pile.txt"   // fichier sans commentaire
#define FILE_NAME_W2 "lexeme.txt"           // fichier sans commentaire sans caracteres inutiles
#define FILE_NAME_W3 "arbre_syntaxique.txt" // fichier pour dessiner l'arbre syntaxique
#define FILE_NAME_W4 "VM.txt"               // fichier pour enregistrer la machine virtuelle
#define FILE_NAME_W5 "tableau_symbole.txt"  // fichier pour enregistrer les addresses et les noms des etats
#define MAX_LINE 1024                       // limite de la maximum longueur de ligne
#define MAX_ETAT 20                         // limite de la maximum longueur de l'etat

int main(int argc, char *argv[])    // argc nombre de string, argv liste de string
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
    int VM[100] = {0};              // machine virtuelle
    int length_VM;                  // longueur de VM
    int symbole[100][2] = {0};      // addresse des etats:  0 13 | 1 18 | 2 23 | 3 6

    int flag_lexical_1 = -1;
    int flag_lexical_2 = -1;
    int flag_syntaxique = -1;
    int flag_semantique = -1;
    
    // lexical
    char Input_file[100] = {0};
    printf("Donner le nom de fichier d'entree : ");
    scanf("%s", Input_file);
    char *tmp = ".txt";
    strcat(Input_file, tmp);
    supprimer_commentaire(Input_file, FILE_NAME_W1);
    traiter_file(FILE_NAME_W1, FILE_NAME_W2);
    flag_lexical_1 = examiner_caracteres(FILE_NAME_W2);
    flag_lexical_2 = examiner_mots(FILE_NAME_W2);

    if ( flag_lexical_1 == 1 | flag_lexical_2 == 1 ){
        printf("Compilation erreur! Type: Faute lexicale.\n");
        return 0;
    }

    // syntaxique
    flag_syntaxique = examiner_syntaxique(FILE_NAME_W2, FILE_NAME_W3);
    if ( flag_syntaxique == 1 ){
        printf("Compilation erreur! Type: Faute syntaxique.\n");
        return 0;
    }

    // semantique
    flag_semantique = examiner_arbre_syntaxique(FILE_NAME_W3, etats, &initial, final, transitions, caractere, pile1, pile2, &num_etats, &num_final, &num_transitions, &num_pile);
    if ( flag_semantique == 1 ){
        printf("Compilation erreur! Type: Faute semantique.\n");
        return 0;
    }

    // compilation
    compilateur(etats, &initial, final, transitions, caractere, pile1, pile2, &num_etats, &num_final, &num_transitions, &num_pile, VM, &length_VM, symbole);
    enregistrer_VM(FILE_NAME_W4, FILE_NAME_W5, VM, length_VM, symbole, etats, num_etats); 

    // execution
    char *FILE_NAME = argv[argc-1];
    char *str = ".txt";
    strcat(FILE_NAME, str);
    if( ((strcmp(argv[1],"-debug")) == 0) && (argc == 3) ){
        execution_debug(FILE_NAME, symbole, etats, num_etats);
    }
    else if( argc == 2 ){
        execution_normale(FILE_NAME, symbole, etats, num_etats);
    }
    else{
        printf("Erreur de Execution!\n");
        return -1;
    }
	return 0;
}