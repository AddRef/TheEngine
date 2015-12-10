#pragma once

struct IReleasable
{
    virtual void Release() 
    { 
        delete this;
    }
}