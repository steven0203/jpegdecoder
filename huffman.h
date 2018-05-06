#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "segment.h"



typedef struct node
{
    struct node *left;
    struct node *right;
    unsigned char symbol;
    unsigned short codeword;
    unsigned char depth;
}node;


int isLeaf(node *);
node *createNode();
node *createHuffmanTree(DHTdata);
void *insertHuffmanNode(node *,unsigned short,unsigned char );
unsigned short checkCodeWord(unsigned short,unsigned char);
void removeTree(node *);
void bfs(node *);


#endif

