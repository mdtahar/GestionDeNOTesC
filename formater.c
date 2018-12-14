#include "union.h"
char* suprim_blanc(char* c){
	char* resultat;
	int compteur = 0;
	// resultat = malloc(sizeof(resultat* (strlen(c)+1)));
	resultat = malloc(LIGNE_MAX*sizeof(char));
	int i,j;
	for(i = 0,j=0;i<strlen(c);i++){
		if(c[i] == ' '){
			if(compteur == 0){
				resultat[j] = c[i];
				compteur = 1;
				j++;
			}
		}
		else {
			resultat[j] = c[i];
			compteur = 0;
			j++;
		}
	}
	resultat[j] = '\0';
	while(resultat[0] == ' '){
		memmove(resultat,resultat+1,strlen(resultat)*sizeof(char));
	}
	return resultat;
}
void formater(char* fichier){
	FILE *f = fopen(fichier,"r");
	if(f==NULL){
      perror("erreur ouverture_fichier ");
      exit (1);
	}
	FILE *f_tmp = tmpfile();
	char buf[LIGNE_MAX];
	char *resultat = malloc(LIGNE_MAX*sizeof(char));
  	while(fgets(buf,LIGNE_MAX,f)!= NULL){
  		char *tmp = suprim_blanc(buf);
			strcpy(resultat, tmp);
			free(tmp);
  		if((fputs(resultat,f_tmp))== EOF ){
  			perror("fputs suprim_blanc");
				free(resultat);
				fclose(f_tmp);
  			exit(1);
  		}
  	}
  	fclose(f);
  	rewind(f_tmp);
  	FILE *fe = fopen(fichier,"w");
  	while(fgets(buf,LIGNE_MAX,f_tmp)!=NULL){
  		if(isspace(buf[0])==1){
  			continue;
  		}
  		if((fputs(buf,fe))== EOF ){
  			perror("nouvelle ecriture");
				free(resultat);
				fclose(f_tmp);
				exit(1);
  		}
  	}
		free(resultat);
  	rewind(fe);
  	fclose(fe);
  	fflush(f_tmp);
		fclose(f_tmp);
}
