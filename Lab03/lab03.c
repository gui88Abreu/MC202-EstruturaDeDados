/*
NOME: GUILHERME DE BRITO ABREU
RA: 173691

DESCRIÇÃO DO FUNCIONAMENTO DO PROGRAMA: ESTE PROGRAMA VISA IMPLEMENTAR UMA CALCULADORA DE PRECISÃO PARA NUMEROS INTEIROS, OS NUMEROS
DADOS COMO ENTRADA PODEM CONTER ATÉ 1998 DÍGITOS E A PRECISÃO DO NUMEROS DADO COMO RESPOSTA VARIA DE ACORDO
COM O TAMANHO DA MEMÓRIA DISPONÍVEL. A CALCULADORA POSSUI APENAS 5 OPERAÇÕES, AS QUAIS SÃO: SOMA, SUBTRAÇÃO,
DIVISÃO, MULTIPLICAÇÃO E RESTO DA DIVISÃO. A EXPRESSÃO SERÁ INTERPRETADA NO MODO PÓS-FIXO, OU SEJA, A OPERAÇÃO SERÁ 
APLICADA NOS DOIS ÚLTIMOS INTEIROS DADOS.

DESCRIÇÃO DO CÓDIGO: PARA A IMPLEMENTAÇÃO DO PROGRAMA FORAM UTILIZADAS DUAS ESTRUTURAS DE DADOS, PILHA E LISTA LIGADA.
A PILHA FOI UTILIZADA PARA ARMAZENAR OS NUMEROS NA ORDEM DESEJADA DAS OPERAÇÕES, POIS BASTA DESEMPILHAR DOIS NUMEROS PARA
APLICAR UMA OPERAÇÃO E EMPILHAR O RESULTADO. A LISTA LIGADA FOI UTILIZADA PARA REPRESENTAR OS NUMEROS, JÁ QUE
OS ALGORITMOS IMPLEMENTADOS PARA REALIZAR AS OPERAÇÕES APLICAM AS OPERAÇÕES DÍGITO A DÍGITO. ALÉM DA API's NECESSÁRIAS
PARA CADA ESTRUTURA DE DADOS, TAMBÉM FOI NECESSÁRIO CRIAR FUNÇÕES PARA REALIZAR AS OPERAÇÕES, FORAM CRIADAS UMA PARA 
COPIAR O NUMERO, UMA PARA VERIFICAR SE UM NUMERO É MAIOR OU IGUAL A OUTRO E OUTRA PARA REMOVER ZEROS A ESQUERDA. 

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _longLongLongNumber{/*STRUCT PARA UM NUMERO GIGANTESCO*/
	int numDigits;/*ARMAZENA O NUMERO DE DÍGITOS DO NUMERO*/
	char signal;/*ARAMZENA O SINAL DO NUMERO*/
	int dig;/*ARMAZENA UM DIGITO*/
	struct _longLongLongNumber *next, *previous;
}longLongLongNumber, *number;

typedef struct _stackNumbers{/*STRUCT PARA PILHA*/
	number num;
	struct _stackNumbers *next;
}stackNumbers, *stack;

void inserts(int , number);/*INSERE UM DIGITO NO INICIO DA LISTA LIGADA*/
void printNumber(number);/*IMPRIME NA TELA UM NUMERO*/
void eraseNumber(number);/*DESALOCA A MEMÓRIA DE UM NUMERO*/
number removeFromStack(stack );/*REMOVE UM NUMERO DA PILHA E RETORNA O SEU ENDEREÇO*/
void insertsResultStack(number , stack );/*INSERE UM NUMERO NA PILHA*/
void insertsNumberStack(stack, char *);/*TRANSFORMA UMA STRING DADA EM UM NUMEROS E O INSERE NA PILHA*/
number add(number , number);/*RETORNA O RESULTADO DA SOMA ENTRE DOIS NUMEROS*/
int removeFromNumber(number);/*REMOVE O PRIMEIRO DIGITO DE UM NUMERO E RETORNA O SEU VALOR*/
void insertsInEnd(int , number);/*INSERE UM DIGITO NO FINAL DA LISTA LIGADA*/
number insertsHead(int, char);/*RETORNA UMA CABEÇA PARA LISTA LIGADA VAZIA*/
number sub(number, number);/*RETORNA O RESULTADO DA SUBTRAÇÃO ENTRE DOIS NUMEROS*/
int greaterEqual(number , number);/*RETORNA 1 SE O NUMEROS FOR MAIOR, 0 SE FOR MENOR E -1 CASO SEJA IGUAL AO OUTRO NUMERO*/
void removeZeros(number );/*REMOVE ZEROS A ESQUERDA DE UM NUMERO*/
number mult(number, number);/*RETORNA O RESULTADO DA MULTICLAÇÃO ENTRE DOIS NUMEROS*/
number *Div(number, number);/*REALIZA UMA DIVISÃO SOBRE DOIS NUMEROS E RETORNA O RESULTADO DA DIVISÃO E O RESTO*/
number copyNumber(number );/*COPIA UMA LISTA LIGADA E  RETORNA UMA CABECA PARA A COPIA*/
void eraseStack(stack);/*DESALOCA MEMORIA DE UMA PILHA*/

