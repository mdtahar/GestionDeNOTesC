#include "union.h"
#include "formater.h"
#include "structure.h"
#include "tri.h"
void fusion(char* formule,char* fichier1,char* fichier2,char car_sep,char* fichier_sorti, unsigned short bit_fusion){
	mem_fichier *mem_file1 = stockage_fichier(fichier1);
	mem_fichier *mem_file2 = stockage_fichier(fichier2);



char sep =';';
	char *file_out = "resultat_fusion.csm";
	unsigned short bit_file = 0x1;
	unsigned short bit_sep = 0x2;
	if( !( bit_fusion & bit_file) )
		strcat(fichier_sorti, file_out );
	file_out = fichier_sorti;
	// free(nom_fichier_sortie);




	if(bit_fusion & bit_sep){
		sep = car_sep;
	}
	char separateur[2] = "\0";
	separateur[0]=sep;
	int champ1 = formule[0]-'0';
	int champ2 = formule[2]-'0';


	char *resultat = malloc(LIGNE_MAX*sizeof(char));
	int k=0;
	int *tri_file1 = tri_ligne(0,mem_file1,champ1);
	int *tri_file2  = tri_ligne(0,mem_file2,champ2);
	FILE *f = fopen(fichier_sorti,"w");

	int cmpt =0;
	int boolean = 0;
	int *tab = malloc(mem_file2->nbr_ligne*sizeof(int));
	int taille_tab = 0;
	for(int i = 0;i<mem_file1->nbr_ligne;i++){
		boolean = 0;
		resultat[0] = '\0';
		for(int j=cmpt;j<mem_file2->nbr_ligne;j++){
			if( strcmp ( mem_file1->tab[ tri_file1[i] ][champ1] , mem_file2->tab[ tri_file2[j] ][champ2] ) == 0){
				tab[taille_tab] = j;
				taille_tab++;
				boolean = 1;
				cmpt = j;

				for(k=0;k<mem_file1->nbr_champs;k++){              // dans le cas ou y'a pas le meme nmbr de champ on met deux boucle
					strcat(resultat,(mem_file1->tab[tri_file1[i]][k]));
					if(k!=mem_file1->nbr_champs-1){
						strcat(resultat,separateur);
					}
				}
				strcat(resultat,separateur);  // on met le separateur ;
				for(k=0;k<mem_file2->nbr_champs;k++){

					strcat(resultat,(mem_file2->tab[tri_file2[j]][k]));
					if(k!=mem_file2->nbr_champs-1){
						strcat(resultat,separateur);
					}
				}

			strcat(resultat,"\n");
			if(fputs(resultat,f)== EOF ){
				perror("ecriture dans destination");
				exit(1);
			}

			}

		}
		if(boolean ==0){
			for(k=0;k<mem_file1->nbr_champs;k++){
					strcat(resultat,(mem_file1->tab[tri_file1[i]][k]));
					if(k!=mem_file1->nbr_champs-1){
						// printf("%d nb = %d \n",k,mem_file2->nbr_champs );
						strcat(resultat,separateur);
					}
				}
				fprintf(stderr, "%s -- : %s\n",fichier1,resultat );
		}

	}
	for(int i = 0;i<mem_file2->nbr_ligne;i++){
		boolean = 0;
		resultat[0] = '\0';
		for(int j = 0;j<taille_tab;j++){
			if(tab[j] == i ){
				boolean = 1;
				break;
			}
		}
		if(boolean ==0){
			for(k=0;k<mem_file2->nbr_champs;k++){
					strcat(resultat,(mem_file2->tab[tri_file2[i]][k]));
					if(k!=mem_file2->nbr_champs-1){
						// printf("%d nb = %d \n",k,mem_file2->nbr_champs );
						strcat(resultat,separateur);
					}
				}
				fprintf(stderr, "%s -- : %s\n",fichier2,resultat );
		}
	}
	free_memfichier(mem_file2, mem_file2->nbr_champs, mem_file2->nbr_ligne);
	free_memfichier(mem_file1, mem_file1->nbr_champs, mem_file1->nbr_ligne);
	free(resultat);
	free(tab);
	// free(separateur);
	free(tri_file1);
	free(tri_file2);
	fclose(f);
}
