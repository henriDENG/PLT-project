#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FILE_NAME_R "Dpile.txt"             // fichier original
#define FILE_NAME_W1 "nocomment_pile.txt"   // fichier sans commentaire
#define FILE_NAME_W2 "lexeme.txt"           // fichier sans commentaire sans caracteres inutiles
#define FILE_NAME_W3 "arbre_syntaxique.txt" // fichier pour dessiner l'arbre syntaxique
#define FILE_NAME_W4 "VM.txt"               // fichier pour enregistrer la machine virtuelle
#define FILE_NAME_W5 "tableau_symbole.txt"  // fichier pour enregistrer les addresses et les noms des etats
#define MAX_LINE 1024                       // limite de la maximum longueur de ligne
#define MAX_ETAT 20                         // limite de la maximum longueur de l'etat

void push(char *pile, int *idx_pile, char ch)   // push un element dans la pile
{   
    pile[(*idx_pile)] = ch;
    (*idx_pile)++;  // index pointe sur le toit de pile + 1, initial 0
}

void pop(char *pile, int *idx_pile)     // pop un element de la pile
{
    (*idx_pile)--;
    pile[(*idx_pile)] = 0;
}

int execution_normale(char *File_name, int symbole[][2], char etats[][10], int num_etats)   // mode normale de l'execution
{
    FILE *pFile_VM = fopen(File_name, "r");
    if( pFile_VM == NULL ){
        printf("Can't open the file %s!\n", File_name);
        getchar();
		exit(1);
    }
    int temp;                           // variable temporaire
    int VM[MAX_LINE] = {0};             // liste pour enregistrer VM
    int len_VM = 0;                     // longueur de VM

    int num_pile;                       // nombre de pile  
    int num_final;                      // nombre de final
    int addr_init;                      // addresse d'initial, addresse correspondante
    int addr_final[100] = {0};          // liste d'addresse des finals, addresse correspondante
    int num_transitions[100] = {0};     // nombre de transitions pour chaque etat, addresse correspondante
    int transitions[100][100] = {0};    // tableau des transitions pour tous les etats, addresse correspondante
    char nom_etats[100][100] = {0};     // tableau des noms d'etat, addresse correspondante

    char str[100];                      // input
    char ch;                            // element d'input
    int asc_ch;                         // ASCII code de input
    int curr_addr;                      // addresse actuelle
    int flag_ch = 0;                    // drapeau pour verifier si l'un caractere est accepte ou pas: 0 non, 1 oui
    int flag_mot = 0;                   // drapeau pour verifier si l'un mot est accepte ou pas: 0 non, 1 oui
    char pile1[100] = {0};              // pile1
    char pile2[100] = {0};              // pile2
    int idx_pile1 = 0;                  // index de pile1
    int idx_pile2 = 0;                  // index de pile2
    char ch_pile1;                      // element de pile1
    char ch_pile2;                      // element de pile2
    int flag_pile1 = 1;                 // drapeau pour verifier si pile1 est correctement fait, 0 non, 1 oui
    int flag_full_pile1 = 0;            // drapeau pour verifier si pile1 est vide a la fin, 0 non, 1 oui
    int flag_pile2 = 1;                 // drapeau pour verifier si pile2 est correctement fait, 0 non, 1 oui
    int flag_full_pile2 = 0;            // drapeau pour verifier si pile2 est vide a la fin, 0 non, 1 oui

    if( pFile_VM == NULL ) {
        printf("Can't open the file %s!\n", File_name);
        getchar();
        exit(1);
    }
    
    // enregistrer la VM dans une liste
    while( fscanf(pFile_VM, "%d", &temp) != EOF ){
        VM[len_VM] = temp;
        len_VM++;
    }

    // transferer les index aux numeros dans la VM
    num_pile = VM[0];
    addr_init = VM[1]; 
    num_final = VM[2];
    for ( int i = 0; i < num_final; i++ ){
        addr_final[i] = VM[i+3];
    }  
    for ( int j = 0; j < num_etats; j++ ){
        num_transitions[symbole[j][1]] = VM[symbole[j][1]];
    }
    for ( int i = 0; i < num_etats; i++ ){
        for ( int j = 0; j < strlen(etats[i]); j++ ){
            nom_etats[symbole[i][1]][j] = etats[i][j];
        }
    }
    
    // cas nombre de pile = 0
    if ( num_pile == 0 ){
 
        for ( int i = 0; i < num_etats; i++ ){
            for ( int j = 0; j < num_transitions[symbole[i][1]] * 2; j++ ){
                transitions[symbole[i][1]][j] = VM[symbole[i][1] + j + 1];
            }
        }
        printf("Donner le mot d'entree : ");
        scanf("%s", str);

        curr_addr = addr_init;        
        for( int i = 0; i < strlen(str); i++ ){
            ch = str[i];
            asc_ch = (int) ch;
            flag_ch = 0;
            flag_mot = 0;
            for ( int j = 0; j < num_transitions[curr_addr] * 2; j+=2 ){
                if ( asc_ch == transitions[curr_addr][j] ){
                    curr_addr = transitions[curr_addr][j+1];
                    flag_ch = 1;
                    break;
                }  
            }
            if ( flag_ch != 1 ){
                flag_mot = 0;
                break;
            }
        }
        for ( int j = 0; j < num_final; j++ ){
            if ( curr_addr == addr_final[j] ){
                flag_mot = 1;
                break;
            }
        }
        if ( flag_mot == 1 && flag_ch == 1 ){
            printf("Le mot %s est accepte!\n", str);
        }
        else{
            printf("Le mot %s est refuse!\n", str);
        }
        flag_ch = 0;
        flag_mot = 0;
    }

    // cas nombre de pile = 1
    else if ( num_pile == 1 ){

        for ( int i = 0; i < num_etats; i++ ){
            for ( int j = 0; j < num_transitions[symbole[i][1]] * 4; j++ ){
                transitions[symbole[i][1]][j] = VM[symbole[i][1] + j + 1];
            }
        }
   
        printf("Donner le mot d'entree : ");
        scanf("%s", str);

        curr_addr = addr_init;  
        for( int i = 0; i < strlen(str); i++ ){
            ch = str[i];
            asc_ch = (int) ch;
            flag_ch = 0;
            flag_mot = 0;
            flag_pile1 = 1;
            flag_full_pile1 = 0;

            for ( int j = 0; j < num_transitions[curr_addr] * 4; j+=4 ){

                if ( asc_ch == transitions[curr_addr][j] ){
                    flag_ch = 1;

                    ch_pile1 = transitions[curr_addr][j+2];                 
                    if ( transitions[curr_addr][j+3] == 1 ){
                        push(pile1, &idx_pile1, ch_pile1);
                    }
                    else if ( transitions[curr_addr][j+3] == -1 ){
                        if ( idx_pile1 != 0 ){
                            pop(pile1, &idx_pile1);
                        }
                        else{
                            flag_pile1 = 0;
                            break;                            
                        }
                    }
                    curr_addr = transitions[curr_addr][j+1];
                    break;
                }  
            }

            if ( flag_ch == 1 && flag_pile1 == 1){ }
            else if( flag_ch == 0 )
                break;
            else if ( flag_pile1 == 0 )
                break;                
        }

        for ( int j = 0; j < num_final; j++ ){
            if ( curr_addr == addr_final[j] ){
                flag_mot = 1;
                break;
            }
        }
        if ( idx_pile1 == 0 ){
            flag_full_pile1 = 1;
        }
        if ( flag_mot == 1 && flag_ch == 1 && flag_pile1 == 1 && flag_full_pile1 == 1 ){
            printf("Le mot %s est accepte!\n", str);
        }
        else{
            printf("Le mot %s est refuse!\n", str);
        }
        flag_ch = 0;
        flag_mot = 0;
        flag_pile1 = 1;
        flag_full_pile1 = 0;
    }

    // cas nombre de pile  = 2
    else if ( num_pile == 2 ){

        for ( int i = 0; i < num_etats; i++ ){
            for ( int j = 0; j < num_transitions[symbole[i][1]] * 6; j++ ){
                transitions[symbole[i][1]][j] = VM[symbole[i][1] + j + 1];
            }
        }
        printf("Donner le mot d'entree : ");
        scanf("%s", str);

        curr_addr = addr_init;
        for( int i = 0; i < strlen(str); i++ ){
            ch = str[i];
            asc_ch = (int) ch;
            flag_ch = 0;
            flag_mot = 0;
            flag_pile1 = 1;
            flag_full_pile1 = 0;
            flag_pile2 = 1;
            flag_full_pile2 = 0;

            for ( int j = 0; j < num_transitions[curr_addr] * 6; j+=6 ){ 

                if ( asc_ch == transitions[curr_addr][j] ){
                    flag_ch = 1;

                    ch_pile1 = transitions[curr_addr][j+2];                 
                    if ( transitions[curr_addr][j+3] == 1 ){
                        push(pile1, &idx_pile1, ch_pile1);
                    }
                    else if ( transitions[curr_addr][j+3] == -1 ){
                        if ( idx_pile1 != 0 ){
                            pop(pile1, &idx_pile1);
                        }
                        else{
                            flag_pile1 = 0;
                            break;
                        }
                    }

                    ch_pile2 = transitions[curr_addr][j+4];                 
                    if ( transitions[curr_addr][j+5] == 1 ){
                        push(pile2, &idx_pile2, ch_pile2);
                    }
                    else if ( transitions[curr_addr][j+5] == -1 ){
                        if ( idx_pile2 != 0 ){
                            pop(pile2, &idx_pile2);
                        }
                        else{
                            flag_pile2 = 0;
                            break;
                        }
                    }
                    curr_addr = transitions[curr_addr][j+1];
                    break;
                }  
            }

            if ( flag_ch == 1 && flag_pile1 == 1 && flag_pile2 == 1 ){ }
            else if( flag_ch == 0 )
                break;
            else if ( flag_pile1 == 0 )
                break;                
            else if ( flag_pile2 == 0 )
                break;                
        }

        for ( int j = 0; j < num_final; j++ ){
            if ( curr_addr == addr_final[j] ){
                flag_mot = 1;
                break;
            }
        }
        if ( idx_pile1 == 0 ){
            flag_full_pile1 = 1;
        }
        if ( idx_pile2 == 0 ){
            flag_full_pile2 = 1;
        }        
        if ( flag_mot == 1 && flag_ch == 1 && flag_pile1 == 1 && flag_full_pile1 == 1 && flag_pile2 == 1 && flag_full_pile2 == 1 ){
            printf("Le mot %s est accepte!\n", str);
        }
        
        else{
            printf("Le mot %s est refuse!\n", str);
        }
        flag_ch = 0;
        flag_mot = 0;
        flag_pile1 = 1;
        flag_full_pile1 = 0;
        flag_pile2 = 1;
        flag_full_pile2 = 0;
    }    
    fclose(pFile_VM);
}

