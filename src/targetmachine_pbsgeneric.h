#ifndef TARGETMACHINE_PBSGENERIC_H
#define TARGETMACHINE_PBSGENERIC_H

#include <QWidget>

#include "addarunconfiguration.h"

namespace Ui {
class TargetMachine_PBSgeneric;
}

class TargetMachine_PBSgeneric : public QWidget
{
    Q_OBJECT

public:
    explicit TargetMachine_PBSgeneric(AddaRunConfiguration*addaconf,QWidget *parent = 0);
    ~TargetMachine_PBSgeneric();

private slots:
    void on_radioButton_target_jobtype_arrayjob_toggled(bool checked);

    void on_radioButton_target_jobtype_forloop_toggled(bool checked);

private:
    AddaRunConfiguration*addaconf;
    Ui::TargetMachine_PBSgeneric *ui;
};

#endif // TARGETMACHINE_PBSGENERIC_H
