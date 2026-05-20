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
struct Nod {
	 Pilot info;
	Nod* stanga;
	Nod* dreapta;
};
typedef struct Nod Nod;
Pilot citirePilotDinfFisier(FILE* f) {
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
void adaugaPilotInArbore(Nod** radacina, Pilot pilotnou) {
	if (*radacina) {
		if ((*radacina)->info.id > pilotnou.id)
			adaugaPilotInArbore(&(*radacina)->stanga, pilotnou);
		else if ((*radacina)->info.id < pilotnou.id)
			adaugaPilotInArbore(&(*radacina)->dreapta, pilotnou);
	}
	else
	{
		Nod* temp = (Nod*)malloc(sizeof(Pilot));
		temp->info = pilotnou;
		temp->stanga = NULL;
		temp->dreapta = NULL;
		(*radacina) = temp;
	}
}
Nod* citireArboreDePilotDinFisier(const char* numefisier) {
	FILE* f = fopen(numefisier, "r");
	Nod* arbore = NULL;
	while (!feof(f)) {
		adaugaPilotInArbore(&arbore, citirePilotDinfFisier(f));
	}
	fclose(f);
	return arbore;
}
void afisarePilotDinArborePostordine(Nod* radacina) {
	if (radacina) {
		afisarePilotDinArborePostordine(radacina->stanga);
		afisarePilotDinArborePostordine(radacina->dreapta);
		afisarePilot(radacina->info);
	}
}
void dezalocare(Nod** radacina) {
	if (*radacina) {
		dezalocare(&(*radacina)->stanga);
		dezalocare(&(*radacina)->dreapta);
		if ((*radacina)->info.echipa) {
			free((*radacina)->info.echipa);
		}
		free(*radacina);
		*radacina = NULL;
	}
}