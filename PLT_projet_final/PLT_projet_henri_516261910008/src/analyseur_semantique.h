#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FILE_NAME_R "Dpile.txt"             // fichier original
#define FILE_NAME_W1 "nocomment_pile.txt"   // fichier sans commentaire
#define FILE_NAME_W2 "lexeme.txt"           // fichier sans commentaire sans caracteres inutiles
#define FILE_NAME_W3 "arbre_syntaxique.txt" // fichier pour dessiner l'arbre syntaxique
#define MAX_LINE 1024                       // limite de la maximum longueur de ligne
#define MAX_ETAT 20                         // limite de la maximum longueur de l'etat

int puissance_dix(int n)
{   
    int temp = 1;
    if ( n == 0 )
        return 1;
    else if ( n < 0 )
        return -1;
    else{
        for ( int i = 0; i < n; i++ ){
            temp = 10 * temp;
        }
        return temp;
    }
}

void afficher_arbre_syntaxique(char etats[][10], int *initial, int *final, int transitions[][2], char *caractere, char pile1[][2], char pile2[][2], int *num_etats, int *num_final, int *num_transitions, int *num_pile )
{
    printf("nombre de pile: %d\n", *num_pile);
    printf("nombre d'etats: %d\n", *num_etats);
    printf("nombre de final: %d\n", *num_final);
    printf("nombre de transitions: %d\n", *num_transitions);
    printf("etats: ");
    for ( int i = 0; i < *num_etats; i++ ){
        for ( int j = 0; j < strlen(etats[i]); j++ ){
            printf("%c", etats[i][j]);
        }
        printf(" ");
    }
    printf("\n");
    printf("initial: %d\n", *initial);
    printf("final: ");
    for ( int i = 0; i < *num_final; i++ ){
        printf("%d ", final[i]);
    }
    printf("\n");
    printf("transitions: ");
    for ( int i = 0; i < *num_transitions; i++ ){
        for ( int j = 0; j < 2; j++ ){
            printf("%d ", transitions[i][j]);
        }
        printf("| ");
    }
    printf("\n");
    printf("caractere: ");
    for ( int k = 0; k < *num_transitions; k++ ){
        printf("%c | ", caractere[k]);
    }
    printf("\n");
    printf("pile1: ");
    for ( int i = 0; i < *num_transitions; i++ ){
        for ( int j = 0; j < 2; j++ ){
            printf("%c ", pile1[i][j]);
        }
        printf("| ");
    } 
    printf("\n");  
    printf("pile2: ");
    for ( int i = 0; i < *num_transitions; i++ ){
        for ( int j = 0; j < 2; j++ ){
            printf("%c ", pile2[i][j]);
        }
        printf("| ");
    } 
    printf("\n");
}

