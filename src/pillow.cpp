#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ao/ao.h>

#include "svgparser.h"
#include "wfassem.h"
#include "wftypes.h"

using namespace std;

const unsigned int num_frames = 11;
char* filenames[num_frames] = {"./pillows/pillow1.svg",
                                "./pillows/pillow2.svg",
                                "./pillows/pillow3.svg",
                                "./pillows/pillow4.svg",
                                "./pillows/pillow5.svg",
                                "./pillows/pillow6.svg",
                                "./pillows/pillow7.svg",
                                "./pillows/pillow8.svg",
                                "./pillows/pillow9.svg",
                                "./pillows/pillow91.svg",
                                "./pillows/pillow92.svg"};
WaveForm wfs[num_frames];
Points loops[num_frames];


int main(int argc, char ** argv)
{
    SampleInfo info;
    info.rate = 44100;
    info.frequency = 500;
    info.bytes_per_sample = 2;
    float scale = 2.5;
    int cycles = 50;

    for (int i = 0; i < num_frames; i++)
    {
        Points loop;
        WaveForm wf;

        svgToPoints(filenames[i],&loop);
        loop.max_val /= scale;

        assembleWaveform(&loop,&wf,&info);

        wfs[i] = wf;
    }

    //send to speakers

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
    
    int frame = 0;
    int direction = 1;
    while (1)
    {
        for (int cycle = 0; cycle < cycles; cycle++)
        {
            ao_play(device,wfs[frame].data,wfs[frame].length);
        }

        frame += direction;
        if (frame == num_frames)
        {
            direction = -1;
            frame = num_frames - 1;
        }
        else if (frame == -1)
        {
            direction = 1;
            frame = 0;
        }
    }

    ao_close(device);

    ao_shutdown();

    return 0;
}
