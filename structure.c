#include "union.h"
#include "structure.h"
#include "formater.h"

/*
 int nb_champs(FILE* fichier)
 compte et renvoie le nombre de champs d'un fichier
 */
char* copie_fichier(FILE* fichier)
{
    char buf[LIGNE_MAX];
    FILE *f = fopen("fichier_tmp.txt","w");
    
    while(fgets(buf,LIGNE_MAX,fichier)!=NULL){
        if(fputs(buf,f)== EOF ){
            perror("fputs ");
        }
    }
    // rewind(f);
    fclose(f);
    return "fichier_tmp.txt";
}
int nb_champs(char* fichier)
{
    
    formater(fichier);
    
    FILE *f = ouverture_fichier(fichier);
    int compteur = 0;
    char buf[LIGNE_MAX];
    char c = separateur(f);
    while(fgets(buf,LIGNE_MAX,f)!=NULL){
        if(buf[0]=='#'){
            continue;
        }
        if(strlen(buf)<=2){
            continue;}
        for(int i =0;i<strlen(buf);i++){
            if(buf[i]==c){
                compteur++;
            }
            if(buf[i]=='\n'){
                fclose(f);
                return compteur+1;
            }
        }
    }
    rewind(f);
    fclose(f);
    return compteur+1;
}
int nb_ligne(char* fichier){
    formater(fichier);
    FILE *f = ouverture_fichier(fichier);
    int compteur = 0;
    char buf[LIGNE_MAX];
    while(fgets(buf,LIGNE_MAX,f)!=NULL){
        if(isalnum(buf[0])!=0){
            compteur++;
        }
    }
    fclose(f);
    return compteur;
}
/*
 int comparer_nb_champs(int nb_param,char* fichier,...)
 compare le nombre de champs de champs de chaque fichier des nb_param donnés
 renvoie 0 si le nombre de champs sont differents
 et 1 si ils ont tous le meme nombre de champs
 
 */
int comparer_nb_champs(int nb_param,char* fichier,...){
    if(nb_param <2){
        printf("nbr fichier insufisant ... \n");
        return 0;
    }
    int compteur = 1;
    int n = nb_champs(fichier);
    va_list ap;
    va_start(ap,fichier);
    char* tmp;
    while(compteur <= nb_param){
        
        tmp = va_arg(ap,char*);
        // ftmp = ouverture_fichier(tmp);
        printf("ouverture fichier %s\n",tmp );
        if(nb_champs(tmp) != n){
            // printf(" b %d \n",n );
            printf("nbr champs differents \n" );
            va_end(ap);
            return 0;
        }
        compteur++;
        // fclose(ftmp);
    }
    va_end(ap);
    // printf("nbr champs kifkif \n" );
    return 1;
}

mem_fichier* allouer_memfichier(int nbr_champs,int nbr_ligne){
    mem_fichier *mf = malloc(sizeof(mem_fichier));
    mf->nbr_champs = nbr_champs;
    mf->nbr_ligne = nbr_ligne;
    mf->separateur= malloc(3*sizeof(char));
    
    mf->tab = malloc(nbr_ligne*sizeof(*(mf->tab)));
    if(mf->tab == NULL){
        perror("allocation echoué");
        exit(0);
    }
    for(int i=0 ;i<nbr_ligne;i++){
        (mf->tab)[i] = malloc(nbr_champs * sizeof(*(mf->tab[i])));
        if((mf->tab)[i] == NULL){
            for(int j=0;j<i;j++){
                free((mf->tab)[j]);
                perror("allocation echoué");
                exit(0);
            }
        }
        for(int j=0;j<nbr_champs;j++){
            (mf->tab)[i][j] = calloc(LIGNE_MAX,sizeof(char));
        }
    }
    
    return mf;
}
void free_memfichier(mem_fichier *mf, int nbr_champs, int nbr_ligne){
    for(int i=0;  i < nbr_ligne; i++){
        for(int j=0; j < nbr_champs; j++){
            free((mf->tab)[i][j]);
        }
        free((mf->tab)[i]);
    }
    free(mf->tab);
    free(mf->separateur);
    free(mf);
}
void affiche_memfichier(mem_fichier *mf){
    for(int i=0; i< mf->nbr_ligne;i++){
        for(int j=0;j<mf->nbr_champs;j++){
            printf("%s ",(mf->tab)[i][j] );
        }
    }
}

void copie_memoire_fichier(char* separateur, mem_fichier* source, FILE* dest){
    
    char* resultat = malloc(LIGNE_MAX*sizeof(char));
    if (resultat == NULL)
    {
        perror("erreur d'allocation");
        exit(1);
    }
    for(int i=0;i<source->nbr_ligne;i++){
        for(int j=0;j<source->nbr_champs;j++){
            
            strcpy(resultat,(source->tab[i][j]));
            if(j != source->nbr_champs-1){
                strcat(resultat,separateur);
                
            }
            else {
                strcat(resultat,"\n");
            }
            if(fputs(resultat,dest)== EOF ){
                perror("ecriture dans destination");
                free(resultat);
                exit(1);
            }
            
        }
    }
    free(resultat);
    
}
char separateur(FILE* fichier){
    char* c = malloc(3*sizeof(char));
    char car;
    char buf[LIGNE_MAX];
    while(fgets(buf,LIGNE_MAX,fichier)!=NULL){
        if(buf[0]=='#'){
            continue;
        }
        if(isalnum(buf[0])!=0){
            free(c);
            rewind(fichier);
            return ';';
            // break;
        }
        else {
            strcpy(c,buf);
            break;
        }
    }
    car = c[0];
    free(c);
    rewind(fichier);
    return car;
}
mem_fichier* stockage_fichier(char* fichier){
    int nbr_ligne = nb_ligne(fichier);
    int nbr_champs = nb_champs(fichier);
    mem_fichier* m = allouer_memfichier(nbr_champs,nbr_ligne);
    
    FILE *f = ouverture_fichier(fichier);
    char buf[LIGNE_MAX];
    char* s;
    char sep =  separateur(f);
    char chaine_sepa[2] = "\0";
    chaine_sepa[0]=sep;
    strcpy(m->separateur,chaine_sepa);
    // free(sep);
    // free(tmp_sep);
    int compteur = 0;
    while(fgets(buf,LIGNE_MAX,f)!=NULL){
        if(buf[0]=='#'){
            continue;
        }
        if(isalnum(buf[0])==0){
            continue;
        }
        s = strtok(buf,chaine_sepa);
        int i = 0;
        
        while(s != NULL){
            if(s[strlen(s)-1] == '\n'){
                s[strlen(s)-1] = '\0';
            }
            strcpy((m->tab)[compteur][i],s);
            s = strtok(NULL,chaine_sepa);
            i++;
        }
        compteur++;
        
    }
    fclose(f);
    return m;
}
