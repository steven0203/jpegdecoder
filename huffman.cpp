#include"huffman.h"
#include<cstdio>
#include<cstdlib>


int isLeaf(node *test)
{
    if(test->left==NULL &&test->right==NULL)
        return 0;
    return 1;
}


node *createNode()
{
    node *result=(node *)malloc(sizeof(node));
    result->left=NULL;
    result->right=NULL;
    return result;
}

int checkCodeWord(unsigned short codeword,unsigned char bit)
{
    unsigned short check=0b1000000000000000;
    if(codeword&(check<<bit))
        return 1;
    else 
        return 0;
}


void *insertHuffmamNode(node *root,unsigned short codeword,unsigned char depth,unsigned char symbol)
{
    unsigned char currentDepth=0;
    node* curNode=root;
    for(;currentDepth<=depth;++depth)
    {   
        if(checkCodeWord(codeword,depth))
        {    
            if(!curNode->right)
                curNode->right=createNode();
            curNode=curNode->right;
        }
        else
        {
            if(!curNode->left)
                curNode->left=createNode();
            curNode=curNode->left;
        }
    }
    curNode->symbol=symbol;
}


node *createHuffamTree(DHTdata DHT)
{
    node *root=createNode();
    unsigned short codeword=0xFF;
    unsigned char number=0;
    node *tmpNode=root;
    for(unsigned char index=0;index<16;)
    {   
        number=DHT.symbolsNum[index];
        if(DHT.symbolsNum[index]==0)
        {
            index+=1;
            continue;
        }

    }
}