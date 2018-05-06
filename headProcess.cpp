#include<vector>
#include<cstdlib>
#include"headProcess.h"
using namespace std;


unsigned short reverseByte(unsigned short data)
{
    return (data<<8)|(data>>8);
}

int headSegment:: headProcess(FILE *img)
{
    unsigned short data=0;
    fread(&data,2,1,img);
    if(data!=SOI)
        return 1;
    
    //APP0 APPn
    for(fread(&data,2,1,img);(data&0xF0FF)==APP0;fread(&data,2,1,img))
    {   
        APPprocess(data,img);
    }

    //DQT
    if(data!=DQT_MARKER)
        return 1;
    
    unsigned short length=0;
    unsigned char *seqData=NULL;
    for(;data==DQT_MARKER;fread(&data,2,1,img))
    {
        DQTprocess(img);
    }

    //SOF0
    unsigned char precision;
    unsigned short height;
    unsigned short width;
    unsigned char colorComNum;
    if(data!=SOF0_MARKER)
        return 1;
    SOF0process(img);


    //DHT
    unsigned short info;
    DHTdata DHT;
    fread(&data,2,1,img);
    if(data!=DHT_MARKER)
        return 1;
    for( ;data==DHT_MARKER; fread(&data,2,1,img))
    {  
        fread(&length,2,1,img);
        fread(&info,1,1,img);
        length=reverseByte(length);
        DHT=DHTdata(length,info);
        seqData=(unsigned char *)malloc(16);
        fread(seqData,1,16,img);
        DHT.symbolsNum=seqData;
        seqData=(unsigned char *)malloc(length-19);
        fread(seqData,1,length-19,img);
        DHT.symbols=seqData;
        DHTs.push_back(DHT);
    }

    //SOS
    unsigned char number;
    unsigned char comId;
    unsigned char tableId;
    if(data!=SOS_MARKER)
        return 1;
    fread(&length,2,1,img);
    fread(&number,1,1,img);

    length=reverseByte(length);
    SOS.length=length;
    SOS.comNum=number;
    scanCom component;
    for(int i=0;i<number;++i)
    {
        fread(&comId,1,1,img);
        fread(&tableId,1,1,img);
        component.id=comId;
        component.ACid=tableId>>4;
        component.DCid=tableId&0x0F;
        SOS.components.push_back(component);
    }

    fseek(img,3,SEEK_CUR);
    return 0;
}

unsigned char headSegment::APPprocess(unsigned short marker,FILE *img)
{
    unsigned short data;
    APPdata APPn;  
    APPn.n=(marker>>8)&0x000F;
    fread(&data,2,1,img);
    APPn.Lp=(data>>8);
    APP.push_back(APPn);
    fseek(img,APPn.Lp-2,SEEK_CUR);
}

unsigned char headSegment::DQTprocess(FILE *img)
{
    unsigned short length=0;
    unsigned char Qtinfo=0;
    DQTdata DQT;
    unsigned char *seqData=NULL;
    fread(&length,2,1,img);
    length=reverseByte(length);
    length-=2;
    while(length>0)
    {
        fread(&Qtinfo,1,1,img);
        length-=1;
        DQT=DQTdata(length,Qtinfo);    
        seqData=(unsigned char *)malloc(64*(DQT.precision+1));
        fread(seqData,1,64*(DQT.precision+1),img);
        length-=64*(DQT.precision+1);
        DQT.setQT(seqData);
        DQTs.push_back(DQT);
    }
}

unsigned char headSegment::SOF0process(FILE *img)
{
    unsigned char precision;
    unsigned short length=0;
    unsigned short height;
    unsigned short width;
    unsigned char colorComNum;
    unsigned char *seqData;
    fread(&length,2,1,img);
    fread(&precision,1,1,img);
    fread(&height,2,1,img);
    fread(&width,2,1,img);
    fread(&colorComNum,1,1,img);
    length=reverseByte(length);
    height=reverseByte(height);
    width=reverseByte(width);
    SOF0=SOF0data(length,precision,height,width,colorComNum);
    seqData=(unsigned char *)malloc(colorComNum*3);
    fread(seqData,1,colorComNum*3,img);
    SOF0.setColorComs(seqData);
    free(seqData);
}
