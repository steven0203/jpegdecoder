#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "segment.h"



typedef struct node
{
    struct node *left;
    struct node *right;
    unsigned char symbol;
    unsigned short codeword;
}node;


int isLeaf(node *);
node *createNode();
node *createHuffmanTree(DHTdata);
void *insertHuffmanNode(node *,unsigned short,unsigned char );
int checkCodeWord(unsigned short,unsigned char);
void removeTree(node *);
#endif

