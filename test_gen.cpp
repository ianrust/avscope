#include "wfassem.h"
#include <string.h>
#include <ao/ao.h>

int main (int argc, char ** argv)
{
    SampleInfo info;
    info.rate = 44100;
    info.frequency = 100;
    info.bytes_per_sample = 2;

    Points square;
    square.length = 4;
    square.max_val = 1.0;
    square.locations = new Point[4];
    square.locations[0] = Point(-1.0,-1.0);
    square.locations[1] = Point(1.0,-1.0);
    square.locations[2] = Point(1.0,1.0);
    square.locations[3] = Point(-1.0,1.0);

    WaveForm wf;

    assembleClosedWaveform(&square,&wf,&info);

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

    for (int i = 0; i<100; i++)
    {
        ao_play(device,wf.data,wf.length);
    }

    ao_close(device);

    ao_shutdown();

    return 0;
}