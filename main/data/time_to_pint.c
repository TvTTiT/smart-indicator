#include "time_to_pint.h"

int hour_pint;

int convert_hour_to_pint(int hour){
    while(hour > 12){
        hour -=  12;
    }
    switch (hour)
    {
    case 0:
        hour_pint = 10;
        break;
    
    case 1:
        hour_pint = 12;
        break;

    case 2:
        hour_pint = 14;
        break;
    
    case 3:
        hour_pint = 16;
        break;

    case 4:
        hour_pint = 18;
        break;
    
    case 5:
        hour_pint = 20;
        break;

    case 6:
        hour_pint = 22;
        break;
    
    case 7:
        hour_pint = 0;
        break;

    case 8:
        hour_pint = 2;
        break;
    
    case 9:
        hour_pint = 4;
        break;

    case 10:
        hour_pint = 6;
        break;
    
    case 11:
        hour_pint = 8;
        break;
    }
    return hour_pint;
}
