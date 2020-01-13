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

int compilateur(char etats[][10], int *initial, int *final, int transitions[][2], char *caractere, char pile1[][2], char pile2[][2], int *num_etats, int *num_final, int *num_transitions, int *num_pile, int *VM, int *length_VM, int symbole[][2])
{
    int pos_Etats_pre = -1;             // position precedente dans VM
    int pos_Etats_curr = -1;            // position actuelle dans VM
    int pos_VM_pre = -1;                // position precedente dans VM
    int pos_VM_curr = -1;               // position actuelle dans VM
    int len_VM = 0;                     // longueur totale de VM
    int reg_addr = 0;                   // variable pour enregistrer l'addresse de l'etat actuel
    int reg_num_trans[10] = {0};        // liste pour enregistrer le nombre des transitions pour chaque etat
    int reg_pos_trans[10][10] = {0};    // liste pour enregistrer les positions precises des transitions pour chaque etat
    int temp;
    
    if ( (*num_pile) == 0 ){

        // obtenir le tableau du symbole

        // obtenir le nombre des transitions pour chaque etat
        for ( int i = 0; i < *num_etats; i++ ){
            for ( int j = 0; j < *num_transitions; j++ ){
                if ( transitions[j][0] == i ){
                    temp = reg_num_trans[i];
                    reg_pos_trans[i][temp] = j;
                    reg_num_trans[i]++;                   
                }
            }
        }
        // obtenir les addresses pour tous les etats
        pos_Etats_curr = *initial;
        pos_VM_curr = (*num_final) + 3;
        symbole[(*initial)][0] = *initial;
        symbole[(*initial)][1] = pos_VM_curr;

        for ( int i = 0; i < *num_etats; i++ ){
            if ( i != (*initial) ){
                pos_Etats_pre = pos_Etats_curr;
                pos_VM_pre = pos_VM_curr;
                pos_Etats_curr = i;
                pos_VM_curr = pos_VM_pre + reg_num_trans[pos_Etats_pre] * 2 + 1;
                symbole[i][0] = i;
                symbole[i][1] = pos_VM_curr;
            }
        }
        printf("tableau de symbole: ");
        for ( int i = 0; i < *num_etats; i++ ){
            printf(" %d %d |", symbole[i][0], symbole[i][1]);
        }
        printf("\n");

        // generer la machine virtuelle
        VM[0] = *num_pile;
        VM[1] = symbole[(*initial)][1];
        VM[2] = *num_final;
        for ( int i = 0; i < *num_final; i++ ){
            VM[i+3] = symbole[final[i]][1];
        }
        pos_VM_curr = *num_final + 3;
        for ( int i = 0; i < *num_etats; i++ ){
            pos_VM_curr = symbole[i][1];
            VM[pos_VM_curr] = reg_num_trans[i];
            pos_VM_curr++;
            for ( int j = 0; j < reg_num_trans[i]; j++ ){
                temp = reg_pos_trans[i][j];
                VM[pos_VM_curr] = (int) caractere[temp];
                pos_VM_curr++;
                temp = symbole[transitions[temp][1]][1];
                VM[pos_VM_curr] = temp;
                pos_VM_curr++;
            }
            if ( len_VM <= pos_VM_curr)
                len_VM = pos_VM_curr;
        }
        *length_VM = len_VM; 

        printf("machine virtuelle: ");
        for ( int i = 0; i < len_VM; i++ ){
            printf("%d ", VM[i]);
        }
        printf("\n");
        // printf("%d\n", len_VM);  

        pos_Etats_pre = -1;           
        pos_Etats_curr = -1;            
        pos_VM_pre = -1;               
        pos_VM_curr = -1; 
        len_VM = 0;                     
        reg_addr = 0;                   
        memset(reg_num_trans, 0, sizeof(reg_num_trans));     
        memset(reg_pos_trans, 0, sizeof(reg_pos_trans));              
    }

    else if ( (*num_pile) == 1 ){

        // obtenir le tableau du symbole

        // obtenir les transitions pour chaque etat
        for ( int i = 0; i < *num_etats; i++ ){
            for ( int j = 0; j < *num_transitions; j++ ){
                if ( transitions[j][0] == i ){
                    temp = reg_num_trans[i];
                    reg_pos_trans[i][temp] = j;
                    reg_num_trans[i]++;                   
                }
            }
        }

        // obtenir les addresses pour tous les etats
        pos_Etats_curr = *initial;
        pos_VM_curr = (*num_final) + 3;
        symbole[(*initial)][0] = *initial;
        symbole[(*initial)][1] = pos_VM_curr;

        for ( int i = 0; i < *num_etats; i++ ){
            if ( i != (*initial) ){
                pos_Etats_pre = pos_Etats_curr;
                pos_VM_pre = pos_VM_curr;
                pos_Etats_curr = i;
                pos_VM_curr = pos_VM_pre + reg_num_trans[pos_Etats_pre] * 4 + 1;
                symbole[i][0] = i;
                symbole[i][1] = pos_VM_curr;
            }
        }
        printf("tableau de symbole: ");
        for ( int i = 0; i < *num_etats; i++ ){
            printf(" %d %d |", symbole[i][0], symbole[i][1]);
        }
        printf("\n");

        // generer la machine virtuelle
        VM[0] = *num_pile;
        VM[1] = symbole[(*initial)][1];
        VM[2] = *num_final;
        for ( int i = 0; i < *num_final; i++ ){
            VM[i+3] = symbole[final[i]][1];
        }
        pos_VM_curr = *num_final + 3;
        for ( int i = 0; i < *num_etats; i++ ){
            pos_VM_curr = symbole[i][1];
            VM[pos_VM_curr] = reg_num_trans[i];
            pos_VM_curr++;
            for ( int j = 0; j < reg_num_trans[i]; j++ ){
                temp = reg_pos_trans[i][j];
                VM[pos_VM_curr] = (int) caractere[temp];
                pos_VM_curr++;
                VM[pos_VM_curr] = symbole[transitions[temp][1]][1];
                pos_VM_curr++;
                if ( pile1[temp][0] == '\0' ){
                    VM[pos_VM_curr] = 0;
                    pos_VM_curr++;
                    VM[pos_VM_curr] = 0;
                    pos_VM_curr++;
                }
                else{
                    VM[pos_VM_curr] = pile1[temp][1];
                    pos_VM_curr++;
                    VM[pos_VM_curr] = 44 - (int) pile1[temp][0];
                    pos_VM_curr++;
                }
            }
            if ( len_VM <= pos_VM_curr)
                len_VM = pos_VM_curr;
        }
        *length_VM = len_VM; 

        printf("machine virtuelle: ");
        for ( int i = 0; i < len_VM; i++ ){
            printf("%d ", VM[i]);
        }
        printf("\n");
        // printf("%d\n", len_VM);  
        pos_Etats_pre = -1;           
        pos_Etats_curr = -1;            
        pos_VM_pre = -1;               
        pos_VM_curr = -1;               
        len_VM = 0;                     
        reg_addr = 0;                   
        memset(reg_num_trans, 0, sizeof(reg_num_trans));     
        memset(reg_pos_trans, 0, sizeof(reg_pos_trans));              
    } 

    else if ( (*num_pile) == 2 ){

        // obtenir le tableau du symbole

        // obtenir le nombre des transitions pour chaque etat
        for ( int i = 0; i < *num_etats; i++ ){
            for ( int j = 0; j < *num_transitions; j++ ){
                if ( transitions[j][0] == i ){
                    temp = reg_num_trans[i];
                    reg_pos_trans[i][temp] = j;
                    reg_num_trans[i]++;                   
                }
            }
        }

        // obtenir les addresses pour tous les etats
        pos_Etats_curr = *initial;
        pos_VM_curr = (*num_final) + 3;
        symbole[(*initial)][0] = *initial;
        symbole[(*initial)][1] = pos_VM_curr;

        for ( int i = 0; i < *num_etats; i++ ){
            if ( i != (*initial) ){
                pos_Etats_pre = pos_Etats_curr;
                pos_VM_pre = pos_VM_curr;
                pos_Etats_curr = i;
                pos_VM_curr = pos_VM_pre + reg_num_trans[pos_Etats_pre] * 6 + 1;
                symbole[i][0] = i;
                symbole[i][1] = pos_VM_curr;
            }
        }
        printf("tableau de symbole: ");
        for ( int i = 0; i < *num_etats; i++ ){
            printf(" %d %d |", symbole[i][0], symbole[i][1]);
        }
        printf("\n");

        // generer la machine virtuelle
        VM[0] = *num_pile;
        VM[1] = symbole[(*initial)][1];
        VM[2] = *num_final;
        for ( int i = 0; i < *num_final; i++ ){
            VM[i+3] = symbole[final[i]][1];
        }
        pos_VM_curr = *num_final + 3;
        for ( int i = 0; i < *num_etats; i++ ){
            pos_VM_curr = symbole[i][1];
            VM[pos_VM_curr] = reg_num_trans[i];
            pos_VM_curr++;
            for ( int j = 0; j < reg_num_trans[i]; j++ ){
                temp = reg_pos_trans[i][j];
                VM[pos_VM_curr] = (int) caractere[temp];
                pos_VM_curr++;
                VM[pos_VM_curr] = symbole[transitions[temp][1]][1];
                pos_VM_curr++;
                if ( pile1[temp][0] == '\0' ){
                    VM[pos_VM_curr] = 0;
                    pos_VM_curr++;
                    VM[pos_VM_curr] = 0;
                    pos_VM_curr++;
                    if ( pile2[temp][0] == '\0' ){
                        VM[pos_VM_curr] = 0;
                        pos_VM_curr++;
                        VM[pos_VM_curr] = 0;
                        pos_VM_curr++;
                    }
                    else{
                        VM[pos_VM_curr] = pile2[temp][1];
                        pos_VM_curr++;
                        VM[pos_VM_curr] = 44 - (int) pile2[temp][0];
                        pos_VM_curr++;                    
                    }
                }
                else{
                    VM[pos_VM_curr] = pile1[temp][1];
                    pos_VM_curr++;
                    VM[pos_VM_curr] = 44 - (int) pile1[temp][0];
                    pos_VM_curr++;
                    if ( pile2[temp][0] == '\0' ){
                        VM[pos_VM_curr] = 0;
                        pos_VM_curr++;
                        VM[pos_VM_curr] = 0;
                        pos_VM_curr++;
                    }
                    else{
                        VM[pos_VM_curr] = pile2[temp][1];
                        pos_VM_curr++;
                        VM[pos_VM_curr] = 44 - (int) pile2[temp][0];
                        pos_VM_curr++;                    
                    }
                }
            }
            if ( len_VM <= pos_VM_curr)
                len_VM = pos_VM_curr;
        }
        *length_VM = len_VM;

        printf("machine virtuelle: ");
        for ( int i = 0; i < len_VM; i++ ){
            printf("%d ", VM[i]);
        }
        printf("\n");
        pos_Etats_pre = -1;           
        pos_Etats_curr = -1;            
        pos_VM_pre = -1;               
        pos_VM_curr = -1;               
        len_VM = 0;                     
        reg_addr = 0;                   
        memset(reg_num_trans, 0, sizeof(reg_num_trans));     
        memset(reg_pos_trans, 0, sizeof(reg_pos_trans));              
    }    
    return 0;
}

int enregistrer_VM(char *File_name1, char *File_name2, int *VM, int length_VM, int symbole[][2], char etats[][10], int num_etats)
{    
    // creer le fichier machine virtuelle
    FILE *pFile_w1 = fopen(File_name1, "w");      // fichier de VM
    if( pFile_w1 == NULL ){
        printf("Can't open the file %s!\n", File_name1);
        getchar();
		exit(1);
    }
    for ( int i = 0; i < length_VM; i++ ){
        fprintf(pFile_w1, "%d", VM[i]);
        fputc(' ', pFile_w1);
    }
    fclose(pFile_w1);

    // creer le fichier tableau de symbole
    FILE *pFile_w2 = fopen(File_name2, "w");
    if( pFile_w2 == NULL ){
        printf("Can't open the file %s!\n", File_name2);
        getchar();
		exit(1);
    }    
    for ( int j = 0; j < num_etats; j++ ){
        fputs("Nom: ", pFile_w2);
        fputs(etats[j], pFile_w2);
        fputc('\t', pFile_w2);
        fputs("Addresse: ", pFile_w2);
        fprintf(pFile_w2, "%d", symbole[j][1]);
        fputc('\n', pFile_w2);
    }
    fclose(pFile_w2);
    return 0;
}
