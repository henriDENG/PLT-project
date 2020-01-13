#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FILE_NAME_R "Dpile.txt"             // fichier original
#define FILE_NAME_W1 "nocomment_pile.txt"   // fichier sans commentaire
#define FILE_NAME_W2 "lexeme.txt"           // fichier sans commentaire sans caracteres inutiles
#define FILE_NAME_W3 "arbre_syntaxique.txt" // fichier pour dessiner l'arbre syntaxique
#define MAX_LINE 1024                       // limite de la maximum longueur de ligne
#define MAX_ETAT 20                         // limite de la maximum longueur de l'etat

typedef enum   // etats pour examiner la synthaxe
{
    SYN_STATE0 = 0,
    SYN_STATE1,
    SYN_STATE2,
    SYN_STATE3,
    SYN_STATE4,
    SYN_STATE5,
    SYN_STATE6,
    SYN_STATE7,
    SYN_STATE8,
    SYN_STATE9,
    SYN_STATE10,
    SYN_STATE11,
    SYN_STATE12,
    SYN_STATE13,
    SYN_STATE14,
    SYN_STATE15,    
    SYN_STATE16,
    SYN_STATE17,
    SYN_STATE18,
    SYN_STATE19,
    SYN_STATE20,
    SYN_STATE21,
    SYN_STATE22,    
    SYN_STATE23,
    SYN_STATE24,
    SYN_STATE25,
    SYN_STATE26,
    SYN_STATE27,
    SYN_STATE28,
    SYN_STATE29,    
    SYN_STATE30,
    SYN_STATE31,
    SYN_STATE32,
    SYN_STATE33,
    SYN_STATE34,
    SYN_STATE35,
    SYN_STATE36,   
    SYN_STATE37,
    SYN_STATE38,
    SYN_STATE39,
    SYN_STATE40,
    SYN_STATE41,
    SYN_STATE42,
    SYN_STATE43,    
    SYN_STATE44,
    SYN_STATE45,
    SYN_STATE46,
    SYN_STATE47,
    SYN_STATE48,
    SYN_STATE49,
}SYN_STATE;


