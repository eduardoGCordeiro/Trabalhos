#include <stdio.h>
#include <stdlib.h>

#ifndef TIPO
  #define TIPO 
#endif

struct hole{
	int tam;				//qual o tamanho do buraco
	struct hole *ant; //buraco anterior;
	struct hole *prox;	//ptr para a proximo buraco
};

struct hole *listaMemoria, *ptrNextFit; //ptr p/ lista de buracos
void *enderecoInicio;

int tamHole = sizeof(struct hole);

int memoria_base = 1024;

int mod(int valor){
	if(valor < 0)
		return valor * (-1);
	else return valor;
}

void mostra_mem(){
	int livre = 0, ocupada = 0, i=-1;
	printf("\n");
	struct hole *atual = listaMemoria;
	while (atual != NULL) {
		if(atual->tam >= 0)
			livre += atual->tam;
		else ocupada += mod(atual->tam);
		printf("%d | ", atual->tam);
		atual = atual->prox;
		i++;
	}
	printf("\nTotal Livre: %d | Total Ocupada: %d\n", livre, ocupada+tamHole*i);
}

void inicializa_mem(){
	listaMemoria = malloc(memoria_base); //listaMemoria aponta para um espaco de hole de espaco tam;
	//printf("%p", listaMemoria);
	listaMemoria->tam = memoria_base;
	listaMemoria->ant = NULL;
	listaMemoria->prox = NULL;
	ptrNextFit = listaMemoria;
	/*void *aux = listaMemoria + listaMemoria->tam;
	listaMemoria = aux;*/
}

void merge(){
	struct hole *atual = listaMemoria, *anterior, *proximo;
	int aux;
	while(atual != NULL){
		if(atual->tam >= 0){//O buraco esta livre e pode ser juntado
			proximo = atual->prox;
			if(proximo != NULL){//Nao esta no fim da lista
				if(proximo->tam >= 0){//Pode ser juntado com seu antecessor
					atual->prox = proximo->prox;
					aux = atual->tam + tamHole + proximo->tam;
					//printf("%d %d %d\n", atual->tam, tamHole, proximo->tam);
					atual->tam = aux;
					if(proximo->prox != NULL)
						proximo->prox->ant = atual;
				}else{
					atual = atual->prox;
				}
			}else{
				return;
			}
		}else{
			atual = atual->prox;
		}
	}
}

void *firstfit(int tam){
	struct hole *atual, *anterior;
	void *aux;
	atual = listaMemoria;
	while (atual != NULL) { //Percorro os buracos
		if(atual->tam >= (tam + tamHole)){ //Posso meu_aloca aqui
			int tamanhoOriginal = atual->tam;
			aux = atual + atual->tam - tam;
			anterior = atual;
			atual = aux;

			atual->ant = anterior;
			atual->prox = anterior->prox;
			anterior->prox = atual;
			anterior->tam = tamanhoOriginal-tam-tamHole;
			atual->tam = (-1)*tam; //Marco com -1 pra dizer que está sendo usada

			if(atual->prox != NULL){ //Nao Estou no fim da lista
				atual->prox->ant = atual; //faz o proximo apontar pra tras
			}
			printf("\nAlocado %d", tam);
			ptrNextFit = aux;
			return aux+tamHole;
		}
		anterior = atual;
		atual = atual->prox;
	}
	printf("\nImpossivel Alocar\n");
	return NULL;
}

void *bestfit(int tam){
	struct hole *atual = listaMemoria, *ptrMenor = NULL, *anterior;
	void *aux;
	int menorTam = memoria_base;

	while(atual != NULL){
		if(atual->tam >= tam+tamHole){//Posso meu_aloca aqui
			if(atual->tam <= menorTam){
				menorTam = atual->tam;
				ptrMenor = atual;
			}
		}
		atual = atual->prox;
	}

	if(ptrMenor == NULL){
		printf("\nImpossivel Alocar\n");
		return NULL;
	}
	if(menorTam == memoria_base)
		atual = listaMemoria;
	else
		atual = ptrMenor;
	int tamanhoOriginal = atual->tam;
	aux = atual + atual->tam - tam;
	anterior = atual;
	atual = aux;
	atual->ant = anterior;
	atual->prox = anterior->prox;
	anterior->prox = atual;
	anterior->tam = tamanhoOriginal-tam-tamHole;
	atual->tam = (-1)*tam; //Marco com -1 pra dizer que está sendo usada

	if(atual->prox != NULL){ //Nao Estou no fim da lista
		atual->prox->ant = atual; //faz o proximo apontar pra tras
	}
	printf("\nAlocado %d", tam);
	ptrNextFit = aux;
	return aux+tamHole;
}

