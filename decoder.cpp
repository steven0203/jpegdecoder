#include"decoder.h"
#include"huffman.h"
#include"dataStream.h"
#include"headProcess.h"
#include<cmath>
#define PI 3.14159262


decoder::decoder(headSegment &head,FILE *path):data(path)
{
    DQTs=vector<DQTdata>(head.DQTs.size());
    for(int i=0;i<DQTs.size();++i)
    {
        int index=head.DQTs[i].number;
        DQTs[index]=head.DQTs[i];
    }
    DCtree=vector<node *>(16);
    ACtree=vector<node *>(16);
    for(int i=0;i<head.DHTs.size();++i)
    {
        node *tree;
        int index=head.DHTs[i].number;
        if(head.DHTs[i].type==DC_TABLE)
            DCtree[index]=createHuffmanTree(head.DHTs[i]);
        else
            ACtree[index]=createHuffmanTree(head.DHTs[i]);
    }
    DCpredict=vector<int>(head.SOF0.colorComNum,0);
    colorComs=head.SOF0.colorComs;
    scanComs=head.SOS.components;
    height=head.SOF0.height;
    width=head.SOF0.width;
    initializeCos();
}

int decoder::decodeBlock(int colorId,int block[64])
{
    int DCid=scanComs[colorId].DCid;
    int ACid=scanComs[colorId].ACid;
    int QTid=colorComs[colorId].QtId;
    node *DCnode=DCtree[DCid];
    for(int i=0;i<16;++i)
    {
        if(data.getFirstBit())
            DCnode=DCnode->right; 
        else    
            DCnode=DCnode->left;
        if(isLeaf(DCnode))
        {
            break;
        }
    }

    unsigned short code;
    int result;
    code=data.getBits(DCnode->symbol);
    result=codeToValue(code,DCnode->symbol);
    block[0]=result+DCpredict[colorId];
    DCpredict[colorId]=block[0];
    for(int i=1;i<64;++i)
    {
        node *tmpNode=ACtree[ACid];
        for(int j=0;j<16;++j)
        {
            if(data.getFirstBit())
                tmpNode=tmpNode->right; 
            else    
                tmpNode=tmpNode->left;
            if(isLeaf(tmpNode))
            {
               // printf("test %d,%d %X %d\n",tmpNode->codeword,tmpNode->symbol,data.currentData,data.currentBit);
                break;
            }
        }
        int zeroCount=(tmpNode->symbol)>>4;
        i+=zeroCount;
        if((((tmpNode->symbol)&0x0F)==0)&&(zeroCount==0))
        {
            break;
        }
        code=data.getBits((tmpNode->symbol)&0x0F);
        result=codeToValue(code,(tmpNode->symbol)&0x0F);
        block[i]=result;
    }
   // printf("\n");

   


    deQuantize(QTid,block);
    zingZang(block);
    IDCT(block);
    levelShift(block);
  
   
    return 0;
}



void decoder::zingZang(int *block)
{
    int tmp[64];
    for(int i=0;i<64;++i)
        tmp[i]=block[i];
    for (int i =0, n = 0; i < 8 * 2; i++)
		for (int j = (i < 8) ? 0 : i-8+1; j <= i && j < 8; j++)
			block[(i&1)? j*(8-1)+i : (i-j)*8+j ] = tmp[n++];
}

int decoder::codeToValue(unsigned short data,unsigned char bitNum)
{
    if(data&0x8000)
        return (int)(data>>(16-bitNum));
    data=(data^0xFFFF)>>(16-bitNum);
    int result=-1*data;
    return result;
}


void decoder::deQuantize(int QTid,int block[64])
{
    for(int i=0;i<64;++i)
        block[i]=block[i]*DQTs[QTid].getQT(i);
}

void decoder::IDCT(int block[64])
{
    int originBlock[64],N=8;
    for(int i=0;i<64;++i)
        originBlock[i]=block[i];
    double coefficient1,coefficient2;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            double tmp=0.0;
            for(int p=0;p<N;p++)
                for(int q=0;q<N;q++)
                {
                    if(p==0) coefficient1=0.353553391;
                    else coefficient1=0.5;
                    if(q==0) coefficient2=0.353553391;
                    else coefficient2=0.5;
                    tmp+=coefficient1*coefficient2*originBlock[p*N+q]*cosData[i][p]*cosData[j][q];
                }
            block[i*N+j]=(int)tmp;
        } 
    }

}

void decoder::levelShift(int block[64])
{
    for(int i=0;i<64;++i)
    {
        block[i]+=128;
        if(block[i]<0)
            block[i]=0;
        if(block[i]>255)
            block[i]=255;
    }
}