int main(){
	stack p = (stack)malloc(sizeof(stackNumbers));
	p->next = NULL;
	number a,b, *c, d;
	char read[2000],i=0, k;
	
	k = scanf("%s",read);/*K EXISTE APENAS PARA ELIMINAR WARNINGS*/
	
	while(strcmp(read,"FIM")!=0){
		i++;
		if(read[0] > 47 && read[0] < 58){
			insertsNumberStack(p,read);
		}
		else{
			switch(read[0]){
				case '+':
					b = removeFromStack(p);
					a = removeFromStack(p);
					removeZeros(a);
					removeZeros(b);
					if(a->signal == '-' && b->signal == '+')
						d= sub(b,a);
					else if(a->signal == '+' && b->signal == '-')
						d = sub(a,b);
					else if(a->signal == '-' && b->signal == '-'){
						d = add(a,b);
						d->signal = '-';
					}
					else
						d = add(a,b);
					
					eraseNumber(a);
					eraseNumber(b);
					printNumber(d);
					insertsResultStack(d,p);
					break;
				case '-':
					if(read[1] == '\0'){
						b = removeFromStack(p);
						a = removeFromStack(p);
						removeZeros(a);
						removeZeros(b);
						if(a->signal == '-' && b->signal == '+'){
							d = add(a,b);
							d->signal = '-';
						}
						else if(a->signal == '+' && b->signal == '-')
							d = add(a,b);
						else if(a->signal == '-' && b->signal == '-'){
							d = sub(b,a);
						}
						else
							d = sub(a,b);

						eraseNumber(a);
						eraseNumber(b);
						printNumber(d);
						insertsResultStack(d,p);
					}
					else{
						insertsNumberStack(p,read);
					}
					break;
				case '*':
					b = removeFromStack(p);
					a = removeFromStack(p);
					removeZeros(a);
					removeZeros(b);
					if(a->signal!=b->signal){
						d = mult(a,b);
						d->signal = '-';
					}
					else{
						d = mult(a,b);
					}

					eraseNumber(a);
					eraseNumber(b);
					printNumber(d);
					insertsResultStack(d,p);
					break;
				case '/':
					b = removeFromStack(p);
					a = removeFromStack(p);
					removeZeros(a);
					removeZeros(b);
					if(a->signal!=b->signal){
						c = Div(a,b);
						c[0]->signal = '-';
					}
					else{
						c = Div(a,b);
					}
					
					eraseNumber(a);
					eraseNumber(b);
					printNumber(c[0]);
					insertsResultStack(c[0],p);
					free(c);
					break;
				case '%':
					b = removeFromStack(p);
					a = removeFromStack(p);
					removeZeros(a);
					removeZeros(b);
					if(a->signal == '-'){
						c = Div(a,b);
						c[1]->signal = '-';
					}
					else{
						c = Div(a,b);
						c[1]->signal = '+';
					}
					
					eraseNumber(a);
					eraseNumber(b);
					printNumber(c[1]);
					insertsResultStack(c[1],p);
					free(c);
					break;
				default:
					printf("Entrada Invalida\n");
					exit(1);
			}
		}
		k = scanf("%s",read);
	}
	eraseStack(p);
	return 0;
}

void eraseStack(stack p){
	while(p->next){
		eraseNumber(removeFromStack(p));
	}
	free(p);
}

int removeFromNumberFromEnd(number x){
	int a = 0;
	number aux;
	if(x->previous){
		aux = x->previous;
		a = aux->dig;
		x->previous = aux->previous;
		free(aux);
		aux = x->previous;
		if(aux)
			aux->next = NULL;
		else{
			x->next = NULL;
		}
	}
	return a;
}

