#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct Pilot {
	int id;
	int nrPasageri;
	float greutate;
	char* echipa;
};
typedef struct Pilot Pilot;
struct Nod {
	Pilot info;
	struct Nod* dreapta;
	struct Nod* stanga;
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
	p.nrPasageri = atoi(strtok(NULL, sep));
	p.greutate = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	p.echipa = malloc(strlen(aux) + 1);
	strcpy_s(p.echipa, strlen ( aux) + 1, aux);
	return p;

}
void afisarePilotDinFisier(Pilot p)
{
	printf("%d\n", p.id);
	printf("%d\n", p.nrPasageri);
	printf("%.2f\n", p.greutate);
	printf("%s\n", p.echipa);
}
int inaltimeArbore(Nod* radacina) {
	if (radacina) {
		return max(inaltimeArbore(radacina->stanga), inaltimeArbore(radacina->dreapta)) + 1;
	}
	else return 0;
}
int calculeazaEchilibru(Nod* radacina) {
	if (radacina) {
		return inaltimeArbore(radacina->stanga) - inaltimeArbore(radacina->dreapta);
	}
	else return 0;
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
void adaugaPilotInArbore(Nod** radacina, Pilot pilotnou) {
	if (*radacina) {
		if (pilotnou.id > (*radacina)->info.id) {
			adaugaPilotInArbore(&(*radacina)->dreapta, pilotnou);
		}
		else {
			adaugaPilotInArbore(&(*radacina)->stanga, pilotnou);
		}
		int verif = calculeazaEchilibru(*radacina);
		if (verif == 2)
		{
			if (calculeazaEchilibru((*radacina)->stanga)==-1) {
				rotireStanga(&(*radacina)->stanga);

			}
			rotireDreapta(&(*radacina));
		}
		if (verif == -2) {
			if (calculeazaEchilibru((*radacina)->dreapta)==1)
			{
				rotireDreapta(&(*radacina)->dreapta);
			}
			rotireStanga(&(*radacina));
		}
	}
	else {
		Nod* aux=(Nod*)malloc(sizeof(Nod));
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
		adaugaPilotInArbore(&radacina,citirePilotDinFisier(f));
	}
	fclose(f);
	return radacina;
}
void afisareArbore(Nod* radacina) {
	if (radacina) {
		afisarePilotDinFisier(radacina->info);
		afisareArbore(radacina->stanga);
		afisareArbore(radacina->dreapta);
	}
}
void dez(Nod** radacina) {
	if (*radacina){
		dez(&(*radacina)->stanga);
	dez(&(*radacina)->dreapta);
	free((*radacina)->info.echipa);
	free(*radacina);
	(*radacina) = NULL;
}
}


Pilot getPilotById(Nod* radacina, int id) {
	Pilot p;
	p.id = -1;
	if (radacina == NULL) {
		return p;
	}
	else if (radacina->info.id > id) {
			return getPilotById(radacina->stanga, id);
		}
		else if (radacina->info.id < id)
	{ return getPilotById(radacina->dreapta, id); }
	
	else {
		p = radacina->info;
		p.echipa = malloc(strlen(radacina->info.echipa) + 1);
		strcpy(p.echipa, radacina->info.echipa);
		return p;
	}
}
int getnrPilot(Nod* radacina) {
	return getnrPilot(radacina->stanga) + getnrPilot(radacina->dreapta) + 1;
}
int main() {
	Nod* radacina = citireArboreDePilotDinFisier("piloti.txt");

	printf("Arbore:\n");
	afisareArbore(radacina);

	printf("Cauta pilot cu id 3:\n");
	Pilot p = getPilotById(radacina, 3);
	if (p.id != -1)
		afisarePilotDinFisier(p);
	else
		printf("Pilotul nu a fost gasit\n");
	free(p.echipa);

	dez(&radacina);
	return 0;
}