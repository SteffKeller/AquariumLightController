#include "TimeHelper.hpp"
#include <bits/stdc++.h>

using namespace std;

TimeHelper::TimeHelper()
{
}

TimeHelper::~TimeHelper()
{
}

/**
 * @brief calculate if the acutal time is between the on and off time
 * 
 * @param onTime time to put the lights on
 * @param offTime time go put the lights off
 * @param actualTime  acutal time
 * @return true time is between -> ON
 * @return false time outside ->OFF
 */
bool TimeHelper::calcTimeBetween(tmElements_t *onTime, tmElements_t *offTime, tmElements_t *actualTime)
{
    uint32_t ein, aus, aktuell;                                                                                           // Hilfsvariablen für Sekunden seit Mitternacht
    ein = ((uint32_t)onTime->Hour * 3600) + ((uint32_t)onTime->Minute * 60) + ((uint32_t)onTime->Second);                 // Einschaltzeit Sekunden seit Mitternacht
    aus = ((uint32_t)offTime->Hour * 3600) + ((uint32_t)offTime->Minute * 60) + ((uint32_t)offTime->Second);              // Ausschaltzeit Sekunden seit Mitternacht
    aktuell = ((uint32_t)actualTime->Hour * 3600) + ((uint32_t)actualTime->Minute * 60) + ((uint32_t)actualTime->Second); // Aktuelle Zeit Sekunden seit Mitternacht

    if (aus < ein) // Wenn der Ausgang über Mitternacht eingeschaltet werden soll -> Auszeit kleiner als Einzeit
    {
        // So muss die Aktuelle Zeit grösser als ein Einzeit sein ODER kleiner als die Ausschaltzeit, dann ist der Verbraucher eingeschaltet
        return ((aktuell > ein) || (aktuell < aus));
    }
    else if (ein == aus) // Regenpumpe einschalten? ein und auszeit gleich
    {
        // So muss die Aktuelle Zeit gleich sein wie die einzeit
        return (ein == aktuell);
    }
    else // Wenn der Ausgang innerhalb eines Tages eingeschaltet werden soll -> Auszeit grösser als Einzeit
    {
        // So muss die Aktuelle Zeit grösser als die Einzeit sein UND kleiner als die Auszeit, dann ist der Verbraucher eingeschaltet
        return ((aktuell > ein) && (aktuell < aus));
    }
}

String TimeHelper::makeTimeString(tmElements_t &time)
{
    char strArray[6];
    sprintf(strArray, "%02d:%02d", time.Hour, time.Minute);
    return String(strArray);
}
tmElements_t TimeHelper::makeTmElement(String timeString)
{
    tmElements_t timeElem;
    String hour = timeString.substring(0, timeString.indexOf(':'));
    String min = timeString.substring(timeString.indexOf(':') + 1, timeString.length());

    timeElem.Hour = hour.toInt();
    timeElem.Minute = min.toInt();

    return timeElem;
}
