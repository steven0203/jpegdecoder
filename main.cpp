#include<cstdio>
#include<cstdlib>
#include<vector>
#include"headProcess.h"
#include"huffman.h"
#include<queue>
#include"dataStream.h"
#include"decoder.h"
#include"bitmap_image.hpp"
using namespace std;

void printHeader(headSegment);

int main()
{
    FILE * testFile;
    headSegment head;
    testFile=fopen("./img/teatime.jpg","rb");
    if (testFile==NULL)
    {
        fputs ("File error",stderr); 
        exit (1);
    }


    head.headProcess(testFile);
    printHeader(head);
   
    decoder testdecoder(head,testFile);
    imgData result(testdecoder.height,testdecoder.width,testdecoder.colorComs.size());
    testdecoder.decode(result);

    unsigned char r,g,b;
    bitmap_image resultImg(testdecoder.width,testdecoder.height);
    for(int i=0;i<testdecoder.height;++i)
    {
        for(int j=0;j<testdecoder.width;++j)
        {
            r=result.get(i,j,0);
            g=result.get(i,j,1);
            b=result.get(i,j,2);
            resultImg.set_pixel(j,i,r,g,b);
        }
    }
    resultImg.save_image("test5.bmp");
    
    return 0;

}

void printHeader(headSegment head)
{
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
        for(int j=0;j<DHTs[i].length-16;++j)
            printf("%d ",DHTs[i].symbols[j]);
        printf("\n");        
    }
    SOSdata SOS=head.SOS;
    printf("SOS length %d number %d \n ",SOS.length,SOS.comNum);
    for(int i=0;i<SOS.comNum;++i)
        printf("%d  %d %d \n",SOS.components[i].id,SOS.components[i].ACid,SOS.components[i].DCid);

}

