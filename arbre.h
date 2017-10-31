// arbre.h

typedef int  booleen;
#define faux 0
#define vrai 1

typedef void Objet;

typedef struct noeud
{
    Objet*        reference;
    struct noeud* gauche;
    struct noeud* droite;
    int    factEq;    // facteur d'équilibre : si l'arbre est équilibré
} Noeud;

typedef struct
{
    Noeud* racine;
    char* (*toString) (Objet*);
    int   (*comparer) (Objet*, Objet*);
} Arbre;


// création de noeuds
Noeud*   cNd                  (Objet* objet, Noeud* Gauche, Noeud* Droite);
Noeud*   cNd                  (Objet* objet);
Noeud*   cF                   (Objet* objet);

// création d'arbre
void     initArbre            (Arbre* arbre, Noeud* racine,  char* (*toString) (Objet*), int (*comparer) (Objet*, Objet*));
Arbre*   creerArbre           (Noeud* racine, char* (*toString) (Objet*), int (*comparer) (Objet*, Objet*));
Arbre*   creerArbre           (Noeud* racine);
Arbre*   creerArbre           ();

Arbre* creerArbreGene ();
Arbre* creerArbreExp () ;

// parcours
void     prefixe              (Arbre* arbre);
void     infixe               (Arbre* arbre);
void     postfixe             (Arbre* arbre);
void     prefixe_goal         (Arbre * arbre, Objet * goal);
void prefixe_with_limit(Arbre * arbre, int limite);
void prefixe_iterative_with_limit(Arbre * arbre, int l);
Noeud*   trouverNoeud         (Arbre* arbre, Objet* objet);
void     enLargeur            (Arbre* arbre);
int      taille               (Arbre* arbre);
booleen  estFeuille           (Noeud* arbre);
int      nbFeuilles           (Arbre* arbre);
void     listerFeuilles       (Arbre* arbre);
int      hauteur              (Arbre* arbre);
booleen  egaliteArbre         (Arbre* arbre1, Arbre* arbre2);
