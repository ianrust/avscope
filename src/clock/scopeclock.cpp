#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ao/ao.h>

#include "svgparser.h"
#include "wfassem.h"
#include "wftypes.h"

using namespace std;

const char resource_dir[] = "resources/clock/";

int main(int argc,char ** argv)
{
    WaveForm ** digits;

    digits = new WaveForm*[5];
    digits[0] = new WaveForm[10];
    digits[1] = new WaveForm[10];
    digits[2] = new WaveForm[1];
    digits[3] = new WaveForm[10];
    digits[4] = new WaveForm[10];

    for (int digit = 0; digit < 10; digit++)
    {
        Points * loop = new Points;
        WaveForm wf;

        char * file = new char[80];
        char number = (char)(((int)'0') + digit);
        strcat(file,resource_dir);
        strncat(file,&number,1);
        strcat(file,".svg");

        std::cout << file << std::endl;

        svgToPoints(file,loop);

        // assembleWaveform(&loop,&wf,&info);

        delete[] file;
    }
}