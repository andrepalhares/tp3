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

int min(int x, int y) { return (x < y)? x: y; }

int vertex_cover(struct Grafo* grafo, int indice, int *checado, int *valor_lista) {
	int prox_no, filho_no, valor = 0;
	checado[indice] = 1;
	if(valor_lista[indice]) {
		checado[indice] = 0;
		return valor_lista[indice];
	}
	
	int vc_com_raiz = 1;
	struct No *no_aux = grafo->adj[indice].cabeca;
	while(no_aux) {
		prox_no = no_aux->indice;
		if(checado[prox_no] == 0) {
			vc_com_raiz += vertex_cover(grafo, prox_no, checado, valor_lista);
		}
		no_aux = no_aux->proximo;
	}
	
	int vc_sem_raiz = 0;
	no_aux = grafo->adj[indice].cabeca;
	while(no_aux) {
		prox_no = no_aux->indice;
		if(checado[prox_no] == 0) {
			vc_sem_raiz++;
			struct No *seg_no = grafo->adj[prox_no].cabeca;
			while(seg_no) {
				filho_no = seg_no->indice;
				if (filho_no != indice) {
					vc_sem_raiz += valor_lista[filho_no];
				}
				seg_no = seg_no->proximo;
			}
		}
		no_aux = no_aux->proximo;
	}
	
	int resultado = min(vc_com_raiz, vc_sem_raiz);
	valor_lista[indice] = resultado;
	checado[indice] = 0;
	return resultado;
}

void main(int argc, char ** argv) {
	int vertices, arestas;
	inicializa_grafo(argv[1], &vertices, &arestas);
	int checado[vertices];
	memset(checado, 0, vertices*sizeof(checado[0]));
	int valor_lista[vertices];
	memset(valor_lista, 0, vertices*sizeof(valor_lista[0]));
	struct Grafo* graph = cria_grafo(vertices, arestas);
	inicializa_arestas(argv[1], graph);
	//print_grafo(graph);
	printf("%d", vertex_cover(graph, 0, checado, valor_lista));
}
