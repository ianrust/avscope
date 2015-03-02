#include "wfassem.h"
#include <string.h>
#include <ao/ao.h>

int main (int argc, char ** argv)
{
    SampleInfo info;
    info.rate = 44100;
    info.frequency = 500;
    info.bytes_per_sample = 2;

    Points square1;
    square1.length = 4;
    square1.max_val = 1.0;
    square1.locations = new Point[4];
    square1.locations[0] = Point(-1.0,-1.0);
    square1.locations[1] = Point(-1.0,-0.5);
    square1.locations[2] = Point(-0.5,-0.5);
    square1.locations[3] = Point(-0.5,-1.0);
    WaveForm wf1;

    assembleWaveform(&square1,&wf1,&info,true);

    Points square2;
    square2.length = 4;
    square2.max_val = 1.0;
    square2.locations = new Point[4];
    square2.locations[0] = Point(1.0,1.0);
    square2.locations[1] = Point(1.0,0.5);
    square2.locations[2] = Point(0.5,0.5);
    square2.locations[3] = Point(0.5,1.0);
    WaveForm wf2;

    assembleWaveform(&square2,&wf2,&info,true);

    //send to speakers

    ao_device * device;
    ao_sample_format format;
    int default_driver;

    ao_initialize();

    default_driver = ao_default_driver_id();

    memset(&format, 0, sizeof(format));
    format.bits = 16;
    format.channels = 2;
    format.rate = 44100;
    format.byte_format = AO_FMT_LITTLE;

    device = ao_open_live(default_driver,&format,NULL);

    for (int i = 0; i<10000; i++)
    {
        ao_play(device,wf1.data,wf1.length);
        ao_play(device,wf2.data,wf2.length);
    }

    ao_close(device);

    ao_shutdown();

    return 0;
}
