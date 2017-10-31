
#include <limits.h>

typedef int  booleen;
#define faux 0
#define vrai 1
#define false 0
#define true 1
typedef char NomSom[20];  // nom d'un sommet

#define INFINI INT_MAX

typedef int* Matrice;
typedef struct Edge {
    int start ;
    int end ;
    int cost ;
};
typedef struct Path
{
    int* ipath ;
    char** spath;
    int cost ;
};
typedef struct Node  {
    Node * parent ;
    int cost ;
    int estimated;
    int total_estimated;
    char * name ;
    booleen visited = false;

};

typedef struct {
  int      n;       // nombre de sommets
  int      nMax;    // nombre max de sommets
  booleen  value;   // graphe valué ou non
  NomSom*  nomS;    // noms des sommets
  Matrice  element; // existence d'un arc (i, j)
  Matrice  valeur;  // cout de l'arc (i, j)
  booleen* marque;  // sommet marqué (visité) ou non
} GrapheMat;

// Node Struct
Node *     creerNode          (Node * parent, int cost, char * name);
GrapheMat* creerGrapheMat     (int nMax, int value);
void       detruireGraphe     (GrapheMat* graphe);
void       ajouterUnSommet    (GrapheMat* graphe, NomSom nom);
void       ajouterUnArc       (GrapheMat* graphe, NomSom somD, NomSom somA, int cout);
void       ecrireGraphe       (GrapheMat* graphe);

void       parcoursProfond    (GrapheMat* graphe);

void       floyd              (GrapheMat* graphe);

// TP
void largeur(GrapheMat * graphe);

void cout_uniform(GrapheMat * graphe, char * sommet);
void gloutonne(GrapheMat * graphe, char * sommet);
void a_etoile(GrapheMat * graphe, char * sommet);

void profondeur_limitee(GrapheMat * graphe, int limit);

void profondeur_iterative_limitee(GrapheMat * graphe, int limit);

void IdA(GrapheMat* graphe,char* but);


char *  afficher_path(Node* node, char * all );

char ** plus_proche_voisin(GrapheMat * graphe);

int** two_opt(GrapheMat* graphe, char** initial);

int* simple_opt(GrapheMat* graphe, int* inital);
int* get_indices(GrapheMat* graphe, char** path);
void escalade(GrapheMat* graphe);

void escalade_complet(GrapheMat* graphe);

void recuit_simule(GrapheMat* graphe);

void perceptron();

