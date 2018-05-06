#include"dataStream.h"
#include<cstdio>
#include<cstdlib>



dataStream::dataStream(FILE *path)
{
    this->path=path;
    fread(&(this->currentData),1,1,path);
    currentBit=0;
    end=false;
}

unsigned short dataStream::getFirstBit()
{
    unsigned short result;
    result=(currentData>>(7-currentBit))&1;
    currentBit+=1;
    if(currentBit==8)
    {
        currentBit=0;
        fread(&(this->currentData),1,1,path);
        checkBlock();
    }
    return result;
}

unsigned short dataStream::getBits(unsigned char num)
{
    unsigned short result=0,bit=0;
    for(int i=0;i<num;++i)
    {  
        bit=(unsigned short)getFirstBit();
        result=result^(bit<<15-i);
    }
    return result;
}


unsigned short dataStream::checkBlock()
{
    unsigned char tmp;
    if(currentData!=0xFF)
        return 0;
    fread(&tmp,1,1,path);
    
    if(tmp==0xD9)
    {
        end=true;
        return EOI_MARKING;
    }
    if(tmp==0x00)
    {
        return 0;
    }
    currentData=tmp;
    
}

bool dataStream::isEnd()
{
    return end;
}
