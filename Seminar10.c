#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct NodArbore {
	Masina info;
	struct NodArbore* right;
	struct NodArbore* left;
};
typedef struct NodArbore NodArbore;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

int calculeazaInaltimeArbore(NodArbore* root) {
	if (root == NULL)
	{
		return 0;

	}
	int dr = calculeazaInaltimeArbore(root->right);
	int st = calculeazaInaltimeArbore(root->left);
	return 1 + max(dr, st);
}
void rotireStanga(NodArbore** root) {
	NodArbore* aux = (*root)->right;
	(*root)->right = aux->left;
	aux->left = (*root);
	(*root) = aux;
}
void rotireDreapta(NodArbore** root) {
	NodArbore* aux = (*root)->left;
	(*root)->left = aux->right;
	aux->right = (*root);
	(*root) = aux;
}
int verificaEchilibru(NodArbore* root) {
	int dr = calculeazaInaltimeArbore(root->right);
	int st = calculeazaInaltimeArbore(root->left);
	return st - dr;

}
void adaugaMasinaInArboreEchilibrat(NodArbore** root, Masina masinaNoua) {
	if ((*root) != NULL) {
		if (masinaNoua.id > (*root)->info.id)
		{
			adaugaMasinaInArboreEchilibrat(&(*root)->right, masinaNoua);
		}
		else
		{
			adaugaMasinaInArboreEchilibrat(&(*root)->left, masinaNoua);
		}
		//incepe verificarea echilibrului
		int factorEchilibru = verificaEchilibru(*root);
		if (factorEchilibru == -2)
		{
			//dezechilibru la dreapta
			if (verificaEchilibru((*root)->right) == 1)
			{
				rotireDreapta(&(*root)->right);
			}
			rotireStanga(&(*root));

		}
		if (factorEchilibru == 2)
		{
			//dezechilibru la stanga
			if (verificaEchilibru((*root)->left) == -1)
			{
				rotireStanga(&(*root)->left);
			}
			rotireDreapta(&(*root));

		}
	}
	else
	{
		NodArbore* nou = malloc(sizeof(NodArbore));
		nou->left = NULL;
		nou->right = NULL;
		nou->info = masinaNoua;
		(*root) = nou;
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	NodArbore* root = NULL;
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		while (!feof(file)) {
			adaugaMasinaInArboreEchilibrat(&root, citireMasinaDinFisier(file));
		}
	}
	fclose(file);
	return root;
}


void afisareMasiniDinArbore(NodArbore* root) {
	if (root) {
		afisareMasina(root->info);
		afisareMasiniDinArbore(root->left);
		afisareMasiniDinArbore(root->right);
	}
}

void dezalocareArboreDeMasini(NodArbore** root) {
	if (*root) {
		dezalocareArboreDeMasini(&(*root)->left);
		dezalocareArboreDeMasini(&(*root)->right);

		free((*root)->info.model);
		free((*root)->info.numeSofer);

		free(*root);

		*root = NULL;
	}
	//sunt dezalocate toate masinile si arborele de elemente
}


Masina getMasinaByID(NodArbore* root, int id) {
	Masina m;
	m.id = -1;
	if (root == NULL) {
		return m;
	}
	else if (root->info.id > id) {
		return getMasinaByID(root->left, id);
	}
	else if (root->info.id < id) {
		return getMasinaByID(root->right, id);
	}
	else
	{
		m = root->info;
		m.model= (char*)malloc(strlen(root->info.model) + 1);
		strcpy(m.model, root->info.model);
		m.numeSofer = (char*)malloc(strlen(root->info.numeSofer) + 1);
		strcpy(m.numeSofer, root->info.numeSofer);
		return m;
	}
}

int determinaNumarNoduri(NodArbore* root) {
	if (root != NULL) {
		return 1 + determinaNumarNoduri(root->left) + determinaNumarNoduri(root->right);
	}
	else {
		return 0;
	}
}
float calculeazaPretTotal(NodArbore * root) {
	if (root) {
		return root->info.pret + calculeazaPretTotal(root->right) + calculeazaPretTotal(root->left);
	}
	else {
		return 0;
		}
	}
	


	float calculeazaPretulMasinilorUnuiSofer(NodArbore* root, const char* numeSofer) {
		float sum = 0;
		if (root) {

			sum += calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer);
			sum += calculeazaPretulMasinilorUnuiSofer(root->right, numeSofer);
			if (strcmp(root->info.numeSofer, numeSofer) == 0) {
				sum += root->info.pret;
			}
		}
		return sum;
	}



int main() {
	NodArbore* root = NULL;
	root = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(root);
	float calculPret = calculeazaPretulMasinilorUnuiSofer(root, "Ionescu");
	printf("Masinile lui %s valoreaza %2.f \n", "Ionescu", calculPret);
	

	printf("Masina cu id-ul %d:\n", 5);
	afisareMasina(getMasinaByID(root, 5));
	

	printf("Arborele are %d noduri.\n", determinaNumarNoduri(root));
	

	printf("Masinile din arbore valoreaza %2.f.\n", calculeazaPretTotal(root));
	

	dezalocareArboreDeMasini(&root);
	printf("Test dezalocare:\n");
	afisareMasiniDinArbore(root);
	
	return 0;
}