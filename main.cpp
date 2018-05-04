#include<cstdio>
#include<cstdlib>
#include<vector>
#include"headProcess.h"
#include"huffman.h"
#include<queue>
using namespace std;

void bfs(node *);

int main()
{
    FILE * testFile;
    headSegment head;
    testFile=fopen("./img/monalisa.jpg","rb");
    if (testFile==NULL)
    {
        fputs ("File error",stderr); 
        exit (1);
    }


    head.headProcess(testFile);
    vector<APPdata> APP=head.APP;
    for(int i=0;i<APP.size();++i)
    {
        printf("APP%d LP : %d\n",APP[i].n,APP[i].Lp);
    }
    vector<DQTdata> DQTs=head.DQTs;
    for(int i=0;i<DQTs.size();++i)
    {
        printf("DQT length :%d number : %d precision : %d \n",DQTs[i].length,DQTs[i].number,DQTs[i].precision);
        for(int j=0;j<64;++j)
        {
            printf("%d ",DQTs[i].getQT(j));
        }
        printf("\n");
    }
    SOF0data SOF0=head.SOF0;
    printf("SOF0 length : %d precision : %d height : %d width : %d Nf :%d \n" ,SOF0.length,SOF0.precision,SOF0.height,SOF0.width,SOF0.colorComNum);
    for(int i=0;i<SOF0.colorComNum;++i)
    {
        printf("C %d H %d V %d Tq %d\n", SOF0.colorComs[i].id,SOF0.colorComs[i].horizontalFactor,SOF0.colorComs[i].verticalFactor,SOF0.colorComs[i].QtId);
    }
    vector<DHTdata>DHTs=head.DHTs;
    for(int i=0;i<DHTs.size();++i)
    {
        printf("DHT number: %d length : %d  type : %d \n",DHTs[i].number,DHTs[i].length, DHTs[i].type);
        printf("numbers :");
        for(int j=0;j<16;++j)
            printf("%d ",DHTs[i].symbolsNum[j]);
        printf("\nsymbols :");
        for(int j=0;j<DHTs[i].length-19;++j)
            printf("%d ",DHTs[i].symbols[j]);
        printf("\n");        
    }
    SOSdata SOS=head.SOS;
    printf("SOS length %d number %d \n ",SOS.length,SOS.comNum);
    for(int i=0;i<SOS.comNum;++i)
        printf("%d  %d %d \n",SOS.components[i].id,SOS.components[i].ACid,SOS.components[i].DCid);

    node *root=createHuffmanTree(head.DHTs[3]);
    bfs(root);
    removeTree(root);
    return 0;
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
            printf("%u,%u\n",tmp->codeword,tmp->symbol);
            continue;
        }
        if(tmp->left)
            q.push(tmp->left);

        if(tmp->right)
            q.push(tmp->right);
    }
}