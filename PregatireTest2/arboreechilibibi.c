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
	struct	Nod* stanga;
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
	p.nrPas=atoi(strtok(NULL, sep));
	p.greutate = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	p.echipa = malloc(strlen(aux) + 1);
	strcpy_s(p.echipa, strlen(aux)+1,aux);
	return p;
}
void afisarePilot(Pilot p) {
	printf("%d\n", p.id);
	printf("%d\n", p.nrPas);
	printf("%.2f\n", p.greutate);
	printf("%s\n", p.echipa);
}
int calculeazaInaltime(Nod* radacina) {
	if (radacina) {
		return max(calculeazaInaltime(radacina->dreapta), calculeazaInaltime(radacina->stanga)) + 1;
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
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = (*radacina);
	(*radacina) = aux;
}
int verificaEchilibru(Nod* radacina) {
	return calculeazaInaltime(radacina->stanga) - calculeazaInaltime(radacina->dreapta);
}
void adaugaNodInArboreEchilibrat(Nod** radacina, Pilot pilotnou) {
	if (*radacina) {
		if (pilotnou.id > (*radacina)->info.id)
		{
			adaugaNodInArboreEchilibrat(&(*radacina)->dreapta, pilotnou);
		}
		else if (pilotnou.id < ((*radacina)->info.id))
		{
			adaugaNodInArboreEchilibrat(&(*radacina)->stanga, pilotnou);
		}
		int factorEchilibru = verificaEchilibru(*radacina);
		if (factorEchilibru == -2)
		{
			if (verificaEchilibru((*radacina)->dreapta) == 1)
			{
				rotireDreapta(&(*radacina)->dreapta);
			}
			rotireStanga(&(*radacina));
		}
		else
			if (factorEchilibru == 2) {
				if (verificaEchilibru((*radacina)->stanga) == -1) {
					rotireStanga(&(*radacina)->stanga);
				}
				rotireDreapta(&(*radacina));
			}
	}
	else
	{
		Nod* aux = (Nod*)malloc(sizeof(Nod));
		aux->dreapta = NULL;
		aux->stanga = NULL;
		aux->info = pilotnou;
		(*radacina) = aux;
	}

	
}
Nod* citireArboreDePilotiDinFisier(const char* numefisier) {
	FILE* f = fopen(numefisier, "r");
	Nod* radacina = NULL;
	while (!feof(f)) {
		adaugaNodInArboreEchilibrat(&radacina, citirePilotDinFisier(f));
	}
	fclose(f);
	return radacina;
}
void afisarePilotiDinArbore(Nod* radacina) {
	if (radacina) {
		afisarePilotiDinArbore(radacina->stanga);
		afisarePilotiDinArbore(radacina->dreapta);
		afisarePilot(radacina->info);
	}
}
void dezalocareArboreDePilot(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreDePilot(&(*radacina)->stanga);
		dezalocareArboreDePilot(&(*radacina)->dreapta);
		free((*radacina)->info.echipa);
		free(*radacina);
		(*radacina) = NULL;
	}
}
int main() {
	Nod* radacina = citireArboreDePilotiDinFisier("piloti.txt");
	afisarePilotiDinArbore(radacina);
	return 0;
}