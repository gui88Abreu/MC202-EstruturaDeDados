/*
NOME: GUILHERME DE BRITO ABREU
RA: 173691
Data: 04/2017

DESCRIÇÃO: Este programa visa implementar um compactador simples de arquivos.
Esta implementação baseia-se no algoritmo clássico de David A. Huffman que foi criado
em 1952 em seu doutorado. O programa tanto compacta como descompacta o arquivo.
*/
#include <stdio.h>
#include <stdlib.h>
#include "bitstream.h"
#include <inttypes.h>
#include <assert.h>
#include <string.h>

typedef struct huffmanTree{
    byte charact;/*Guarda o caracterer*/
    long int frequency;/*Guarda a frequencia do caracterer no arquivo*/
    byte isLeaf;/*Define se é uma folha ou um nó*/
    struct huffmanTree *left, *right;
}HuffmanTree, *Tree;

void packFile(int, char **);/*Compacta arquivo*/
void unpackFile(int , char **);/*Descompacta arquivo*/
uint32_t le_arquivo(int *v, char *arqEntrada, char *arqSaida);/*Lê o arquivo, encontra o seu tamanho, encontra a frequencia dos caracteres
e retorna o numero de caracteres*/
Tree *create_Heap(int*, uint32_t);/*Cria vetor para aramzenar o HeapMIN */
void create_nodes(Tree *, int *);/*Cria os nós da arvore de Huffman*/
void HeapMinFix(Tree[], uint32_t, uint32_t);/*Conserta o HeapMin passado de cima para baixo*/
void fixUpHeapMin(Tree*, uint32_t, uint32_t);/*Conserta o HeapMin passado de baixo para cima*/
void BuildMinHeap(Tree[], uint32_t);/*Constroi um HeapMin baseado na frequencia de cada caracterer*/
Tree HeapMin(Tree *, uint32_t*);/*Retorna o menor nó do HeapMin baseado na frequencia*/
Tree create_Tree(Tree*, uint32_t);/*Cria a árvore de Huffman*/
void encodeTree(Tree, bitstream *, bitstreamoutput *);/*Codifica a Árvore de Huffman*/
void encodeFile(Tree, char*, char*);/*Codifica o arquivo*/
void createDictionary(Tree, bitstream []);/*Cria um dicionário de códigos para o arquivo*/
void fillDict(Tree, bitstream [], bitstream);/*Preenche o dicionário de códigos para o arquivo*/
void encodeInfo(FILE *, bitstreamoutput *, bitstream []);/*Codifica as informações do arquivo baseado no dicionário criado*/
void eraseTree(Tree );/*Exclui a Árvore*/
void buildFile(FILE *, FILE *, bitstreaminput *, Tree, uint32_t);/*Constroi arquivo original*/
void decodeFile(char *, char *);/*Decodifica o arquivo binario*/
void buildTree(bitstreaminput *, Tree);/*Construi arvore de Huffman a partir do arquivo binario*/
Tree createMemoryTree(byte, int);/*Cria memória para um nó da arvore de Huffman*/

int main(int argc, char **argv){
    if(strcmp(argv[1],"c") == 0)
        packFile(argc,argv);
    else if(strcmp(argv[1],"d") == 0)
        unpackFile(argc,argv);
    else
        printf("Argumentos aceitos: (c/d) arquivoDeEntrada arquivoDeSaida\n");
    
    return 0;
}
void unpackFile(int argc, char **argv){
    assert(argc == 4);

    decodeFile(argv[2],argv[3]);
    
    return;
}

void decodeFile(char *arqEntrada, char *arqSaida){
    FILE *arquivoIn = fopen(arqEntrada,"rb");;
    uint32_t t;
    Tree root = createMemoryTree(0,0);
    bitstreaminput *bsi = criaEncodedInput(arquivoIn);
    
    assert(fread(&t, sizeof(uint32_t), 1,arquivoIn));

    buildTree(bsi, root);

    buildFile(arquivoIn, fopen(arqSaida,"wb"), bsi, root, t);

    eraseTree(root);
    return;
}

void buildFile(FILE *arquivoIn, FILE *arquivoOut, bitstreaminput *bsi, Tree p, uint32_t t){
    Tree q = p;
    uint32_t bytes = 0;

    while(bytes < t){
        while(!p->isLeaf){
            if(bsiPop(bsi))
                p = p->right;
            else
                p = p->left;
        }
        fwrite(&p->charact, sizeof(byte), 1, arquivoOut);
        bytes++;
        p = q;
    }

    fclose(arquivoIn);
    fclose(arquivoOut);
    destroiEncodedInput(bsi);
    return;
}

void buildTree(bitstreaminput *bsi, Tree p){
    int bit = bsiPop(bsi);
    if (bit == 0){
        if(p->left == NULL){
            p->left = createMemoryTree(0,0);
            buildTree(bsi,p->left);
            p->right = createMemoryTree(0,0);
            buildTree(bsi,p->right);
        }
        else{
            p->right = createMemoryTree(0,0);
            buildTree(bsi,p->right);
        }
    }
    else{
        p->charact = bsiPopByte(bsi);
        p->isLeaf = 1;
    }

    return;
}

Tree createMemoryTree(byte byt, int isLeaf){
    Tree node = (Tree)malloc(sizeof(HuffmanTree));
    node->left = NULL;
    node->right = NULL;
    node->charact = byt;
    node->isLeaf = isLeaf;
    return node;
}

