#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FILE_NAME_R "Dpile.txt"             // fichier original
#define FILE_NAME_W1 "nocomment_pile.txt"   // fichier sans commentaire
#define FILE_NAME_W2 "lexeme.txt"           // fichier sans commentaire sans caracteres inutiles
#define MAX_LINE 1024                       // limite de la maximum longueur de ligne
#define MAX_ETAT 20                         // limite de la maximum longueur de l'etat

typedef enum   // etats pour supprimer les commentaires
{
    STATE0 = 0,
    STATE1,
    STATE2,
    STATE3,
    STATE4,
}STATE;

int supprimer_commentaire(char* File_name1, char *File_name2)   // supprimer les commentaires
{
    FILE *pFile_r = fopen(File_name1, "r");      // fichier qu'on veut traiter
    FILE *pFile_w = fopen(File_name2, "w");    // fichier qu'on va ecrire
    STATE current_state = STATE0;   // etat initial
    char ch;
    char regis = 0;     // enregistrer le caractere precedent pour le cas speical

    // Verification de l'ouverture des fichiers
    if( pFile_r == NULL ){
        printf("Can't open the file %s!\n", File_name1);
        getchar();
		exit(1);
    }

    if( pFile_w == NULL ){
        printf("Can't create the file %s!\n", File_name2);
        getchar();
		exit(1);
    }
    // !feof(pFile_r)
    // appliquer une automate pour supprimer les commentaires
    while( (ch = fgetc(pFile_r)) != EOF ){
        switch(current_state){
            case STATE0:
                    if ( ch == '/' ){
                        regis = ch;
                        current_state = STATE1;
                    }
                    else if ( ch == '*' )
                        fputc(ch, pFile_w);
                    else 
                        fputc(ch, pFile_w);
                    break;
            case STATE1:
                    if ( ch == '/' ) 
                        current_state = STATE2;
                    else if ( ch == '*')
                        current_state = STATE3;
                    else{
                        current_state = STATE0;
                        fputc(regis, pFile_w);
                        fputc(ch, pFile_w);
                    }
                    break;
            case STATE2:
                    if ( ch == '\n' )
                        current_state = STATE0;
                    else
                        current_state = STATE2;
                    break;
            case STATE3:
                    if ( ch == '/' )
                        current_state = STATE3;
                    else if ( ch == '*' )
                        current_state = STATE4;
                    else
                        current_state = STATE3;
                    break;
            case STATE4:
                    if ( ch == '/' )
                        current_state = STATE0;
                    else if ( ch == '*')
                        current_state = STATE4;
                    else
                        current_state = STATE3;
                    break;
            default: 
                    current_state = STATE0;
                    break;   
        }
    }
    fclose(pFile_r);
    fclose(pFile_w);
    return 0;
}

int traiter_file(char *File_name1, char *File_name2)   // supprimer les caracteres inutiles
{   
    char ch2;
    char strLine[MAX_LINE] = {0};   // liste pour enregistrer une ligne de fichier
    FILE *traiter_pFile_w = fopen(File_name1, "r");  // le fichier qu'on veut traiter
    FILE* pFile_w2 = fopen(File_name2, "w");      // le fichier qu'on va ecrire

    if( traiter_pFile_w == NULL ){
        printf("Can't open the file %s!\n", File_name1);
        getchar();
		exit(1);
    }    
    if( pFile_w2 == NULL ){
        printf("Can't open the file %s!\n", File_name2);
        getchar();
		exit(1);
    }

    //  !feof(traiter_pFile_w)
    // supprimer les blanches et les nouvelles lignes
    while ( (ch2 = fgetc(traiter_pFile_w)) != EOF ){
        if ( ch2 != ' ' && ch2 != '\t' && ch2 != '\n')
            fputc(ch2, pFile_w2);
    }
    fclose(pFile_w2);
    fclose(traiter_pFile_w);

    // montrer le fichier
    FILE *show_pFile_w = fopen(File_name2, "r"); 
    if( show_pFile_w == NULL ){
        printf("Can't open the file %s!\n", File_name2);
        getchar();
		exit(1);
    }
    while( !feof(show_pFile_w) ){
        fgets(strLine, MAX_LINE, show_pFile_w);
        printf("%s", strLine);
    }
    fclose(show_pFile_w);
    printf("\n");
    return 0;
}

