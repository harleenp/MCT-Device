#ifndef THERAPY_H
#define THERAPY_H

#include <QString>

// This class represent a therapy program offered by machine
class Therapy
{
private:
    QString name;
    QString zones;
    QString frequency;
    int durationSeconds;

public:
    Therapy(QString _name, QString _zones, QString _frequency, int _durationSeconds);
    QString& getName();
    QString& getZone();
    QString& getFrequency();
    int getDurationSeconds();

};

#endif // THERAPY_H
