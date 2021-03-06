#include "wfassem.h"
#include <string.h>
#include <ao/ao.h>

void createCircle(float center_x,float center_y,float r,float start_angle,Points * points)
{
    float dtheta = 2.0*M_PI/float(points->length);

    points->locations = new Point[points->length];

    int index = 0;
    for (float angle = start_angle; angle < 2.0*M_PI+start_angle; angle += dtheta)
    {
        Point radial_point;
        radial_point.x = r * cos(angle) + center_x;
        radial_point.y = r * sin(angle) + center_y;

        points->locations[index] = radial_point;

        index++;
    }

    points->closed = true;
}

int main (int argc, char ** argv)
{
    SampleInfo info;
    info.rate = 44100;
    info.frequency = 500;
    info.bytes_per_sample = 2;

    Points vert_line;
    vert_line.length = 2;
    vert_line.max_val = 1.3;
    vert_line.locations = new Point[2];
    vert_line.locations[0] = Point(0.0,-1.0);
    vert_line.locations[1] = Point(0.0,1.0*sqrt(3)-1.0);
    vert_line.closed = true;
    WaveForm wf_vert_line;

    assembleWaveform(&vert_line,&wf_vert_line,&info);

    Points small_triangle;
    small_triangle.length = 3;
    small_triangle.max_val = 1.3;
    small_triangle.locations = new Point[3];
    small_triangle.locations[0] = Point(0.0,-1.0);
    small_triangle.locations[1] = Point(-0.5,sqrt(3)/2.0-1.0);
    small_triangle.locations[2] = Point(0.5,sqrt(3)/2.0-1.0);
    small_triangle.closed = true;
    WaveForm wf_small_triangle;

    assembleWaveform(&small_triangle,&wf_small_triangle,&info);

    Points triangle;
    triangle.length = 4;
    triangle.max_val = 1.3;
    triangle.locations = new Point[4];
    triangle.locations[0] = Point(0.0,-1.0);
    triangle.locations[1] = Point(-1.0,-1.0);
    triangle.locations[2] = Point(0.0,sqrt(3)-1.0);
    triangle.locations[3] = Point(1.0,-1.0);
    triangle.closed = true;
    WaveForm wf_triangle;

    assembleWaveform(&triangle,&wf_triangle,&info);

    Points left_circle;
    left_circle.length = 100;
    left_circle.max_val = 1.3;
    createCircle(-0.5,sqrt(3)/2.0-1.0,1.0,-2.0*M_PI/6.0,&left_circle);
    WaveForm wf_left_circle;
    assembleWaveform(&left_circle,&wf_left_circle,&info);

    Points right_circle;
    right_circle.length = 100;
    right_circle.max_val = 1.3;
    createCircle(0.5,sqrt(3)/2.0-1.0,1.0,-4.0*M_PI/6.0,&right_circle);
    WaveForm wf_right_circle;
    assembleWaveform(&right_circle,&wf_right_circle,&info);

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
        ao_play(device,wf_vert_line.data,wf_vert_line.length);
        ao_play(device,wf_triangle.data,wf_triangle.length);
        ao_play(device,wf_small_triangle.data,wf_small_triangle.length);
        ao_play(device,wf_left_circle.data,wf_left_circle.length);
        ao_play(device,wf_right_circle.data,wf_right_circle.length);
    }

    ao_close(device);

    ao_shutdown();

    return 0;
}
