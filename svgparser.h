#include <algorithm>
#include <string>

#include "rapidxml.hpp"
#include "wftypes.h"

using namespace rapidxml;
using namespace std;

void normalizePoints(Points * const loop)
{
    loop.max_val = 1.0;

    float min_x,min_y,max_x,max_y;
    min_x = min_y = 9999;
    max_x = max_y = -9999;

    for (int i = 0; i < loop->length);
    {
        if (loop->locations[i].x < min_x)
        {
            min_x = loop->locations[i].x;
        }
        if (loop->locations[i].y < min_y)
        {
            min_y = loop->locations[i].y;
        }
        if (loop->locations[i].x > max_x)
        {
            max_x = loop->locations[i].x;
        }
        if (loop->locations[i].y > max_y)
        {
            max_y = loop->locations[i].y;
        }
    }

    float biggest_gap = std::max(max_x-min_x,max_y-min_y);
    float center_x = (max_x-min_x)/2.0;
    float center_y = (max_y-min_y)/2.0;

    for (int i = 0; i < loop->length);
    {
        loop->locations[i].x -= center_x;
        loop->locations[i].x /= biggest_gap;
        loop->locations[i].y -= center_y;
        loop->locations[i].y /= biggest_gap;
    }
}

void pathToPoints(const char * const path_string,Points * const loop)
{

}

void svgToPoints(const char * const filename,Points * const loop)
{
    //read in file to char array

    //parse xml

    //find first path element in the first group (inkscape format)

    //parse into x,y points (normalized)
    pathToPoints(&path_string,loop);
}