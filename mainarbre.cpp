#include <stdio.h>
#include <stdlib.h>
#include "arbre.h"


int menu (Arbre* arbre) {
  printf ("\n\n ARBRES BINAIRES \n\n");
  printf (" 0 - Fin du programme\n");
  printf ("\n");
  printf (" 1 - Creation de l'arbre genealogique\n");
  printf (" 2 - Creation de l'arbre de l'expression arithmetique\n");
  printf ("\n");
  printf (" 3 - Parcours prefixe\n");
  printf (" 4 - Parcours infixe\n");
  printf (" 5 - Parcours postfixe\n");

  printf (" 6 - Trouver un Noeud\n");
  printf (" 7 - Parcours en largeur\n");
  printf (" 8 - Taille de l'arbre\n");
  printf (" 9 - estfeuille\n");
  printf ("10 - nombre des feuilles\n");
  printf ("11 - liste des feuilles\n");
  printf ("12 - Hauteur de l'arbre \n");
  printf ("13 - egalite entre deux arbres\n");
  printf ("14 - Prefixe with Goal\n");
  printf ("15 - Prefixe with limit\n");
  printf("16 - Prefixe iterative with limit\n");

  fprintf (stderr, "Votre choix ? ");
  int cod; scanf ("%d", &cod); getchar();
  printf ("\n");
  return cod;
}


int main () {
  Arbre* arbre ;
  booleen fini = faux;
  char * s = "Sarah";
  int niveau = 2    ;


  while (!fini) {
    switch (menu (arbre) ) {
    case 0:
      fini = vrai;
      break;
    case 1:
      printf ("Creation de l'arbre genealogique\n");
      arbre = creerArbreGene();
      break;
    case 2:
      printf ("Creation de l'arbre de l'expression arithmetique\n");
      arbre = creerArbreExp();
      break;

    case 3: {
      printf ("Parcours prefixe\n");
      prefixe (arbre);
      } break;
    case 4:
      printf ("Parcours infixe\n");
      infixe (arbre);
      break;
    case 5:
      printf ("Parcours postfixe\n");
      postfixe (arbre);
      break;

    case 6:
      printf ("Entez un nom\n");
      char nom[20]; scanf ("%s", nom);
      if (trouverNoeud(arbre, nom))
        printf ("le nom est dans l'arbre");
      else
        printf ("le nom n'est pas dans l'arbre");
      break;
    case 7:
      printf ("Parcours en largeur\n");
      enLargeur (arbre);
      break;
    case 8:
      printf ("La taille de l'arbre est : %d \n", taille (arbre));
      break;
    case 9:
      if (estFeuille(arbre->racine))
        printf ("Le noeud testé est une feuille\n");
      else
        printf ("Le noeud testé n'est pas une feuille\n");
      break;
      break;
    case 10:
      printf ("Le nombre des feuilles est : %d \n", nbFeuilles (arbre));
      break;
    case 11:
      listerFeuilles (arbre);
      break;
    case 12:
      printf ("La hauteur de l'arbre est : %d \n", hauteur (arbre));
      break;
    case 14:

        prefixe_goal(arbre,s);
        break;
    case 15:
        prefixe_with_limit(arbre, niveau);
        break;
    case 16:
        prefixe_iterative_with_limit(arbre, niveau);
        break;
    case 13:
        Arbre* arbre2 = creerArbreGene();
        if (egaliteArbre (arbre, arbre2))
            printf ("Les arbres testés sont egaux");
          else
            printf ("Les arbres testés ne sont pas egaux");
        break;

    }  // switch
    if (!fini) {
      printf ("\n\nTaper Return pour continuer\n"); getchar();
    }
  } // while
  return 0;
}
