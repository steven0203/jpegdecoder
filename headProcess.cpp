#include<vector>
#include<cstdlib>
#include"headProcess.h"
#include"segment.h"

using namespace std;


unsigned short reverseByte(unsigned short data)
{
    return (data<<8)|(data>>8);
}

int headSegment:: headProcess(FILE *img)
{
    unsigned short marker=0;
    fread(&marker,2,1,img);
    if(marker!=SOI)
        return 1;
    int finish=0;
    while(finish==0)
    {
        fread(&marker,2,1,img);
        switch(marker)
        {
            case(APP0):
            case(APP1):
            case(APP2):
            case(APP3):
            case(APP4):
            case(APP5):
            case(APP6):
            case(APP7):
            case(APP8):
            case(APP9):
            case(APP10):
            case(APP11):
            case(APP12):
            case(APP13):
            case(APP14):
            case(APP15):
                APPprocess(marker,img);
                break;
            case(DQT_MARKER):
                DQTprocess(img);
                break;
            case(SOF0_MARKER):
                SOF0process(img);
                break;
            case(DHT_MARKER):
                DHTprocess(img);
                break;
            case(SOS_MARKER):
                SOSprocess(img);
                finish=1;
                break;
            case(COM_MARKER):
                COMprocess(img);  
                break;    
            default:
                return 1;
        }
    }

    

    return 0;
}

unsigned char headSegment::APPprocess(unsigned short marker,FILE *img)
{
    unsigned short data;
    APPdata APPn;  
    APPn.n=(marker>>8)&0x000F;
    fread(&data,2,1,img);
    APPn.Lp=reverseByte(data);
    APP.push_back(APPn);
    fseek(img,APPn.Lp-2,SEEK_CUR);
    return 0;
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
    return 0;
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
    return 0;
}

unsigned char headSegment::DHTprocess(FILE *img)
{
    unsigned short length;
    unsigned char info;
    DHTdata DHT;
    unsigned char *seqData;
    fread(&length,2,1,img);
    length=reverseByte(length);
    length-=2;
    int number=0;
    while(length>0)
    {
        number=0;
        fread(&info,1,1,img);
        length-=1;
        DHT=DHTdata(length,info);
        seqData=(unsigned char *)malloc(16);
        fread(seqData,1,16,img);
        length-=16;
        for(int i=0;i<16;++i)
        {
            number+=seqData[i];
        }
        DHT.length=number+16;
        DHT.symbolsNum=seqData;
        seqData=(unsigned char *)malloc(number);
        fread(seqData,1,number,img);
        length-=number;
        DHT.symbols=seqData;
        DHTs.push_back(DHT);
    }
    return 0;
}

unsigned char headSegment::SOSprocess(FILE* img)
{
    unsigned short length;
    unsigned char number;
    unsigned char comId;
    unsigned char tableId;
 
    fread(&length,2,1,img);
    fread(&number,1,1,img);

    length=reverseByte(length);
    SOS.length=length;
    SOS.comNum=number;
    scanCom component;
    SOS.components=vector<scanCom>(number);
    for(int i=0;i<number;++i)
    {
        fread(&comId,1,1,img);
        fread(&tableId,1,1,img);
        component.id=comId;
        component.ACid=tableId>>4;
        component.DCid=tableId&0x0F;
        SOS.components[comId-1]=component;
    }
    fseek(img,3,SEEK_CUR);
    return 0;
}

unsigned char headSegment::COMprocess(FILE*img)
{
    unsigned short length;
    fread(&length,2,1,img);
    length=reverseByte(length);
    fseek(img,length-2,SEEK_CUR);
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


