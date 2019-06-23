#include <stdio.h> 
#include <stdlib.h> 

struct No {
	int indice;
	struct No* proximo;
};

struct Lista {
	int adjacencias;
	int checado;
	struct No* cabeca;
};

struct Grafo {
	int vertices;
	int arestas;
	struct Lista* adj;
};

struct No* cria_no(int vertice) {
	struct No* novo_no = (struct No*) malloc (sizeof(struct No));
	novo_no->indice = vertice;
	novo_no->proximo = NULL;
	return novo_no;
}

struct Grafo* cria_grafo(int V, int A) {
	struct Grafo* grafo = (struct Grafo*) malloc(sizeof(struct Grafo));
	grafo->vertices = V;
	grafo->arestas = A;
	grafo->adj = (struct Lista*) malloc(V * sizeof(struct Lista));
	
	for(int i=0; i < V; i++) {
		grafo->adj[i].cabeca = NULL;
		grafo->adj[i].checado = 0;
	}
	
	return grafo;
}

void adiciona_filho(struct Grafo* grafo, int a, int b) {
	struct No* novo_no = cria_no(b);
	novo_no->proximo = grafo->adj[a].cabeca;
	grafo->adj[a].cabeca = novo_no;
	grafo->adj[a].adjacencias++;
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

int vertex_cover(struct Grafo* graph, int index) {
	if(graph->adj[index].adjacencias == 0) {
		printf("%d\n", graph->adj[index].adjacencias);
		return 0;
	}
	
	if(graph->adj[index].checado != 0) {
		return graph->adj[index].checado;
	}
	
	//Calcula a solução mínima quando o nó raiz faz parte da solução
	int vc_com_raiz = 1; //o próprio nó raiz 
	int indice_filho = -1;
	struct No* filho = graph->adj[index].cabeca;
	while(filho) {
		indice_filho = filho->indice;
		vc_com_raiz += vertex_cover(graph, indice_filho);
		filho = filho->proximo;
	}
	
	//Calcula a solução mínima quando o nó raiz NÃO faz parte da solução
	int vc_sem_raiz = 0;
	int indice_neto = -1;
	struct No* neto = graph->adj[index].cabeca;
	filho = graph->adj[index].cabeca;
	while(filho) {
		vc_sem_raiz++;
		indice_filho = filho->indice;
		neto = graph->adj[indice_filho].cabeca;
		while(neto) {
			indice_neto = neto->indice;
			vc_sem_raiz += vertex_cover(graph, indice_neto);
			neto = neto->proximo;
		}
		filho = filho->proximo;
	}
	
	//printf("\n\ncom %d sem %d\n\n", vc_com_raiz, vc_sem_raiz);
	
	if(vc_com_raiz < vc_sem_raiz)
		graph->adj[index].checado = vc_com_raiz;
	graph->adj[index].checado = vc_sem_raiz;
	
	//printf("\n Node raiz da vez: %d\nGrau de cobertura: %d\n", index, graph->adj[index].checado);
	
	return graph->adj[index].checado;	
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
			adiciona_filho(grafo, vertice1, vertice2);
			j++;
		}
	}
	fclose(trilha);
}

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

void main(int argc, char ** argv) {
	int vertices, arestas;
	inicializa_grafo(argv[1], &vertices, &arestas);
	struct Grafo* graph = cria_grafo(vertices, arestas);
	inicializa_arestas(argv[1], graph);
	//print_grafo(graph);
	printf("%d", vertex_cover(graph, 0));
}