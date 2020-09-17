#ifndef PCM_H
#define PCM_H

#include <QList>
#include <QString>
#include <QRandomGenerator>
#include "therapy.h"

// Defines various states the machine can be in
// Each state refers to a separate screen on machine
enum PCMStates {MAIN_MENU, PROGRAMS, FREQUENCY, CHILDREN, POWER, PROGRAMS_DETAIL, PROGRAM_TIMER, FREQUENCY_TIMER, SETTINGS, SOUND};

class PCM
{
public:
    PCM();
    void turnOn();
    void turnOff();
    bool isTurnedOn();
    QList<QString> getMainMenu();
    QList<QString> getMenu(PCMStates state);
    void setPower(int power);
    int getPower();
    void setFrequency(int index);
    void setChildren(int index);
    void setCurrentProgram(int index);
    Therapy& getCurrentProgram();
    int getFrequency();
    int getChildren();
    bool onSkin();
    int getBattery();
    void useBattery(double amount);

private:
    bool turnedOn;
    int power = -1;
    int iFrequency = -1;
    int iChildren = -1;
    int currentProgram;
    double battery;
    QList<Therapy> programs;

};

#endif // PCM_H
