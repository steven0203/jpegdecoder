#ifndef DATA_STREAM_H
#define DATA_STREAM_H
#include<cstdio>
static const unsigned short EOI_MARKING=0xFFD9; 



class dataStream
{
    public:
        dataStream(FILE *);
        unsigned short getFirstBit();
        unsigned short getBits(unsigned char);
        unsigned short checkBlock();
        bool isEnd();
        unsigned char currentData;
        unsigned char currentBit;

    private:
        FILE *path;
        bool end;
};






#endif