#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ao/ao.h>

#include "svgparser.h"

using namespace std;

void printUsage()
{
    cerr << "Usage is:" < endl;
    cerr << "svg2scope filename.svg num_repeats frequency(optional) sample_rate(optional) bytes_per_sample(optional):" < endl;
}

int main(int argc,char ** argv)
{
    SampleInfo info;
    if (argc < 3)
    {
        cerr << "Too few inputs" << endl;
        printUsage();
        return 1;
    }
    else if (argc == 3)
    {
        info.rate = 44100;
        info.frequency = 500;
        info.bytes_per_sample = 2;
    }
    else if (argc != 6)
    {
        cerr << "Incorrect number of inputs" << endl;
        printUsage();
        return 1;        
    }
    else if (argc == 6)
    {
        info.rate = atoi(argc[4]);
        info.frequency = atoi(argc[3]);
        info.bytes_per_sample = atoi(argc[5]);
    }

    int cycles = atoi(argv[2]);

    Points loop;
    Waveform wf;

    svgToPoints(argv[1],&loop);

    assembleWaveform(&loop,&wf,&info,true);


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