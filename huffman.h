#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "segment.h"



typedef struct
{
    node *left;
    node *right;
    unsigned char symbol;
}node;


int isLeaf(node *);
node *createNode();
node *createHuffmamTree(DHTdata);
void *insertHuffmamNode(node *,unsigned short,unsigned char );
int checkCodeWord(unsigned short,unsigned char);

#endif