number *Div(number a, number b){
	number *c = (number*)malloc(2*sizeof(number));
	c[0] = insertsHead(0,'+');
	number aux1 = insertsHead(0, '+'), aux2 = insertsHead(0, '+');
	number one = insertsHead(0, '+');
	insertsInEnd(1,one);
	number p,q,o, aux3;
	int first = 1, i,GE = greaterEqual(a,b);

	if( GE == 0){
		insertsInEnd(0,c[0]);
		c[0]->numDigits++;
		c[1] = copyNumber(a);
		eraseNumber(aux2);
		eraseNumber(one);
	}
	else if(GE == -1){
		insertsInEnd(1,c[0]);
		c[0]->numDigits++;
		c[1] = aux1;
		insertsInEnd(0,c[1]);
		c[1]->numDigits++;
		eraseNumber(aux2);
		eraseNumber(one);
	}
	else{
		q = copyNumber(b);
		o = copyNumber(one);
		while(a->next){
			if(first){
				first = 0;
				do{
					inserts(removeFromNumberFromEnd(a),aux1);
					removeZeros(aux1);
					aux1->numDigits++;
				}while(a->next && !greaterEqual(aux1,b));
			}
			else{
				inserts(removeFromNumberFromEnd(a),aux1);
				removeZeros(aux1);
				aux1->numDigits++;
				while(a->next && !greaterEqual(aux1,b)){
					inserts(0,c[0]);
					c[0]->numDigits++;
					inserts(removeFromNumberFromEnd(a),aux1);
					removeZeros(aux1);
					aux1->numDigits++;
				}				
			}
			i = 0;
			do{
				aux2 = add(aux2,one);
				p = copyNumber(aux2);
				aux3 = mult(b,aux2);
				b = copyNumber(q);
				one = copyNumber(o);
				aux2 = copyNumber(p);
				i++;
			}while(greaterEqual(aux1,aux3));
			inserts(i-1,c[0]);
			c[0]->numDigits++;
			aux3 = sub(aux3,b);
			aux1 = sub(aux1,aux3);
			b = copyNumber(q);
			eraseNumber(p);
			eraseNumber(aux2);
			aux2 = insertsHead(0, '+');
		}
		eraseNumber(o);
		eraseNumber(q);
		eraseNumber(aux2);
		eraseNumber(one);
		c[1] = aux1;
	}
	return c;
}

number copyNumber(number x){
	number y = insertsHead(x->numDigits,x->signal);
	while(x->next){
		x = x->next;
		insertsInEnd(x->dig,y);
	}
	return y;
}

number mult(number a, number b){
	int carry,x,y,z,i,j;
	number c = insertsHead(0,'+');
	number p,q;
	
	insertsInEnd(0,c);
	c->numDigits++;
	if(a->numDigits > b->numDigits){
		number s[b->numDigits];
		p = a;
		q = b;
		for(i=0; i < b->numDigits; i++){
			s[i] = insertsHead(0,'+');
			j = 0;
			while(j < i){
				insertsInEnd(0,s[i]);
				s[i]->numDigits++;
				j++;
			}	
			p = a;
			q = q->next;
			y = q->dig;
			carry = 0;
			while(p->next){
				p = p->next;
				x = p->dig;
				z = (x*y + carry)%10;
				carry = (x*y + carry)/10;
				insertsInEnd(z,s[i]);
				s[i]->numDigits++;
			}
			if(carry){
				insertsInEnd(carry,s[i]);
				s[i]->numDigits++;
			}
			c = add(c,s[i]);
			eraseNumber(s[i]);
		}
	}
	else{
		number s[a->numDigits];
		p = b;
		q = a;
		for(i=0; i < a->numDigits; i++){
			s[i] = insertsHead(0,'+');
			j = 0;
			while(j < i){
				insertsInEnd(0,s[i]);
				s[i]->numDigits++;
				j++;
			}	
			p = b;
			q = q->next;
			y = q->dig;
			carry = 0;
			while(p->next){
				p = p->next;
				x = p->dig;
				z = (x*y + carry)%10;
				carry = (x*y + carry)/10;
				insertsInEnd(z,s[i]);
				s[i]->numDigits++;
			}
			if(carry){
				insertsInEnd(carry,s[i]);
				s[i]->numDigits++;
			}
			c = add(c,s[i]);
			eraseNumber(s[i]);
		}
	}

	removeZeros(c);
	return c;
}

int greaterEqual(number a, number b){
	number aux = a;
	if(a->numDigits > b->numDigits)
		return 1;
	else if(a->numDigits < b->numDigits)
		return 0;
	else{
		while(a->previous!=aux){
			a = a->previous;
			b = b->previous;
			if(a->dig > b->dig)
				return 1;
			else if(a->dig < b->dig)
				return 0;
		}
	}
	
	return -1;
}