void decoder::decode(imgData &result)
{
    int MCUheight,MCUwidth;
    int maxV=0,maxH=0;
    for(int i=0;i<colorComs.size();++i)
    {
        if(colorComs[i].horizontalFactor>maxH)
            maxH=colorComs[i].horizontalFactor;
        if(colorComs[i].verticalFactor>maxV)
            maxV=colorComs[i].verticalFactor;
    }
    MCUheight=maxV*8;
    MCUwidth=maxH*8;
    int horizontalNum=width/MCUwidth+(width%MCUwidth?1:0);
    int verticalNum=height/MCUheight+(height%MCUheight?1:0);
    imgData image(height,width,colorComs.size());
    //imgData MCU(MCUheight,MCUwidth,colorComs.size());
    unsigned char tmp;
    int imageH,imageV;
    for(int i=0;i<verticalNum;++i)
    {
        for(int j=0;j<horizontalNum;++j)
        {   
            getMCU(image,i*MCUheight,j*MCUwidth,maxV,maxH);
        }
    }

    YCbCrtoRGB(image,result);
    return ;
}
imgData::imgData(int h,int w,int n)
{
    height=h;
    width=w;
    colorNum=n;
    data=vector<unsigned char*>(colorNum);
    for(int i=0;i<colorNum;++i)
    {
        data[i]=(unsigned char *)malloc(h*w*(sizeof(unsigned char)));
    }
}

imgData::~imgData()
{
    for(int i=0;i<colorNum;++i)
    {
        free(data[i]);
    }
}

unsigned char imgData::get(int h,int w,int n)
{
    return data[n][h*width+w];
}

void imgData::set(int h,int w,int n,unsigned char input)
{
   data[n][h*width+w]=input;
}

void initializeBlock(int block[64])
{
    for(int i=0;i<64;++i)
        block[i]=0;
}
void YCbCrtoRGB(imgData &input,imgData &result)
{
    double r,g,b,y,Cb,Cr;
    for(int i=0;i<input.height;++i)
        for(int j=0;j<input.width;++j)
        {
            y=(double)input.get(i,j,0);
            Cb=(double)input.get(i,j,1);
            Cr=(double)input.get(i,j,2);
            r=y+1.402*(Cr-128.0);
            g=y-0.34414*(Cb-128.0)-0.71414*(Cr-128.0);
            b=y+1.774*(Cb-128.0);
            r=r>255?255:r;
            g=g>255?255:g;
            b=b>255?255:b;
            r=r<0?0:r;
            g=g<0?0:g;
            b=b<0?0:b;

            result.set(i,j,0,(unsigned char)r);
            result.set(i,j,1,(unsigned char)g);
            result.set(i,j,2,(unsigned char)b);

        }
}

void decoder::initializeCos()
{
    int N=8;
    for(int i=0;i<8;++i)
    {
        for(int j=0;j<8;++j)
        {
            cosData[i][j]=cos((2*i+1)*PI*j/(2*N));
        }
    }
}


imgData * decodeJPEG(FILE *path)
{   
    headSegment head;
    head.headProcess(path);
    decoder imgDecoder(head,path);
    imgData *result=new  imgData(imgDecoder.height,imgDecoder.width,3);
    imgDecoder.decode(*result);
    return result;
}

imgData::imgData()
{

}

void decoder::getMCU(imgData &resultImg,int currentV,int currentH,int maxV,int maxH)
{
    int block[64]={0},blockH=0,blockV=0;
    int imgV,imgH;
    for(int colorId=0;colorId<colorComs.size();++colorId)
    {    
        for(int i=0;i<colorComs[colorId].verticalFactor;++i)
        {
            for(int j=0;j<colorComs[colorId].horizontalFactor;++j)
            {
                initializeBlock(block);
                decodeBlock(colorId,block);
                for(int h=0;h<maxV*8/colorComs[colorId].verticalFactor;++h)
                {
                    imgV=i*8+h+currentV;
                    if(imgV>=height)
                        break;
                    for(int k=0;k<maxH*8/colorComs[colorId].horizontalFactor;++k)
                    {
                        blockV=h/(maxV/(int)colorComs[colorId].verticalFactor);
                        blockH=k/(maxH/(int)colorComs[colorId].horizontalFactor);
                        imgH=j*8+k+currentH;
                        if(imgH>=width)
                            break;
                        resultImg.set(imgV,imgH,colorId,(unsigned char)block[blockV*8+blockH]);
                    }
                }
            }
        }
    }
}
