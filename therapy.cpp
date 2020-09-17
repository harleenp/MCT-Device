#include "therapy.h"

Therapy::Therapy(QString _name, QString _zones, QString _frequency, int _durationSeconds)
{
    name = _name;
    zones = _zones;
    frequency = _frequency;
    durationSeconds = _durationSeconds;
}

QString& Therapy::getName(){
    return name;
}

QString& Therapy::getZone(){
    return zones;
}
QString& Therapy::getFrequency(){
    return frequency;
}

int Therapy::getDurationSeconds(){
    return durationSeconds;
}
