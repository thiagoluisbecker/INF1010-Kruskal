#include <stdio.h>
#include <stdlib.h>

/*
Eduardo Corrêa Fontoura de Oliveira 1710220 
Thiago Luis Becker da Rocha 2110449
*/

typedef struct no 
{
  int vertice;
  int peso;
  struct no* prox;
}No;

typedef struct grafo 
{
  int numVertices;
  No** listaAdj;
}Grafo;

typedef struct aresta
{
  int src;
  int dest;
  int peso;
  struct aresta *prox;
}Aresta;

Grafo* criaGrafo(int vertices) 
{
  Grafo* grafo = malloc(sizeof(Grafo));
  if(grafo == NULL)
  {
	  printf("Erro na alocação do grafo\n");
	  exit(1);
  }
  grafo->numVertices = vertices;					// Define o número de vértices do grafo de acordo com o parâmetro
  grafo->listaAdj = malloc(vertices * sizeof(No*));	// Cria uma lista de adjacência com (número de vértices) elementos
  if(grafo->listaAdj == NULL)
  {
	  printf("Erro na alocação da lista de adjacência\n");
	  exit(1);
  }
  
  for (int i = 0; i < vertices; i++)	// Faz todos os elementos da lista apontar para NULL
    grafo->listaAdj[i] = NULL;

  return grafo;
}

No* criaNo(int v) 
{
  No* no = malloc(sizeof(No));
  if(no == NULL)
  {
	  printf("Erro na alocação de no\n");
	  exit(1);
  }
  no->vertice = v;	// Define vértice como parâmetro v
  no->peso = 0;		// Inicializa seu peso como 0
  no->prox = NULL;	// Faz o próximo apontar para NULL
  
  return no;
}

Aresta* criaAresta(int src, int dest, int peso)
{
  Aresta* nova = (Aresta *) malloc(sizeof(Aresta));
  if(nova == NULL)
  {
	  printf("Erro na alocação da aresta\n");
	  exit(1);
  }
  nova->src = src;		// Define aresta de origem como parâmetro src
  nova->dest = dest;	// Define aresta de destino como parâmetro dest
  nova->peso = peso;	// Define peso da aresta como parâmetro peso
  nova->prox = NULL;	// Faz o próximo apontar para NULL

  return nova;
}

Aresta* insereAresta(Aresta* lAresta, int src, int dest, int peso)
{
  Aresta *nova = criaAresta(src, dest, peso);
  Aresta* anterior = NULL;
  Aresta* atual = lAresta;
  if(lAresta == NULL) // Lista de Arestas vazia
  {
    return nova;	// Retorna nova
  }
  
  while(atual != NULL && peso >= atual->peso)	// Anda para colocar a nova em ordem crescente
  {
    if(peso == atual->peso && (src == atual->dest && dest == atual->src))	// Se a aresta já estiver na lista de arestas
    {
      free(nova);			// Libera a aresta nova
      return lAresta;		// Retorna a lista já existente
    }
    anterior = atual;		// Anda com os ponteiros 
    atual = atual->prox;
  }
  nova->prox = atual;		// Coloca a nova antes da atual
  if(anterior == NULL)		// Se nova for agora o primeiro elemento da lista
  {
    return nova;			// Retorna nova
  }
  anterior->prox = nova;	// Aponta anterior para nova, finalizando a inserção de nova entre dois elementos
  
  return lAresta;			// Retona o endereço da primeira aresta da lista
}

Aresta *criaListaAresta(Grafo *grafo)
{
  Aresta* lAresta = NULL;
  Aresta* auxAresta;
  for(int i = 0; i < grafo->numVertices; i++)	// Percorre todos os vértices de origem
  {
    for(No* aux = grafo->listaAdj[i]; aux != NULL; aux = aux->prox)	// Percorre todos os vértices de destino
    {
      lAresta = insereAresta(lAresta, i, aux->vertice, aux->peso);	// Insere elementos na lista de arestas
    }
  }
  for(auxAresta = lAresta; auxAresta != NULL; auxAresta = auxAresta->prox)
  {
    printf("Aresta %d - %d Peso: %d\n", auxAresta->src, auxAresta->dest, auxAresta->peso);	// Exibe os elementos da lista de arestas
  }
  
  return lAresta;	// Retorna a lista recém criada
}

