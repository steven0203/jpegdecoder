#include<cstdio>
#include<cstdlib>
#include<vector>
#include<queue>
#include"decoder.h"
#include"bitmap_image.hpp"
using namespace std;


int main(int argc,char**argv)
{
    FILE * inputPath;
    if(argc>=2)
    {
        inputPath=fopen(argv[1],"rb");
        if (inputPath==NULL)
        {
            fputs ("Input file error",stderr); 
            exit (1);
        }
    }
    else
    {
        printf("Too few arguments\n");
        return 0;
    }
    imgData *result;
    result=decodeJPEG(inputPath);
    unsigned char r,g,b;
    bitmap_image resultImg(result->width,result->height);
    for(int i=0;i<result->height;++i)
    {
        for(int j=0;j<result->width;++j)
        {
            r=result->get(i,j,0);
            g=result->get(i,j,1);
            b=result->get(i,j,2);
            resultImg.set_pixel(j,i,r,g,b);
        }
    }

    if(argc>=3)
    {
        resultImg.save_image(argv[2]);
    }
    resultImg.save_image("output.bmp");

    delete result;
   
    return 0;

}

