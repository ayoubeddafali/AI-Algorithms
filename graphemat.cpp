#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphemat.h"
#include "liste.h"
#include <time.h>
#include <math.h>
#define ln(x) log(x)




//static char * path = (char *) malloc(sizeof(char[20])*10);
static int* costs = (int *) malloc(sizeof(int)*50);
static int lenght = 0 ;
static int  neighbors_length =  0;
static int* total_estimated_costs = (int *)malloc(sizeof(int)*50);
static int estimated[] = {366,253,329,374,176,380,193,0,160,100};
static char * total_path = (char*)malloc(sizeof(char*));
static char ** nearest_path = (char **) malloc(sizeof(char*));
static int total ;
static booleen found = false ;
static Node * GOAL = NULL;
static int rang (GrapheMat* graphe, NomSom nom)
{
    int      i      = 0;
    booleen  trouve = faux;
    while ( (i<graphe->n) && !trouve)
    {
        trouve = strcmp (graphe->nomS [i], nom) == 0;
        if (!trouve) i++;
    }
    return trouve ? i : -1;
}
void init_costs()
{
    for(int i ; i< 50 ; i++)
    {
        costs[i] = 0;

    }
}


void init_all()
{
    total = 0;
    strcpy(total_path, "");
    found = false;
    init_costs();

}

static char * total_cost(GrapheMat* graphe, char * but)
{
    int numSommet = rang(graphe, but);
    char * total_to_char = (char*)malloc(sizeof(char*));
    total = costs[numSommet];
    sprintf(total_to_char, "%d", total);
    return total_to_char;

}

static char * total_estimated_cost(GrapheMat* graphe, char * but)
{
    int numSommet = rang(graphe, but);
    char * total_to_char = (char*)malloc(sizeof(char*));
    total = total_estimated_costs[numSommet];
    sprintf(total_to_char, "%d", total);
    return total_to_char;

}

char *  afficher_path(Node* node, char * all )
{
    if (node != NULL)
    {
        afficher_path(node->parent, all);
        strcat(all, node->name);
        strcat(all, " -> ");
    }
    return all ;
}




booleen  grapheDetruit = vrai;

Node * creerNode (Node * parent, int cost, char * name)
{
    Node * node = (Node * )malloc(sizeof(Node));
    node->parent = parent;
    node->cost = cost;
    node->name = name;
    node->estimated = 0;
    node->total_estimated = 0;

    return node ;
}
Path* creerPath (int* ipath, char** spath, int cost)
{
    Path * path = (Path *) malloc(sizeof(Path));
    path->cost = cost;
    path->ipath = ipath;
    path->spath = spath;

    return path ;
}

static char* show_path(Objet* o)
{
    Path * p = (Path*)o ;
    char * spath = (char*)malloc(sizeof(char*) * lenght) ;
    for (int i= 0; i< lenght; i++)
    {
        strcat(spath, p->spath[i]);
    }
    return spath;

    }

static char* show_node(Objet * o)
{
    Node * res = (Node*)o ;

    return (char *) res->name;
}
static int compare_node(Objet * n1, Objet * m1)
{
    Node * n = (Node *)n1 ;
    Node * m = (Node *)m1 ;
    if (n->cost > m->cost ) return 1;
    else if (n->cost == m->cost ) return 0;
    else if (n->cost < m->cost ) return -1 ;
}

static int compare_path(Objet* n1, Objet* m1)
{
    Path * n = (Path *)n1 ;
    Path * m = (Path*)m1 ;
    if (n->cost > m->cost ) return 1;
    else if (n->cost == m->cost ) return 0;
    else if (n->cost < m->cost ) return -1 ;
}
static int compare_node_estimated(Objet *n1, Objet * m1)
{
    Node * n = (Node *)n1 ;
    Node * m = (Node *)m1 ;
    if (n->estimated > m->estimated ) return 1;
    else if (n->estimated == m->estimated ) return 0;
    else if (n->estimated < m->estimated ) return -1 ;
}

static int compare_total_estimated_node(Objet *n1, Objet * m1)
{
    Node * n = (Node *)n1 ;
    Node * m = (Node *)m1 ;
    if (n->total_estimated > m->total_estimated ) return 1;
    else if (n->total_estimated == m->total_estimated ) return 0;
    else if (n->total_estimated < m->total_estimated ) return -1 ;
}
// remise à zéro du tableau de marquage
static void razMarque (GrapheMat* graphe)
{

    for (int i=0; i<graphe->n; i++) graphe->marque [i] = faux;
}

GrapheMat* creerGrapheMat (int nMax, int value)
{
    // allocation de graphe
    GrapheMat* graphe  = (GrapheMat*) malloc (sizeof (GrapheMat));
    graphe->n       = 0;
    graphe->nMax    = nMax;
    graphe->value   = value; // value : vrai si le graphe est valué
    graphe->nomS    = (NomSom*)  malloc (sizeof(NomSom)  *nMax);
    graphe->marque  = (booleen*) malloc (sizeof(booleen) *nMax);
    graphe->element = (int*)     malloc (sizeof(int)*nMax*nMax);
    graphe->valeur  = (int*)     malloc (sizeof(int)*nMax*nMax);
    // initialisation par défaut
    for (int i=0; i<nMax; i++)
    {
        for (int j=0; j<nMax; j++)
        {
            graphe->element [i*nMax+j] = faux;
            graphe->valeur  [i*nMax+j] = INFINI;
        }
    }
    razMarque (graphe);
    grapheDetruit = faux;
    return graphe;
}