void colocaVertice(Grafo* grafo, int v1, int v2, int peso) 
{
  // Inicio inserção da aresta v1 - v2 na lista de adjacência posição v1
  No* no = criaNo(v2);
  no->peso = peso;
  no->prox = grafo->listaAdj[v1];
  grafo->listaAdj[v1] = no;
  // Fim inserção

  // Inicio inserção da aresta v1 - v2 na lista de adjacência posição v2
  no = criaNo(v1);
  no->peso = peso;
  no->prox = grafo->listaAdj[v2];
  grafo->listaAdj[v2] = no;
  // Fim inserção
}

void exibeGrafo(Grafo* grafo) 
{ 
  for (int i = 0; i < grafo->numVertices; i++)	// Para todos os vértices do grafo
  {
    printf("\n Vertice %d: ", i);	// Exibe vértice de origem
    for(No* aux = grafo->listaAdj[i]; aux != NULL; aux = aux->prox)	// Para todos os vértices de destino
	{
      printf("[[%d][%d]] > ", aux->vertice, aux->peso);	// Exibe vértice de destino
    }
    printf("NULL\n\n");
  }
}

int find(int vRep[], int i)
{
    if (vRep[i] == -1)	// Se o representante da posição i for -1
        return i;		// Retorna i
    return find(vRep, vRep[i]);	// Senão chama recursivamente utilizando o representante da posição i como parâmetro
}

void Union(int vRep[], int x, int y)
{
    vRep[x] = y;	// Faz y se tornar o representante de x
}

void kruskal(Aresta* lAresta, Grafo *grafoK)
{
    Aresta *auxAresta = lAresta;
    int *vRep = (int*) malloc(grafoK->numVertices * sizeof(int));
	if(vRep == NULL)
	{
		printf("Erro ao alocar vetor de representantes\n");
		exit(1);
	}
 
    for(int i = 0; i < grafoK->numVertices; i++)	// Para todos os elementos do vetor de representantes
    {
      vRep[i] = -1;	// Inicializa todos os elementos como singletons
    }
 
    while(auxAresta != NULL)	// Enquanto a lista de arestas não for vazia
    {
      int x = find(vRep, auxAresta->src);	// Acha o representande do vértice de origem
      int y = find(vRep, auxAresta->dest);	// Acha o representande do vértice de destino
 
      if (x != y)	// Se forem diferentes
      {
        Union(vRep, x, y);	// Faz a união dos dois sets
        colocaVertice(grafoK, auxAresta->src, auxAresta->dest, auxAresta->peso);	// Coloca o vértice no grafo de Árvore Geradora
      }
      auxAresta = auxAresta->prox;	// Anda com o ponteiro
    }
}

int main() 
{
  Grafo* grafo = criaGrafo(9);
  Aresta *lAresta;
  Grafo* arvGeradora = criaGrafo(9);

  // Inicializa lista de adjacência do grafo
  colocaVertice(grafo, 0, 1, 6);
  colocaVertice(grafo, 0, 2, 9);
  colocaVertice(grafo, 0, 3, 15);
  colocaVertice(grafo, 0, 5, 23);
  colocaVertice(grafo, 1, 4, 20);
  colocaVertice(grafo, 2, 5, 13);
  colocaVertice(grafo, 2, 8, 28);
  colocaVertice(grafo, 3, 5, 10);
  colocaVertice(grafo, 3, 6, 7);
  colocaVertice(grafo, 3, 4, 4);
  colocaVertice(grafo, 4, 6, 24);
  colocaVertice(grafo, 5, 8, 18);
  colocaVertice(grafo, 5, 6, 5);
  colocaVertice(grafo, 6, 7, 15);
  colocaVertice(grafo, 7, 8, 17);
 
  printf (" \n [[vertice][peso]] > Próximo Adjacente do Vértice \n\n");
  exibeGrafo(grafo);	// Exibe o grafo

  lAresta = criaListaAresta(grafo);	// Gera a lista de aresta a partir do grafo acima
  kruskal(lAresta, arvGeradora);	// Cria árvore geradora com o algoritmo de Kruskal

  printf (" \n [[vertice][peso]] > Próximo Adjacente do Vértice \n\n");
  exibeGrafo(arvGeradora);			// Exibe árvore geradora
  
  return 0;
}