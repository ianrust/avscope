#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ao/ao.h>
#include <time.h>

#include "svgparser.h"
#include "wfassem.h"
#include "wftypes.h"

using namespace std;

const char resource_dir[] = "/usr/local/share/avscope/clock/";

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

        Point offset(-(2.0-position)/1.5,0);

        if (position == 4)
        {
           offset = Point(1.6,0);
        }

        if (position != 2)
        {
            for (int digit = 0; digit < 10; digit++)
            {
                Points * loop = new Points;
                WaveForm wf;

                char * file = new char[80];
                memset(file,0,80);
                char number = (char)(((int)'0') + digit);
                strcat(file,resource_dir);
                strncat(file,&number,1);
                strcat(file,".svg");

                svgToPoints(file,loop);
                loop->max_val = 2.0;
                translatePoints(loop,&offset);

                assembleWaveform(loop,&wf,&info);

                digits[position][digit] = wf;

                delete[] file;
            }
        }
        else
        {
            Points * loop = new Points;
            WaveForm wf;

            char * file = new char[80];
            memset(file,0,80);
            strcat(file,resource_dir);
            strcat(file,"colon");
            strcat(file,".svg");

            svgToPoints(file,loop);
            loop->max_val = 2.0;
            translatePoints(loop,&offset);

            assembleWaveform(loop,&wf,&info);

            digits[position][0] = wf;

            delete[] file;
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

    time_t rawtime;
    struct tm * timeinfo;

    int hour,first_digit_hour,second_digit_hour,minute,first_digit_minute,second_digit_minute;

    while(1)
    {
        //get current time
        time (&rawtime);
        timeinfo = localtime (&rawtime);

        hour = (*timeinfo).tm_hour % 12;
        if (hour == 0) hour = 12;
        second_digit_hour = hour % 10;
        first_digit_hour = (hour - second_digit_hour)/10;
        minute = (*timeinfo).tm_min;
        second_digit_minute = minute % 10;
        first_digit_minute = (minute - second_digit_minute)/10;

        ao_play(device,digits[0][first_digit_hour].data,digits[0][first_digit_hour].length);
        ao_play(device,digits[1][second_digit_hour].data,digits[1][second_digit_hour].length);
        ao_play(device,digits[2][0].data,digits[2][0].length);
        ao_play(device,digits[3][first_digit_minute].data,digits[3][first_digit_minute].length);
        ao_play(device,digits[4][second_digit_minute].data,digits[4][second_digit_minute].length);
    }

    delete timeinfo;

    ao_close(device);

    ao_shutdown();

    return 0;
}