int examiner_syntaxique(char* File_name1, char* File_name2)   // supprimer les commentaires
{
    char mot1[] = "Automate";
    char mot2[] = "etats";
    char mot3[] = "initial";
    char mot4[] = "final";
    char mot5[] = "transitions";  
    char special1[] = "→";      

    FILE *pFile_r = fopen(File_name1, "r");      // fichier qu'on veut traiter    
    FILE *pFile_w = fopen(File_name2, "w");      // fichier qu'on veut  creer  
    SYN_STATE curr_SYN_STATE = SYN_STATE0;  // etat initial
    char ch;
    int asc_ch;             // ASCII code du caractere
    char temp[100] = {0};   // liste temporaire
    int flag_match = -1;    // 0 bon, 1 pas bon
    int syn_flag = 0;       // 0 correct, 1 erreur
    char quot_left;         // enregistrer le " a gauche
    char quot_right;        // enregistrer le " a droite
    // int ii;              // pour iteration
    // int flag_mot_cle[6] = {0};   // pour verifier qu'on a tous les mots cles, 0 non detecte, 1 detecte

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

    // appliquer une automate pour examiner les syntaxes et creer un fichier de l'arbre syntaxique
    while( (ch = fgetc(pFile_r)) != EOF ){

        if ( syn_flag == 1 )
            break;

        switch(curr_SYN_STATE){
            case SYN_STATE0:
                    // printf("SYN_STATE0: \n");           
                    if ( ch == 'A' ){
                        temp[0] = ch;
                        for ( int i = 1; i <= 7; i++ ){
                            ch = fgetc(pFile_r);
                            temp[i] = ch;
                        }

                        flag_match = strcmp(temp, mot1);
                        if ( flag_match == 0 ){
                            curr_SYN_STATE = SYN_STATE1;
                            // flag_mot_cle[1] = 1;    // Automate detecte
                            fputs("digraph Automate {\n", pFile_w);     // write digraph Automate {
                        }
                        else{
                            syn_flag = 1;
                            printf("Mot cle inconnu: %s\n", temp);
                        }
                    }
                    else{
                        syn_flag = 1;
                        printf("Faute au debut de mot cle: %c\n", ch);
                    }
                    memset(temp, 0, sizeof(temp));
                    break;
            case SYN_STATE1:
                    // printf("SYN_STATE1: \n");               
                    if ( ch == '(' )
                        curr_SYN_STATE = SYN_STATE2;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE2:
                    // printf("SYN_STATE2: \n");               
                    if ( ch == '0' || ch == '1' || ch == '2' ){
                        curr_SYN_STATE = SYN_STATE3;
                        fputs("Automate -> \"pile: ", pFile_w);
                        fputc(ch, pFile_w);
                        fputs("\";\n", pFile_w);    // write Automate -> "pile: 0";
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE3:
                    // printf("SYN_STATE3: \n");               
                    if ( ch == ')' )
                        curr_SYN_STATE = SYN_STATE4;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE4:
                    // printf("SYN_STATE4: \n");             
                    if ( ch == '=' )
                        curr_SYN_STATE = SYN_STATE5;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE5:  
                    // printf("SYN_STATE5: \n");             
                    if ( ch == '{' )
                        curr_SYN_STATE = SYN_STATE6;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE6: 
                    // printf("SYN_STATE6: \n");              
                    if ( ch == 'e' ){
                        temp[0] = ch;
                        for ( int i = 1; i <= 4; i++ ){
                            ch = fgetc(pFile_r);
                            temp[i] = ch;
                        }
                        
                        flag_match = strcmp(temp, mot2);
                        if ( flag_match == 0 ){
                            curr_SYN_STATE = SYN_STATE7;
                            // flag_mot_cle[2] = 1;    // etats detecte
                            fputs("Automate -> etats;\n", pFile_w);   // write Automate -> etats;
                        }
                            
                        else{
                            syn_flag = 1;
                            printf("Mot cle inconnu: %s\n", temp);
                        }
                    }
                    else{
                        syn_flag = 1;
                        printf("Faute au debut de mot cle: %c\n", ch);
                    }
                    memset(temp, 0, sizeof(temp));
                    break;
            case SYN_STATE7: 
                    // printf("SYN_STATE7: \n");              
                    if ( ch == '=' )
                        curr_SYN_STATE = SYN_STATE8;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE8:
                    // printf("SYN_STATE8: \n");              
                    if ( ch == '[' )
                        curr_SYN_STATE = SYN_STATE9;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE9: 
                    // printf("SYN_STATE9: \n");             
                    if ( ch == '`' || ch == '"' ){
                        quot_left = ch;
                        curr_SYN_STATE = SYN_STATE10;
                        fputs("etats -> \"", pFile_w);    // write etats -> "
                    }
                    else if ( ch == ']')
                        curr_SYN_STATE = SYN_STATE12;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE10:         
                    // printf("SYN_STATE10: \n");
                    if ( ch == '`' || ch == '"' ){
                        quot_right = ch;
                        if ( quot_right == quot_left ){
                            curr_SYN_STATE = SYN_STATE11;
                            fputs("\";\n", pFile_w);    // write ";
                        }   // verifier si les deux guillemets se correspondent   
                        else{
                            syn_flag == 1;
                            printf("Les deux guillemets ne correspondent pas: %c\n", ch);
                        }
                    }
                    else{
                        curr_SYN_STATE = SYN_STATE10;
                        fputc(ch, pFile_w); // write 1, a, 一
                    }
                    break;
            case SYN_STATE11:
                    // printf("SYN_STATE11: \n");              
                    if ( ch == ',' )
                        curr_SYN_STATE = SYN_STATE9;
                    else if ( ch == ']' ){
                        curr_SYN_STATE = SYN_STATE12;
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE12:  
                    // printf("SYN_STATE12: \n");
                    if ( ch == 'i' ){
                        temp[0] = ch;
                        for ( int i = 1; i <= 6; i++ ){
                            ch = fgetc(pFile_r);
                            temp[i] = ch;
                        }
                        
                        flag_match = strcmp(temp, mot3);
                        if ( flag_match == 0 ){
                            curr_SYN_STATE = SYN_STATE13;
                            // flag_mot_cle[3] = 1;    // initial detect
                            fputs("Automate -> initial;\n", pFile_w);    // write Automate -> initial;
                        }   
                        else{
                            syn_flag = 1;
                            printf("Mot cle inconnu: %s\n", temp);
                        }
                    }
                    else{
                        syn_flag = 1;
                        printf("Faute au debut de mot cle: %c\n", ch);
                    }
                    memset(temp, 0, sizeof(temp));
                    break;  
            case SYN_STATE13:
                    // printf("SYN_STATE13: \n");  
                    if ( ch == '=' ){
                        curr_SYN_STATE = SYN_STATE14;
                        fputs("initial -> ", pFile_w);    // write initial -> 
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE14: 
                    // printf("SYN_STATE14: \n"); 
                    if ( ch >= '0' && ch <= '9' ){
                        curr_SYN_STATE = SYN_STATE15;
                        fputc(ch, pFile_w);     // write 1
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE15:
                    // printf("SYN_STATE15: \n");   
                    if ( ch == 'f' ){
                        temp[0] = ch;
                        for ( int i = 1; i <= 4; i++ ){
                            ch = fgetc(pFile_r);
                            temp[i] = ch;
                        }
                        
                        flag_match = strcmp(temp, mot4);
                        if ( flag_match == 0 ){
                            curr_SYN_STATE = SYN_STATE16;
                            // flag_mot_cle[4] = 1;    // final detecte
                            fputs(";\n", pFile_w);    // write ";
                            fputs("Automate -> final;\n", pFile_w);   // write Automate -> final;
                        }
                        else{
                            syn_flag = 1;
                            printf("Mot cle inconnu: %s\n", temp);
                        }
                    }
                    else if ( ch >= '0' && ch <='9'){
                        curr_SYN_STATE = SYN_STATE15;
                        fputc(ch, pFile_w);     // write 1
                    }
                    else{
                        syn_flag = 1;
                        printf("Faute au debut de mot cle: %c\n", ch);
                    }
                    memset(temp, 0, sizeof(temp));
                    break;    
            case SYN_STATE16:
                    // printf("SYN_STATE16: \n");   
                    if ( ch == '=' )
                        curr_SYN_STATE = SYN_STATE17;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE17: 
                    // printf("SYN_STATE17: \n");  
                    if ( ch == '[' )
                        curr_SYN_STATE = SYN_STATE18;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE18:  
                    // printf("SYN_STATE18: \n"); 
                    if ( ch >= '0' && ch <='9' ){
                        curr_SYN_STATE = SYN_STATE19;
                        fputs("final -> ", pFile_w);
                        fputc(ch, pFile_w);
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE19:
                    // printf("SYN_STATE19: \n");   
                    if ( ch == ',' ){
                        curr_SYN_STATE = SYN_STATE18;
                        fputs(";\n", pFile_w);
                    }
                    else if ( ch >= '0' && ch <='9' ){
                        curr_SYN_STATE = SYN_STATE19;
                        fputc(ch, pFile_w);
                    }
                    else if ( ch == ']' ){
                        curr_SYN_STATE = SYN_STATE20;
                        fputs(";\n", pFile_w);
                    }                    
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE20:
                    // printf("SYN_STATE20: \n");   
                    if ( ch == 't' ){
                        temp[0] = ch;
                        for ( int i = 1; i <= 10; i++ ){
                            ch = fgetc(pFile_r);
                            temp[i] = ch;
                        }
                        
                        flag_match = strcmp(temp, mot5);
                        if ( flag_match == 0 ){
                            curr_SYN_STATE = SYN_STATE21;
                            // flag_mot_cle[5] = 1;    // transitions detecte
                            fputs("Automate -> transitions;\n", pFile_w);   // write Automate ->transitions;
                        }
                        else{
                            syn_flag = 1;
                            printf("Mot cle inconnu: %s\n", temp);
                        }
                    }
                    else{
                        syn_flag = 1;
                        printf("Faute au debut de mot cle: %c\n", ch);
                    }
                    memset(temp, 0, sizeof(temp));
                    break;
            case SYN_STATE21:
                    // printf("SYN_STATE21: \n");   
                    if ( ch == '=' )
                        curr_SYN_STATE = SYN_STATE22;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE22:  
                    // printf("SYN_STATE22: \n"); 
                    if ( ch == '[' )
                        curr_SYN_STATE = SYN_STATE23;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE23:  
                    // printf("SYN_STATE23: \n"); 
                    if ( ch == '(' )
                        curr_SYN_STATE = SYN_STATE24;
                    else if ( ch == ']')
                        curr_SYN_STATE = SYN_STATE48;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE24: 
                    // printf("SYN_STATE24: \n");  
                    if ( ch >= '0' && ch <='9' ){
                        curr_SYN_STATE = SYN_STATE25;
                        fputs("transitions -> \"(", pFile_w);     // write transitions - > "(
                        fputc(ch, pFile_w);     // write 1
                    }     
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE25: 
                    // printf("SYN_STATE25: \n"); 
                    asc_ch = (int) ch;
                    if ( asc_ch == -30 ){
                        temp[0] = ch;
                        for ( int i = 1; i <= 2; i++ ){
                            ch = fgetc(pFile_r);
                            temp[i] = ch;
                        }
                        
                        flag_match = strcmp(temp, special1);
                        if ( flag_match == 0 ){
                            curr_SYN_STATE = SYN_STATE26;
                            fputs(special1, pFile_w);
                        }
                        else{
                            syn_flag = 1;
                            printf("Caractere inconnu: %s\n", temp);
                        }
                    }
                    else if ( ch >= '0' && ch <= '9' ){
                        curr_SYN_STATE = SYN_STATE25;
                        fputc(ch, pFile_w);
                    }   // modified
                    else{
                        syn_flag = 1;
                        printf("Faute au debut de caractere: %c\n", ch);
                    }
                    memset(temp, 0, sizeof(temp));
                    break; 
            case SYN_STATE26:  
                    // printf("SYN_STATE26: \n"); 
                    if ( ch >= '0' && ch <='9' ){
                        curr_SYN_STATE = SYN_STATE27;
                        fputc(ch, pFile_w);
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;   
            case SYN_STATE27:  
                    // printf("SYN_STATE27: \n"); 
                    if ( ch == ',' ){
                        curr_SYN_STATE = SYN_STATE28;
                        fputc(ch, pFile_w);
                    }
                    else if ( ch >= '0' && ch <='9' ){
                        curr_SYN_STATE = SYN_STATE27;
                        fputc(ch, pFile_w);
                    }   // modified
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE28:  
                    // printf("SYN_STATE28: \n"); 
                    if ( ch == '`' ){
                        fputc(ch, pFile_w);
                        curr_SYN_STATE = SYN_STATE29;
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE29:  
                    // printf("SYN_STATE29: \n"); 
                    if ( (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') ){
                        curr_SYN_STATE = SYN_STATE30;
                        fputc(ch, pFile_w);
                    }
                    else if (  ch == '`' ){
                        curr_SYN_STATE = SYN_STATE31;
                        fputc(ch, pFile_w);
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE30:  
                    // printf("SYN_STATE30: \n"); 
                    if ( ch == '`' ){
                        curr_SYN_STATE = SYN_STATE31;
                        fputc(ch, pFile_w);
                    }    
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE31:  
                    // printf("SYN_STATE31: \n"); 
                    if ( ch == ')' ){
                        curr_SYN_STATE = SYN_STATE32;
                        fputc(ch, pFile_w);
                        fputs("\";\n", pFile_w);
                    }  
                    else if (  ch == ',' ){
                        curr_SYN_STATE = SYN_STATE34;
                        fputc(ch, pFile_w);
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE32:  
                    // printf("SYN_STATE32: \n"); 
                    if ( ch == ',' )
                        curr_SYN_STATE = SYN_STATE33;
                    else if (  ch == ']' )
                        curr_SYN_STATE = SYN_STATE48;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE33:  
                    // printf("SYN_STATE33: \n"); 
                    if ( ch == '(' )
                        curr_SYN_STATE = SYN_STATE24;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE34:  
                    // printf("SYN_STATE34: \n"); 
                    if ( ch == '(' ){
                        curr_SYN_STATE = SYN_STATE35;
                        fputc(ch, pFile_w);
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;  
            case SYN_STATE35:
                    // printf("SYN_STATE35: \n"); 
                    asc_ch = (int) ch;  
                    if ( ch == ')' ){
                        curr_SYN_STATE = SYN_STATE45;
                        fputc(ch, pFile_w);
                    }
                    else if (  ch == '`'  ){
                        curr_SYN_STATE = SYN_STATE36;
                        fputc(ch, pFile_w);
                    }
                    else if( asc_ch == -30 ){
                        temp[0] = ch;
                        for ( int i = 1; i <= 2; i++ ){
                            ch = fgetc(pFile_r);
                            temp[i] = ch;
                        }
                            
                        flag_match = strcmp(temp, special1);
                        if ( flag_match == 0 ){
                            curr_SYN_STATE = SYN_STATE40;
                            fputs(special1, pFile_w);
                        }
                        else{
                            syn_flag = 1;
                            printf("Caractere inconnu: %s\n", temp);
                        }
                    }
                    else{
                        syn_flag = 1;
                        printf("Faute au debut de caractere: %c\n", ch);
                    }
                    memset(temp, 0, sizeof(temp));                        
                    break; 
            case SYN_STATE36: 
                    // printf("SYN_STATE36: \n");  
                    if ( (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') ){
                        curr_SYN_STATE = SYN_STATE37;
                        fputc(ch, pFile_w);
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;  
            case SYN_STATE37:  
                    // printf("SYN_STATE37: \n"); 
                    if ( ch == '`' ){
                        curr_SYN_STATE = SYN_STATE38;
                        fputc(ch, pFile_w);
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;  
            case SYN_STATE38:  
                    // printf("SYN_STATE38: \n"); 
                    if ( ch == ',' ){
                        curr_SYN_STATE = SYN_STATE39;
                        fputc(ch, pFile_w);                       
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE39: 
                    // printf("SYN_STATE39: \n"); 
                    asc_ch = (int) ch;
                    if ( asc_ch == -30 ){
                        temp[0] = ch;
                        for ( int i = 1; i <= 2; i++ ){
                            ch = fgetc(pFile_r);
                            temp[i] = ch;
                        }
                        
                        flag_match = strcmp(temp, special1);
                        if ( flag_match == 0 ){
                            curr_SYN_STATE = SYN_STATE44;
                            fputs(special1, pFile_w);
                        }
                        else{
                            syn_flag = 1;
                            printf("Caractere inconnu: %s\n", temp);
                        }
                    }
                    else{
                        syn_flag = 1;
                        printf("Faute au debut de caractere: %c\n", ch);
                    }
                    memset(temp, 0, sizeof(temp));
                    break;                     
            case SYN_STATE40:
                    // printf("SYN_STATE40: \n");   
                    if ( ch == ',' ){
                        curr_SYN_STATE = SYN_STATE41;
                        fputc(ch, pFile_w);
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE41:  
                    // printf("SYN_STATE41: \n"); 
                    if ( ch == '`' ){
                        curr_SYN_STATE = SYN_STATE42;
                        fputc(ch, pFile_w);
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;  
            case SYN_STATE42:  
                    // printf("SYN_STATE42: \n"); 
                    if ( (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') ){
                        curr_SYN_STATE = SYN_STATE43;
                        fputc(ch, pFile_w);
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE43:  
                    // printf("SYN_STATE43: \n"); 
                    if ( ch == '`' ){
                        curr_SYN_STATE = SYN_STATE44;
                        fputc(ch, pFile_w);
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE44: 
                    // printf("SYN_STATE44: \n");  
                    if ( ch == ')' ){
                        curr_SYN_STATE = SYN_STATE45;
                        fputc(ch, pFile_w);
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE45:
                    // printf("SYN_STATE45: \n");   
                    if ( ch == ')' ){
                        curr_SYN_STATE = SYN_STATE46;
                        fputc(ch, pFile_w);
                        fputs("\";\n", pFile_w);
                    } 
                    else if ( ch == ','){
                        curr_SYN_STATE = SYN_STATE34;
                        fputc(ch, pFile_w);
                    }
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE46:  
                    // printf("SYN_STATE46: \n"); 
                    if ( ch == ']' )
                        curr_SYN_STATE = SYN_STATE48;
                    else if (  ch == ',' )
                        curr_SYN_STATE = SYN_STATE47;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;
            case SYN_STATE47:  
                    // printf("SYN_STATE47: \n"); 
                    if ( ch == '(' )
                        curr_SYN_STATE = SYN_STATE24;
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break; 
            case SYN_STATE48:  
                    // printf("SYN_STATE48: \n"); 
                    if ( ch == '}' ){
                        curr_SYN_STATE = SYN_STATE49;
                        fputc(ch, pFile_w);
                    } 
                    else{
                        syn_flag = 1;
                        printf("Incorrect caractere: %c\n", ch);
                    }
                    break;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
            default: 
                    curr_SYN_STATE = SYN_STATE0;
                    break;   
        }
    }

    /*
    for ( ii = 1; ii <= 5; ii ++ ){
        if ( flag_mot_cle[ii] != 1 ){
            syn_flag = 1;
            flag_mot_cle[0] = -1;
            break;
        }
    }   // examiner si tous les mots cles sont dans le fichier
    */

    if ( syn_flag == 1 ){
        printf("Le fichier est syntaxiquement faux!\n");
    }
    
    fclose(pFile_w);
    fclose(pFile_r);

    printf("-------------------------------------------------------------------------------------------------------------------------\n");
    printf("L'analyse syntaxique est finie!\n");

    return syn_flag;
}