void packFile(int argc, char **argv){
    int *characters = (int *)calloc(256,sizeof(int));
    uint32_t numChar = 0;
    Tree *heap, root;

    assert(argc == 4);
    
    numChar = le_arquivo(characters, argv[2], argv[3]);

    heap = create_Heap(characters, numChar);
    
    root = create_Tree(heap, numChar);

    encodeFile(root,argv[3], argv[2]);

    eraseTree(root);
    free(characters);
    return;
}

void eraseTree(Tree p){
    if(p){
        eraseTree(p->left);
        eraseTree(p->right);
        free(p);
    }
    
    return;
}

void encodeFile(Tree root, char *arqSaida, char *arqEntrada){
    FILE *arquivo1 = fopen(arqSaida,"ab");
    FILE *arquivo2 = fopen(arqEntrada,"rb");
    bitstreamoutput *bso = criaEncodedOutput(arquivo1);
    bitstream bs, dict[256];
    bsClean(&bs);

    encodeTree(root, &bs, bso);
    
    createDictionary(root,dict);
    
    encodeInfo(arquivo2, bso, dict);

    destroiEncodedOutput(bso);
    
    fclose(arquivo1);
    fclose(arquivo2);
    return;
}

void encodeInfo(FILE *arquivo, bitstreamoutput *bso, bitstream dict[]){
    byte byt;
    
    while (fread(&byt, sizeof(byte), 1, arquivo))
        encodedOutputWrite(bso,&dict[(int)byt]);
    
    return;
}

void createDictionary(Tree root, bitstream dict[]){
    bitstream bs;
    bsClean(&bs);

    fillDict(root, dict, bs);
  
    return;
}

void encodeTree(Tree p, bitstream *bs, bitstreamoutput *bso){
    if (p) {
        if(p->isLeaf){
            bsPushInPlace(1,bs);
            encodedOutputWrite(bso,bs);
            encodedOutputRawWrite(bso,p->charact);
            bsClean(bs);
        }
        else{
            bsPushInPlace(0,bs);
        }
        encodeTree(p->left,bs,bso);
        encodeTree(p->right,bs,bso);
    }
}

void fillDict(Tree p, bitstream dict[], bitstream bs){
    
    if(p->isLeaf){
        dict[(int)(p->charact)] = bs;
    }
    else{
        fillDict(p->left, dict, bsPush (0, bs));
        fillDict(p->right, dict, bsPush (1, bs));
    }
    
    return;
}

uint32_t le_arquivo(int *v, char *arqEntrada, char *arqSaida){
    FILE *arquivo = fopen(arqEntrada,"rb");
    uint32_t lSize = 0, numChar = 0;
    byte byt;

    while (fread(&byt, sizeof(byte), 1, arquivo)){
        v[(int)byt]++;
        if(v[(int)byt] == 1)
            numChar++;
        lSize++;
    }

    if(!numChar)
        numChar = -1;

    fclose(arquivo);

    arquivo = fopen(arqSaida,"wb");

    fwrite(&lSize, sizeof(uint32_t), 1, arquivo);

    fclose(arquivo);

    return numChar;
}

Tree *create_Heap(int *v, uint32_t w){
    Tree *r = (Tree*)malloc((w+1)*sizeof(Tree));

    create_nodes(r, v);
    
    BuildMinHeap(r,w);

    return r;
}

void create_nodes(Tree *r, int *v){
    int i = 0,k = 1;

    while(i<256){
        if(v[i] > 0){
            r[k] = (Tree)calloc(1,sizeof(HuffmanTree));
            r[k]->charact = i;
            r[k]->isLeaf = 1;
            r[k]->frequency = v[i];
            r[k]->left = NULL;
            r[k]->right = NULL;
            k++;
        }
        i++;
    }

    return;
}

void BuildMinHeap(Tree *A, uint32_t n){
    uint32_t i;

    for (i = n/2; i > 0; i--){
        HeapMinFix(A, n, i);
    }

    return;
}

void HeapMinFix(Tree *A, uint32_t n, uint32_t i){
    Tree t; 
    uint32_t f = 2*i;

    while (f <= n) {
        if (f < n && A[f]->frequency > A[f+1]->frequency) 
            f++;

        if (A[f/2]->frequency <= A[f]->frequency) 
            break;
        
        t = A[f/2]; 
        A[f/2] = A[f]; 
        A[f] = t;
        f = f*2;
    }

    return;
}

Tree HeapMin(Tree *A, uint32_t *n){
    Tree a;
    a = A[1];
    A[1] = A[*n];
    (*n)--;
    HeapMinFix(A,*n,1);
    return a;
}

Tree create_Tree(Tree *A, uint32_t n){
    Tree a,b,c = NULL;

    while(n!=1){
        a = HeapMin(A,&n);
        b = HeapMin(A,&n);
        c = (Tree)malloc(sizeof(HuffmanTree));
        c->frequency = a->frequency + b->frequency;
        c->isLeaf = 0;
        c->left = a;
        c->right = b;
        n++;
        A[n] = c;
        fixUpHeapMin(A,n,n);
    }
    free(A);
    return c;
}

void fixUpHeapMin(Tree *A, uint32_t n, uint32_t i){
    Tree t; 
    uint32_t f = i;

    assert(i <= n);

    while (f > 1) {
        if (f > 1 && A[f]->frequency >= A[f/2]->frequency) 
            break;
        
        t = A[f/2]; 
        A[f/2] = A[f]; 
        A[f] = t;
        f = f/2;
    }

    return;
}