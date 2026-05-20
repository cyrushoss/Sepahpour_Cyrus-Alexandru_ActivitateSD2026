#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include<stdio.h>
#include<string.h>
struct Pilot {
	int id;
	int nrPas;
	float greutate;
	char* echipa;
};
typedef struct Pilot Pilot;
struct Heap {
	int lungime;
	int nrElemviz;
	Pilot* vector;
};
typedef struct Heap Heap;
Pilot citirePilotDinFisiier(FILE* f) {
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
	strcpy(p.echipa, aux);
	return p;
}
void afisarePilot(Pilot p) {
	printf("%d\n", p.id);
	printf("%d\n", p.nrPas);
	printf("%.2f\n", p.greutate);
	printf("%s\n", p.echipa);
}
Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrElemviz = 0;
	heap.vector = malloc(lungime * sizeof(Pilot));
	return heap;
}
void filtrareHeap(Heap heap,int  pozNod) {
	int stanga = 2 * pozNod + 1;
	int dreapta = 2 * pozNod + 2;
	int pozmax = pozNod;
	if (stanga<heap.nrElemviz && heap.vector[stanga].id > heap.vector[pozmax].id) {
		pozmax = stanga;
	}
	if (dreapta<heap.nrElemviz && heap.vector[dreapta].id > heap.vector[pozmax].id) {
		pozmax = dreapta;
	}
	if (pozmax != pozNod) {
		Pilot aux;
		aux = heap.vector[pozmax];
		heap.vector[pozmax] = heap.vector[pozNod];
		heap.vector[pozNod] = aux;
		if (pozmax < ((heap.nrElemviz - 2) / 2)) {
			filtrareHeap(heap, pozmax);
		}
	}
}
Heap citireHeapdePilotDinFisier(const char* numefisier) {
	FILE* f = fopen(numefisier, "r");
	Heap heap = initializareHeap(10);
	while (!feof(f)) {
		heap.vector[heap.nrElemviz++] = citirePilotDinFisiier(f);
	}fclose(f);
	for (int i = (heap.nrElemviz - 2) / 2; i >= 0; i--) 
	{
		filtrareHeap(heap, i);
	}
	return heap;
}
void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemviz; i++) {
		afisarePilot(heap.vector[i]);

	}
}
Pilot extraPilot(Heap* heap) {
	Pilot aux;
	aux.id = -1;
	if (heap->nrElemviz > 0) {
		heap->nrElemviz--;
		aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElemviz];
		heap->vector[heap->nrElemviz] = aux;
		filtrareHeap(*heap, 0);
	}
	return aux;
}
int main() {
	Heap heap = citireHeapdePilotDinFisier("piloti.txt");
	afisareHeap(heap);
	afisarePilot(extraPilot(&heap));
	return 0;
}