// désallocation d'un graphe
void detruireGraphe (GrapheMat* graphe)
{
    free (graphe->nomS);
    free (graphe->marque);
    free (graphe->element);
    free (graphe->valeur);
    free (graphe);
    grapheDetruit = vrai;
}


void ajouterUnSommet (GrapheMat* graphe, NomSom nom)
{
    if (rang (graphe, nom) == -1)
    {
        if (graphe->n < graphe->nMax)
        {
            strcpy (graphe->nomS [graphe->n++], nom);
        }
        else
        {
            printf ("\nNombre de sommets > %d\n", graphe->nMax);
        }
    }
    else
    {
        printf ("\n%s déjà défini\n", nom);
    }
}

void ajouterUnArc (GrapheMat* graphe, NomSom somD, NomSom somA, int cout)
{
    int nMax = graphe->nMax;
    int rd = rang (graphe, somD);
    int rg = rang (graphe, somA);
    graphe->element [rd*nMax+rg] = vrai;
    graphe->valeur  [rd*nMax+rg] = cout;
}

void ecrireGraphe (GrapheMat* graphe)
{
    if (grapheDetruit == vrai)
    {
        printf ("Le graphe a ete detruit ou n'est pas encore cree\n");
    }
    else
    {
        int nMax = graphe->nMax;
        for (int i=0; i<graphe->n; i++) printf ("%s ", graphe->nomS[i]);
        printf (";\n");
        for (int i=0; i<graphe->n; i++)
        {
            printf ("\n%s : ", graphe->nomS[i]);
            for (int j=0; j<graphe->n; j++)
            {
                if (graphe->element [i*nMax+j] == vrai)
                {
                    printf ("%s ", graphe->nomS[j]) ;
                    if (graphe->value)
                    {
                        printf (" (%3d) ", graphe->valeur [i*nMax+j] );
                    }
                }
            }
            printf (";");
        }
    }
}

void largeur(GrapheMat * graphe)
{
    clock_t begin, end ;
    begin = clock();
	int nMax = graphe->nMax ;
	razMarque(graphe);
	Liste * file = creerListe();
	char *  start = (char *)graphe->nomS[0];
	insererEnFinDeListe(file, start);
	while(!listeVide(file))
	{
		char * out = (char *)extraireEnTeteDeListe(file);
		printf("%s " , out);
		int out_index = rang(graphe, out);
        graphe->marque[out_index] = vrai;
		for (int j = 0; j < graphe->n ; j++)
		{
			if ((graphe->element[out_index * nMax + j] == vrai) &&  graphe->marque[j] == faux )
			{
				char * temp = (char *) graphe->nomS[j];
				insererEnFinDeListe(file, temp);
				 graphe->marque[j]  = vrai;
			}
		}

	}
	end = clock();
	printf("\nTime elapsed : %f ms",(double)(end - begin)*1000/CLOCKS_PER_SEC);
}

void cout_uniform(GrapheMat* graphe, char * sommet)
{
    clock_t begin, end ;
    begin = clock();
    int nMax = graphe->nMax ;
    razMarque(graphe);
    init_costs();
    GOAL  = (Node *)malloc(sizeof(Node));
    Liste * file = creerListe(1, show_node, compare_node);
    char * start = (char *)graphe->nomS[0];
    strcat(total_path, start);
    strcat(total_path, " -> ");

    Node * init = creerNode(NULL, 0, start);
    if (strcmp(start, sommet) == 0) found = !found ;
    else
    {
        insererEnOrdre(file, init);
        while(!listeVide(file))
        {
            Node * father = (Node *)extraireEnTeteDeListe(file);
            char * father_name = father->name;
            printf("\n En cours :  %s  \n",father_name);
            int father_index = rang(graphe, father_name);
            graphe->marque[father_index] = vrai;

            if(strcmp(father_name, sommet) == 0 && !found)
            {
                GOAL = father;
                printf("\nThe goal is : %s\n",show_node(GOAL));
                found = !found;
            }
            for (int j = 0; j < graphe->n ; j++)
            {
             if ((graphe->element[father_index * nMax + j] == vrai) &&  !graphe->marque[j] )
                {
                    if (costs[j] == 0)
                    {
                        int cost = graphe->valeur[father_index * nMax + j];
                        char * son_name = (char *) graphe->nomS[j];
                        costs[j] = cost + costs[father_index];
                        Node * son = creerNode(father, cost , son_name);
                        insererEnOrdre(file, son);
                    }
                     else if (  ( costs[father_index] + graphe->valeur[father_index * nMax + j] ) < costs[j] )
                    {
                        int cost = graphe->valeur[father_index * nMax + j];
                        char * son_name = (char *) graphe->nomS[j];
                        costs[j] = cost + costs[father_index];
                        Node * son = creerNode(father, cost , son_name);
                        insererEnOrdre(file, son);
                    }
                }
            }
        }
    }
        if (found)
        {
                end = clock();
                char * answer = (char *)malloc(sizeof(char *));
                afficher_path(GOAL, answer);
                strcat(answer,total_cost(graphe, GOAL->name));
                printf("\n The shortest path is :  ");
                printf("%s",answer);
                printf("\nTime elapsed : %f ms",(double)(end - begin)*1000/CLOCKS_PER_SEC);
        }
        else {
                printf("Goal not found");
        }
        init_all();}

