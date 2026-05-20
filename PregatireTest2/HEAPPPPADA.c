#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct Pilot {
	int id;
	int nrPas;
	float greutate;
	char* echipa;

};
typedef struct Pilot Pilot;
struct Heap {
	Pilot* vector;
	int lungime;
	int nrElemviz;
};
typedef struct Heap Heap;
Pilot citestePilotDinFisier(FILE* f){
	char buffer[100];
	char sep[3] = (",\n");
	fgets(buffer, 100, f);
	Pilot p;
	char* aux;
	aux = strtok(buffer, sep);
	p.id = atoi(aux);
	p.nrPas = atoi(strtok(NULL, sep));
	p.greutate = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	p.echipa = malloc(strlen(aux) + 1);
	strcpy_s(p.echipa, strlen(aux) + 1, aux);
	return p;
	}
void afisarePiloat(Pilot p) {
	printf("%d\n",p.id);
	printf("%d\n",p.nrPas);
	printf("%.2f\n", p.greutate);
	printf("%s\n",p.echipa);
}
Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrElemviz = 0;
	heap.vector = malloc(lungime * sizeof(Pilot));
	return heap;
}
void filtrareHeap(Heap heap, int pozitieNod) {
	int stanga = 2 * pozitieNod + 1;
	int dreapta = 2 * pozitieNod + 2;
	int pozmax = pozitieNod;
	if (stanga < heap.nrElemviz && heap.vector[stanga].id > heap.vector[pozmax].id) {
		pozmax = stanga;
	}
	if (dreapta<heap.nrElemviz && heap.vector[dreapta].id>heap.vector[pozmax].id) {
		pozmax = dreapta;
	}
	if (pozmax != pozitieNod) {
		Pilot aux = heap.vector[pozmax];
		heap.vector[pozmax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozmax < ((heap.nrElemviz - 2) / 2)) {
			filtrareHeap(heap, pozmax);
		}
	}
}
Heap citireHeapDePilotDinFisier(const char* numefisier) {
	FILE* f = fopen(numefisier, "r");
	Heap heap = initializareHeap(10);
	while (!feof(f)) {
		heap.vector[heap.nrElemviz++] = citestePilotDinFisier(f);

	}
	for (int i = ((heap.nrElemviz - 2) / 2); i >= 0; i--) {
		filtrareHeap(heap, i);
	}
	return heap;
}
void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemviz; i++) {
		afisarePiloat(heap.vector[i]);
	}
}
void afisareHeapAscuns(Heap heap) {
	for (int i =heap.nrElemviz; i < heap.lungime; i++) {
		afisarePiloat(heap.vector[i]);
	}
}
Pilot extragePilot(Heap* heap) {
	Pilot aux;
	aux.id = -1;
	if (heap->nrElemviz > 0)
	{
		heap->nrElemviz--;
		aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElemviz];
		heap->vector[heap->nrElemviz] = aux;
		filtrareHeap(*heap,0 );
	}
		return aux;
	
}
void  dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].echipa);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->nrElemviz = 0;
	heap->lungime = 0;
}
int main() {
	Heap heap = citireHeapDePilotDinFisier("piloti.txt");

	printf("Heap:\n");
	afisareHeap(heap);

	printf("Extrageri:\n");
	afisarePiloat(extragePilot(&heap));
	afisarePiloat(extragePilot(&heap));
	afisarePiloat(extragePilot(&heap));

	printf("Heap ascuns:\n");
	afisareHeapAscuns(heap);

	dezalocareHeap(&heap);
	return 0;
}