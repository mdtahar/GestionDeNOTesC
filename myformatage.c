#include "union.h"
#include "structure.h"
#include "formater.h"


int *tab_nbr_champs(char *fichier){
	formater(fichier);
	FILE *f = ouverture_fichier(fichier);
	int compteur = 0;
	char buf[LIGNE_MAX];
	char c = separateur(f);
	int i =0;
	int *tab_res = malloc(nb_ligne(fichier)*sizeof(int));
	int taille_tab =0;
	while(fgets(buf,LIGNE_MAX,f)!=NULL){
		compteur= 0;
		if(buf[0]=='#'){
			continue;
		}
		if(strlen(buf)<=2){
			continue;
		}
		for(i =0;i<strlen(buf);i++){
			if(buf[i]==c){
				compteur++;

			}
			if(buf[i]=='\n'){
				tab_res[taille_tab] = compteur+1;
				taille_tab++;
				// return compteur+1;
			}
		}
	}
	if(buf[i]!='\n'){
		tab_res[taille_tab] = compteur+1;
		taille_tab++;
	}

	rewind(f);
	fclose(f);

	return tab_res;
}
int max_tab(int *tab,int taille){
	int max = tab[0];
	for(int i=1;i<taille;i++){
		if(tab[i]>max)
		max = tab[i];
	}
	return max;
}

void formatage(char* fichier, char* fichier_sorti, unsigned short bit_formatage)
{
	char buf[LIGNE_MAX];
	int taille = nb_ligne(fichier);
	// int *tab = malloc(sizeof(taille*sizeof(int)));
	int* tab = tab_nbr_champs(fichier);
	int max = max_tab(tab,taille);
	unsigned short bit_file = 0x1;
	int i =0;
	FILE *f = fopen(fichier,"r");
	if(f==NULL){
		perror("ouverture fichier");
		exit(-1);
	}

	FILE *tmp_f = tmpfile();
	char sep_char = separateur(f);
	char sep[2]="\0";
	sep[0] = sep_char;
	while(fgets(buf,LIGNE_MAX,f)!=NULL){
		if(buf[strlen(buf)-1]=='\n' || buf[strlen(buf)-1]==EOF){
			if(buf[0]=='#'){
				continue;
			}
			if(strlen(buf)<=2){
				continue;
			}
			if(max == tab[i]){
				i++;
			}
			else {
				int difference = max - tab[i];
				buf[strlen(buf)-1] = '\0';
				for(int j = 0;j<difference;j++){
					strcat(buf,sep);
					strcat(buf," ");
				}
				strcat(buf,"\n");
				i++;

			}
			if(fputs(buf,tmp_f)== EOF ){
				perror("ecriture dans destination");
				exit(1);
			}
		}

	}
	fclose(f);
	FILE *f_in=NULL;
	char *file_out=fichier;
	if( !( bit_formatage & bit_file) ){
			f_in = fopen(file_out,"w");
}else{
		f_in = fopen(fichier_sorti,"w");
}

	fputs(sep,f_in);
	fputs("\n",f_in);
	rewind(tmp_f);
	while(fgets(buf,LIGNE_MAX,tmp_f)!=NULL){
		if(fputs(buf,f_in)== EOF ){
			perror("ecriture dans destination");
			exit(1);
		}
	}
	fclose(f_in);
	fclose(tmp_f);
 free(tab);
}
