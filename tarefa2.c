#include<stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>


struct No {
	int indice;
	struct No *proximo;
};

struct Lista {
	int indice;
	struct No* cabeca;
};

struct Grafo {
	int vertices;
	int arestas;
	struct Lista * adj;
};

void inicializa_grafo(char nome[64], int * vertices, int * arestas) {
	FILE *trilha;
	trilha = fopen(nome, "r");
	int qte_vertices, qte_arestas;

	fscanf (trilha, "%d", &qte_vertices);
	fscanf (trilha, "%d", &qte_arestas);
	
	*vertices = qte_vertices;
	*arestas = qte_arestas;

	fclose(trilha);
}

struct No* cria_no(int vertice) {
	struct No* novo_no = (struct No*) malloc (sizeof(struct No));
	novo_no->indice = vertice;
	novo_no->proximo = NULL;
	return novo_no;
}

void adiciona_aresta(struct Grafo* grafo, int a, int b) {
	struct No* no_a = cria_no(b);
	struct No* no_b = cria_no(a);
	
	no_a->proximo = grafo->adj[a].cabeca;
	no_b->proximo = grafo->adj[b].cabeca;
	
	grafo->adj[a].cabeca = no_a;
	grafo->adj[b].cabeca = no_b;
}

void inicializa_arestas(char nome[64], struct Grafo* grafo) {
	FILE *trilha;
	trilha = fopen(nome, "r");
	char line[256];
	int i = 0;
	int j = 0;
	int vertice1, vertice2;
	while (fgets(line, sizeof(line), trilha)) {
		i++;
		if(i <= grafo->arestas ) {
			fscanf(trilha, "%d %d", &vertice1, &vertice2);
			adiciona_aresta(grafo, vertice1, vertice2);
			j++;
		}
	}
	fclose(trilha);
}


struct Grafo* cria_grafo(int V, int A) {
	struct Grafo* grafo = (struct Grafo*) malloc(sizeof(struct Grafo));
	grafo->vertices = V;
	grafo->arestas = A;
	grafo->adj = (struct Lista*) malloc(V * sizeof(struct Lista));
	
	for(int i=0; i < V; i++) {
		grafo->adj[i].indice = i;
		grafo->adj[i].cabeca = NULL;
	}
	
	return grafo;
}

void print_grafo(struct Grafo* grafo) {
	for(int i=0; i < grafo->vertices; i++) {
		struct No* aux = grafo->adj[i].cabeca;
		printf("[%d] -> ", i);
		
		while(aux) {
			printf("%d ", aux->indice);
			aux = aux->proximo;
		}
		printf("\n");
	}
}

struct Lista retorna_vizinhos(struct Grafo* grafo, int indice) {
	struct Lista vizinhos;
	vizinhos = grafo->adj[indice];
	return vizinhos;
}

void vertex_cover (struct Grafo* grafo) {
	int checado[grafo->vertices], tamanho_solucao = 0;;
	memset(checado, 0, grafo->vertices*sizeof(checado[0])); 
	
	for(int i=0; i < grafo->vertices; i++) {
		if(checado[i] == 0) {
			struct Lista vizinhos = retorna_vizinhos(grafo, i);
			struct No *no_aux = vizinhos.cabeca;
			while(no_aux) {
				int u = no_aux->indice;
				if(checado[u] == 0) {
					checado[i] = 1;
					checado[u] = 1;
					break;
				}
				no_aux = no_aux->proximo;
			}
		}
	}
	
	for(int i=0; i < grafo->vertices; i++) {
		if(checado[i]) {
			tamanho_solucao++;
		}
	}
	printf("%d \n", tamanho_solucao);
	for(int i=0; i < grafo->vertices; i++) {
		if(checado[i]) {
			printf("%d\n", i);
		}
	}
}

void main(int argc, char ** argv) {
	int vertices, arestas;
	inicializa_grafo(argv[1], &vertices, &arestas);
	struct Grafo* graph;
	graph = cria_grafo(vertices, arestas);
	inicializa_arestas(argv[1], graph);
	//print_grafo(graph);
	vertex_cover(graph);
}
