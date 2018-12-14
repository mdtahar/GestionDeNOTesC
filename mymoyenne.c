#include "union.h"
#include "structure.h"
#include "formater.h"

typedef struct{
	double* tab;
	int taille;
} Formule;

Formule* allouer_Formule(int taille){
	Formule *f = malloc(sizeof(Formule));
	// f->tab = malloc(taille *sizeof(double));
	f->taille = taille;

	return f;
}

/*former la formule à applicuer lors de la moyenne */
Formule* formule(char *formule){
	char sep[]="*+";
	double *res = malloc(strlen(formule) * sizeof(double));
	char *token;
	token = strtok(formule,sep);
	int i = 0;
	double d;
	while( token != NULL){
		// printf("aaaaaa%s \n",token );
		sscanf(token,"%lf",&d);
		res[i] = d;
		// printf("%.2f \n",res[i] );  // pour afficher deux chiffres je sais pas comment les stoker comme tel
		token = strtok(NULL,sep);
		i++;
		// printf("i  : %d \n",i );
	}
	Formule *f = allouer_Formule(i);
	f->tab = res;

	// free(res);
	return f;
}
void moyenne(char* fichier,char* nom_fichier_sortie,char *f, char sep, unsigned short bit_moyenne){
	mem_fichier* memoire = stockage_fichier(fichier);
	Formule *formule_tab = formule(f);




		char *file_out = "resultat_moyenne.csm";
		unsigned short bit_file = 0x1;
		unsigned short bit_sep = 0x2;
		if( !( bit_moyenne & bit_file) )
			strcat(nom_fichier_sortie, file_out );
		file_out = nom_fichier_sortie;
		// free(nom_fichier_sortie);



		char separateur [2]="\0";
		if(bit_moyenne & bit_sep){
			separateur[0]=sep;
		}
		else{
			// char * resultat = malloc(LIGNE_MAX*sizeof(char));
			strcpy(separateur, memoire->separateur);
		}

	char **tab_moyenne;
	tab_moyenne = malloc(memoire->nbr_ligne*(sizeof(*tab_moyenne)));
	if(tab_moyenne==NULL){
		perror("error allocation ");
		exit(1);
	}
	for(int i =0;i<memoire->nbr_ligne;i++){
		tab_moyenne[i] = malloc(10*sizeof(char));
		if(tab_moyenne[i] == NULL){
			for(int j=0;j<i;j++){
				free(tab_moyenne[j]);
			}
			free(tab_moyenne);
			perror("allocation");
			exit(1);
		}
	}

	char *res = malloc(10*sizeof(char));
	for(int i=0;i<memoire->nbr_ligne;i++){
		double num = 0,denom = 0,moyenne = 0;
		int bool_abs = 1;
		for(int j=0;j<formule_tab->taille-1;j+=2){
			if(strstr(memoire->tab[i][ (int)( formule_tab->tab[j] ) ],"abs") != NULL){
				continue;
			}
			if(strcmp(memoire->tab[i][ (int)( formule_tab->tab[j] ) ]," ") == 0 ){
				num+= 0;
			}
			else {
				num += atoi(memoire->tab[i][ (int)( formule_tab->tab[j] ) ] )*formule_tab->tab[j+1];
			}
			bool_abs = 0;
			denom += formule_tab->tab[j+1];
		}
		if(bool_abs == 0){
			moyenne = num/denom;
			sprintf(res,"%f",moyenne);
			// printf("%lf \n",moyenne );
		}
		else {
			strcpy(res,"abs");
			// printf("abs \n");
		}
		strcpy(tab_moyenne[i],res);
		// tab_moyenne[i] = res;
		// printf("%s \n",tab_moyenne[i] );
	}
	free(res);
	free(formule_tab->tab);
	free(formule_tab);
	// free(dest);
	/*for(int j=0;j<memoire->nbr_ligne;j++){
		free(tab_moyenne[j]);
	}*/

	FILE *f_in = fopen(fichier,"r");
	FILE *dest = fopen(nom_fichier_sortie,"w");
	char buf[LIGNE_MAX];
	int i = 0;
	while(fgets(buf,LIGNE_MAX,f_in)!=NULL){
      	if(buf[0]=='#'){
        	continue;
      	}
      	if(isalnum(buf[0])==0){
        	continue;
      	}
      	if(buf[strlen(buf)-1] == '\n'){
      		buf[strlen(buf)-1] = separateur[0];
        	strcat(buf,tab_moyenne[i]);
        	// printf("%s ---\n",tab_moyenne[i] );
        	strcat(buf,"\n");
        	i++;
    	}
    	// printf("%s\n", buf);
      	if(fputs(buf,dest)== EOF ){
          perror("ecriture dans destination");
          exit(1);
        }

    }
		for (size_t i = 0; i < memoire->nbr_ligne; i++) {
			free(tab_moyenne[i]);
		}
		free(tab_moyenne);
    free_memfichier(memoire, memoire->nbr_champs, memoire->nbr_ligne);
    fclose(f_in);
		fclose(dest);

}
/*Si c'est pair ça sera l'indice de la case si c'est impair ça sera le coefficient */
