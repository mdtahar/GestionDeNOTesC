#include "union.h"
#include "formater.h"
#include "structure.h"
#include <unistd.h>
#include <dirent.h>
char* copie_fichier(FILE* fichier);

/*
 GERERER LES ESPACES DANS LA COMPARAISON
 */
int nbr_parametre(char* parametre);
typedef struct Champ
{
    char* champs;
    int trouver;
}Champ;
Champ* allouer_Champ(){
    Champ* c = malloc(sizeof(Champ));
    c->champs = malloc(3*sizeof(char));
    c->trouver = 0;
    return c;
}
void free_Champ(Champ* s){
    free(s->champs);
    free(s);
}

Champ* remplire_Champ(char *champs){
    Champ* new_c = allouer_Champ();
    new_c->champs = champs;
    return new_c;
}

void myselect(char *fichier,char* parametre, char sep, char* nom_fichier_sortie, unsigned short bit_select){
    char *file_out = "resultat_select.csm";
    unsigned short bit_file = 0x1;
    unsigned short bit_sep = 0x2;
    if( !( bit_select & bit_file) )
        strcat(nom_fichier_sortie, file_out );
    file_out = nom_fichier_sortie;
    int nb_parametre = nbr_parametre(parametre);
    char **tab_parametre;
    tab_parametre = malloc(nb_parametre*sizeof(tab_parametre));
    if(tab_parametre==NULL){
        perror("allocation echoué ");
        exit(1);
    }
    for(int i = 0;i<nb_parametre;i++){
        tab_parametre[i] = malloc(LIGNE_MAX*sizeof(char));   ///////// / ////////////////LIGNE_MAX
        if(tab_parametre[i]==NULL){
            for(int j=0;j<nb_parametre;j++){
                free(tab_parametre[j]);
            }
            free(tab_parametre);
            perror("allocation echoué ");
            exit(1);
        }
    }
    int i =0;
    char *s = strtok(parametre,",");
    while(s!=NULL){
        strcpy(tab_parametre[i],s);
        s = strtok(NULL,",");
        i++;
    }
    mem_fichier* ma_memoire = stockage_fichier(fichier);
    Champ *tableau[ma_memoire->nbr_champs];
    FILE* fout = NULL;
    fout = fopen(file_out,"w");
    int k;
    for(int i=0;i<ma_memoire->nbr_ligne;i++){
        k = 0;
        for(int j=0;j<ma_memoire->nbr_champs;j++){
            tableau[k] = remplire_Champ(ma_memoire->tab[i][j]);
            k++;
        }
        int z = 0;
        int booleen = 0;
        
        while(z<nb_parametre){
            booleen = 0;
            for(int n=0;n<k;n++){
                if(strcmp(tab_parametre[z],tableau[n]->champs)==0){
                    if(tableau[n]->trouver==0){
                        tableau[n]->trouver =1;
                        booleen = 1;
                        break;
                    }
                }
            }
            if(booleen==0){
                break;
            }
            if(booleen==1){
                z++;
            }
        }
        if(booleen==1){
            char *separateur = malloc(2*sizeof(char));
            char resultat[LIGNE_MAX];
            if(bit_select & bit_sep){
                separateur[1] = '\0';
                separateur[0]=sep;
            }
            else{
                // char * resultat = malloc(LIGNE_MAX*sizeof(char));
                strcpy(separateur,ma_memoire->separateur);
                separateur[1] = '\0';
            }
            resultat[0] = '\0';
            for(int n=0;n<ma_memoire->nbr_champs;n++){
                strcat(resultat,(ma_memoire->tab[i][n]));
                if(n!=ma_memoire->nbr_champs-1){
                    strcat(resultat,separateur);
                }
                else
                {
                    strcat(resultat,"\n");
                }
            }
            if(fputs(resultat, fout)==EOF)
            {
                perror("write in file");
                free(separateur);
                exit(3);
            }
            free(separateur);
        }
    }
    // free(separateur);
    
    fclose(fout);
    free_memfichier(ma_memoire, ma_memoire->nbr_champs, ma_memoire->nbr_ligne);
    for(int j=0;j<nb_parametre;j++){
        free(tab_parametre[j]);
    }
    free(tab_parametre);
}
int nbr_parametre(char *chaine){
    char *c = malloc((strlen(chaine)+1)*sizeof(char));
    strcpy(c, chaine);
    char *s = strtok(c,",");
    int compteur = 0;
    while(s!=NULL){
        s = strtok(NULL,",");
        
        compteur++;
    }
    free(c);
    return compteur;
}

