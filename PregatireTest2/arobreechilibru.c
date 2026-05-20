#define _CRT_SEUCRE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
struct Pilot {
	int id;
	int nrPasager;
	float greutate;
	char* echipe;

};
typedef struct Pilot Pilot;
struct Nod {
	Pilot info;
	Nod* stanga;
	Nod* dreapta;
};
typedef struct Nod Nod;
Pilot citestePilotDinFisier(FILE* f)
{
	char buffer[100];
	char sep[3] = (",\n");
	fgets(buffer, 100, f);
	char* aux;
	Pilot p;
	aux = strtok(buffer, sep);
	p.id = atoi(aux);
	p.nrPasager = atoi(strtok(NULL, sep));
	p.greutate = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	p.echipe = malloc(strlen(aux) + 1);
	strcpy(p.echipe, aux);
	return p;
}
void afisarePilot(Pilot p) {
	printf("%d\n", p.id);
	printf("%d\n", p.nrPasager);
	printf("%.2f\n", p.greutate);
	printf("%s\n", p.echipe);
}
int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina) {
		return max(calculeazaInaltimeArbore(radacina->stanga) , calculeazaInaltimeArbore(radacina->dreapta)) + 1;
	}
}
void rotireStanga(Nod** radacina) {
	Nod* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = (*radacina);
	(*radacina) = aux;
}
void rotireDreapta(Nod** radacina) {
	Nod* aux = (*radacina)->stanga;
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = (*radacina);
	(*radacina) = aux;
}
int verificaEchilibru(Nod* radacina) {
	return calculeazaInaltimeArbore(radacina->stanga) - calculeazaInaltimeArbore(radacina->dreapta);
}
void adaugaNodInArboreEchilibrat(Nod** radacina, Pilot pilotnou) {
	if (*radacina) {
		if (pilotnou.id > (*radacina)->info.id) {
			adaugaNodInArboreEchilibrat(&(*radacina)->dreapta, pilotnou);
		}
		else
		{
			adaugaNodInArboreEchilibrat(&(*radacina)->stanga, pilotnou);
		}
		int factorEchilibru = verificaEchilibru(*radacina);
		if (factorEchilibru == -2) {
			if (verificaEchilibru((*radacina)->dreapta == 1))
			{
				rotireDreapta(&(*radacina)->dreapta);
			}
			rotireStanga(&(*radacina));
		}
		if (factorEchilibru == 2) {
			if (verificaEchilibru((*radacina)->dreapta == -1))
			{
				rotireStanga(&(*radacina)->stanga);
			}
			rotireDreapta(&(*radacina));
		}
	
	

	}
	else
	{
		Nod* aux = malloc(sizeof(Nod));
		aux->stanga = NULL;
		aux->dreapta = NULL;
		aux->info = pilotnou;
		(*radacina) = aux;

	}
}
void* citireNodDePilotDinFisier(const char* numefisier) {
	FILE* f = fopen(numefisier, "r");
	Nod* radacina = NULL;
	while (!feof(f)) {
		adaugaNodInArboreEchilibrat(&radacina, citestePilotDinFisier(f));
	}
	fclose(f);
	return radacina;

}
void afisarePilotDinArbore(Nod* radacina) {
	if (radacina)
	{
		afisarePilot(radacina->info);
		afisarePilotDinArbore(radacina->stanga);
		afisarePilotDinArbore(radacina->dreapta);
	}
}
void dezalocareArboreDePilot(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreDePilot(&(*radacina)->stanga);
		dezalocareArboreDePilot(&(*radacina)->dreapta);
		free((*radacina)->info.echipe);
		free((*radacina));
		(*radacina) = NULL;
	}

}