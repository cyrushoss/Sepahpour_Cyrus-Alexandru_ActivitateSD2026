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
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	Pilot p;
	char* aux = strtok(buffer, sep);
	p.id = atoi(aux);
	p.nrPas = atoi(strtok(NULL, sep));
	p.greutate = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	p.echipe = malloc(strlen(aux) + 1);
	strcpy(p.echipe, aux);
	return p;
}
void afisarePilot(Pilot p) {
	printf("%d\n", p.id);
	printf("%d\n", p.nrPas);
	printf("%.2f\n", p.greutate);
	printf("%s\n", p.echipe);
}
void adaugaPilotInArbore(Nod** radacina, Pilot pilotnou)
{
	if (*radacina) {
		if ((*radacina)->info.id > pilotnou.id)
			adaugaPilotInArbore(&(*radacina)->stanga, pilotnou);
		else if ((*radacina)->info.id < pilotnou.id)
		adaugaPilotInArbore(&(*radacina)->dreapta, pilotnou);
	}
	else
	{
		Nod* temp = (Nod*)malloc(sizeof(Nod));
		temp->info = pilotnou;
		temp->stanga = NULL;
		temp->dreapta = NULL;
		(*radacina) = temp;
	}
}
Nod* citireArboreDeMPilotiDinFisier(const char* numefisier) {
	FILE* f = fopen(numefisier, "r");
	Nod* arbore = NULL;
	while (!feof(f)) {
		adaugaPilotInArbore(&arbore, citirePilotDinFisier(f));
	}
	fclose(f);
	return arbore;
}
void afisarePilotDinArborePostordine(Nod* radacina) {
	if (radacina) {
		afisarePilotDinArborePostordine(radacina->stanga);
		afisarePilotDinArborePostordine(radacina->dreapta);
		afisarePilot(radacina->info);//post lini asta la sf pre inainte de toate si inter la mijloc
	}
}
void dezalocare(Nod** radacina) {
	if (*radacina) {
		dezalocare(&(*radacina)->stanga);
		dezalocare(&(*radacina) -> dreapta);
		if ((*radacina)->info.echipe) {
			free((*radacina)->info.echipe);
		}
		free(*radacina);
		*radacina = NULL;

	}
}
Pilot getPilotByID(Nod* radacina, int id) {
	if (radacina) {
		if (radacina->info.id == id) {
			Pilot p = radacina->info;
			p.echipe = (char*)malloc(strlen(radacina->info.echipe) + 1);
			strcpy(p.echipe, radacina->info.echipe);
			return p;
		}
		if (radacina->info.id < id)
		{
			return getPilotByID(radacina->dreapta, id);
		}
		if (radacina->info.id > id) {
			return getPilotByID(radacina->stanga, id);
		}
		else {
			Pilot p;
			p.id = -1;
			return p;
		}
	}
}
int main() {
	Nod* radacina = citireArboreDeMPilotiDinFisier("piloti.txt");
	afisarePilotDinArborePostordine(radacina);
	afisarePilot(getPilotByID(radacina, 3));
	return 0;
}