int examiner_caracteres(char* File_name) // verifier que les mots sont dans le lexeme qu'on cree
{
    char number[10] = {0};          // 0-9
    char min_alphabet[26] = {0};    // a-z
    char maj_alphabet[26] = {0};    // A-Z
    char common_symbol[] = {'(', ')', '{', '}', '[', ']', '=', '`', '"', ',', ' ', '\t', '\n'}; // caracteres normales dans les fichiers
    char special_symbol[] ={"零一二三四五六七八九十→"}; // caracteres speciaux dans les fichiers
    char special_matrix[13][4];     // matrice pour enregistrer les caracteres speciaux et chaque caractere a 3 chars
    char zero = '0';   
    char a = 'a';
    char A = 'A';
     
    int i, j, k;        // iterations
    char ch3, ch4, ch5; // ch4 et ch5 pour les caracteres speciaux
    char str[10] = {0}; // pour enregistrer les caracteres chinois
    int flag1 = 0;      // drapeau pour verifier si le caractere est dans les premiers quatre listes
    int flag2 = 0;      // drapeau pour verifier si le premier char est dans la matrice des caracteres speciaux
    int flag3 = 0;      // drapeau pour verifier si le caractere est dans la matrice des caracteres speciaux
    int flag_lexical = 0; // drapeau pour verifier l'analyse lexical et 0 correct, 1 faux

    // creer le lexeme
    for ( k = 0; k < 10; k++ ){
        number[k] = zero + k;
    }
    for ( i = 0; i < 26; i++ ){
        min_alphabet[i] = (char) (a + i);
    }
    for ( j = 0; j < 26; j++ ){
        maj_alphabet[j] = (char) (A + j);
    }

    j = 0;
    for ( i = 0; i < 12; i++ ){
        for ( k = 0; k < 3; k++ ){
            special_matrix[i][k] = special_symbol[k+j];
        }
        j = j + 3;
    }
    
    // faire le jugement du caractere
    FILE *pFile_r2 = fopen(File_name, "r");
    if( pFile_r2 == NULL ){
        printf("Can't open the file %s!\n", File_name);
        getchar();
		exit(1);
    }

    while( (ch3 = fgetc(pFile_r2)) != EOF ){
        flag1 = 0;
        flag2 = 0;
        flag3 = 0;
        for( int i = 0; i < 10; i++ ){
            if ( ch3 == number[i] ){
                flag1 = 1;
                break;
            }
        }
        for ( int j = 0; j < 26; j++ ){
            if ( ch3 == min_alphabet[j] || ch3 == maj_alphabet[j] ){
                flag1 = 1;
                break;
            }
        }
        for ( int k = 0; k < 13; k++ ){
            if ( ch3 == common_symbol[k] ){
                flag1 = 1;
                break;
            }
        }
        if ( flag1 == 0 ){
            for ( int i = 0; i < 12; i++ ){
                if ( ch3 == special_matrix[i][0] ){
                    flag2 = 1;
                    break;
                }
            }
            if ( flag2 == 0 ){
                printf("Caractere inconnu: %c\n", ch3);
                flag_lexical = 1;
            }
            else{
                ch4 = fgetc(pFile_r2);
                ch5 = fgetc(pFile_r2);
                str[0] = ch3;
                str[1] = ch4;
                str[2] = ch5;
                for ( int j = 0; j < 12; j++ ){
                    if ( str[0] == special_matrix[j][0] && str[1] == special_matrix[j][1] && str[2] == special_matrix[j][2]){
                        flag3 = 1;
                        break;
                    }
                }
                if ( flag3 == 0 ){
                    printf("Caractere inconnu: %s\n", str);
                    flag_lexical = 1;
                }
            }
        }
    }
    fclose(pFile_r2);
    
    // faire le jugement pour les caracteres invalides
    if ( flag_lexical == 1 )
        printf("Le fichier est lexicalement faux! Type: Caractere invalide.\n");

    return flag_lexical;    // premier drapeau pour lexical
}

