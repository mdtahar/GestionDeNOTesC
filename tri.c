#include "union.h"
#include "formater.h"
#include "structure.h"
#include "tri.h"
#include <unistd.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    if(argc<2)
    {
        perror("la commande union sans arguments n'est pas suffisasnte\n ");
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
    extern int opterr;
    opterr =0;
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
    int taille_fichier = 0;
    int numerique=0;
    int bool_valeur=0;
    while ((opt = getopt(argc, argv, "o:s:h:n")) != -1) {
        
        switch (opt) {
            default:
                bool_valeur=1;
                break;
            case 'o':   strcpy(nom_fichier_sortie, optarg);
                bit_fichier = 0x1;
                break;
            case 's' : strcpy(separateur, optarg);
                bit_separateur = 0x2;
                break;
                
            case 'n' :
                numerique=1;
                break;
            case 'h' :
                printf("| Aide  | -----%s : \n"
                       "Option obligatoire ---------------- : -c indique le tri doit se faire avec les valeurs du champs c  \n"
                       "tri -c [-n]  [fichier]  \n "
                       "Options  facultatives : ------ ' -o [fichier_sorti ]' : permet de specifier le fichier de sortie (.csm )\n"
                       "----- '-n' : pour indiquer que le tri doit se faire par les valeurs numeriques"
                       "------- '-s [c]' : pour specifier le seperateur c \n"
                       "------ '-h' AIDE \n",argv[0]);
                free(separateur);
                return 0;
        }
    }
    if(bool_valeur ==0)
    {
        perror("vous n'avez pas ecrit l'arguments pour la valeur");
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
        }
    }
    char cwd_fichier[1024];
    
    if (getcwd(cwd_fichier, 1024*sizeof(char)) == NULL)
    {
        perror("getcwd() error");
        exit(6);
    }
    char* fichier_a_tester = malloc(LIGNE_MAX*sizeof(char));
    if(fichier_a_tester==NULL)
    {
        perror("error alloc");
        exit(9);
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
                    exit(8);
                }
            }
    }
    else{
        strcpy(fichier_a_tester, copie_fichier(stdin));
    }
    char car_valeur;
    if (argv[1][0]=='-')
    {
        car_valeur = argv[1][1];
    }
    int valeur = car_valeur-'0';
    char car_sep = separateur[0];
    mytri(numerique,  fichier_a_tester, valeur, car_sep, cwd, bit_fichier|bit_separateur);
    closedir(rep);
    free(nom_fichier_sortie);
    free(fichier_a_tester);
    free(separateur);
    return 0;
}

