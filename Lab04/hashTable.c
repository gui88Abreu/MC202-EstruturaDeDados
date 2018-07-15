#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define M 100

typedef struct _lista{
/*ESTRUTURA DE DADOS PARA UMA LISTA LIGADA*/
	int info;
	char name[20];
	struct _lista *prox;
}Lista,*list;

typedef struct _hashTable{
/*ESTRUTURA DE DADOS PARA UMA HASHTABLE*/
	list l;
}hashTable, *table;

int hash(char *, int );/*CALCULA HASH DE UMA STRING E RETORNA O SEU VALOR*/
void add_Table(int , table *, char*, int );/*ADICIONA UM ELEMENTO NA TABELA*/
table TableMemory();/*CRIA UM ESPACO DE MEMORIA PARA UMA POSICAO DE UMA TABELA E RETORNA O SEU ENDERECO*/
list ListMemory();/*CRIA UM ESPACO DE MEMORIA PARA UMA POSICAO DE UMA LISTA E RETORNA O SEU ENDERECO*/
void append(table , char *, int );/*INSERE UM NOVO ELEMENTO NUME LISTA*/
list search(char*, table*, int );/*PROCURA POR UM ELEMENTO DA TABELA E RETORNA O ENDERECO DE MEMORIA CASO TENHA ACHADO E NULL CASO CONTRARIO*/
void printInfo(list , char*);/*IMPRIMI AS INFORMACOES DE UM ELEMENTO DA TABELA*/
void eraselist(list);/*DESALOCA A MEMORIA DE UMA LISTA INTEIRA*/
void eraseTable(table *, int );/*DESALOCA A MEMORIA DE UMA TABELA INTEIRA*/

int main(int argc, char *argv[]){
	table *t = (table*)calloc(M,sizeof(table));
	char reading[20];
	int i = 0, x;

	while(i < 10){
		scanf("%s",reading);
		scanf("%d",&x);
		add_Table(hash(reading,M),t,reading,x);
		i++;
	}

	while(!(strcmp(reading,"FIM") == 0)){
		scanf("%s",reading);
		printInfo(search(reading,t,M), reading);
	}

	x = 0;
	for(i=0;i < M; i++){
		if(t[i])
			x++;
	}

	printf("Numero de posicoes preenchidas: %d\n",x);
	eraseTable(t,M);
	return 0;
}

int hash(char *reading, int N){
	float A = (sqrt(5) - 1.0)/2.0;
	long long int key = 0, str = strlen(reading);
	int i;
	
	for(i = 0; i < str; i++){
		key = key + pow(reading[i],i);
		key = key%N;
	}

	return floor((float)N*(float)((float)key*A - floor(key*A)));
}

void add_Table(int h, table *t, char *reading,int x){
	if(t[h]){
		append(t[h],reading,x);
	}
	else{
		t[h] = TableMemory();
		append(t[h],reading,x);
	}
	return;
}

table TableMemory(){
	table new = (table)malloc(sizeof(hashTable));
	new->l = ListMemory();
	return new;
}

list ListMemory(){
	list new = (list)malloc(sizeof(Lista));
	new->prox = NULL;
	return new;
}

void append(table t, char *reading,int x){
	list new = ListMemory();
	strcpy(new->name,reading);
	new->info = x;
	new->prox = t->l->prox;
	t->l->prox = new;
	return;
}

list search(char *reading, table *t, int N){
	int h = hash(reading,N);
	list current;
	if(t[h]){
		current = t[h]->l->prox;
		while(current!=NULL && !(strcmp(current->name,reading)==0)){
			current = current->prox;
		}
	}
	else{
		current = NULL;
	}

	return current;
}

void printInfo(list aux, char *reading){
	if(aux){
		printf("\nNome: %s\n",aux->name);
		printf("Numero: %d\n", aux->info);
	}
	else{
		switch(strcmp(reading, "FIM")){
			case 0:
				break;
			default:
				printf("\nNome inexistente!!!\n");
		}
	}
}

void eraselist(list x){
	list aux;
	while(x->prox!=NULL){
		aux = x->prox;
		x->prox = aux->prox;
		free(aux);
	}
	free(x);
	return;
}

void removeElement(table e){
	if(e){
		eraselist(e->l);
		free(e);
	}
	return;
}

void eraseTable(table *t, int N){
	int i;

	for(i = 0; i < N; i++){
		removeElement(t[i]);
	}

	free(t);
}