int examiner_arbre_syntaxique(char *File_name, char etats[][10], int *initial, int *final, int transitions[][2], char *caractere, char pile1[][2], char pile2[][2], int *num_etats, int *num_final, int *num_transitions, int *num_pile)   // enregistrer l'arbre syntaxique pour l'utiliser
{   
    char strLine[MAX_LINE] = {0};   // liste pour enregistrer une ligne de fichier
    FILE *traiter_pFile_w3 = fopen(File_name, "r"); // le fichier qu'on veut traiter
    if( traiter_pFile_w3 == NULL ) {
        printf("Can't open the file %s!\n", File_name);
        getchar();
        exit(1);
    }
    int len = -1;           // longueur de la ligne actuelle
    char ch;
    int j_etats = 0;        // position actuelle du caractere de l'etat, pour enregister la longueur de chaque etat
    int j_initial = 0;      // position actuelle du caractere de l'initial, pour enregister la longueur d'initial
    int j_final = 0;        // position actuelle du caractere du final, pour enregister la longueur de chaque final
    int j_transitions = 0;  // numero actuel de transitions, utilise pour les transitions et les piles 
    int curr_pos = 0;       // position actuelle du a la ligne
    int num_parenthese = 0; // nombre de parenthese a droite, pour verifier le nombre de pile
    char temp[100] = {0};   // liste temporaire pour enregistrer les variables temporaires
    int flag_semantique = 0;    // 0 vrai, 1 faux
    int flag_repeter = 1;   // 0 non, 1 oui
    int flag_transitions = 0;   // 0 vrai, 1 faux

    while( !feof(traiter_pFile_w3) ){
        fgets(strLine, MAX_LINE, traiter_pFile_w3);
        len = strlen(strLine);

        if ( strLine[0] == 'A' ){
            for ( int i = 0; i < len; i++ ){
                ch = strLine[i];
                if ( ch >= '0' && ch <= '2' ){
                    *num_pile = (int) (ch - '0');
                    break;
                }
            }
        }
        else if ( strLine[0] == 'e' ){
            for ( int i = 10; i < len; i++ ){
                ch = strLine[i];
                if ( ch != '"' ){
                    etats[(*num_etats)][j_etats] = ch;
                    j_etats++;
                }
                else if ( ch == '"')
                    break;
            }
            (*num_etats)++;
            j_etats = 0;
        }
        else if ( strLine[0] == 'i' ){
            for ( int i = 0; i < len; i++ ){
                ch = strLine[i];
                if ( ch >= '0' && ch <= '9' ){
                    temp[j_initial] = ch;
                    j_initial++;
                }
            }
            for ( int j = 0; j < j_initial; j++ ){
                (*initial) += puissance_dix(j_initial-1-j) * (temp[j]-'0');     // transferer char a nombre
            }
            j_initial = 0; 
            memset(temp, 0, sizeof(temp));           
        }
        else if ( strLine[0] == 'f' ){
            for ( int i = 0; i < len; i++ ){
                ch = strLine[i];
                if ( ch >= '0' && ch <= '9' ){
                    temp[j_final] = ch;
                    j_final++;
                }
            }
            for ( int j = 0; j < j_final; j++ ){
                final[(*num_final)] += puissance_dix(j_final-1-j) * (temp[j]-'0');     // transferer char a nombre
            }
            (*num_final)++;
            j_final = 0;
            memset(temp, 0, sizeof(temp));            
        }
        else if( strLine[0] == 't' ){
            if ( (*num_pile) == 0 ){
                for ( int i = 0; i < len; i++ ){
                    if ( strLine[i] == '(' ){
                        num_parenthese++;
                        curr_pos = i + 1;
                        ch = strLine[curr_pos];
                        while( ch >= '0' && ch <= '9' ){
                            temp[j_transitions] = ch;
                            j_transitions++;
                            curr_pos++;
                            ch = strLine[curr_pos];
                        }
                        for ( int j = 0; j < j_transitions; j++ ){
                            transitions[(*num_transitions)][0] += puissance_dix(j_transitions-1-j) * (temp[j]-'0');     // transferer char a nombre
                        }
                        j_transitions = 0;
                        memset(temp, 0, sizeof(temp)); 
                    }
                    else if ( (int) (strLine[i]) == -110 ){
                        curr_pos = i + 1;
                        ch = strLine[curr_pos];
                        while( ch >= '0' && ch <= '9' ){
                            temp[j_transitions] = ch;
                            j_transitions++;
                            curr_pos++;
                            ch = strLine[curr_pos];
                        }
                        for ( int j = 0; j < j_transitions; j++ ){
                            transitions[(*num_transitions)][1] += puissance_dix(j_transitions-1-j) * (temp[j]-'0'); 
                        }
                        j_transitions = 0;
                        memset(temp, 0, sizeof(temp)); 
                    }
                    else if ( strLine[i] == '`' ){
                        curr_pos = i + 1;
                        ch = strLine[curr_pos];
                        if ( (ch >= 'a' && ch <= 'z') || ( ch >= '0' && ch <= '9') ){
                            caractere[(*num_transitions)] = strLine[curr_pos];
                        }    
                    }
                }
                // verifier si le nombre de pile depasse
                if ( num_parenthese > (*num_pile) + 1 ){
                    flag_semantique = 1;
                    printf("Le fichier est semantiquement faux! Type: Nombre de piles exces.\n");
                }
            }
            else{
                for ( int i = 0; i < len; i++ ){
                    if ( strLine[i] == '(' ){
                        num_parenthese++;
                        curr_pos = i + 1;
                        ch = strLine[curr_pos];
                        if ( ch >= '0' && ch <= '9' ){
                            while( ch >= '0' && ch <= '9' ){
                                temp[j_transitions] = ch;
                                j_transitions++;
                                curr_pos++;
                                ch = strLine[curr_pos];
                            }
                            for ( int j = 0; j < j_transitions; j++ ){
                                transitions[(*num_transitions)][0] += puissance_dix(j_transitions-1-j) * (temp[j]-'0');     // transferer char a nombre
                            }
                            j_transitions = 0;
                            memset(temp, 0, sizeof(temp));                            
                        }
                        else if ( (int) (ch) == -30 ){
                            if ( num_parenthese == 2 ){
                                pile1[(*num_transitions)][0] = '+';
                                ch = strLine[i+6];
                                pile1[(*num_transitions)][1] = ch;
                            }
                            else if ( num_parenthese == 3 ){
                                pile2[(*num_transitions)][0] = '+';
                                ch = strLine[i+6];
                                pile2[(*num_transitions)][1] = ch;                                
                            }

                        }
                        else if ( ch == '`' ){
                            if ( num_parenthese == 2 ){
                                pile1[(*num_transitions)][0] = '-';
                                ch = strLine[i+2];
                                pile1[(*num_transitions)][1] = ch; 
                            }
                            else if ( num_parenthese == 3 ){
                                pile2[(*num_transitions)][0] = '-';
                                ch = strLine[i+2];
                                pile2[(*num_transitions)][1] = ch;                                 
                            }
                        }                   
                    }
                    else if ( (int) (strLine[i]) == -110 ){
                        curr_pos = i + 1;
                        ch = strLine[curr_pos];
                        if ( ch >= '0' && ch <= '9' ){
                            while( ch >= '0' && ch <= '9' ){
                                temp[j_transitions] = ch;
                                j_transitions++;
                                curr_pos++;
                                ch = strLine[curr_pos];
                            }
                            for ( int j = 0; j < j_transitions; j++ ){
                                transitions[(*num_transitions)][1] += puissance_dix(j_transitions-1-j) * (temp[j]-'0'); 
                            }
                            j_transitions = 0;
                            memset(temp, 0, sizeof(temp)); 
                        }
                    }
                    else if ( strLine[i] == '`' && ( strLine[i+4] == '(' || strLine[i+4] == '"') ){
                        curr_pos = i + 1;
                        ch = strLine[curr_pos];
                        if ( (ch >= 'a' && ch <= 'z') || ( ch >= '0' && ch <= '9') ){
                            caractere[(*num_transitions)] = strLine[curr_pos];
                        }    
                    }
                }
                // printf("nombre de parenthese: %d\n", num_parenthese);
                // verifier si le nombre de pile depasse
                if ( num_parenthese > (*num_pile) + 1 ){
                    flag_semantique = 1;
                    printf("Le fichier est semantiquement faux! Type: Nombre de piles exces.\n");
                }
            } 
            num_parenthese = 0;                        
            (*num_transitions)++;
        }
        // printf("length of current line: %ld\n", strlen(strLine));
    }
    fclose(traiter_pFile_w3);

    // verifier si'l y a des repetitions de nom de l'etat
    for( int i = 1; i < *num_etats; i++ ){
        for ( int k = 0; k < i; k++ ){
            flag_repeter = 1;
            if ( strlen(etats[i]) != strlen(etats[k]) ){
                flag_repeter = 0;
            }
            else{
                for ( int j = 0; j < strlen(etats[i]); j++ ){
                    if ( etats[i][j] != etats[k][j] ){
                        flag_repeter = 0;
                        break;
                    }  
                }
                if ( flag_repeter == 1 )
                    break;
            }
        }
        if ( flag_repeter == 1 ){
            flag_semantique = 1;
            printf("Le fichier est semantiquement faux! Type: Redefinition de l'etat.\n");
            break;
        }
    }

    // verifier si le numero d'initial est bon
    if ( (*initial) >= (*num_etats) ){
        flag_semantique = 1;
        printf("Le fichier est semantiquement faux! Type: Numero d'initial exces.\n");
    }

    // verifier si les numeros de final sont bon
    for ( int i = 0; i < *num_final; i++ ){
        if ( final[i] >= (*num_etats) ){
            flag_semantique = 1;
            printf("Le fichier est semantiquement faux! Type: Numero de final exces.\n");
            break;
        }
    }

    // verifier si les numeros des etats dans les transitions sont bon
    for ( int i = 0; i < *num_transitions; i++ ){
        for ( int j = 0; j < 2; j++ ){
            if ( transitions[i][j] >= (*num_etats) ){
                flag_semantique = 1;
                flag_transitions = 1;
                printf("Le fichier est semantiquement faux! Type: Numero de l'etat exces dans les transitions.\n");
                break;
            }
        }
        if ( flag_transitions == 1 )
            break;
    }
    if ( flag_semantique == 0 )
        afficher_arbre_syntaxique(etats, initial, final, transitions, caractere, pile1, pile2, num_etats, num_final, num_transitions, num_pile);

    printf("-------------------------------------------------------------------------------------------------------------------------\n");
    printf("L'analyse semantique est finie!\n");

    return flag_semantique;
}
