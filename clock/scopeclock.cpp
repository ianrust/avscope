#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ao/ao.h>

#include "svgparser.h"
#include "wfassem.h"
#include "wftypes.h"

int main(int argc,char ** argv)
{
    WaveForm ** digits;

    digits = new Waveform*[5];
    digits[0] = new WaveForm[10];
    digits[1] = new WaveForm[10];
    digits[2] = new WaveForm[1];
    digits[3] = new WaveForm[10];
    digits[4] = new WaveForm[10];

    for (int digit = 0; digit < 10; digit++)
    {
        Points loop;
        WaveForm wf;

        char number = (char)(((int)'0') + 1);

        cout << number << endl;

        // svgToPoints("resources/",&loop);

        // assembleWaveform(&loop,&wf,&info);
    }
}