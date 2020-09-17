#include "pcm.h"

// constructor creates programs and turn off the machine
PCM::PCM()
{
    programs = {};
    programs << Therapy("HEAD", "Neck and Collar Zone", "60Hz", 300);
    programs << Therapy("COUGH", "Sub Jaw Zone", "7710Hz", 300);
    programs << Therapy("BACK", "Spinal Column Painful Zone", "140;77;20Hz", 900);

    turnOff();
}

// turn on
void PCM::turnOn(){
    battery = 50 + QRandomGenerator::global()->bounded(51);
    turnedOn = true;
}

// turn off
void PCM::turnOff(){
    turnedOn = false;
}


// check if turned on
bool PCM::isTurnedOn(){
    return turnedOn;
}

// get main menu
QList<QString> PCM::getMainMenu() {
    return getMenu(MAIN_MENU);
}

// get menu based on given state
QList<QString> PCM::getMenu(PCMStates state) {

    useBattery(3);

    if(state == MAIN_MENU) {
       return {"Main Menu", "Programs", "Frequency", "Children", "Power", "Settings"};
    }

    if(state == PROGRAMS) {
        return {"Programs", "Head", "Cough", "Back"};
    }

    if(state == FREQUENCY) {
        return {"Frequency", "1.0-9.9Hz", "10Hz", "20Hz", "60Hz", "77Hz", "125Hz"};
    }

    if(state == CHILDREN) {
        return {"Children", "DISABLE", "UP TO 1 Year", "1-3 YEARS", "4-7 YEARS", "7-12 YEARS"};
    }

    if(state == SETTINGS) {
        return {"Settings", "Language", "Brightness", "Sound", "Skin Color"};
    }

    if(state == SOUND) {
        return {"Sound", "General", "Sound Cont.", "Sound Button"};
    }

    if(state == POWER) {
        QList<QString> list;
        list << "Power";
        for (int i = 0 ; i <= 100; i++) {
            list << QString::number(i);
        }

        return list;
    }

    return {};
}


// Setters and Getters

void PCM::setPower(int power){
    this->power = power;
}

int PCM::getPower(){
    return power;
}

void PCM::setFrequency(int index){
     useBattery(1);
    iFrequency = index;
}

void PCM::setChildren(int index){
     useBattery(1);
    iChildren = index;
}

int PCM::getFrequency(){
    return iFrequency;
}

int PCM::getChildren(){
    return iChildren;
}

void PCM::setCurrentProgram(int index) {
     useBattery(1);
    currentProgram = index;
}

Therapy& PCM::getCurrentProgram(){
    return programs[currentProgram];
}

bool PCM::onSkin() {
    return QRandomGenerator::global()->bounded(100) <= 90;
}

int PCM::getBattery() {
    return int(battery);
}

void PCM::useBattery(double amount){
    battery -= amount;
    if(battery <= 0) {
        battery = 0;
        turnOff();
    }
}