//}

void gloutonne(GrapheMat * graphe, char * sommet)
{
    clock_t begin, end;
    begin = clock();
    int nMax = graphe->nMax ;
    razMarque(graphe);
    init_costs();
    GOAL  = (Node *)malloc(sizeof(Node));
    Liste * file = creerListe(1, show_node, compare_node_estimated);
    char * start = (char *)graphe->nomS[0];
    strcat(total_path, start);
    strcat(total_path, " -> ");
    Node * init = creerNode(NULL, 0, start);
    int index = rang(graphe, init->name);
    init->estimated = estimated[index];


    if (strcmp(start, sommet) == 0) found = !found ;
    else
    {
        insererEnOrdre(file, init);
        while(!listeVide(file) && !found)
        {
            Node * father = (Node *)extraireEnTeteDeListe(file);
            char * father_name = father->name;
            int father_index = rang(graphe, father_name);
            graphe->marque[father_index] = vrai;
            //printf("\n%s",father_name);
            if(strcmp(father_name, sommet) == 0 && !found)
            {
                GOAL = father;
                printf("\nThe goal is : %s\n",show_node(GOAL));
                found = !found;
            }
            for (int j = 0; j < graphe->n ; j++)
            {
             if ((graphe->element[father_index * nMax + j] == vrai) &&  !graphe->marque[j] )
                {
                        int cost = graphe->valeur[father_index * nMax + j];
                        char * son_name = (char *) graphe->nomS[j];
                        costs[j] = cost + costs[father_index];
                        Node * son = creerNode(father, cost , son_name);
                        int index = rang(graphe, son->name);
                        son->estimated = estimated[index];
                        insererEnOrdre(file,son);
                }
            }

        }

    }
        if (found)
        {
                end = clock();
                char * answer = (char *)malloc(sizeof(char *));
                afficher_path(GOAL, answer);
                strcat(answer,total_cost(graphe, GOAL->name));
                printf("\n The shortest path is :  ");
                printf("%s",answer);

                printf("\nTime elapsed : %f s",(double)(end - begin)*1000/CLOCKS_PER_SEC);
        }
        else {
                printf("Goal not found");
        }

    init_all();}

//}

void a_etoile(GrapheMat * graphe, char * sommet)
{

    clock_t begin, end;
    begin = clock();
    int nMax = graphe->nMax ;
    razMarque(graphe);
    init_costs();
    GOAL  = (Node *)malloc(sizeof(Node));
    Liste * file = creerListe(1, show_node, compare_total_estimated_node);
    char * start = (char *)graphe->nomS[0];
    strcat(total_path, start);
    strcat(total_path, " -> ");

    Node * init = creerNode(NULL, 0, start);
    if (strcmp(start, sommet) == 0) found = !found ;
    else
    {
        insererEnOrdre(file, init);
        while(!listeVide(file) && !found )
        {
            Node * father = (Node *)extraireEnTeteDeListe(file);
            char * father_name = father->name;
            //printf("\n En cours :  %s  \n",father_name);
            int father_index = rang(graphe, father_name);
            graphe->marque[father_index] = vrai;

            if(strcmp(father_name, sommet) == 0 && !found)
            {
                GOAL = father;
                printf("\nThe goal is : %s\n",show_node(GOAL));
                found = !found;
                // adding break to not iterate over the stack when found
            }
            else {
            for (int j = 0; j < graphe->n ; j++)
            {
             if ((graphe->element[father_index * nMax + j] == vrai) &&  !graphe->marque[j] )
                {
                        int cost = graphe->valeur[father_index * nMax + j];
                        char * son_name = (char *) graphe->nomS[j];
                        costs[j] = cost + costs[father_index];
                        Node * son = creerNode(father, cost , son_name);
                        int index = rang(graphe, son->name);
                        son->estimated = estimated[index];
                        son->total_estimated = costs[index] + estimated[index];
                        total_estimated_costs[index] = son->total_estimated;
                        //printf("\n Estimated %s =  %d \n",son->name,son->total_estimated);
                        insererEnOrdre(file, son);
                }
            }
            }
        }
    }
        if (found)
        {
                end = clock();
                char * answer = (char *)malloc(sizeof(char *));
                afficher_path(GOAL, answer);
                strcat(answer,total_cost(graphe, GOAL->name));
                printf("\n The shortest path is :  ");
                printf("%s",answer);
                //printf("\nCout total estime %s",total_estimated_cost(graphe, GOAL->name));
                printf("\nTime elapsed : %f s",(double)(end - begin)*1000/CLOCKS_PER_SEC);
        }
        else {
                printf("Goal not found");
        }
        init_all();}
