#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Pilot {
    int id;
    float greutate;
    int nrPasageri;
    char* echipa;
};
typedef struct Pilot Pilot;

struct Heap {
    int lungime;
    int nrElemViz;
    Pilot* vector;
};
typedef struct Heap Heap;

Pilot citirePilotDinFisier(FILE* f) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, f);
    char* aux;
    Pilot p1;
    aux = strtok(buffer, sep);
    p1.id = atoi(aux);
    p1.greutate = atof(strtok(NULL, sep));
    p1.nrPasageri = atoi(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    p1.echipa = malloc(strlen(aux) + 1);
    strcpy(p1.echipa, aux);
    return p1;
}

void afisarePilot(Pilot pilot) {
    printf("Id: %d\n", pilot.id);
    printf("Greutate: %.2f\n", pilot.greutate);
    printf("Nr pasageri: %d\n", pilot.nrPasageri);
    printf("Echipa: %s\n", pilot.echipa);
}

Heap initializare(int lungime) {
    Heap heap;
    heap.lungime = lungime;
    heap.nrElemViz = 0;
    heap.vector = malloc(lungime * sizeof(Pilot));
    return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
    int stanga = 2 * pozitieNod + 1;
    int dreapta = 2 * pozitieNod + 2;
    int pozmax = pozitieNod;

    if (stanga < heap.nrElemViz && heap.vector[stanga].id > heap.vector[pozmax].id)
        pozmax = stanga;

    if (dreapta < heap.nrElemViz && heap.vector[dreapta].id > heap.vector[pozmax].id)
        pozmax = dreapta;

    if (pozmax != pozitieNod) {
        Pilot aux = heap.vector[pozmax];
        heap.vector[pozmax] = heap.vector[pozitieNod];
        heap.vector[pozitieNod] = aux;
        if (pozmax < ((heap.nrElemViz - 2) / 2)) {
            filtreazaHeap(heap, pozmax);
        }// FIX: f?r? condi?ie restrictiv?
    }
}

Heap citireHeapDePilotDinFisier(const char* numefisier) {
    FILE* f = fopen(numefisier, "r");
    Heap heap = initializare(10);
    if (f) {
        while (!feof(f)) {
            heap.vector[heap.nrElemViz++] = citirePilotDinFisier(f);
        }
        fclose(f);
    }
    for (int i = (heap.nrElemViz - 2) / 2; i >= 0; i--)  // FIX: >= în loc de <=
        filtreazaHeap(heap, i);

    return heap;
}

void afisareHeap(Heap heap) {
    for (int i = 0; i < heap.nrElemViz; i++)
        afisarePilot(heap.vector[i]);
}

Pilot extragerePilot(Heap* heap) {
    Pilot aux;
    aux.id = -1;
    if (heap->nrElemViz > 0) {
        heap->nrElemViz--;
        aux = heap->vector[0];
        heap->vector[0] = heap->vector[heap->nrElemViz];
        heap->vector[heap->nrElemViz] = aux;
        filtreazaHeap(*heap, 0);
    }
    return aux;
}

int main() {
    Heap heap = citireHeapDePilotDinFisier("piloti.txt");
    afisareHeap(heap);
    printf("Extrageri:\n");
    afisarePilot(extragerePilot(&heap));
    afisarePilot(extragerePilot(&heap));
    afisarePilot(extragerePilot(&heap));
    return 0;
}