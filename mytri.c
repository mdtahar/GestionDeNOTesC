#include "union.h"
#include "formater.h"
#include "structure.h"

int tri_numerique(const char* chaine1,const char* chaine2 );

int *tri_ligne(int drapeau,mem_fichier* fichier,int champ){
    int i,j;
    int taille = fichier->nbr_ligne;
    int (*fonction)(const char*,const char*);
    
    int *tab_vide= malloc(taille*sizeof(int));;
    int *tab_abs=malloc(taille*sizeof(int));;
    int *tab_num=malloc(taille*sizeof(int));;
    
    int *tmp = malloc(taille*sizeof(int));
    int taille_abs=0;
    
    for(i=0;i<taille;i++){
        tmp[i] = i;
    }
    // 0 lexico on prend en compte
    if(drapeau == 0) {
        fonction = strcmp;
        
    }
    else {
        fonction  = tri_numerique;
        
        for(i = 0;i<taille;i++){
            if(strstr(fichier->tab[tmp[i]][champ],"abs") != NULL)
            {
                tab_abs[taille_abs]=tmp[i];
                taille_abs++;
            }
        }
        
    }
    
    int taille_vide=0;
    
    for(i = 0;i<taille;i++){
        if(strcmp(fichier->tab[tmp[i]][champ]," ") == 0)
        {
            tab_vide[taille_vide]=tmp[i];
            taille_vide++;
        }
    }
    
    int taille_num=0;
    
    for(i = 0;i<taille;i++){
        if( (strcmp(fichier->tab[tmp[i]][champ]," ") != 0))
        {
            if(drapeau==0){
                
                tab_num[taille_num]=tmp[i];
                taille_num++;
                
            }
            else
            {
                if(strstr(fichier->tab[tmp[i]][champ],"abs") == NULL)
                {
                    tab_num[taille_num]=tmp[i];
                    taille_num++;
                }
            }
            
        }
    }
    
    
    for (i = 1; i <taille_num; i++) {
        int elem = tab_num[i];
        for (j = i; j > 0 && fonction(fichier->tab[tab_num[j-1]][champ],fichier->tab[elem][champ])>0; j--)
        {
            tab_num[j] = tab_num[j-1];
        }
        tab_num[j] = elem;
    }
    for (i = 0; i <taille_num; i++) {
        tmp[i]=tab_num[i];
    }
    if(drapeau == 1){
        for (i = taille_num, j=0; i <taille_num+taille_abs; i++,j++) {
            tmp[i]= tab_abs[j];
        }
    }
    for (i = taille_num+taille_abs ,j=0; i <taille_num+taille_abs+taille_vide; i++,j++) {
        tmp[i]= tab_vide[j];
    }
    
    
    free(tab_vide);
    free(tab_abs);
    free(tab_num);
    
    return tmp;
}
int tri_numerique(const char* chaine1,const char* chaine2 ){
    
    if(atoi(chaine1)>atoi(chaine2))
        return 1;
    else if(atoi(chaine1)<atoi(chaine2))
        return -1;
    else return 0;
}
void mytri(int drapeau, char* fichier, int champ, char car_sep, char *fichier_sorti, unsigned short bit_tri){
    
    char *file_out = "resultat_tri.csm";
    unsigned short bit_file = 0x1;
    unsigned short bit_sep = 0x2;
    if( !( bit_tri & bit_file) )
        strcat(fichier_sorti, file_out );
    file_out = fichier_sorti;
    mem_fichier *memoire = stockage_fichier(fichier);
    char *separateur = malloc(2*sizeof(char));
    if(bit_tri & bit_sep){
        
        separateur[1] = '\0';
        separateur[0]=car_sep;
    }else{
        strcpy(separateur,memoire->separateur);
        separateur[1] = '\0';
    }
    char *resultat = malloc(LIGNE_MAX*sizeof(char));
    resultat[0] = '\0';
    FILE *dest = fopen(file_out,"w");
    fputs(separateur,dest);
    fputs("\n",dest);
    int* res = NULL;
    res = tri_ligne(drapeau,memoire,champ);
    for(int i = 0;i<memoire->nbr_ligne;i++){
        resultat[0] = '\0';
        for(int j=0;j<memoire->nbr_champs;j++){
            strcat(resultat,(memoire->tab[res[i]][j]));
            if(j!=memoire->nbr_champs-1){
                strcat(resultat,separateur);
            }else{
                strcat(resultat, "\n");
            }
        }
        if(fputs(resultat,dest)== EOF ){
            perror("ecriture dans destination");
            exit(1);
        }
    }
    free(res);
    free(separateur);
    free(resultat);
    free_memfichier(memoire, memoire->nbr_champs, memoire->nbr_ligne);
    fclose(dest);
}

