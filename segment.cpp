#include"segment.h"
#include<vector>
using namespace std;

DQTdata::DQTdata()
{

}
DQTdata::DQTdata(unsigned short length,unsigned char QTinfo)
{
    this->length=length;
    this->number=QTinfo;
    this->precision=QTinfo>>4;
}
void DQTdata::setQT(unsigned char *data)
{
    this->Qt=data;
}
unsigned short DQTdata::getQT(int index )
{
    unsigned short result=Qt[index*(1+precision)];
    if(precision==0)
        return result;
    else
    {
        result=(result<<8)+Qt[index*(1+precision)+1];
    }
}


SOF0data::SOF0data()
{

}

SOF0data::SOF0data(unsigned short length,unsigned char precision,unsigned short height,unsigned short width,unsigned char colorComNum)
{
    this->length=length;
    this->precision=precision;
    this->height=height;
    this->width=width;
    this->colorComNum=colorComNum;
    this->colorComs=vector<colorCom>(colorComNum);
}

void SOF0data::setColorComs(unsigned char *data)
{   
    int id;
    for(int i=0;i<colorComNum;++i)
    {   
        id=data[i*3]-1;
        colorComs[id].id=data[i*3];
        colorComs[id].verticalFactor=data[i*3+1]&0x0F;
        colorComs[id].horizontalFactor=data[i*3+1]>>4;
        colorComs[id].QtId=data[i*3+2];
    }
}


DHTdata::DHTdata()
{

}
DHTdata::DHTdata(unsigned short length, unsigned char info)
{
    this->length=length;
    this->number=info&0x0F;
    this->type=info>>4;
}