//}
static void profondeur (GrapheMat* graphe, int numSommet)
{
    int nMax = graphe->nMax;
    graphe->marque [numSommet] = vrai;
    printf ("%s\n", graphe->nomS [numSommet]);
    for (int i=0; i<graphe->n; i++)
    {
        if ( (graphe->element [numSommet*nMax+i] == vrai)
                && !graphe->marque [i] )
        {
            profondeur (graphe, i);
        }
    }
}

void parcoursProfond (GrapheMat* graphe)
{
    clock_t begin, end ;
    begin = clock();
    razMarque (graphe);
    for (int i=0; i<graphe->n; i++)
    {
        if (!graphe->marque [i]) profondeur (graphe, i);
    }
    end = clock();
    printf("\nTime elapsed : %f s",(double)(end - begin)*1000/CLOCKS_PER_SEC);

}

static void profondeur_limitee(GrapheMat * graphe, int limit, int order)
{
    razMarque(graphe);
    Liste * file = creerListe();
    int nMax = graphe->nMax;
    char * start = (char *)graphe->nomS[0];
    insererEnFinDeListe(file, start);
    if (limit == 0)
    {
     char * out = (char *)extraireEnTeteDeListe(file);
     printf("%s ",out);
    }
    else {
        printf("%s ",start );
    while( limit > order)
    {
        int taille = file->nbElt;
        int k = 1;
        while (k <= taille){
        char * out = (char *)extraireEnTeteDeListe(file);
        int index = rang(graphe, out);
        graphe->marque[index] = vrai;
        //printf("%s ",out);
        for(int j = 0; j < graphe->n ; j++)
        {
            if ((graphe->element[index * nMax + j ] == vrai) && !graphe->marque[j])
            {
                insererEnFinDeListe(file, graphe->nomS[j]);

                printf("%s ",(char *)graphe->nomS[j]);
                graphe->marque[j] = vrai;
            }
        }
        //taille = file->nbElt - taille ;
        k++;
        }
        order += 1 ;
    }
        }
}

void profondeur_limitee(GrapheMat * graphe, int limit)
{
    profondeur_limitee(graphe, limit, 0);
}

static void profondeur_iter_limitee(GrapheMat * graphe, int limit, int order)
{
razMarque(graphe);
    Liste * file = creerListe();
    int nMax = graphe->nMax;
    char * start = (char *)graphe->nomS[0];
    insererEnFinDeListe(file, start);
    if (limit == 0)
    {
     char * out = (char *)extraireEnTeteDeListe(file);
     printf("%s ",out);
    }
    else {
        printf("%s ",start );
    while( limit > order)
    {
        int taille = file->nbElt;
        int k = 1;
        while (k <= taille){
        char * out = (char *)extraireEnTeteDeListe(file);
        int index = rang(graphe, out);
        graphe->marque[index] = vrai;
        //printf("%s ",out);
        for(int j = 0; j < graphe->n ; j++)
        {
            if ((graphe->element[index * nMax + j ] == vrai) && !graphe->marque[j])
            {
                insererEnFinDeListe(file, graphe->nomS[j]);

                printf("%s ",(char *)graphe->nomS[j]);
                graphe->marque[j] = vrai;
            }
        }
        //taille = file->nbElt - taille ;
        k++;
        }
        order += 1 ;
    }
        }
}

void profondeur_iterative_limitee(GrapheMat * graphe, int limit)
{
  for (int i = 0; i <= limit + 1 ; i++)
    {
        profondeur_iter_limitee(graphe, i, 0);
    }
}


