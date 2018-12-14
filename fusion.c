#include "union.h"
#include "structure.h"
#include "formater.h"
#include "tri.h"
#include <unistd.h>
#include <dirent.h>
int main(int argc, char *argv[])
{
  if(argc<4)
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
  int trouve_y =0;
  int trouve = 0;
  while ((opt = getopt(argc, argv, "o:s:hc:")) != -1) {
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
      case 's' : strcpy(separateur, optarg);
      bit_separateur = 0x2;
      break;
      case 'c' : strcpy(formule, optarg);
      if (formule[1]!=':')
      {
        perror("formule fausse");
        exit(11);
      }
      trouve =1;
      break;
      case 'h' :
      printf("| Aide  | -----%s : \n"
             "Option obligatoire ---------------- : '-c' qui sera suivi de n:m"
             "tel que n est l'indice du champ du fichier 1  et m les champs du fichier2 \n"
             " fusion -c n:m fa fb \n"
      "Options  facultatives : ------ ' -o [fichier_sorti ]' : permet de specifier le fichier de sortie (.csm )\n"
      "------- '-s [c]' : pour specifier le seperateur c \n"
      "------ '-h' AIDE \n",argv[0]);
      free(separateur);
      return 0;
    }
  }
  char cwd_fichier[1024];

  char* fichier_a_testerA = malloc(LIGNE_MAX*sizeof(char));
  if(fichier_a_testerA==NULL)
  {
    perror("error alloc");
    exit(9);
  }
  char* fichier_a_testerB = malloc(LIGNE_MAX*sizeof(char));
  if(fichier_a_testerB==NULL)
  {
    perror("error alloc");
    exit(9);
  }
  char cwd_fichierB[1024];
  if (getcwd(cwd_fichier, 1024*sizeof(char)) == NULL)
  {
    perror("getcwd() error");
    exit(6);
  }
  strcat(cwd_fichier,"/CSM/");
  strcpy(cwd_fichierB, cwd_fichier);

  if(argv[optind]!=NULL){
    strcpy(fichier_a_testerA,argv[optind]);
    trouve++;
    optind++;
  }
  if(argv[optind]!=NULL){

    strcpy(fichier_a_testerB,argv[optind]);
    trouve++;
  }
  if(trouve!=3)
  {
    perror("pass assez d'argument");
    exit(12);
  }
  strcat(cwd_fichier, fichier_a_testerA);
  strcpy(fichier_a_testerA, cwd_fichier);
  strcat(cwd_fichierB, fichier_a_testerB);
  strcpy(fichier_a_testerB, cwd_fichierB);




  char car_sep = separateur[0];
  fusion(formule, fichier_a_testerA, fichier_a_testerB, car_sep, cwd, bit_separateur|bit_fichier);
  free(separateur);
  free(formule);
  free(nom_fichier_sortie);
  free(fichier_a_testerA);
  free(fichier_a_testerB);
  closedir(rep);
  return 0;
}
