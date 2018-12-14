#ifndef UNION_H
#define UNION_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define LIGNE_MAX 1024
FILE* ouverture_fichier(char* nom_fichier);
void copy_fic(char *fsrc, FILE *fdst);
void myunion(int nbr_fichier,char** tableau_fichier,char separateur,char* nom_fichier_sortie,unsigned short bit_union);
// void myunion(int nbr_fichier,char *fichier,...);
void affiche(FILE *file);

#endif
