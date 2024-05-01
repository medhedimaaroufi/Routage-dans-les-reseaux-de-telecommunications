#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define Nbs 30
#define infinie 9999

typedef struct sommet{
    int noeud;
    float poids;
    struct sommet *suiv;
}sommet;

typedef struct sommet* liste;

void affiche_matrice(float M[Nbs][Nbs],int nb_sommets){
    int i,j;
    printf("*/ Matrice Distance : \n\t");
    if (M!=NULL && nb_sommets<Nbs) {
        for(i=0;i<nb_sommets;i++){
            for(j=0;j<nb_sommets;j++){
                printf("| %6.2f ",M[i][j]);
            }
            printf("|\n\t");
        }
    }
}

void affiche_P(int P[Nbs] , int nb_sommets){
    int i ;
    for(i=0 ; i< nb_sommets;i++){
        if (P[i]){
            printf("Le predecesseur de %d est ",i+1);
            printf("%d\n",P[i]);
        }
        else
            printf("La sommet %d est une source.\n",i+1);
    }
}


void affiche_Lambda(float Lambda[Nbs],int nb_sommets){
    int i;
    for(i=0;i<nb_sommets;i++){
        printf("Le plus court chemin jusqu'a %d est de poids ",i+1);
        printf("%.2f\n",Lambda[i]);
    }
    printf("\n");
}

void affiche_chemin(int sommet, int chemin[Nbs],int nb_sommets){
    int i;
    printf("le chemin le plus court pour aller a %d est : ", sommet) ;
    for(i=nb_sommets-1;i>=0;i--){
        if (chemin[i]==sommet){
            printf("%d", sommet);
        }
        else printf("%d -> ",chemin[i]);
    }
    printf("\n");
}

void MDist(char *file ,float Mdist[Nbs][Nbs], int *nb){
    int x,y;
    int p;
    FILE *f=fopen(file,"r");
    fscanf(f,"%d %d",nb, &x);
    while (!feof(f)){
        fscanf(f,"%d %d %d",&x,&y,&p);
        Mdist[x-1][y-1]=p;
    }
    fclose(f);
}

void init_Lambda(float Lambda[Nbs],int nb_sommets){
    int i ;
    Lambda[0]=0;
    for(i=1; i < nb_sommets ; i++) Lambda[i]=infinie;
}

void init_P(int P[Nbs] , int nb_sommets){
    int i ;
    for(i=0 ; i< nb_sommets ; i++) P[i] = 0 ;
}

void succ(int sommet,int *SUCC, float M[Nbs][Nbs], int nb_sommets){
    int i;
    init_P(SUCC, nb_sommets);
    for (i=0;i<nb_sommets;i++) {
        if (M[sommet - 1][i]) SUCC[i] = i + 1;
    }
}

int apparitent(int sommet , int *D,int nb_sommets){
    int i;
    for (i=0; i<nb_sommets;i++){
        if ( D[i] && sommet == D[i]) return 1;
    }
    return 0;
}

int min(int nb_sommet , float Lambda[Nbs], int *D){
    int i, index=0, app=0;
    float min=infinie;
    for(i=1;i<nb_sommet;i++){
        app=apparitent(i+1,D,nb_sommet);
        if( !app && Lambda[i]<min) {
            min=Lambda[i];
            index=i+1;
        }
    }
    return index;
}

////////////Algorithme de recherche des plus courts chemins Bellman Ford.
void Ford_bellmann(float M[Nbs][Nbs],float Lambda[Nbs], int P[Nbs] ,int nb_sommets){
    int i,j,stop=0;
    init_Lambda(Lambda,nb_sommets);
    init_P(P,nb_sommets);

    while (!stop){
        stop=1;
        for (i = 0; i < nb_sommets; i++ ){
            for (j = 0; j < nb_sommets; ++j) {
                if ( M[i][j] && Lambda[i] + M[i][j] < Lambda[j]){
                    stop=0;
                    Lambda[j] = Lambda[i] + M[i][j];
                    P[j] = i+1 ;
                }
            }
        }
    }
}

////////////Algorithme de recherche des plus courts chemins Dijksta.
void Dijksta(float M[Nbs][Nbs],float Lambda[Nbs], int P[Nbs] ,int nb_sommets) {
    int i, j;
    int D[Nbs]={1};
    int SUCC[Nbs]={0};
    init_Lambda(Lambda, nb_sommets);
    init_P(P, nb_sommets);
    for (i =0 ; i<nb_sommets; i++){
        succ(D[i], SUCC, M, nb_sommets);
        for (j = 0; j <nb_sommets; ++j) {
            if (SUCC[j] && Lambda[SUCC[j]-1] > Lambda[D[i]-1] + M[D[i]-1][SUCC[j]-1]) {
                Lambda[SUCC[j] - 1] = Lambda[D[i] - 1] + M[D[i] - 1][SUCC[j] - 1];
                P[SUCC[j] - 1] = D[i];
            }
        }
        if ( i+1 < nb_sommets ) {
            D[i+1] = min(nb_sommets, Lambda, D);
        }
    }
}


int Plus_Court_Chemins(int dest,int chemin[Nbs], int P[Nbs], int nb_sommets){
    int i=dest-1,j=0;
    while (P[i]){
        chemin[j]=i+1;
        i=P[i]-1;
        j++;
    }
    chemin[j]=i+1;
    return j+1;
}



int main() {
    printf("-------Algorithme Routage dans les reseaux de telecommunications-------");
    float M[Nbs][Nbs]={{0}};
    float LambdaF[Nbs], LambdaD[Nbs];
    int PF[Nbs],PD[Nbs];
    int cheminF[Nbs]={0}, cheminD[Nbs]={0};
    int nb_sommets,c;
    clock_t start_ford, end_ford, start_Dijkstra, end_Dijkstra;
    double cpu_time_used_Ford, cpu_time_used_Dijkstra;

    MDist("graph.txt",M,&nb_sommets);
    printf("\n");
    affiche_matrice(M,nb_sommets);
    printf("\n");

    start_ford = clock();
    Ford_bellmann(M,LambdaF,PF,nb_sommets);
    end_ford = clock();
    cpu_time_used_Ford = ((double) (end_ford - start_ford)) / CLOCKS_PER_SEC;

    start_Dijkstra = clock();
    Dijksta(M, LambdaD, PD, nb_sommets);
    end_Dijkstra = clock();
    cpu_time_used_Dijkstra = ((double) (end_Dijkstra - start_Dijkstra)) / CLOCKS_PER_SEC;


    printf("\n-- Resultats Bellman Ford :\n");
    printf("\n---Le temps d'execution de l'algorithme Bellman Ford est : %f\n", cpu_time_used_Ford);
    affiche_Lambda(LambdaF,nb_sommets);
    printf("\n");

    affiche_P(PF,nb_sommets);
    printf("\n");
    for (c=0;c<nb_sommets;c++){
        affiche_chemin(c+1,cheminF, Plus_Court_Chemins(c+1,cheminF,PF, nb_sommets));
    }

    printf("\n\n\n\n-- Resultats Dijkstra :\n");
    printf("\n--- Le temps d'execution de l'algorithme Dijkstra est : %f\n", cpu_time_used_Dijkstra);

    affiche_Lambda(LambdaD, nb_sommets);
    printf("\n");
    
    affiche_P(PD, nb_sommets);
    printf("\n");
    for (c=0;c<nb_sommets;c++){
        affiche_chemin(c+1,cheminD, Plus_Court_Chemins(c+1, cheminD, PD, nb_sommets));
    }

    printf("\n");
    return 0;
}
