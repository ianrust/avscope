#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ao/ao.h>

#include "svgparser.h"
#include "wfassem.h"
#include "wftypes.h"

using namespace std;

void printUsage()
{
    cerr << "Usage is:" << endl;
    cerr << "svg2scope filename.svg scale num_repeats frequency(optional) sample_rate(optional) bytes_per_sample(optional):" << endl;
}

int main(int argc,char ** argv)
{
    SampleInfo info;
    if (argc < 4)
    {
        cerr << "Too few inputs" << endl;
        printUsage();
        return 1;
    }
    else if (argc == 4)
    {
        info.rate = 44100;
        info.frequency = 500;
        info.bytes_per_sample = 2;
    }
    else if (argc != 7)
    {
        cerr << "Incorrect number of inputs" << endl;
        printUsage();
        return 1;        
    }
    else if (argc == 7)
    {
        info.rate = atoi(argv[5]);
        info.frequency = atoi(argv[4]);
        info.bytes_per_sample = atoi(argv[6]);
    }

    float scale = atof(argv[2]);
    int cycles = atoi(argv[3]);

    Points loop;
    WaveForm wf;

    svgToPoints(argv[1],&loop);
    loop.max_val /= scale;

    assembleWaveform(&loop,&wf,&info);


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
    
    for (int cycle = 0; cycle < cycles; cycle++)
    {
        ao_play(device,wf.data,wf.length);
    }

    ao_close(device);

    ao_shutdown();

    return 0;
}