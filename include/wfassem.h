#ifndef WF_ASSEM
#define WF_ASSEM

#include <math.h>
#include <iostream>

#include "wftypes.h"

using namespace std;

static float segmentLength(Point p1,Point p2)
{
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrt(dx*dx + dy*dy);
}

static float totalLength(const Points * const loop,bool closed)
{
    float total_length = 0;

    for (int i = 0; i < loop->length-1; i++)
    {
        total_length += segmentLength(loop->locations[i],loop->locations[i+1]);
    }

    if (closed)
    {
        total_length += segmentLength(loop->locations[loop->length-1],loop->locations[0]);
    }

    return total_length;
}

static Point interpolate(Point p1,Point p2,float dist)
{
    //interpolate dist from p1 to p2

    //unit vector from p1 to p2
    float v_x = (p2.x - p1.x)/segmentLength(p1,p2);
    float v_y = (p2.y - p1.y)/segmentLength(p1,p2);

    Point p_interp;

    p_interp.x = p1.x + dist * v_x;
    p_interp.y = p1.y + dist * v_y;

    return p_interp;
}

static void pointsToBytes(const Points * const samples,WaveForm * const wf,const SampleInfo * const info)
{
    wf->length = 2 * info->bytes_per_sample * samples->length;

    wf->data = new char[wf->length];

    int max_val = pow(2,8*info->bytes_per_sample-2);

    for (int i = 0; i < samples->length; i++)
    {
        int x_val = max_val * samples->locations[i].x / samples->max_val;
        int y_val = max_val * samples->locations[i].y / samples->max_val;

        for (int c = 0; c < info->bytes_per_sample; c++)
        {
            wf->data[2*info->bytes_per_sample*i + c] = (x_val >> 8*c) & 0xFF;
            wf->data[2*info->bytes_per_sample*i + info->bytes_per_sample + c] = (y_val >> 8*c) & 0xFF;
        }
    }
}

void assembleWaveform(const Points * const loop,WaveForm * const wf,const SampleInfo * const info)
{
    int ub_length = info->rate / info->frequency; // a priori length for samples, will be updated with something slightly smaller later

    float total_edge_length = totalLength(loop,loop->closed);

    float sample_length = total_edge_length/float(ub_length);
    float leftover_length = sample_length;

    int act_samples_encoded = 0;

    Points sampled_points;
    sampled_points.locations = new Point[ub_length+1];
    sampled_points.max_val = loop->max_val;
    sampled_points.length = 0;

    int end_index = loop->closed ? loop->length : loop->length-1;

    for (int i = 0; i < end_index; i++)
    {
        Point p1,p2;

        if (i != loop->length-1)
        {
            p1 = loop->locations[i];
            p2 = loop->locations[i+1];
        }
        else
        {
            p1 = loop->locations[i];
            p2 = loop->locations[0];
        }

        float this_segment_length = segmentLength(p1,p2);
        float buffer_from_last = sample_length - leftover_length;

        float final_dist = 0;
        for (float s = buffer_from_last; s < this_segment_length; s+=sample_length)
        {
            final_dist = s;

            Point sample_point = interpolate(p1,p2,s);
            sampled_points.locations[sampled_points.length] = sample_point;
            sampled_points.length++;
        }

        leftover_length = this_segment_length - final_dist;
    }

    sampled_points.length--;

    pointsToBytes(&sampled_points,wf,info);
}

#endif