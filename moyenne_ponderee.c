#include "union.h"
#include "structure.h"
#include "formater.h"
#include "moyenne.h"
#include <unistd.h>
#include <dirent.h>
int main(int argc, char *argv[])
{
    if(argc<3)
    {
        perror("la commande moyenne sans arguments n'est pas suffisasnte\n ");
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
    char *separateur  = malloc(sizeof(char)*2);
    if (separateur == NULL)
    {
        perror("erreur d'allocation");
        exit(3);
    }
    char *formule  = malloc(sizeof(char)*LIGNE_MAX);
    if (formule == NULL)
    {
        perror("erreur d'allocation");
        exit(3);
    }
    DIR* rep = NULL;
    struct dirent* file_read =NULL;
    rep = opendir(cwd);
    if(rep==NULL)
    {
        perror("error lors de l'ouverture du dossier");
        exit(7);
    }
    char touche;
    int trouve =0;
    while ((opt = getopt(argc, argv, "o:s:hm:")) != -1) {
        switch (opt) {
            case 'o':   strcpy(nom_fichier_sortie, optarg);
                bit_fichier = 0x1;
                
                break;
            case 's' : strcpy(separateur, optarg);
                bit_separateur = 0x2;
                break;
            case 'm' : strcpy(formule, optarg);
                break;
            case 'h' :
                printf("| Aide  | -----%s : \n"
                       "Option obligatoire ---------------- : -m sera suivi par la formule  \n"
                       "moyenne_ponderee  -m formule  [fichier] \n"
                       "Options  facultatives : ------ ' -o [fichier_sorti ]' : permet de specifier le fichier de sortie (.csm )\n"
                       "------- '-s [c]' : pour specifier le seperateur c \n"

                       "------ '-h' AIDE \n",argv[0]);
                free(separateur);
                closedir(rep);
                return 0;
        }
    }
    
    
    if(bit_fichier){
        int taille_fichier = strlen(nom_fichier_sortie);
        if (taille_fichier > 5 && (nom_fichier_sortie[taille_fichier-1]=='m')
            && (nom_fichier_sortie[taille_fichier-2]=='s')
            && (nom_fichier_sortie[taille_fichier-3]=='c')
            && (nom_fichier_sortie[taille_fichier-4]=='.') )
        {
            strcat(cwd, nom_fichier_sortie);
        }else {
            strcat(nom_fichier_sortie, ".csm");
            strcat(cwd, nom_fichier_sortie);
        }
    }
    
    
    
    
    char cwd_fichier[1024];
    char* fichier_a_tester = malloc(LIGNE_MAX*sizeof(char));
    if(fichier_a_tester==NULL)
    {
        perror("error alloc");
        exit(9);
    }
    if (getcwd(cwd_fichier, 1024*sizeof(char)) == NULL)
    {
        perror("getcwd() error");
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
                touche = getchar();
                if (touche == 'y')
                {
                    break;
                }else{
                    printf("Merci et à tres bientôt \n" );
                    exit(8);
                }
            }
    }
    else{
        strcpy(fichier_a_tester, copie_fichier(stdin));
    }
    
    
    
    
    
    char car_sep = separateur[0];
    
    moyenne(fichier_a_tester, cwd, formule, car_sep, bit_separateur|bit_fichier);
    free(separateur);
    free(nom_fichier_sortie);
    free(fichier_a_tester);
    free(formule);
    closedir(rep);
    return 0;
}