int examiner_mots(char* File_name)  //  verifier si les mot cles sont corrects
{   
    char mot1[] = "Automate";
    char mot2[] = "etats";
    char mot3[] = "initial";
    char mot4[] = "final";
    char mot5[] = "transitions";  // les mots cles corrects

    char ch6;
    char expr[20][MAX_LINE] = {0};    // matrice pour enregistrer les expressions des lettres qu'on obtient
    int curr_line = 0;      // ligne actuelle de la matrice
    int curr_col = 0;       // colonne actuelle de la matrice
    int len_line = -1;      // nombre total de ligne
    int len_col = -1;       // nombre total de colonne de chaque ligne 
    int flag_pre = 0;       // drapeau du caractère précédent
    int flag_curr = 0;      // drapeau du caractère actuel
    int flag_match = -1;    // drapeau pour verifier le mot cle
    int flag_lexical = 0;   // drapeau pour verifier l'analyse lexical et 0 correct, 1 faux
    int flag_guillemet = 1; // drapeau pour verifier que les guillemets correspondent et 0 oui, 1 non

    FILE *pFile_r3 = fopen(File_name, "r");
    if( pFile_r3 == NULL ){
        printf("Can't open the file %s!\n", File_name);
        getchar();
		exit(1);
    }

    while( (ch6 = fgetc(pFile_r3)) != EOF ){
        flag_pre = flag_curr;

        // trouver les lettres
        if (  (ch6 >= 'a') && (ch6 <= 'z') ){
            flag_curr = 1;
        }
        else if ( (ch6 >= 'A') && (ch6 <= 'Z') ){
            flag_curr = 1;
        }
        else{
            flag_curr = 0;
        }

        // eviter les cas speciaux c.s. omettre les mots dans le crochet d'etats 
        /*
        if ( ch6 == '"' ){
            ch6 = fgetc(pFile_r3);
            while( ch6 != '"'){
                ch6 = fgetc(pFile_r3);
            }
        }
        if ( ch6 == '`' ){
            ch6 = fgetc(pFile_r3);
            while( ch6 != '`'){
                ch6 = fgetc(pFile_r3);
            }
        }
        */

        // eviter les cas speciaux c.s. omettre les mots dans le crochet d'etats
        if ( ch6 == '"' ){
            ch6 = fgetc(pFile_r3);
            if( ch6 == '"'){
                flag_lexical = 1;
                printf("Vide definition de l'etat!\n");
                break;
            }
            else{
                for ( int i = 1; i < MAX_ETAT; i++ ){
                    if ( ch6 != '"')
                        ch6 = fgetc(pFile_r3);
                    else{
                        flag_guillemet = 0;
                        ch6 = fgetc(pFile_r3);
                        if ( ch6 == '"' ){
                            flag_guillemet = 1;
                        }
                        break;
                    }     
                }
            }
            if ( flag_guillemet == 1 ){
                flag_lexical = 1;
                printf("Les deux guillemets ne correspondent pas!\n");
                break;
            }
        }
        // `
        if ( ch6 == '`' ){
            ch6 = fgetc(pFile_r3);
            if( ch6 == '`'){
                flag_lexical = 1;
                printf("Vide definition de l'etat!\n");
                break;
            }
            else{
                for ( int i = 0; i < MAX_ETAT; i++ ){
                    if ( ch6 != '`')
                        ch6 = fgetc(pFile_r3);
                    else{
                        flag_guillemet = 0;
                        ch6 = fgetc(pFile_r3);
                        if ( ch6 == '`' ){
                            flag_guillemet = 1;
                        }                        
                        break;
                    }     
                }
            }
            if ( flag_guillemet == 1 ){
                flag_lexical = 1;
                printf("Les deux guillemets ne correspondent pas!\n");
                break;
            }
        }   

        // cas normals et enregistrer les expressions des lettres dans la matrice
        // ligne plus 1 lorsqu'on passe par une lettre à une non lettre 
        if ( flag_curr == 1 ){
            expr[curr_line][curr_col] = ch6;
            curr_col++;
        }
        else if ( flag_pre == 1 && flag_curr == 0 ){
            curr_line++;
            curr_col = 0;
        }
    }
    len_line = curr_line;

    // montrer les expressions qu'on trouve
    /*
    for ( int i = 0; i < len_line; i++ ){
        for ( int j = 0; j < MAX_LINE ; j++ ){
            printf("%c", expr[i][j]);
        }
    }
    */
    
    // comparer avec les mots cles corrects
    for ( int i = 0; i < len_line; i++ ){
        if ( expr[i][0] == 'A' ){
            len_col = strlen(expr[i]);
            if ( len_col != 8 ){
                flag_lexical = 1;
                printf("Mot clé inconnu: %s\n", expr[i]);
            }
            else{
                flag_match = strcmp(expr[i], mot1);
                if ( flag_match != 0 ){
                    flag_lexical = 1;
                    printf("Mot clé inconnu: %s\n", expr[i]);
                }
            }
        }
        else if ( expr[i][0] == 'e' ){
            len_col= strlen(expr[i]);
            if ( len_col != 5 ){
                flag_lexical = 1;
                printf("Mot clé inconnu: %s\n", expr[i]);
            }
            else{
                flag_match = strcmp(expr[i], mot2);
                if ( flag_match != 0 ){
                    flag_lexical = 1;
                    printf("Mot clé inconnu: %s\n", expr[i]);
                }
            }
        }
        else if ( expr[i][0] == 'i' ){
            len_col = strlen(expr[i]);
            if ( len_col != 7 ){
                flag_lexical = 1;
                printf("Mot clé inconnu: %s\n", expr[i]);
            }
            else{
                flag_match = strcmp(expr[i], mot3);
                if ( flag_match != 0 ){
                    flag_lexical = 1;
                    printf("Mot clé inconnu: %s\n", expr[i]);
                }
            }
        }

        else if ( expr[i][0] == 'f' ){
            len_col = strlen(expr[i]);
            if ( len_col != 5 ){
                flag_lexical = 1;
                printf("Mot clé inconnu: %s\n", expr[i]);
            }
            else{
                flag_match = strcmp(expr[i], mot4);
                if ( flag_match != 0 ){
                    flag_lexical = 1;
                    printf("Mot clé inconnu: %s\n", expr[i]);
                }
            }
        }
        else if ( expr[i][0] == 't' ){
            len_col = strlen(expr[i]);
            if ( len_col != 11 ){
                flag_lexical = 1;
                printf("Mot clé inconnu: %s\n", expr[i]);
            }
            else{
                flag_match = strcmp(expr[i], mot5);
                if ( flag_match != 0 ){
                    flag_lexical = 1;
                    printf("Mot clé inconnu: %s\n", expr[i]);
                }
            }
        }
        else{
            flag_lexical = 1;
            printf("Mot clé inconnu: %s\n", expr[i]);
        }
    }
    
    // faire le jugement pour les mots cles invalides
    if ( flag_lexical == 1 && flag_guillemet == 0 ){
        printf("Le fichier est lexicalement faux! Type: Mot cle invalide.\n");
    }
    if ( flag_lexical == 1 && flag_guillemet == 1 ){
        printf("Le fichier est lexicalement faux! Type: Probleme de guillemets.\n");
    }
    fclose(pFile_r3);

    printf("-------------------------------------------------------------------------------------------------------------------------\n");
    printf("L'analyse lexicale est finie!\n");

    return flag_lexical;    // second drapeau pour lexical
}
