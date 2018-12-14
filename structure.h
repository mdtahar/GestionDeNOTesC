#ifndef STRUCTURE_H
#define STRUCTURE_H
typedef struct mem_fichier
{
    int nbr_champs;
    int nbr_ligne;
    char *separateur;
    char *(**tab);
}mem_fichier;
void fusion(char* formule,char* fichier1,char* fichier2,char car_sep,char* fichier_sorti, unsigned short bit_fusion);
int nb_champs(char* fichier);
int nb_ligne(char* fichier);
int comparer_nb_champs(int nb_param,char* fichier,...);
mem_fichier* allouer_memfichier(int nbr_champs,int nbr_ligne);
void free_memfichier(mem_fichier *mf,int nbr_champs,int nbr_ligne);
void affiche_memfichier(mem_fichier *mf);
mem_fichier* stockage_fichier(char* fichier);
char separateur(FILE* fichier);
void copie_memoire_fichier(char* separateur, mem_fichier* source,FILE* dest);
void formatage(char* fichier, char* fichier_sorti, unsigned short bit_formatage);
char* copie_fichier(FILE* fichier);

#endif

