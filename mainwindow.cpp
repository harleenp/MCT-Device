#include "mainwindow.h"
#include "ui_mainwindow.h"

// creates a machine in turn off state
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));

    // go to turn off state
    turnOffState();
}

MainWindow::~MainWindow()
{
    delete ui;
}


// toggles the display
// At a time, there is only one thing than be shown: menu and the menu title or the giant label
void MainWindow::toggleDisplay(bool showMenuList) {
    if(!showMenuList) { // if hide menu
        ui->lstMenu->hide();
        ui->lblMenuTitle->hide();
        ui->lblMainScreen->show();
    } else { // show menu
        ui->lstMenu->show();
        ui->lblMenuTitle->show();
        ui->lblMainScreen->hide();
    }
}


// turn off the machine
void MainWindow::turnOffState() {
    timer->stop();
    pcm.turnOff();
    prevStates.clear();
    toggleDisplay(false);
    ui->lblBattery->hide();
    ui->lblMainScreen->setText("");
}


// turn on the machine
void MainWindow::turnOnState() {
    timer->stop();
    pcm.turnOn();
    ui->lblMainScreen->setText("");
     ui->lblBattery->show();
     updateBattery();
    mainMenuState();
}


// main menu state
void MainWindow::mainMenuState() {
    // if pcm is off, it will not do anything
    if(pcm.isTurnedOn() == false)
        return;

        // if menu is already shown
    if(prevStates.size() > 0 && prevStates.last() == MAIN_MENU)
        return;

    // show main menu
    prevStates.append(MAIN_MENU);
    QList<QString> menuItems = pcm.getMainMenu();
    showMenu(menuItems, -1);
    updateBattery();
}


// slot for on/off button
void MainWindow::on_btnOnOff_clicked()
{
    if(pcm.isTurnedOn())
        turnOffState();
     else
        turnOnState();

    updateBattery();
}


// main-menu button slot
void MainWindow::on_btnMenu_clicked()
{
    mainMenuState();
}


// up button
void MainWindow::on_btnUp_clicked()
{
    if(pcm.isTurnedOn() == false)
        return;

    // go one step up if possible
    int row = ui->lstMenu->currentRow();
    if(row -1 >= 0 && row - 1 < ui->lstMenu->count())
        ui->lstMenu->setCurrentRow(row - 1);
    updateBattery();

}


// down button
void MainWindow::on_btnDown_clicked()
{
    if(pcm.isTurnedOn() == false)
        return;
    int row = ui->lstMenu->currentRow();
    if(row + 1 >= 0 && row + 1 < ui->lstMenu->count())
        ui->lstMenu->setCurrentRow(row + 1);
    updateBattery();
}


// helper method for OK action
void MainWindow::btnOk() {
    if(pcm.isTurnedOn() == false)
        return;

    //get selected row
    int row = ui->lstMenu->currentRow();
    if(row>= 0 && row < ui->lstMenu->count()) { // valid selection
        PCMStates state = prevStates.last();

        // if current state is main menu
        if(state == MAIN_MENU) {
            if(row == 0) { // programs
                prevStates.append(PROGRAMS);
                showMenu(pcm.getMenu(PROGRAMS), -1);
            }

            else if(row == 1) { // frequency
                prevStates.append(FREQUENCY);
                showMenu(pcm.getMenu(FREQUENCY), pcm.getFrequency());
            }

            else if(row == 2) { // Children
                prevStates.append(CHILDREN);
                showMenu(pcm.getMenu(CHILDREN), pcm.getChildren());
            }

            else if(row == 3) { // power
                prevStates.append(POWER);
                showMenu(pcm.getMenu(POWER), pcm.getPower());
            }

            else if(row == 4) { // settings
                prevStates.append(SETTINGS);
                showMenu(pcm.getMenu(SETTINGS), -1);
            }
        }

        else if(state == CHILDREN) { // if current state is children
            pcm.setChildren(ui->lstMenu->currentRow());
            btnBack();
        }

        else if(state == POWER) { // current state is power
            pcm.setPower(ui->lstMenu->currentRow()); // set selected power
            btnBack();
        }

        else if(state == SETTINGS) { // settings
            if(row == 2) { // Sound
                prevStates.append(SOUND);
                showMenu(pcm.getMenu(SOUND), -1);
            } else {
                 btnBack(); // do  nothing and go one step back
            }

        }

        else if(state == PROGRAMS) { // programs
            // set cuyrrent program
            pcm.setCurrentProgram(ui->lstMenu->currentRow());
            prevStates.append(PROGRAMS_DETAIL);

            // show label and show the current program
            toggleDisplay(false);
            Therapy current = pcm.getCurrentProgram();
            timer->stop();
            remainingTime = 0;
            ui->lblMainScreen->setText(current.getName() +"\n" + current.getZone() + "\n" + current.getFrequency() + "\nPlease OK to Start");
        }

        else if(state == PROGRAMS_DETAIL) { // into program detail and program neeeds to be started
            // set remianig time
            remainingTime = pcm.getCurrentProgram().getDurationSeconds();
            prevStates.append(PROGRAM_TIMER);
            ui->lblMainScreen->setText("");
            timer->stop();
            timer->start(1000); //start timer
       }

        else if(state == FREQUENCY) { // Frequency program selected
            // start timer
               remainingTime = 0;
               prevStates.append(FREQUENCY_TIMER);
               toggleDisplay(false);
               ui->lblMainScreen->setText("00:00");
               timer->stop();
               timer->start(1000);
        }

        else if(state == SOUND) {
            btnBack();
        }


    }

    updateBattery();
}