void IdA(GrapheMat* graphe,char* but)
    {
    clock_t begin, end;
    begin = clock();
    bool trouve = false;

        for(int L=0;L<=800;L+=115)
        {

        if (trouve) break;

        int* depart=(int*) malloc(sizeof(int));
        *depart=0;
        Liste* listParcourt=creerListe();
        insererEnOrdre(listParcourt,depart);
        graphe->marque [0] = vrai;
        razMarque(graphe);
        //sommets
        int* intSommet=(int*)malloc(sizeof(int)*graphe->n);
        for(int j=0;j<graphe->n;j++){
        *(intSommet+j)=j;
        }
        //cout
        int* intCout=(int*)malloc(sizeof(int)*graphe->n);
        for(int j=0;j<graphe->n;j++){
        *(intCout+j)=0;
        }
        // f(n)
        int* fn=(int*)malloc(sizeof(int)*graphe->n);
        for(int j=0;j<graphe->n;j++){
        *(fn+j)=0;
        }

        //h(n)
        int* h=(int*)malloc(sizeof(int)*graphe->n);
        *(h)=366;
        *(h+1)=253;
        *(h+2)=329;
        *(h+3)=374;
        *(h+4)=176;
        *(h+5)=380;
        *(h+6)=193;
        *(h+7)=0;
        *(h+8)=160;
        *(h+9)=100;


        //vecteur precedent
        int* intPrec=(int*)malloc(sizeof(int)*graphe->n);
        for(int j=0;j<graphe->n;j++){
        *(intPrec+j)=0;
        }

        //-----------------------------


        printf("\nPour la limite L = %d\t",L);
        while(!listeVide(listParcourt)){
        int* ex=(int*)extraireEnTeteDeListe(listParcourt);
        //printf("(%s,%d)\t", graphe->nomS[*ex],fn[*ex]);
        if(strcmp(but,graphe->nomS[*ex])==0) {
        trouve=true;
        break;
        }


         graphe->marque [*ex] = vrai;


        for(int j=0;j<graphe->n;j++){
        if ( (graphe->element [(*ex)*graphe->nMax+j] == vrai) && !graphe->marque [j] && (intCout[*ex]+graphe->valeur[(*ex)*graphe->nMax+j]+h[j])<L)
                {
                    if(intCout[j]==0){
                    intCout[j]=intCout[*ex]+graphe->valeur[(*ex)*graphe->nMax+j];
                    fn[j]=intCout[j]+h[j];
                    insererEnOrdreCout(listParcourt,intSommet+j,fn);
                    intPrec[j]=*ex;
                    }
                    else if(intCout[j]>=intCout[*ex]+graphe->valeur[(*ex)*graphe->nMax+j]){
                    intCout[j]=intCout[*ex]+graphe->valeur[(*ex)*graphe->nMax+j];
                    fn[j]=intCout[j]+h[j];
                    insererEnOrdreCout(listParcourt,intSommet+j,fn);
                    intPrec[j]=*ex;
                    }
                }
        }
        }
        int j=rang (graphe, but);
         printf("\n");
        while(j!=0){
            printf("%2s<-",graphe->nomS[j]);
            j= *(intPrec+j) ;
        }
        printf("%2s",graphe->nomS[0]);
    }
    end = clock();
     printf("\nTime elapsed : %f s",(double)(end - begin)*1000/CLOCKS_PER_SEC);
}

static void show_indices(GrapheMat* graphe, char** path )
{
    for (int i = 0 ; i < lenght ; i++)
    {
        if(i !=  lenght  - 1 )
            printf("%d - ", rang(graphe, path[i]));
        else
            printf("%d", rang(graphe, path[i]));
    }
    //printf("\n");
}
static void show_indices(GrapheMat* graphe, int* path )
{
    for (int i = 0 ; i < lenght ; i++)
    {
        if(i != lenght - 1 )
            printf("%d - ", path[i]);
        else
            printf("%d", path[i]);
    }
    //printf("\n");
}

static void show_path(GrapheMat* graphe, int* path)
{
     for (int i = 0 ; i < lenght ; i++)
    {
        if(i != lenght - 1 )
            printf("%s - ", graphe->nomS[path[i]]);
        else
            printf("%s", graphe->nomS[path[i]]);
    }
}

int* get_indices(GrapheMat* graphe, char** path)
{
    int* indices = (int *)malloc(lenght * sizeof(int));
    for (int j = 0 ; j < lenght; j++)
    {
        indices[j] = rang(graphe, path[j]);
    }
    return indices;
}

static char** get_path(GrapheMat* graphe, int* path)
{
    char ** spath = (char**)malloc(sizeof(char*) * lenght) ;
    for (int i= 0; i< lenght; i++)
    {
        spath[i] = graphe->nomS[path[i]];
    }
    return spath;
}
static int path_cost(GrapheMat* graphe ,int* path)
{
    int nMax = graphe->nMax;
    int path_cost = 0 ;
    for (int i = 1; i< lenght ; i++)
    {
        path_cost += graphe->valeur[path[i-1]*nMax + path[i]];
    }
    return path_cost;
}

