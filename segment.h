#ifndef SEGMENT_H
#define SEGMENT_H


#include<vector>
using namespace std;

static const unsigned short SOI =0xD8FF;
static const unsigned short APP0 =0xE0FF;
static const unsigned short APP1=0xE1FF;
static const unsigned short APP2=0xE2FF;
static const unsigned short APP3=0xE3FF;
static const unsigned short APP4=0xE4FF;
static const unsigned short APP5=0xE5FF;
static const unsigned short APP6=0xE6FF;
static const unsigned short APP7=0xE7FF;
static const unsigned short APP8=0xE8FF;
static const unsigned short APP9=0xE9FF;
static const unsigned short APP10=0xEAFF;
static const unsigned short APP11=0xEBFF;
static const unsigned short APP12=0xECFF;
static const unsigned short APP13=0xEDFF;
static const unsigned short APP14=0xEEFF;
static const unsigned short APP15=0xEFFF;



static const unsigned short DQT_MARKER = 0xDBFF;
static const unsigned short SOF0_MARKER= 0xC0FF;
static const unsigned short DHT_MARKER=0xC4FF;
static const unsigned short SOS_MARKER=0xDAFF;
static const unsigned short COM_MARKER=0xFEFF;
#define AC_TABLE 1
#define DC_TABLE 0



typedef struct
{
    unsigned short n;
    unsigned short Lp;
}APPdata;

class DQTdata
{
    public:
        DQTdata();
        DQTdata(unsigned short,unsigned char);
        void setQT(unsigned char *);
        unsigned short getQT(int);
        unsigned short length;
        unsigned char number;
        unsigned char precision;
    private:
        unsigned char *Qt;
};





typedef struct
{
    unsigned char id;
    unsigned char horizontalFactor;
    unsigned char verticalFactor;
    unsigned char QtId;
}colorCom;

class SOF0data
{
    public:
        unsigned short length;
        unsigned char precision;
        unsigned short height;
        unsigned short width;
        unsigned char colorComNum;
        vector<colorCom>colorComs;
        void setColorComs(unsigned char *);
        SOF0data();
        SOF0data(unsigned short,unsigned char,unsigned short,unsigned short,unsigned char);
};

class DHTdata
{
    public:
        unsigned short length;
        unsigned short number;
        unsigned short type; // 0 is DC Table , 1 is AC Table
        unsigned char *symbolsNum;
        unsigned char *symbols;  
        DHTdata();
        DHTdata(unsigned short, unsigned char);
};

typedef struct
{
    unsigned short id;
    unsigned char ACid;
    unsigned char DCid;
}scanCom;

class SOSdata
{
    public:
        unsigned short length;
        unsigned char comNum;
        vector<scanCom> components;
};

#endif