// helper method to display updated time for the timer
void MainWindow::updateTime() {
    // update time
    pcm.useBattery(0.2);
    if(prevStates.last() == PROGRAM_TIMER)
        remainingTime -= 1;
    else
        remainingTime += 1;

    // update label
    int mins = remainingTime/60;
    int secs = remainingTime%60;
    QTime* time = new QTime(0, mins, secs);
    QString timeStr = time->toString("mm:ss");
    ui->lblMainScreen->setText(timeStr);

    updateBattery();

    if(remainingTime <= 0) { // if time up
        timer->stop();
        btnBack();
    }
    else {
       if(!pcm.onSkin()) {
            timer->stop();
            btnBack();
       }
    }

    delete time;
}


// show a menu
void MainWindow::showMenu(const QList<QString> menuItems, int selected){
    toggleDisplay(true);
    ui->lstMenu->clear();
    ui->lblMenuTitle->setText(menuItems[0]);
    for (int i = 1; i < menuItems.size(); ++i) {
        ui->lstMenu->addItem(menuItems[i]);
    }
    if(selected != -1)
         ui->lstMenu->setCurrentRow(selected);

    updateBattery();
}


// ok button
void MainWindow::on_btnOk_clicked()
{
    btnOk();
    updateBattery();
}

// left button behave as back button
void MainWindow::on_btnLeft_clicked()
{
    btnBack();
    updateBattery();
}


// helper method for back button operation
void MainWindow::btnBack() {
    if(pcm.isTurnedOn() == false)
        return;

    if(prevStates.size() > 0 && prevStates.last() == MAIN_MENU)
        return;

    prevStates.removeLast(); // remove the last state and get the current state
    PCMStates state = prevStates.last();


    if(state == PROGRAMS) {
        showMenu(pcm.getMenu(PROGRAMS), - 1);
    }

    else if(state == FREQUENCY) {
        timer->stop();
        showMenu(pcm.getMenu(FREQUENCY), pcm.getFrequency());
    }

    else if(state == CHILDREN) {
        showMenu(pcm.getMenu(CHILDREN), pcm.getChildren());
    }

    else if(state == POWER) {
        showMenu(pcm.getMenu(POWER), pcm.getPower());
    }

    else if(state == SETTINGS) {
        showMenu(pcm.getMenu(SETTINGS), -1);
    }

    else if(state == PROGRAMS_DETAIL) {
        timer->stop();
        btnBack();
    }

    else if(state == MAIN_MENU) {
        showMenu(pcm.getMenu(MAIN_MENU), -1);
    }

    updateBattery();

}


// back button
void MainWindow::on_btnBack_clicked()
{
    btnBack();
    updateBattery();
}

// right button behave as back button
void MainWindow::on_btnRight_clicked()
{
    btnOk();
    updateBattery();
}

void MainWindow::updateBattery(){
    ui->lblBattery->setText(QString("Battery: %1").arg(pcm.getBattery()));
    if(!pcm.isTurnedOn()) {
        turnOffState();
    }
}
