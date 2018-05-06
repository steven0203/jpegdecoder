#ifndef DECODER_H
#define DECODER_H
#include"segment.h"
#include<vector>
#include"huffman.h"
#include"dataStream.h"
#include"headProcess.h"
#include<cstdio>
#include<cstdlib>
using namespace std;

class imgData
{
    public:
        int height;
        int width;
        int colorNum;
        imgData(int,int,int);
        ~imgData();
        unsigned char get(int,int,int);
        void set(int,int,int,unsigned char);
    private:
        vector<unsigned char*> data;
};


class decoder
{
    public:
        decoder(headSegment&,FILE *);
        int decodeBlock(int, int []);
        int height;
        int width;
        void decode(imgData &);
        vector<colorCom>colorComs;
        vector<node *>DCtree;
        vector<node *>ACtree;
    private:
        vector<int>DCpredict;
        vector<DQTdata>DQTs;

        vector<scanCom>scanComs;
        dataStream data;
        void zingZang(int *);
        int codeToValue(unsigned short,unsigned char);
        void deQuantize(int ,int []);
        void IDCT(int []);
        void levelShift(int []);
        void getMCU(imgData &);
};

void YCbCrtoRGB(imgData &,imgData &);
void initializeBlock(int []);






#endif