int execution_debug(char *File_name, int symbole[][2], char etats[][10], int num_etats)     // mode debug de l'execution 
{
    FILE *pFile_VM = fopen(File_name, "r");
    if( pFile_VM == NULL ){
        printf("Can't open the file %s!\n", File_name);
        getchar();
		exit(1);
    }
    int temp;                           // variable temporaire
    int VM[MAX_LINE] = {0};             // liste pour enregistrer VM
    int len_VM = 0;                     // longueur de VM

    int num_pile;                       // nombre de pile  
    int num_final;                      // nombre de final
    int addr_init;                      // addresse d'initial, addresse correspondante
    int addr_final[100] = {0};          // liste d'addresse des finals, addresse correspondante
    int num_transitions[100] = {0};     // nombre de transitions pour chaque etat, addresse correspondante
    int transitions[100][100] = {0};    // tableau des transitions pour tous les etats, addresse correspondante
    char nom_etats[100][100] = {0};     // tableau des noms d'etat, addresse correspondante

    char str[100];                      // input
    char ch;                            // element d'input
    int asc_ch;                         // ASCII code de input
    int curr_addr;                      // addresse actuelle
    int flag_ch = 0;                    // drapeau pour verifier si l'un caractere est accepte ou pas: 0 non, 1 oui
    int flag_mot = 0;                   // drapeau pour verifier si l'un mot est accepte ou pas: 0 non, 1 oui
    char pile1[100] = {0};              // pile1
    char pile2[100] = {0};              // pile2
    int idx_pile1 = 0;                  // index de pile1
    int idx_pile2 = 0;                  // index de pile2
    char ch_pile1;                      // element de pile1
    char ch_pile2;                      // element de pile2
    int flag_pile1 = 1;                 // drapeau pour verifier si pile1 est correctement fait, 0 non, 1 oui
    int flag_full_pile1 = 0;            // drapeau pour verifier si pile1 est vide a la fin, 0 non, 1 oui
    int flag_pile2 = 1;                 // drapeau pour verifier si pile2 est correctement fait, 0 non, 1 oui
    int flag_full_pile2 = 0;            // drapeau pour verifier si pile2 est vide a la fin, 0 non, 1 oui

    if( pFile_VM == NULL ) {
        printf("Can't open the file %s!\n", File_name);
        getchar();
        exit(1);
    }
    
    while( fscanf(pFile_VM, "%d", &temp) != EOF ){
        VM[len_VM] = temp;
        len_VM++;
    }

    num_pile = VM[0];
    addr_init = VM[1]; 
    num_final = VM[2];
    for ( int i = 0; i < num_final; i++ ){
        addr_final[i] = VM[i+3];
    }  
    for ( int j = 0; j < num_etats; j++ ){
        num_transitions[symbole[j][1]] = VM[symbole[j][1]];
    }
    for ( int i = 0; i < num_etats; i++ ){
        for ( int j = 0; j < strlen(etats[i]); j++ ){
            nom_etats[symbole[i][1]][j] = etats[i][j];
        }
    }
    
    // cas nombre de pile = 0
    if ( num_pile == 0 ){
 
        for ( int i = 0; i < num_etats; i++ ){
            for ( int j = 0; j < num_transitions[symbole[i][1]] * 2; j++ ){
                transitions[symbole[i][1]][j] = VM[symbole[i][1] + j + 1];
            }
        }

        printf("Donner le mot d'entree : ");
        scanf("%s", str);

        curr_addr = addr_init;
        printf("  -> Etat : ");
        for( int i = 0; i < strlen(nom_etats[curr_addr]); i++ ){
            printf("%c", nom_etats[curr_addr][i]);
        }
        printf("\n");
        
        for( int i = 0; i < strlen(str); i++ ){
            ch = str[i];
            asc_ch = (int) ch;
            flag_ch = 0;
            flag_mot = 0;

            for ( int j = 0; j < num_transitions[curr_addr] * 2; j+=2 ){
                if ( asc_ch == transitions[curr_addr][j] ){
                    curr_addr = transitions[curr_addr][j+1];
                    flag_ch = 1;
                    break;
                }  
            }

            if ( flag_ch == 1 ){
                printf("%c -> Etat : ", ch);
                for( int i = 0; i < strlen(nom_etats[curr_addr]); i++ ){
                    printf("%c", nom_etats[curr_addr][i]);
                }
                printf("\n");
            }
            else{
                printf("%c -> Erreur : Caractere invalid!\n", ch);
                flag_mot = 0;
                break;
            }
        }

        for ( int j = 0; j < num_final; j++ ){
            if ( curr_addr == addr_final[j] ){
                flag_mot = 1;
                break;
            }
        }
        if ( flag_mot == 1 && flag_ch == 1 ){
            printf("Le mot %s est accepte!\n", str);
        }
        else{
            printf("Le mot %s est refuse!\n", str);
        }
        flag_ch = 0;
        flag_mot = 0;
    }

    // cas nombre de pile = 1
    else if ( num_pile == 1 ){

        for ( int i = 0; i < num_etats; i++ ){
            for ( int j = 0; j < num_transitions[symbole[i][1]] * 4; j++ ){
                transitions[symbole[i][1]][j] = VM[symbole[i][1] + j + 1];
            }
        }
   
        printf("Donner le mot d'entree : ");
        scanf("%s", str);

        curr_addr = addr_init;
        printf("  -> Etat : ");
        for( int i = 0; i < strlen(nom_etats[curr_addr]); i++ ){
            printf("%c", nom_etats[curr_addr][i]);
        }
        printf("\t");
        printf("Pile 1 : Vide");
        printf("\n");
        
        for( int i = 0; i < strlen(str); i++ ){
            ch = str[i];
            asc_ch = (int) ch;
            flag_ch = 0;
            flag_mot = 0;
            flag_pile1 = 1;
            flag_full_pile1 = 0;

            for ( int j = 0; j < num_transitions[curr_addr] * 4; j+=4 ){

                if ( asc_ch == transitions[curr_addr][j] ){
                    flag_ch = 1;

                    ch_pile1 = transitions[curr_addr][j+2];                 
                    if ( transitions[curr_addr][j+3] == 1 ){
                        push(pile1, &idx_pile1, ch_pile1);
                    }
                    else if ( transitions[curr_addr][j+3] == -1 ){
                        if ( idx_pile1 != 0 ){
                            pop(pile1, &idx_pile1);
                        }
                        else{
                            flag_pile1 = 0;
                            break;                            
                        }
                    }

                    curr_addr = transitions[curr_addr][j+1];
                    break;
                }  
            }

            if ( flag_ch == 1 && flag_pile1 == 1){
                printf("%c -> Etat : ", ch);
                for( int i = 0; i < strlen(nom_etats[curr_addr]); i++ ){
                    printf("%c", nom_etats[curr_addr][i]);
                }
                printf("\t");
                printf("Pile1 : ");
                if ( idx_pile1 == 0 ){
                    printf("Vide");
                }
                else{
                    for ( int i = 0; i < idx_pile1; i++ ){
                        printf("%c", pile1[i]);
                    }
                }
                printf("\n");
            }
            else if( flag_ch == 0 ){
                printf("%c -> Erreur : Caractere invalid!\n", ch);
                break;
            }
            else if ( flag_pile1 == 0 ){
                printf("%c -> Erreur : Pile1 vide!\n", ch);
                break;                
            }
        }

        for ( int j = 0; j < num_final; j++ ){
            if ( curr_addr == addr_final[j] ){
                flag_mot = 1;
                break;
            }
        }
        if ( idx_pile1 == 0 ){
            flag_full_pile1 = 1;
        }
        if ( flag_mot == 1 && flag_ch == 1 && flag_pile1 == 1 && flag_full_pile1 == 1 ){
            printf("Le mot %s est accepte!\n", str);
        }
        else{
            if ( flag_full_pile1 == 0 ){
                printf("Le mot %s est refuse! Pile1 non vide.\n", str);
            }
            else{
                printf("Le mot %s est refuse!\n", str);
            }
        }
        flag_ch = 0;
        flag_mot = 0;
        flag_pile1 = 1;
        flag_full_pile1 = 0;
    }

    // cas nombre de pile = 2
    else if ( num_pile == 2 ){

        for ( int i = 0; i < num_etats; i++ ){
            for ( int j = 0; j < num_transitions[symbole[i][1]] * 6; j++ ){
                transitions[symbole[i][1]][j] = VM[symbole[i][1] + j + 1];
            }
        }
        printf("Donner le mot d'entree : ");
        scanf("%s", str);

        curr_addr = addr_init;
        printf("  -> Etat : ");
        for( int i = 0; i < strlen(nom_etats[curr_addr]); i++ ){
            printf("%c", nom_etats[curr_addr][i]);
        }
        printf("\t");
        printf("Pile1 : Vide");
        printf("\t");
        printf("Pile2 : Vide");        
        printf("\n");
        
        for( int i = 0; i < strlen(str); i++ ){
            ch = str[i];
            asc_ch = (int) ch;
            flag_ch = 0;
            flag_mot = 0;
            flag_pile1 = 1;
            flag_full_pile1 = 0;
            flag_pile2 = 1;
            flag_full_pile2 = 0;

            for ( int j = 0; j < num_transitions[curr_addr] * 6; j+=6 ){
                
                if ( asc_ch == transitions[curr_addr][j] ){
                    flag_ch = 1;

                    ch_pile1 = transitions[curr_addr][j+2];                 
                    if ( transitions[curr_addr][j+3] == 1 ){
                        push(pile1, &idx_pile1, ch_pile1);
                    }
                    else if ( transitions[curr_addr][j+3] == -1 ){
                        if ( idx_pile1 != 0 ){
                            pop(pile1, &idx_pile1);
                        }
                        else{
                            flag_pile1 = 0;
                            break;
                        }
                    }
                    ch_pile2 = transitions[curr_addr][j+4];                 
                    if ( transitions[curr_addr][j+5] == 1 ){
                        push(pile2, &idx_pile2, ch_pile2);
                    }
                    else if ( transitions[curr_addr][j+5] == -1 ){
                        if ( idx_pile2 != 0 ){
                            pop(pile2, &idx_pile2);
                        }
                        else{
                            flag_pile2 = 0;
                            break;
                        }
                    }

                    curr_addr = transitions[curr_addr][j+1];
                    break;
                }  
            }

            if ( flag_ch == 1 && flag_pile1 == 1 && flag_pile2 == 1 ){
                printf("%c -> Etat : ", ch);
                for( int i = 0; i < strlen(nom_etats[curr_addr]); i++ ){
                    printf("%c", nom_etats[curr_addr][i]);
                }
                printf("\t");
                printf("Pile1 : ");
                if ( idx_pile1 == 0 ){
                    printf("Vide");
                }
                else{
                    for ( int i = 0; i < idx_pile1; i++ ){
                        printf("%c", pile1[i]);
                    }
                }
                printf("\t");
                printf("Pile2 : ");
                if ( idx_pile2 == 0 ){
                    printf("Vide");
                }
                else{
                    for ( int i = 0; i < idx_pile2; i++ ){
                        printf("%c", pile2[i]);
                    }
                }                
                printf("\n");
            }
            else if( flag_ch == 0 ){
                printf("%c -> Erreur : Caractere invalid!\n", ch);
                break;
            }
            else if ( flag_pile1 == 0 ){
                printf("%c -> Erreur : Pile1 vide!\n", ch);
                break;                
            }
            else if ( flag_pile2 == 0 ){
                printf("%c -> Erreur : Pile2 vide!\n", ch);
                break;                
            }
        }

        for ( int j = 0; j < num_final; j++ ){
            if ( curr_addr == addr_final[j] ){
                flag_mot = 1;
                break;
            }
        }
        if ( idx_pile1 == 0 ){
            flag_full_pile1 = 1;
        }
        if ( idx_pile2 == 0 ){
            flag_full_pile2 = 1;
        }        
        if ( flag_mot == 1 && flag_ch == 1 && flag_pile1 == 1 && flag_full_pile1 == 1 && flag_pile2 == 1 && flag_full_pile2 == 1 ){
            printf("Le mot %s est accepte!\n", str);
        }
        else{
            if ( flag_full_pile1 == 0 && flag_full_pile2 == 1 ){
                printf("Le mot %s est refuse! Pile1 non vide.\n", str);
            }
            else if ( flag_full_pile1 == 1 && flag_full_pile2 == 0 ){
                printf("Le mot %s est refuse! Pile2 non vide.\n", str);
            }
            else if ( flag_full_pile1 == 0 && flag_full_pile2 == 0 ){
                printf("Le mot %s est refuse! Pile1 et Pile2 non vides.\n", str);
            }            
            else{
                printf("Le mot %s est refuse!\n", str);
            }
        }
        flag_ch = 0;
        flag_mot = 0;
        flag_pile1 = 1;
        flag_full_pile1 = 0;
        flag_pile2 = 1;
        flag_full_pile2 = 0;
    }    
    fclose(pFile_VM);
}