char **  plus_proche_voisin(GrapheMat * graphe)
{
    clock_t begin, end;
    begin = clock();
    init_all();
    razMarque(graphe);
    int index = 1 ;
    int nMax = graphe->nMax;
    char * start = (char *) graphe->nomS[0];
    nearest_path[index] = start;
    int nearest_cost = 0 ;
    Node * init = creerNode(NULL, 0, start);
    Liste * file = creerListe(0);
    insererEnFinDeListe(file, init);
    Liste * temp_file;
    Node * nearest;
    while (!listeVide(file))
    {
        temp_file = creerListe(1, show_node, compare_node);
        Node * out = (Node *) extraireEnTeteDeListe(file);
        int out_index = rang(graphe, out->name);
        graphe->marque[out_index] = vrai ;

        //printf("Current Node : %s \n",show_node(out));
        for (int j = 0; j < graphe->n ; j ++)
       {
            if (graphe->element[out_index * nMax + j ] && !graphe->marque[j] && ( j != out_index )  )
            {
                    int cost = graphe->valeur[out_index * nMax + j];
                    char * name = (char *) graphe->nomS[j];
                    Node * cur = (Node *) creerNode(NULL, cost , name);
                    insererEnOrdre(temp_file, cur);
                    //printf("inserted %s\n", show_node(cur));
                    //graphe->marque[j] = vrai ;
            }
       }

            nearest = (Node *) extraireEnTeteDeListe(temp_file);
        if ( nearest != NULL )
        {
            //index = index + 1;
            insererEnFinDeListe(file, nearest);
            //printf("nearest %s\n", show_node(nearest));
            nearest_path[index++] = nearest->name;
            nearest_cost += nearest->cost;
        }
       temp_file = NULL;


    }
   nearest_path[0] = start;
   nearest_cost += graphe->valeur[0 * nMax + 2 ];
   nearest_path[index++] = start ;
    printf("PVC : \t");
    for(int i = 0 ; i < index ; i ++)
    {
        if (i != index - 1)
            printf("%s - ", nearest_path[i]);
        else
            printf("%s", nearest_path[i]);

    }
    lenght = index;

    //printf("index = %d", index);
    printf("\t Cost : %d\n",nearest_cost );

    //show_indices(graphe, nearest_path);

    return nearest_path;



}

static void copy_to(int* from, int* to, int start = 0, int limit = lenght)
{
    for(int i= start; i< limit; i++)
    {
        to[i] = from[i];
    }
}
static int* swap(int* tab, int i, int j)
{
    int* new_tab = (int *)malloc(lenght * sizeof(int));
    copy_to(tab, new_tab,0, lenght);
    new_tab[i] = tab[j];
    new_tab[j] = tab[i];
    return new_tab;

}

int* simple_opt(GrapheMat* graphe, int* inital)
{
    int i,k ;
    int* new_index = (int*) malloc(lenght * sizeof(int));
    do {
         i = (rand() % (lenght - 2) ) + 1  ; // r between 0 and  length - 1
         k = ( rand() % ( lenght - 2)) + 1 ;
    }while( (k >= i-1 && k <= i + 1 ) || i > k   );
    printf("\n\t ------------------------------------\n ");
    printf("\t\ti = %d, k = %d \n", i, k);
    copy_to(inital, new_index,0, i);
    int s = i ;
    for (int j = k; j >= i ; j--)
    {
        new_index[s] = inital[j];
        s++;
    }

    copy_to(inital, new_index, k+1, lenght);
    // ------------ IMPORTANT
    printf("Random neighbor Simple opt \t");
    show_path(graphe, new_index);
    printf(" Cost : %d", path_cost(graphe,new_index));
    // ------------ IMPORTANT

    return new_index;

}
int** two_opt(GrapheMat* graphe, char** initial)
{

    int k = 0 ;
    int** all_indicies = (int**)malloc(sizeof(int*) * 20);
    int* init_indices = get_indices(graphe, initial);
    int*  tmp = (int*)malloc(sizeof(int)* lenght);
    copy_to(init_indices, tmp, 0, lenght);
    printf("\t\t--------------------\n"); // IMPORTANT
    printf("\n");
    show_indices(graphe, init_indices); printf("\t ");
    show_path(graphe, init_indices);
    int cost = path_cost(graphe, init_indices);
// ----------- IMPORTANT
    printf("\t Cost : %d\n",  cost );
    printf("\t\t--------------------\n");
    printf("paires non adjacents \t"); printf("\tTours voisins \n");
// ----------- IMPORTANT

    for (int i = 0 ; i < lenght - 2 ; i++)
    {
        int* new_tmp = get_indices(graphe, initial);
        for (int j = i+2; j < lenght - 1 ; j++)
        {

            if (i == 0 && j == lenght - 2 ) continue;

                int * new_index = swap(new_tmp, i+1, j);
               all_indicies[k] = new_index;
                k++;
                // -------------- IMPORTANT
                printf("(%d,%d) , (%d,%d)    --> \t",new_tmp[i], new_tmp[i+1], new_tmp[j], new_tmp[j+1]);
                show_indices(graphe,new_tmp);
                printf("\n");
                // -------------- IMPORTANT


        }
        free(new_tmp);
    }
    neighbors_length = k ;

    printf("\t\t--------------------\n"); // IMPORTANT
    printf("\n");
    for (int i = 0; i < neighbors_length; i++)
    {
        show_indices(graphe, all_indicies[i]);
        printf("\t "); show_path(graphe, all_indicies[i]);
        printf("\t Cost : %d\n", path_cost(graphe, all_indicies[i]));

    }

    return all_indicies;
}

