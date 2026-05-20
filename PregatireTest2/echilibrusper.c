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
struct Nod {
	Pilot info;
	struct Nod* stanga;
	struct Nod* dreapta;

};
typedef struct Nod Nod;
Pilot citestePilotDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = (",\n");
	fgets(buffer, 100, f);
	char* aux;
	Pilot p;
	aux = strtok(buffer, sep);
	p.id = atoi(aux);
	p.nrPas = atoi(strtok(NULL, sep));
	p.greutate = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	p.echipa = malloc(strlen(aux) + 1);
	strcpy_s(p.echipa, strlen(aux) + 1, aux);
	return p;

}
void afisarePilot(Pilot p) {
	printf("%d\n", p.id);
	printf("%d\n", p.nrPas);
	printf("%.2f\n", p.greutate);
	printf("%s\n", p.echipa);
}
int inaltimeArbore(Nod* radacina) {
	if (radacina) {
		return max(inaltimeArbore(radacina->stanga), inaltimeArbore(radacina->dreapta)) + 1;
	}
	else return 0;
}
int verificaEchilibru(Nod* radacina) {
	if (radacina) {
		return inaltimeArbore(radacina->stanga) - inaltimeArbore(radacina->dreapta);
	}
	else
		return 0;
}
void rotireStanga(Nod** radacina) {
	Nod* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = (*radacina);
	(*radacina) = aux;
}
void rotireDreapta(Nod** radacina) {
	Nod* aux = (*radacina)->stanga;
	(*radacina)->stanga = aux-> dreapta;
	aux->dreapta = (*radacina);
	(*radacina) = aux;
}
void adaugaPilotInArboreEchilibrat(Nod** radacina, Pilot pilotnou) {
	if (*radacina) {
		if ((*radacina)->info.id > pilotnou.id) {
			adaugaPilotInArboreEchilibrat(&(*radacina)->stanga, pilotnou);
		}
		else
		{
			adaugaPilotInArboreEchilibrat(&(*radacina)->dreapta, pilotnou);
		}
		int verif = verificaEchilibru(*radacina);
		if (verif == 2) {
			if (verificaEchilibru((*radacina)->stanga) == -1) {
				rotireStanga(&(*radacina)->stanga);
			}
			rotireDreapta(radacina);
		}
		if (verif == -2) {
			if (verificaEchilibru((*radacina)->dreapta) == 1) {
				rotireDreapta(&(*radacina)->dreapta);
			}
			rotireStanga(radacina);
		}
	}
	else
	{
		Nod* aux = (Nod*)malloc(sizeof(Nod));
		aux->dreapta = NULL;
		aux->stanga = NULL;
		aux->info = pilotnou;
		(*radacina) =aux;
	}
}

Nod* citireeArboreDepilot(const char* numefisier) {
	FILE* f = fopen(numefisier, "r");
	Nod* radacina = NULL;
	
	while (!feof(f)) {
		adaugaPilotInArboreEchilibrat(&radacina, citestePilotDinFisier(f));
	}
	fclose(f);
	return radacina;
}
void afisarePilotDinFisier(Nod* radacina) {
	if (radacina) {
		afisarePilotDinFisier(radacina->stanga);
		afisarePilotDinFisier(radacina->dreapta);
		afisarePilot(radacina->info);

	}
	else
		return;
}
void dezalocare(Nod** radacina) {
	dezalocare(&(*radacina)->stanga);
	dezalocare(&(*radacina)->dreapta);
	free((*radacina)->info.echipa);
	free((*radacina));
	(*radacina) = NULL;
}