int main(int argc, char *argv[])
{
    if(argc<3)
    {
        perror("la commande select sans arguments n'est pas suffisasnte\n ");
        exit(1);
    }
    char cwd[1024];
    if (getcwd(cwd, 1024*sizeof(char)) == NULL)
    {
        perror("getcwd() error");
        exit(6);
    }
    strcat(cwd,"/CSM/");
    int opt;
    unsigned short bit_fichier = 0x0;
    unsigned short bit_separateur = 0x0;
    char* nom_fichier_sortie = malloc(LIGNE_MAX*sizeof(char));
    if (nom_fichier_sortie == NULL)
    {
        perror("erreur d'allocation");
        exit(2);
    }
    char *separateur  = malloc(sizeof(char)*3);
    if (separateur == NULL)
    {
        free(nom_fichier_sortie);
        perror("erreur d'allocation");
        exit(3);
    }
    DIR* rep = NULL;
    struct dirent* file_read =NULL;
    rep = opendir(cwd);
    if(rep==NULL)
    {
        free(nom_fichier_sortie);
        perror("error lors de l'ouverture du dossier");
        exit(7);
    }
    char* champs = malloc(LIGNE_MAX*sizeof(char));
    if(champs==NULL)
    {
        closedir(rep);
        free(nom_fichier_sortie);
        perror("error alloc");
        exit(9);
    }
    char touche;
    int trouve =0;
    int taille_fichier = 0;
    int bool_champs =0;
    
    while ((opt = getopt(argc, argv, "o:s:h:l:")) != -1) {
        switch (opt) {
            case 'o':   strcpy(nom_fichier_sortie, optarg);
                bit_fichier = 0x1;
                
                
                break;
            case 's' : strcpy(separateur, optarg);
                bit_separateur = 0x2;
                break;
                
            case 'l' :
                strcpy(champs, optarg);
                bool_champs=1;
                break;
                
            case 'h' :
                printf("| Aide  | -----%s : \n"
                       "Option obligatoire ---------------- : -l suivi de n1,...,nk  \n"
                       " La commande s ́electionne les champs n1,...,nk  \n"
                       "select -l  n1,...,nk   [fichier] \n"
                       "Options  facultatives : ------ ' -o [fichier_sorti ]' : permet de specifier le fichier de sortie (.csm )\n"
                       "------- '-s [c]' : pour specifier le seperateur c \n"
                       "------ '-h' AIDE \n",argv[0]);
                free(separateur);
                free(nom_fichier_sortie);
                closedir(rep);
                return 0;
        }
    }
    if(bool_champs ==0)
    {
        perror("vous n'avez pas ecrit l'arguments -l");
        free(nom_fichier_sortie);
        closedir(rep);
        exit(10);
    }
    if(bit_fichier){
        taille_fichier = strlen(nom_fichier_sortie);
        if (taille_fichier > 5 && (nom_fichier_sortie[taille_fichier-1]=='m')
            && (nom_fichier_sortie[taille_fichier-2]=='s')
            && (nom_fichier_sortie[taille_fichier-3]=='c')
            && (nom_fichier_sortie[taille_fichier-4]=='.') )
        {
            strcat(cwd, nom_fichier_sortie);
        }else {
            strcat(nom_fichier_sortie, ".csm");
            strcat(cwd, nom_fichier_sortie);
        }}
    char* fichier_a_tester = malloc(LIGNE_MAX*sizeof(char));
    if(fichier_a_tester==NULL)
    {
        perror("erreur aloc\n");
        exit(16);
    }
    char cwd_fichier[1024];
    
    if (getcwd(cwd_fichier, 1024*sizeof(char)) == NULL)
    {
        perror("getcwd() error");
        free(nom_fichier_sortie);
        closedir(rep);
        exit(6);
    }
    strcat(cwd_fichier,"/CSM/");
    if(argv[optind]!=NULL){
        strcpy(fichier_a_tester,argv[optind]);
        strcat(cwd_fichier, fichier_a_tester);
        strcpy(fichier_a_tester, cwd_fichier);
        
        
        while ((file_read = readdir(rep)) != NULL && trouve==0)
            if (strcmp(nom_fichier_sortie, file_read->d_name)==0)
            {
                trouve = 1;
                printf("voulez vous ecraser le fichier existant ? y pour valider autre touche pour annuler\n" );
                touche=getchar();
                if (touche == 'y')
                {
                    break;
                }else{
                    printf("Merci et à tres bientôt \n" );
                    free(nom_fichier_sortie);
                    closedir(rep);
                    exit(8);
                }
            }
        
    }else{
        
        strcpy(fichier_a_tester, copie_fichier(stdin));
    }
    
    char car_sep = separateur[0];
    myselect(fichier_a_tester, champs, car_sep, cwd, bit_fichier|bit_separateur);
    closedir(rep);
    free(fichier_a_tester);
    free(separateur);
    free(champs);
    free(nom_fichier_sortie);
    return 0;
}

