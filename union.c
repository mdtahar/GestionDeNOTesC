#include "union.h"
#include "formater.h"
#include "structure.h"
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
  unsigned short bit_fichier = 0x0;
  unsigned short bit_separateur = 0x0;
  char **tableau_fichier = (char**)malloc(argc*sizeof(char*));
  if (tableau_fichier==NULL) {
    perror("erreur lors de l'allocation");
    exit(4);
  }
  int taille = argc;
  for (size_t i = 0; i < argc; i++) {
    tableau_fichier[i]= (char*)malloc(sizeof(char)*LIGNE_MAX);
    if (tableau_fichier[i]==NULL) {
      for(int j =i-1; j>=0; j--)
      {
        free(tableau_fichier[j]);
      }
      free(tableau_fichier);
      perror("erreur d'allocation");
      exit(5);
    }
  }
  char* nom_fichier_sortie = malloc(LIGNE_MAX*sizeof(char));
  if (nom_fichier_sortie == NULL)
  {
    for (size_t i = 0; i < taille; i++) {
      free(tableau_fichier[i]);
      /* code */
    }
    free(tableau_fichier);
    perror("erreur d'allocation");
    exit(2);
  }
  char *separateur  = malloc(sizeof(char)*2);
  if (separateur == NULL)
  {
    for (size_t i = 0; i < taille; i++) {
      free(tableau_fichier[i]);
      /* code */
    }
    free(tableau_fichier);
    perror("erreur d'allocation");
    exit(3);
  }
  DIR* rep = NULL;
  struct dirent* file_read =NULL;
  rep = opendir(cwd);
  if(rep==NULL)
  {
    perror("error lors de l'ouverture du dossier");
    for (size_t i = 0; i < taille; i++) {
      free(tableau_fichier[i]);
      /* code */
    }
    free(tableau_fichier);
    exit(7);
  }
  char touche;
  int trouve =0;
  int taille_fichier = 0;
  while ((opt = getopt(argc, argv, "o:s:h")) != -1) {
    switch (opt) {
      case 'o':   strcpy(nom_fichier_sortie, optarg);
      bit_fichier = 0x1;
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

      while ((file_read = readdir(rep)) != NULL && trouve==0){
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
          for (size_t i = 0; i < taille; i++) {
            free(tableau_fichier[i]);
            /* code */
          }
          free(tableau_fichier);
          exit(8);
        }
      }
    }
      closedir(rep);

      break;
      case 's' : strcpy(separateur, optarg);
      bit_separateur = 0x2;
      break;
      case 'h' :
      printf("| Aide  | -----%s : \n"
      "Option obligatoire ---------------- : AUCUNE \n"
      "Options  facultatives : ------ ' -o [fichier_sorti ]' : permet de specifier le fichier de sortie (.csm )\n"
      "------- '-s [c]' : pour specifier le seperateur c \n"
      "------ '-h' AIDE \n",argv[0]);
      for (size_t i = 0; i < taille; i++) {
        free(tableau_fichier[i]);
        /* code */
      }
      free(tableau_fichier);
      free(separateur);
      return 0;
    }
  }
  int nbr_fichier = argc-optind;
  int i =0;
  char cwd_fichier[1024];

  while(argv[optind]!=NULL){
    if (getcwd(cwd_fichier, 1024*sizeof(char)) == NULL)
    {
      perror("getcwd() error");
      for (size_t i = 0; i < taille; i++) {
        free(tableau_fichier[i]);
        /* code */
      }
      free(tableau_fichier);
      exit(6);
    }
    strcat(cwd_fichier,"/CSM/");

    strcpy(tableau_fichier[i],argv[optind]);
    strcat(cwd_fichier, tableau_fichier[i]);
    strcpy(tableau_fichier[i], cwd_fichier);
    i++;
    optind++;
  }
  char car_sep = separateur[0];
  myunion(nbr_fichier, tableau_fichier, car_sep, cwd, bit_fichier|bit_separateur);
  for (size_t i = 0; i < taille; i++) {
    free(tableau_fichier[i]);
  }
  free(tableau_fichier);
  free(separateur);
  closedir(rep);
  free(nom_fichier_sortie);
}
