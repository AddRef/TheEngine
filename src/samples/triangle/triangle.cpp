#include "sample.h"

int main()
{    
    Sample sample;
    if (sample.Load())
        return -1;
    sample.Run();
    return 0;
}