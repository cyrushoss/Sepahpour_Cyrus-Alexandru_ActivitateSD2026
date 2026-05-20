#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct Pilot {
	int id;
	int nrPas;
	float greutate;
	char* echipe;
};
typedef struct Pilot Pilot;
struct Nod {
	Pilot info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

Pilot citirePilotDinFisier(FILE* f) {
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
	p.echipe = malloc(strlen(aux) + 1);
	strcpy_s(p.echipe, strlen(aux) + 1, aux);
	return p;
}
void afisarePilotDinFisier(Pilot p)
{
	printf("%d\n", p.id);
	printf("%d\n", p.nrPas);
	printf("%.2f\n", p.greutate);
	printf("%s\n", p.echipe);
};
int inaltimeArbore(Nod* radacina) {
	return max(inaltimeArbore(radacina->stanga), inaltime(radacina->dreapta)) + 1;
}
int calculeazaEchil(Nod* radacina) {
	return inaltimeArbore(radacina->stanga) - inaltimeArbore(radacina->dreapta);

}
void rotireDreapta(Nod** radacina) {
	Nod* aux = (*radacina)->stanga;
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = (*radacina);
	(*radacina) = aux;
}
void rotireStanga(Nod** radacina) {
	Nod* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = (*radacina);
	(*radacina) = aux;
}
int adaugaPilotInArboreEchilibrat(Nod** radacina, Pilot pilotnou) {
	if (*radacina) {
		if (pilotnou.id > (*radacina)->info.id)
		{
			adaugaPilotInArboreEchilibrat(&(*radacina)->dreapta, pilotnou);
		}
		else {
			adaugaPilotInArboreEchilibrat(&(*radacina)->stanga, pilotnou);
		}
		int verifEchil = calculeazaEchil((*radacina));
		if (verifEchil == -2) {
			if (calculeazaEchil((*radacina)->dreapta) == 1) {
				rotireDreapta(&(*radacina)->dreapta);
			}
			rotireStanga(&(*radacina));
		}
		else if (verifEchil == 2) {
			if (calculeazaEchil((*radacina)->stanga) == -1) {
				rotireStanga(&(*radacina)->stanga);
			}
			rotireDrtaptaa(&(*radacina));
		}
	}
	else {
		Nod* aux = (Nod*)malloc(sizeof(Nod));
		aux->dreapta = NULL;
		aux->stanga = NULL;
		aux->info = pilotnou;
		(*radacina) = aux;
	}
}
Nod* citireArboreDePilotDinFisier(const char* numefisier) {
	FILE* f = fopen(numefisier, "r");
	Nod* radacina = NULL;
	while (!feof(f)) {
		adaugaPilotInArboreEchilibrat(&radacina, citirePilotDinFisier(f));
	}
	fclose(f);
	return radacina;
}
void afisareArboreDePilotDinFisier(Nod* radacina) {
	afisareArboreDePilotDinFisier(radacina->stanga);
	afisareArboreDePilotDinFisier(radacina->dreapta);
	afisarePilotDinFisier(radacina->info);
}
int main() {
	Nod* radacina = citireArboreDePilotDinFisier("piloti.txt");
	afisareArboreDePilotDinFisier(radacina);
	return 0;
}