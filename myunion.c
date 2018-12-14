
#include "union.h"
#include "formater.h"
#include "structure.h"
/*
FILE* ouverture_fichier(char* nom_fichier)
fait l'ouvrture du fichier donné en parametre

*/
FILE* ouverture_fichier(char* nom_fichier)
{
  FILE* fichier;
  if ( (fichier = fopen(nom_fichier, "r")) == NULL)
  {
    perror("erreur ouverture_fichier ");
    exit (1);
  }
  else
  return fichier;
}

void copy_fic(char *src, FILE* fdst){

  FILE *fsrc = ouverture_fichier(src);

  char buf[LIGNE_MAX];
  while((fgets(buf,LIGNE_MAX,fsrc)) != NULL ){

    if(isalnum(buf[0])==0)
    {
      continue;
    }

    if(fputs(buf,fdst) == EOF){
      perror ("fputs");
    }
  }
  fclose(fsrc);
}
void myunion(int nbr_fichier,char** tableau_fichier,char separateur,char* nom_fichier_sortie,unsigned short bit_union)
{
  char *file_out = "resultat.csm";
  char sep =';';
  unsigned short bit_file = 0x1;
  unsigned short bit_sep = 0x2;
  if( !( bit_union & bit_file) )
    strcat(nom_fichier_sortie, file_out );
  file_out = nom_fichier_sortie;
  // free(nom_fichier_sortie);
  if(bit_union & bit_sep){
    sep = separateur;
  }
  char chaine_sepa[2] = "\0";
  chaine_sepa[0]=sep;
  // int compteur = 1;
  FILE *fout=NULL;
  // FILE* ftmp;
  mem_fichier* new_memoire;
  int nbr_champs = nb_champs(tableau_fichier[0]);
  fout = fopen(file_out,"w");
  new_memoire = stockage_fichier(tableau_fichier[0]);
  copie_memoire_fichier(chaine_sepa, new_memoire, fout);
  free_memfichier(new_memoire, nbr_champs, new_memoire->nbr_ligne );

  for (size_t i = 1; i < nbr_fichier; i++) {
    // ftmp = ouverture_fichier(tableau_fichier[compteur]);
    if(nb_champs(tableau_fichier[i])!=nbr_champs){
      printf("Nombre de champs differents ! arret du programme \n");
      exit(1);
    }
    new_memoire = stockage_fichier(tableau_fichier[i]);
    copie_memoire_fichier(chaine_sepa, new_memoire, fout);

    free_memfichier(new_memoire, new_memoire->nbr_champs, new_memoire->nbr_ligne );
    // compteur++;

    /*rewind(fout);
    fclose(fout);*/
  }
  rewind(fout);
  fclose(fout);
}
/*void myunion(int nbr_fichier,char *fichier,...){
int nbr_champs = nb_champs(fichier);
int compteur = 1;
FILE *fout = fopen("resultat.csm","w");
FILE* ftmp;
va_list ap;
va_start(ap,fichier);
mem_fichier* new_memoire = stockage_fichier(fichier);

copie_memoire_fichier(";",new_memoire,fout);
free_memfichier(new_memoire, nbr_champs, new_memoire->nbr_ligne );

char* fichier_tmp;

while(compteur <nbr_fichier){
fichier_tmp = va_arg(ap,char*);
ftmp = ouverture_fichier(fichier_tmp);

if(nb_champs(fichier_tmp)!=nbr_champs){
printf("Nombre de champs differents ! arret du programme \n");
exit(1);
}
new_memoire = stockage_fichier(fichier_tmp);
copie_memoire_fichier(";",new_memoire,fout);
free_memfichier(new_memoire, new_memoire->nbr_champs, new_memoire->nbr_ligne );
compteur++;

fclose(ftmp);
}
rewind(fout);
fclose(fout);
va_end(ap);
}
*/
void affiche(FILE *file){
  char buf[LIGNE_MAX];
  while((fgets(buf,LIGNE_MAX,file))!=NULL){
    printf("%s ",buf );
  }
  printf("\n");
}
