#ifndef TARGETMACHINE_LINUXBASH_H
#define TARGETMACHINE_LINUXBASH_H

#include <QWidget>
#include <thread>

#include "addarunconfiguration.h"

namespace Ui {
class TargetMachine_LinuxBash;
}

class TargetMachine_LinuxBash : public QWidget

{
    Q_OBJECT

public:
    explicit TargetMachine_LinuxBash(AddaRunConfiguration *addaconf, QWidget *parent = 0);
    ~TargetMachine_LinuxBash();

private:

    AddaRunConfiguration*addaconf;
    Ui::TargetMachine_LinuxBash *ui;
};

#endif // TARGETMACHINE_LINUXBASH_H
