#include "wfassem.h"
#include <string.h>
#include <ao/ao.h>

int main (int argc, char ** argv)
{
    SampleInfo info;
    info.rate = 44100;
    info.frequency = 500;
    info.bytes_per_sample = 2;

    Points star;
    star.length = 10;
    star.max_val = 1.0;
    star.locations = new Point[10];
    star.locations[0] = Point(-0.6,-1.0);
    star.locations[1] = Point(0.0,-0.3);
    star.locations[2] = Point(0.6,-1.0);
    star.locations[3] = Point(0.35,-0.2);
    star.locations[4] = Point(1.0,0.2);
    star.locations[5] = Point(0.3,0.3);
    star.locations[6] = Point(0.0,1.0);
    star.locations[7] = Point(-0.3,0.3);
    star.locations[8] = Point(-1.0,0.2);
    star.locations[9] = Point(-0.35,-0.2);

    WaveForm wf;

    assembleClosedWaveform(&star,&wf,&info);

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
        ao_play(device,wf.data,wf.length);
    }

    ao_close(device);

    ao_shutdown();

    return 0;
}
