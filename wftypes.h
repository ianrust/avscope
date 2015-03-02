struct WaveForm
{
    char * data;
    int length;
};

struct SampleInfo
{
    float rate;
    float frequency;
    int bytes_per_sample;
};

struct Point
{
    float x;
    float y;
    Point(float x_init,float y_init)
    {
        x = x_init;
        y = y_init;
    }
    Point()
    {
        x = 0;
        y = 0;
    }
};

struct Points
{
    Point * locations;
    int length;
    float max_val;
}; 