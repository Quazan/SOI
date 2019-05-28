#ifndef PRINTMONITOR_H
#define PRINTMONITOR_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "monitor.h"

class printMonitor : Monitor
{

public:
        void print(std::string s);
};

void printMonitor::print(std::string s)
{
    enter();
    std::cout<<s<<"\n";
    leave();
}



#endif // PRINTMONITOR_H

