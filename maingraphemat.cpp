
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphemat.h"
#include "arbre.h"
#include "liregraphe.h"
#include<time.h>

typedef GrapheMat Graphe;


int menu () {

  printf("\t-------------------------------------------------\n");
  printf("\t|\t\t   Graphes \t\t\t| \n");
  printf("\t-------------------------------------------------\n");
  printf ("0 - Fin du programme\n");
    printf ("\n");
  printf ("1 - Creation d'un graphe  a partir d'un fichier\n");

  //printf ("2 - Initialisation d'un graphe vide\n");
  //printf ("3 - Ajout d'un sommet\n");
  //printf ("4 - Ajout d'un arc\n");
  //printf ("5 - Liste des sommets et des arcs\n");
  printf ("6 - Destruction du graphe\n");
  //printf ("7 - Parcours en profondeur d'un graphe\n");
  printf ("8  - Floyd \n");
  printf("9 - Parcours Largeur\n");
  printf("\t-------------------------------------------------\n");
  printf("\t|\t   Algorithmes d'Exploration \t\t| \n");
  printf("\t-------------------------------------------------\n");
  printf("10 - Recherche Cout uniform \n");
  printf("11 - Meilleur d'abord (GLOUTONNE)\n");
  printf("12 - A* \n");
  printf("13 - Profondeur limitee\n");
  printf("14 - Profondeur Iterative limitee");
  printf("\n100 - IDA*\n");
  printf("\t-------------------------------------------------\n");
  printf("\t|\t   Algorithmes d'optimisation \t\t| \n");
  printf("\t-------------------------------------------------");
  printf("\n101 - Plus proche voisin ");
  printf("\n102 - 2-OPT Simple");
  printf("\n103 - Escalade Simple  ");
  printf("\n104 - 2-OPT Complet");
  printf("\n105 - Escalade Complet ");
  printf("\n106 - Recuit Simulé ");
      printf ("\n");
  printf("\t-------------------------------------------------\n");
  printf("\t|\t   Reseaux de Neurons \t\t\t| \n");
  printf("\t-------------------------------------------------\n");
  printf("\n107 - Perceptron\n");
  printf("\n");
  printf("\t-------------------------------------------------\n");
  printf("\t|\t\t   Arbres \t\t\t| \n");
  printf("\t-------------------------------------------------\n");
  printf("15 - Creation de l'arbre genealogique\n");
  printf ("16 - Prefixe with Goal\n");
  printf ("17 - Prefixe with limit\n");
  printf("18 - Prefixe iterative with limit\n");
  printf ("\n");
  printf ("Votre choix ? ");
  int cod; scanf ("%d", &cod); getchar();
  printf ("\n");
  return cod;
}
int main () {
  Graphe* graphe;
  Arbre* arbre ;

    srand(time(NULL));
  char** pvc;
  char * s = "Sarah";
  int niveau = 2;
  booleen fini = faux;
  while (!fini) {
    switch ( menu() ) {
    case 0:
      fini = vrai;
      break;
    case 1: {  // création à partir d'un fichier
      printf ("Nom du fichier contenant le graphe ? ");
      char nomFe [50];
      //scanf  ("%s", nomFe);
      strcpy (nomFe, "meta_graph.txt");
//      strcpy (nomFe, "cours_graph.txt");
      FILE* fe = fopen (nomFe, "r");
      if (fe == NULL) {
        perror (nomFe);
      } else {
        graphe = lireGraphe (fe, 20); // 20 sommets maximum
        fclose (fe);
      }
      } break;
   case 2: {             // création d’un graphe vide
      printf ("Nombre maximum de sommets ? ");
      int nMaxSom; scanf  ("%d", &nMaxSom);
      printf ("0) non valué; 1) graphe valué ? ");
      int value; scanf ("%d", &value);

      graphe = creerGrapheMat (nMaxSom, value);

    } break;
    case 3: {  // ajouter un sommet
      printf ("Nom du sommet à insérer ? ");
      NomSom somD; scanf  ("%s", somD);
      ajouterUnSommet (graphe, somD);
    } break;
    case 4: {  // ajouter un arc
      printf ("Nom du sommet de départ ? ");
      NomSom somD; scanf  ("%s", somD);
      printf ("Nom du sommet d'arrivée ? ");
      NomSom somA; scanf  ("%s", somA);
      int cout;
      if (graphe->value) {
        printf ("Cout de la relation ? ");
        scanf ("%d", &cout);
      } else {
        cout = 0;
      }
      ajouterUnArc (graphe, somD, somA, cout);
    } break;
    case 5:
      ecrireGraphe (graphe);
      break;
    case 6:
      detruireGraphe (graphe);
      break;
    case 7:
      parcoursProfond (graphe);
      break;
    case 8:
     if (graphe->value) {
        printf ("\nLes plus courts chemins\n\n");
          floyd (graphe);
      } else {
        printf ("Graphe non valué\n");
      }
      break;
    case 9 :
    	largeur(graphe);
    	break;
    case 10 :{
       printf ("Nom du sommet a Chercher ? ");
        NomSom somD; scanf  ("%s", somD);
        cout_uniform(graphe,(char *)somD);
        break;
        }
    case 11:{
        printf ("Nom du sommet a Chercher ? ");
        NomSom somD; scanf  ("%s", somD);
        gloutonne(graphe, (char*)somD);
        break ;
    }
    case 12:{
        printf ("Nom du sommet a Chercher ? ");
        NomSom somD; scanf  ("%s", somD);
            a_etoile(graphe,(char*)somD);
            break;
            }
    case 13:{
            int niveau = 0;
            printf ("Entrer le niveau  ");
            scanf("%d",&niveau);
            profondeur_limitee(graphe,niveau);
            break;
    }
    case 14:
    {
            int niveau = 0;
            printf ("Entrer le niveau  ");
            scanf("%d",&niveau);
            profondeur_iterative_limitee(graphe,1);
            break;
    }

    case 15:
        printf ("Creation de l'arbre genealogique\n");
        arbre = creerArbreGene();

        break;
    case 16:
    {
        printf ("Nom du sommet a Chercher ? ");
        NomSom somD; scanf  ("%s", somD);
        prefixe_goal(arbre,(char*)somD);
        break;
    }
    case 17:
    {
        int niveau = 0;
        printf ("Entrer la limite  ");
        scanf("%d",&niveau);
        prefixe_with_limit(arbre, niveau);
        break;
    }
    case 18:{
        int niveau = 0;
        printf ("Entrer la limite  ");
        scanf("%d",&niveau);
        prefixe_iterative_with_limit(arbre, niveau);
        break;
    }
    case 100:
     {
     printf ("Nom du sommet a Chercher ? ");
        NomSom somD; scanf  ("%s", somD);
        IdA(graphe, (char*)somD);
        break;
    }
    case 101 :
    {   clock_t begin , end ;
        begin = clock();
        pvc =  plus_proche_voisin(graphe);
        end = clock();
        printf("\nTime elapsed : %f s",(double)(end - begin)*1000/CLOCKS_PER_SEC);
        break;
    }
    case 102:
    {
        clock_t begin , end ;
        begin = clock();
        simple_opt(graphe, get_indices(graphe, pvc));
        end = clock();
        printf("\nTime elapsed : %f s",(double)(end - begin)*1000/CLOCKS_PER_SEC);
        break;
    }
    case 103:
    {
        escalade(graphe);
        break;
    }
    case 104:
    {
        two_opt(graphe, pvc);
        break;
    }
    case 105:
    {
        escalade_complet(graphe);
        break;
    }
    case 106:
    {
        recuit_simule(graphe);
        break;
    }
    case 107 :
    {
        //printf("Not yet");
        perceptron();
        break;
    }

    }   // switch
    if (!fini) {
      printf ("\n\nTaper Return pour continuer\n");
      getchar();
    }
  }

return 0;
}
