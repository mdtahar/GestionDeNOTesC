#ifndef TRI_H
#define TRI_H
typedef struct{
    double* tab;
    int taille;
} Formule;
Formule* formule(char *formule);
void moyenne(char* fichier,char* nom_fichier_sortie,char *formule, char sep, unsigned short bit_moyenne);
#endif