void *worstfit(int tam){
	struct hole *atual = listaMemoria, *ptrMaior, *anterior;
	void *aux;
	int maiorTam = 0;

	while(atual != NULL){
		if(atual->tam >= tam+tamHole){//Posso meu_aloca aqui
			if(atual->tam > maiorTam){
				maiorTam = atual->tam;
				ptrMaior = atual;
			}
		}
		atual = atual->prox;
	}
	if(maiorTam == 0){
		printf("\nImpossivel Alocar!\n");
		return NULL;
	}

	atual = ptrMaior;
	int tamanhoOriginal = atual->tam;
	aux = atual + atual->tam - tam;
	anterior = atual;
	atual = aux;
	atual->ant = anterior;
	atual->prox = anterior->prox;
	anterior->prox = atual;
	anterior->tam = tamanhoOriginal-tam-tamHole;
	atual->tam = (-1)*tam; //Marco com -1 pra dizer que está sendo usada

	if(atual->prox != NULL){ //Nao Estou no fim da lista
		atual->prox->ant = atual; //faz o proximo apontar pra tras
	}
	printf("\nAlocado %d", tam);
	ptrNextFit = aux;
	return aux+tamHole;
}

void *nextfit(int tam){
	struct hole *atual, *anterior;
	void *aux;
	atual = ptrNextFit;
	do { //Percorro os buracos
		if(atual->tam >= (tam + tamHole)){ //Posso meu_aloca aqui
			int tamanhoOriginal = atual->tam;
			aux = atual + atual->tam - tam;
			anterior = atual;
			atual = aux;

			atual->ant = anterior;
			atual->prox = anterior->prox;
			anterior->prox = atual;
			anterior->tam = tamanhoOriginal-tam-tamHole;
			atual->tam = (-1)*tam; //Marco com -1 pra dizer que está sendo usada

			if(atual->prox != NULL){ //Nao Estou no fim da lista
				atual->prox->ant = atual; //faz o proximo apontar pra tras
			}
			printf("\nAlocado %d", tam);
			ptrNextFit = aux;
			return aux+tamHole;
		}
		anterior = atual;
		atual = atual->prox;
		if(atual == NULL)
			atual = listaMemoria;
	}while (atual != ptrNextFit);
	printf("\nImpossivel Alocar\n");
	return NULL;
}

void *meu_aloca(int metodo, int tam){
	switch (metodo) {
		case 1:
			return firstfit(tam);
			break;
		case 2:
			return bestfit(tam);
			break;
		case 3:
			return worstfit(tam);
			break;
		case 4:
			return nextfit(tam);
			break;
		default:
			return firstfit(tam);
	}
}
void meu_libera(void *ponteiro){
	if(ponteiro == NULL){
		printf("\nPonteiro Nulo!\n");
		return;
	}

	struct hole *atual;
	void *aux;

	aux = ponteiro - tamHole;
	atual = aux;

	int tam = mod(atual->tam);
	atual->tam = tam;
	merge();
	merge();
	printf("\nDesalocado %d", tam);
}

int main(){
 
  inicializa_mem();
  void *a1  =  meu_aloca(TIPO, 10);
  mostra_mem();
  void *a2  =  meu_aloca(TIPO, 5);
  mostra_mem();
  void *a3  =  meu_aloca(TIPO, 20);
  mostra_mem();
  void *a4  =  meu_aloca(TIPO, 30);
  mostra_mem();
  void *a5  =  meu_aloca(TIPO, 15);
  void *a6  =  meu_aloca(TIPO, 40);
  void *a7  =  meu_aloca(TIPO, 60);
  void *a8  =  meu_aloca(TIPO, 80);
  void *a9  =  meu_aloca(TIPO, 100);
  void *a14 = meu_aloca(TIPO, 459);
  mostra_mem();
  meu_libera( a2 );
  meu_libera( a4 );
  meu_libera( a5 );
  meu_libera( a7 );
  meu_libera( a9 );
  mostra_mem();
  void *a10 =  meu_aloca(TIPO, 3);
  mostra_mem();
  void *a11 =  meu_aloca(TIPO, 20);
  mostra_mem();
  void *a12 =  meu_aloca(TIPO, 12);
  mostra_mem();
  void *a13 =  meu_aloca(TIPO, 40);
  mostra_mem();

	return 0;
}
