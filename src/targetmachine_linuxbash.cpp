#include "targetmachine_linuxbash.h"
#include "ui_targetmachine_linuxbash.h"

TargetMachine_LinuxBash::TargetMachine_LinuxBash(AddaRunConfiguration*addaconf,QWidget *parent) :
    QWidget(parent),
    addaconf(addaconf),
    ui(new Ui::TargetMachine_LinuxBash)
{
    ui->setupUi(this);

    unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
    ui->label_thismachine_cores->setText( "(this machine has: " + QString::number(concurentThreadsSupported)+")");

    repaint();
}


TargetMachine_LinuxBash::~TargetMachine_LinuxBash()
{
    delete ui;
}
