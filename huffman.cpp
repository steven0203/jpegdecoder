#include"huffman.h"
#include<cstdio>
#include<cstdlib>
#include<queue>

using namespace std;

int isLeaf(node *test)
{
    if(test->left==NULL &&test->right==NULL)
        return 1;
    return 0;
}


node *createNode()
{
    node *result=(node *)malloc(sizeof(node));
    result->left=NULL;
    result->right=NULL;
    return result;
}

unsigned short checkCodeWord(unsigned short codeword,unsigned char bit)
{
    return (codeword>>(15-bit))&1;
}


void *insertHuffmanNode(node *root,unsigned short codeword,unsigned char depth,unsigned char symbol)
{
    unsigned char currentDepth=0;
    node* curNode=root;
    for(;currentDepth<=depth;++currentDepth)
    {   
        if(checkCodeWord(codeword,currentDepth))
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
    curNode->codeword=codeword;
}


node *createHuffmanTree(DHTdata DHT)
{
    node *root=createNode();
    unsigned short codeword=0;
    unsigned char index=0,number=0;
    node *tmpNode=root;
    for(unsigned char i=0;i<16;++i)
    {   
        number=DHT.symbolsNum[i];
        for(;number>0;--number)
        {
            insertHuffmanNode(root,codeword<<15-i,i,DHT.symbols[index]);
            codeword=codeword+1;
            index+=1;
        }
        codeword=codeword<<1;
    }
    return root;
}


void removeTree(node *root)
{
    queue<node *>q;
    q.push(root);
    node *tmp;
    while(!q.empty())
    {
        tmp=q.front();
        q.pop();
        if(isLeaf(tmp))
        {
            free(tmp);
            continue;
        }
        if(tmp->left)
            q.push(tmp->left);

        if(tmp->right)
            q.push(tmp->right);
                free(tmp);

    }
}

void bfs(node *root)
{
    queue<node *>q;
    q.push(root);
    node *tmp;
    while(!q.empty())
    {
        tmp=q.front();
        q.pop();
        if(isLeaf(tmp))
        {
            printf("%u,%x\n",tmp->codeword,tmp->symbol);
            continue;
        }
        if(tmp->left)
            q.push(tmp->left);

        if(tmp->right)
            q.push(tmp->right);
    }
}