void escalade(GrapheMat* graphe)
{
    clock_t begin, end ;
    begin = clock();
    int iterations   = 0;
    char** pvc = plus_proche_voisin(graphe);
    int* current = get_indices(graphe, pvc);
    //printf("------ INIT --------\n ");
    //show_path(graphe, current);
    int init_cost = path_cost(graphe, current);
    //printf("\tInit Cost: %d\n",init_cost);

    int* neighbor ;

    while (iterations < 100 )
    {
        neighbor = simple_opt(graphe, current);
        int cout = path_cost(graphe, neighbor);
        printf("\nCurrent :  \t  "); show_path(graphe, current); printf(" Cost:  %d ", path_cost(graphe, current));
        printf("\nNeighbor :  \t  "); show_path(graphe, neighbor); printf(" Cost:  %d ", cout);
        if ( cout  < path_cost(graphe, current) )
        if ( cout  < path_cost(graphe, current) )
        {
            current = neighbor;
        }
        iterations++;
    }

    printf("\n\n\n\t ------------------------------------\n ");
    printf("\n Escalade Simple -> \t");
    show_path(graphe, current);
    printf(" Cost : %d", path_cost(graphe, current));
    end = clock();
    printf("\nTime elapsed : %f s",(double)(end - begin)*1000/CLOCKS_PER_SEC);

}

static float probability_of_moving(int delta_E, float T)
{
    return (float) exp((-1.0 * delta_E)/T);
}

void escalade_complet(GrapheMat* graphe)
{


    clock_t begin, end ;
    begin = clock();
    Liste* file = creerListe();
    int * best = (int *) malloc(sizeof(int) * lenght );
    char** pvc = plus_proche_voisin(graphe);
    int* ipvc = get_indices(graphe, pvc);
    int least_cost = path_cost(graphe, ipvc);
    Path* p = creerPath(ipvc, pvc, path_cost(graphe, ipvc));
    insererEnFinDeListe(file, p);
    //Liste* temp_file ;
    //Path* in;
    //Path * out;
    //int** neighbors;
    while(!listeVide(file))
    {
        Liste* temp_file = creerListe(1, show_path, compare_path);
        Path * out = (Path *)extraireEnTeteDeListe(file);
        int ** neighbors = two_opt(graphe, out->spath);
        for (int k = 0; k < neighbors_length; k++)
        {
            Path* path_neighbor = creerPath(neighbors[k],get_path(graphe, neighbors[k]), path_cost(graphe,neighbors[k]));
            insererEnOrdre(temp_file, path_neighbor);
            show_path(path_neighbor);

        }
        Path* in = (Path*) extraireEnTeteDeListe(temp_file);
        if (path_cost(graphe, in->ipath) < least_cost)
        {
            insererEnFinDeListe(file, in);
            least_cost = path_cost(graphe, in->ipath);
            copy_to(in->ipath, best, 0, lenght);
            free(in);

        }
        else break ;


//        temp_file = NULL ;
//        neighbors = NULL ;



    }

    printf("\n\t - ESCALADE 2OPT Complet  -> ");
  show_path(graphe, best ); printf(" Cost %d" , path_cost(graphe, best));
  end = clock();

    printf("\nTime elapsed : %f s",(double)(end - begin)*1000/CLOCKS_PER_SEC);



}

static float delta_moyenne(GrapheMat* graphe, char** pvc)
{
    int ** neighbors = two_opt(graphe, pvc);
    int * neighbor = simple_opt(graphe, get_indices(graphe, pvc));
    float * deltas = (float * ) malloc(sizeof(float) * ( neighbors_length ));
    float delta_moy;
    float sum_deltas = 0 ;

    for (int i = 0; i < neighbors_length; i++)
    {
        deltas[i] = abs(path_cost(graphe, neighbor) - path_cost(graphe, neighbors[i]));
        sum_deltas += deltas[i];
    }
    delta_moy = (float)sum_deltas / neighbors_length;
    return delta_moy;
}

void recuit_simule(GrapheMat* graphe)
{
    char ** nearest = plus_proche_voisin(graphe);
    int * pvc = get_indices(graphe, nearest);
    //Path* p = creerPath(pvc, nearest, path_cost(graphe, pvc));
    float alpha = 0.93;
    float delta_moy = delta_moyenne(graphe, nearest);
    float T_init = (-1 * delta_moy ) / ln(0.5);

    Liste* file = creerListe();
    //insererEnFinDeListe(file, p);
    float T = T_init;
    float T_min = 1 ;
    int * current = pvc;
    float test ;
    T = T * 1000.0 ;
    while ( T > T_min)
    {
        //printf("\n Start Again\n");
        printf("\nCurrent "); show_path(graphe, current); printf(" Cost %d", path_cost(graphe, current));
        char** another = get_path(graphe, current);

        printf("\n  T = %f \n ", T);
        int ** neighbors = two_opt(graphe, another);

        if (neighbors_length != 0)
        {
            int r = rand() % neighbors_length ;
            int * neighbor = neighbors[r];
            printf("random neighbor ");show_path(graphe, neighbor); printf(" Cost : %d", path_cost(graphe, neighbor));
            int delta_E = path_cost(graphe, neighbor) - path_cost(graphe, current);
            printf("\nCost neighbor %d -- Cost current %d ", path_cost(graphe, neighbor), path_cost(graphe, current));
            printf("\ndelta_E = %d  T = %f  " , delta_E, T );
            printf(" probabilty = %f \n", probability_of_moving(delta_E, T));
            if (T == T ) test = probability_of_moving(delta_E, T);
            float prand = rand();
            if (delta_E <  0 )
            {
                current = neighbor;
            }
            else if ( probability_of_moving(delta_E, T) > prand  )
            {
                current = neighbor ;
            }
        }
         T = T * alpha;
        free(neighbors);
    }

    //printf("CHeikking %f", test);

    //printf("\n Finally \n");
    //printf("\nT init : %f\n ", T_init);
    //printf("\n delta_moy %f \n", delta_moy);
    printf("Plus court chemin avec recuit simulé est  "); show_path(graphe, current); printf(" Cost %d ", path_cost(graphe, current));

}

