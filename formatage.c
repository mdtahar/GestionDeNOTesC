#include <unistd.h>
#include <dirent.h>
#include "union.h"
#include "structure.h"
#include "formater.h"
#include "tri.h"
int main(int argc, char *argv[])
{

    if(argc<2)
    {
      perror("la commande fusion sans arguments n'est pas suffisasnte\n ");
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
    char* nom_fichier_sortie = malloc(LIGNE_MAX*sizeof(char));
    if (nom_fichier_sortie == NULL)
    {
      perror("erreur d'allocation");
      exit(2);
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
    int trouve_y =0;
    while ((opt = getopt(argc, argv, "o:h")) != -1) {
      switch (opt) {
        case 'o':   strcpy(nom_fichier_sortie, optarg);
        bit_fichier = 0x1;
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

        while ((file_read = readdir(rep)) != NULL && trouve_y==0)
        if (strcmp(nom_fichier_sortie, file_read->d_name)==0)
        {
          trouve_y = 1;
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

        break;
        case 'h' :
        printf("| Aide  | -----%s : \n"
        "Option obligatoire ---------------- : AUCUNE \n"
        "Options  facultatives : ------ ' -o [fichier_sorti ]' : permet de specifier le fichier de sortie (.csm )\n"
        "------- '-s [c]' : pour specifier le seperateur c \n"
        "------ '-h' AIDE \n",argv[0]);
        return 0;
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
    }else{
      perror("manque fichier d'entree\n");
      exit(14);
    }
    strcat(cwd_fichier, fichier_a_tester);
    strcpy(fichier_a_tester, cwd_fichier);
    formatage(fichier_a_tester, cwd, bit_fichier);
    free(nom_fichier_sortie);
    free(fichier_a_tester);
    closedir(rep);  
	return 0;
}
