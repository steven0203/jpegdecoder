#ifndef _HEAD_PROCESS_H
#define _HEAD_PROCESS_H
#include"segment.h"
#include<vector>
#include<cstdio>
using namespace std;





unsigned short reverseByte(unsigned short);
void segProcess(FILE *);



class headSegment
{
    public:
        vector<APPdata>APP;
        vector<DQTdata>DQTs;
        SOF0data SOF0;
        vector<DHTdata>DHTs;
        SOSdata SOS;
        int headProcess(FILE *);
    private:
        
        
};




#endif