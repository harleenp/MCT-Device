#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include "pcm.h"
#include <QRandomGenerator>

namespace Ui {
class MainWindow;
}

// This class holds the View details and Controller methods to perform various actions
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnOnOff_clicked();
    void on_btnMenu_clicked();
    void on_btnUp_clicked();
    void on_btnDown_clicked();
    void on_btnOk_clicked();
    void on_btnLeft_clicked();
    void on_btnBack_clicked();
    void on_btnRight_clicked();
    void updateTime();

private:
    QTimer* timer; // timer
    int remainingTime = 0; // remianing time for the timer to show
    QList<PCMStates> prevStates; // previous states to implement back functionality
    Ui::MainWindow *ui;
    PCM pcm; // the machine
    void toggleDisplay(bool showMenuList);
    void turnOffState();
    void turnOnState();
    void mainMenuState();
    void btnOk();
    void btnBack();
    void showMenu(QList<QString> menu, int selected);
    void updateBattery();
};

#endif // MAINWINDOW_H