float sigmoid(float value )
{
    return 1 / ( 1 + exp(-1 * value));
}
int signe(float value)
{
    return value > 0 ? 1 : 0 ;
}


void perceptron()
{
    int entries[2][4] = {{0,0,1,1},{0,1,0,1}};
    int result[4] = {0,0,0,1};
    int input_len = 4;
    float teta = 0.2;
    float mu = 0.1 ;
    float weights[2] = {0.3,-0.1};
    float output;
    int* revisit = (int *)malloc(sizeof(int));
    int k = 0 ;
    int satisfied  = 0;
    booleen flag = false;
    printf("\t e1  e2 \t        x \tw1          \tw2 \n");
    printf("\t----------------------------------------------------------\n");
    while ( !flag )
    {

        for (int i = 0 ; i < input_len ; i++)
        {
            float a = entries[0][i] * weights[0]  + entries[1][i] * weights[1] - teta ;
            if (signe(a) == result[i] )
            {
                satisfied++ ;
                printf("\t%d     %d \t\t%d  \t%f \t%f\n" ,entries[0][i], entries[1][i], signe(a),  weights[0], weights[1]);
            }
            else
            {
                weights[0] = weights[0] + mu * ( result[i] - signe(a) ) * entries[0][i];
                weights[1] = weights[1] + mu * ( result[i] - signe(a) ) * entries[1][i];
                printf("\t%d     %d \t\t%d   \t%f   \t%f\n" , entries[0][i], entries[1][i], signe(a)  ,weights[0], weights[1]);
                satisfied =  1;
            }
        }
        if ( satisfied == input_len ){

            flag = true ;
        } else
            satisfied = 0 ;


    }
    printf("\n \t ------------ Checking Result ---------------");
    printf(" \n \t-----------   w1 = %.2f , w2 = %.2f ----------- \n" , weights[0], weights[1]);
       for (int i = 0 ; i < input_len ; i++)
        {
            float a = entries[0][i] * weights[0]  + entries[1][i] * weights[1] - teta ;
            printf("\t%d     %d \t\t%d  \t%f \t%f\n" ,entries[0][i], entries[1][i], signe(a),  weights[0], weights[1]);

        }
}


static void ecrireEtape (Matrice a, Matrice p, int k, int ns, int nMax)
{
    printf ("Passage par le sommet numéro %d\n", k);

    for (int i=0; i<ns; i++)
    {
        for (int j=0; j<ns; j++)
        {
            if (a [i*nMax+j]==INFINI)
            {
                printf (" %3s", "*");

            }
            else
            {
                printf (" %3d", a [i*nMax+j]);
            }
        }
        printf ("%6s", " ");
        for (int j=0; j<ns; j++)
        {
            printf ("%3d", p [i*nMax+j]);
        }
        printf ("\n");
    }
    printf ("\n");
}

// initialiser les matrices a et p à partir de graphe
static void initFloyd (GrapheMat* graphe, Matrice* a, Matrice* p)
{
    int nMax = graphe->nMax;

    Matrice ta = (int*) malloc (sizeof(int)*nMax*nMax);
    Matrice tp = (int*) malloc (sizeof(int)*nMax*nMax);


    for (int i=0; i<graphe->n; i++)
    {
        for (int j=0; j<graphe->n; j++)
        {
            if(i==j){
                ta [i*nMax+j]=0;
                tp [i*nMax+j] = i;
            }
            else{
                ta [i*nMax+j] = graphe->valeur [i*nMax+j];
                tp [i*nMax+j] = i;
            }
        }
    }
    *a = ta;
    *p = tp;
}

void floyd (GrapheMat* graphe)
{
    Matrice a, p;
    int n = graphe->n;
    int nMax = graphe->nMax;

    initFloyd (graphe, &a, &p);


    for (int k=0; k<n; k++)
    {
        for (int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                if ( (a [i*nMax+k] != INFINI) &&
                        (a [k*nMax+j] != INFINI) &&
                        (a [i*nMax+k] + a [k*nMax+j] < a [i*nMax+j]) )
                {
                    a [i*nMax+j] = a [i*nMax+k] + a [k*nMax+j];
                    p [i*nMax+j] = p [k*nMax+j];
                }
            }
        }
        ecrireEtape (a, p, k, n, nMax);
    }
}