number sub(number a, number b){
	int carry = 0, x, y, z, GE = greaterEqual(a,b);
	number c = insertsHead(0,'+');

	if(GE == 1){
		while(a->next || b->next){
			x = removeFromNumber(a);
			y = removeFromNumber(b);
			if(x >= y + carry){
				z = x - y - carry;
				carry = 0;
			}
			else{
				z = x - y + 10 - carry;
				carry = 1;
			}
			insertsInEnd(z,c);
			c->numDigits++;
		}
	}
	else if(GE == -1){
		z = 0;
		insertsInEnd(z,c);
		c->numDigits++;
	}
	else{
		while(a->next || b->next){
			x = removeFromNumber(b);
			y = removeFromNumber(a);
			if(x >= y + carry){
				z = x - y - carry;
				carry = 0;
			}
			else{
				z = x - y + 10 - carry;
				carry = 1;
			}
			insertsInEnd(z,c);
			c->numDigits++;
		}
		c->signal = '-';
	}
	
	removeZeros(c);
	return c;
}

void removeZeros(number c){
	number q,aux = c->previous;
	while(aux!=c->next && aux->dig == 0){
		q = aux->previous;
		c->previous = q;
		q->next = aux->next;
		aux = q;
		c->numDigits--;
	}
}

number add(number a, number b){
	int carry = 0, x,y,z;
	number c = insertsHead(0,'+');

	while(a->next || b->next){
		x = removeFromNumber(a);
		y = removeFromNumber(b);
		z = x + y + carry;
		if(z>9){
			z = z - 10;
			carry = 1;
		}
		else{
			carry = 0;
		}
		insertsInEnd(z,c);
		c->numDigits++;
	}
	if(carry){
		insertsInEnd(carry,c);
		c->numDigits++;
	}

	return c;
}

int removeFromNumber(number x){
	int a = 0;
	number aux;
	if(x->next){
		aux = x->next;
		a = aux->dig;
		x->next = aux->next;
		free(aux);
		aux = x->next;
		if(aux)
			aux->previous = x;
		else{
			x->previous = NULL;
		}
	}
	return a;
}

void insertsInEnd(int a, number x){
	number aux, new = (number)malloc(sizeof(longLongLongNumber));
	new->dig = a;
	if(x->next){
		aux = x->previous;
		new->previous = aux;
		new->next = aux->next;
		aux->next = new;
		x->previous = new;
	}
	else{
		new->previous = x;
		new->next = x->next;
		x->next = new;
		x->previous = new;
	}
}

number removeFromStack(stack p){
	number x = NULL;
	stack aux;

	if(p->next){
		aux = p->next;
		x = aux->num;
		p->next = aux->next;
		free(aux);
	}
	return x;
}

void insertsResultStack(number a, stack p){
	stack z = (stack)malloc(sizeof(stackNumbers));
	z->num = a;
	z->next = p->next;
	p->next = z;
}

void insertsNumberStack(stack p, char *a){
	int i,j, k = strlen(a);
	number new;
	stack x = (stack)malloc(sizeof(stackNumbers));

	if(a[0] == '-'){
		j = 1;
		new = insertsHead(k-1,'-');
	}
	else{
		j = 0;
		new = insertsHead(k,'+');
	}

	for(i=j;i<k;i++){
		inserts(a[i] - 48,new);
	}

	x->num = new;
	x->next = p->next;
	p->next = x;
}

void inserts(int a, number x){
	number aux, new = (number)malloc(sizeof(longLongLongNumber));
	new->dig = a;
	aux = x->next;
	new->previous = x;
	new->next = aux;
	x->next = new;
	if(x->previous==NULL)
		x->previous = new;
	else
		aux->previous = new;
}

number insertsHead(int length, char a){
	number x = (number)malloc(sizeof(longLongLongNumber));
	x->numDigits = length;
	x->signal = a;
	x->previous = NULL;
	x->next = NULL;
	return x;
}

void printNumber(number x){
	number aux = x;
	if(!x) return;
	if(x->next){
		if(x->numDigits == 1 && x->next->dig == 0)
			x->signal = '+';
		
		if(x->signal == '-')
			printf("-");
		while(x->previous!=aux){
			x = x->previous;
			printf("%d",x->dig);
		}
		printf("\n");
	}
}

void eraseNumber(number x){
	if (x){
		while(x->next){
			removeFromNumber(x);
		}
		free(x);
	}
}