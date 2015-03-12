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

    SampleInfo info;
    info.rate = 44100;
    info.frequency = 500;
    info.bytes_per_sample = 2;

    digits = new WaveForm*[5];
    digits[0] = new WaveForm[10];
    digits[1] = new WaveForm[10];
    digits[2] = new WaveForm[1];
    digits[3] = new WaveForm[10];
    digits[4] = new WaveForm[10];

    for (int position = 0; position < 5; position++)
    {

        Point offset((2.5-position)/5.0,0);

        if (position != 2)
        {
            for (int digit = 0; digit < 10; digit++)
            {
                Points * loop = new Points;
                WaveForm wf;

                char * file = new char[80];
                char number = (char)(((int)'0') + digit);
                strcat(file,resource_dir);
                strncat(file,&number,1);
                strcat(file,".svg");

                svgToPoints(file,loop);

                translatePoints(loop,&offset);

                assembleWaveform(loop,&wf,&info);

                digits[position][digit] = wf;

                delete[] file;
            }
        }
    }

    ao_device * device;
    ao_sample_format format;
    int default_driver;

    ao_initialize();

    default_driver = ao_default_driver_id();

    memset(&format, 0, sizeof(format));
    format.bits = info.bytes_per_sample * 8;
    format.channels = 2;
    format.rate = info.rate;
    format.byte_format = AO_FMT_LITTLE;

    device = ao_open_live(default_driver,&format,NULL);
    
    int cycles = 10000;

    for (int cycle = 0; cycle < cycles; cycle++)
    {
        ao_play(device,digits[0][1].data,digits[0][1].length);
        ao_play(device,digits[1][2].data,digits[1][2].length);
        ao_play(device,digits[2][0].data,digits[2][0].length);
        ao_play(device,digits[3][3].data,digits[3][3].length);
        ao_play(device,digits[4][4].data,digits[4][4].length);
    }

    ao_close(device);

    ao_shutdown();

    return 0;
}