#ifndef TARGETMACHINE_ARCHER_H
#define TARGETMACHINE_ARCHER_H

#include <QSettings>
#include <QWidget>

#include "addarunconfiguration.h"

namespace Ui {
class TargetMachine_Archer;
}


class TargetMachine_Archer : public QWidget
{
    Q_OBJECT

public:
    explicit TargetMachine_Archer(AddaRunConfiguration *addaconf, QWidget *parent = 0);
    ~TargetMachine_Archer();

private slots:

    void on_comboBox_targetmachine_archer_budgetcodes_currentIndexChanged(const QString &arg1);

    void on_checkBox_targetmachine_archer_add_stuckjobmonitor_toggled(bool checked);


    void on_lineEdit_targetmachine_percent_editingFinished();

    void on_comboBox_targetmachine_archer_queues_activated(const QString &arg1);

    void on_comboBox_targetmachine_archer_budgetcodes_currentTextChanged(const QString &arg1);

    void on_comboBox_archer_jobtye_currentIndexChanged(const QString &arg1);

private:

    void loadSettings();
    void saveSettings();

    AddaRunConfiguration*addaconf;
    Ui::TargetMachine_Archer *ui;
};

#endif // TARGETMACHINE_ARCHER_H
