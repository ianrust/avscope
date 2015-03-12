#ifndef SVG_PARSER
#define SVG_PARSER

#include <algorithm>
#include <string.h>
#include <fstream>
#include <iostream>

#include "rapidxml.hpp"
#include "wftypes.h"

using namespace rapidxml;
using namespace std;

const char point_delim[] = " ";
const char value_delim[] = ",";

static void normalizePoints(Points * const loop)
{
    loop->max_val = 1.0;

    float min_x,min_y,max_x,max_y;
    min_x = min_y = 9999;
    max_x = max_y = -9999;

    for (int i = 0; i < loop->length; i++)
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
    float center_x = (max_x-min_x)/2.0 + min_x;
    float center_y = (max_y-min_y)/2.0 + min_y;

    for (int i = 0; i < loop->length; i++)
    {
        loop->locations[i].x -= center_x;
        loop->locations[i].x /= biggest_gap;
        loop->locations[i].y -= center_y;
        loop->locations[i].y /= biggest_gap;
    }
}

static void pointStringToPoint(char * const point_string,char * last_pos,Point * const point,Point * const last_point)
{
    char * value;

    value = strtok_r(point_string,value_delim,&last_pos);

    int index = 0;
    while (value != NULL)
    {
        float numeric_value = (float)atof(value);
        
        switch (index)
        {
            case 0:
                point->x = numeric_value + last_point->x;
                break;
            case 1:
                point->y = -numeric_value + last_point->y;
                break;
        }

        value = strtok_r(NULL,value_delim,&last_pos);
        index++;
    }
}

static bool checkIfPoint(const char * const query_string)
{
    return (query_string[0] != 'L' && query_string[0] != 'l' && query_string[0] != 'm' && query_string[0] != 'M' && query_string[0] != 'z' && query_string[0] != 'Z');
}

static bool absMove(const char * const query_string)
{
    return (query_string[0] == 'L' || query_string[0] == 'M');
}

static bool checkIfClose(const char * const query_string)
{
    return (query_string[0] == 'z' || query_string[0] == 'Z');
}

static int getPathLength(char * path_string)
{
    char * initial_pos = path_string;
    char * possible_point;
    char * last_pos;

    possible_point = strtok_r(path_string,point_delim,&last_pos);

    int length = 0;
    while (possible_point != NULL)
    {
        if (checkIfPoint(possible_point))
        {
            length++;
        }
        else if (checkIfClose(possible_point))
        {
            break;
        }
        possible_point = strtok_r(NULL,point_delim,&last_pos);
    }

    path_string = initial_pos;

    return length;
}

static void pathToPoints(char * const path_string,Points * const loop)
{
    char * point;
    char * last_pos;

    char * count_string = new char[strlen(path_string)];
    strncpy(count_string,path_string,strlen(path_string));
    int path_length = getPathLength(count_string);

    loop->locations = new Point[path_length];
    loop->length = path_length;

    point = strtok_r(path_string,point_delim,&last_pos);

    bool absolute_move = false;

    Point last_point;
    Point origin;

    int index = 0;
    while (point!=NULL)
    {
        if (checkIfPoint(point))
        {
            Point this_point;
            if (absolute_move)
            {
                pointStringToPoint(point,last_pos,&this_point,&origin);
            }
            else
            {
                pointStringToPoint(point,last_pos,&this_point,&last_point);
            }
            loop->locations[index] = this_point;
            index++;

            last_point = this_point;
        }
        else if (!checkIfClose(point))
        {
            absolute_move = absMove(point);
        }
        else if (checkIfClose(point))
        {
            loop->closed = true;
        }
        point = strtok_r(NULL,point_delim,&last_pos);
    }

    normalizePoints(loop);
}

void translatePoints(Points * const loop,const Point * const translation)
{
    for (int i = 0; i < loop->length; i++)
    {
        loop->locations[i].x += translation->x;
        loop->locations[i].y += translation->y;
    }   
}

void svgToPoints(const char * const filename,Points * const loop)
{
    //read in file to char array
    ifstream infile(filename,ios::in|ios::binary);
    infile.seekg(0,infile.end);
    int file_length = infile.tellg();
    infile.seekg(0,infile.beg);
    char * buffer =  new char[file_length+1];
    infile.read(buffer,file_length);
    buffer[file_length] = '\0';
    infile.close();

    //parse xml
    xml_document<>doc;
    xml_node<> * root_node;
    xml_node<> * shape_node;
    doc.parse<parse_trim_whitespace>(buffer);
    
    // find first path element in the first group (inkscape format)
    root_node = doc.first_node("svg");
    shape_node = root_node->first_node("g")->first_node("path");

    int path_string_length = shape_node->first_attribute("d")->value_size();
    char * path_string = shape_node->first_attribute("d")->value();

    //parse into x,y points (normalized)
    pathToPoints(path_string,loop);

    delete[] buffer;
}
#endif