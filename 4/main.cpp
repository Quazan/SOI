#include <stdio.h>
#include <thread>
#include <stdlib.h>
#include <time.h>

#include "car.h"
#include "proQueue.h"
#include "crossMonitor.h"
#include "printMonitor.h"

crossMonitor monitor[3];
printMonitor print;

ProQueue production[3];

int carQuantity;
int carFrequency;
int COND;

void cross(int that)
{
    int with = (that + 2) % 3;
    Car c;
    std::string s;
    while(COND)
    {
        if(getProQueueSize(&production[that]) == 0 && monitor[with].getSize() == 0) continue;

        if(proPriority(&production[that]) || (getProQueueSize(&production[that]) > 0 && monitor[with].checkPriority() == 0))
        {
            c = removeCarProQueue(&production[that]);
        }
        else if(monitor[with].getSize() != 0)
        {
            c = monitor[with].take();
        }
        else continue;

        if(that == 0) s = "A: ";
        else if(that == 1) s = "B: ";
        else s = "C: ";
        s += " wjezdza samochod o id: " + std::to_string(c.id);
        print.print(s);

        sleep(1);

        if(c.destination == that)
        {
            if(that == 0) s= "A: ";
            else if(that == 1) s= "B: ";
            else s = "C: ";
            s += "samochod o id: ";
            s += std::to_string(c.id);
            s += " kończy jazde";

            print.print(s);

            continue;
        }

        if(that == 0) s= "A: ";
        else if(that == 1) s= "B: ";
        else s = "C: ";
        s += "samochod o id: " + std::to_string(c.id) + " zjezdza w strone skrzyzowania ";
        if((that+1)%3 == 0) s+= "A ";
        else if((that+1)%3 == 1) s+= "B ";
        else s += "C ";

        print.print(s);
        monitor[that].add(c);
    }
}

int main()
{
    COND = 1;
    Car c;
    std::string s;

    std::thread crossA(cross, 0);
    std::thread crossB(cross, 1);
    std::thread crossC(cross, 2);

    printf("Podaj ilosc samochodow do symulacji\n");
    scanf("%d", &carQuantity);
    printf("Podaj czestotliwosc generacji samochodow\n");
    scanf("%d", &carFrequency);

    for(int i = 1; i <= carQuantity; i++)
    {
        c = generateCar(i);
        addCarProQueue(&production[c.start], c);

        s = "id: " + std::to_string(c.id) + " pri: " + std::to_string(c.priority) + " start: " + std::to_string(c.start) + " dest: " + std::to_string(c.destination);
        print.print(s);

        usleep(carFrequency * 100000);
    }

    scanf("%d", &carQuantity);
    COND = 0;
    crossA.join();
    crossB.join();
    crossC.join();
}
