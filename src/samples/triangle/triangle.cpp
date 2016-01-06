#include "sample.h"

int main()
{    
    Sample sample;
    if (sample.Init())
        return -1;
    sample.Run();
    return 0;
}