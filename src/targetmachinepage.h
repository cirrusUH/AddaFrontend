#ifndef TARGETMACHINEPAGE_H
#define TARGETMACHINEPAGE_H

#include <QWidget>

#include "addarunconfiguration.h"

namespace Ui {
class TargetMachinePage;
}

class TargetMachinePage : public QWidget
{
    Q_OBJECT

public:
    explicit TargetMachinePage(AddaRunConfiguration*addaconf, QWidget *parent = 0);
    ~TargetMachinePage();

private slots:
    void on_comboBox_targetmachine_addabinarypath_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked(bool checked);

    void on_comboBox_targetMachine_currentIndexChanged(int index);

signals:
    void errmsg(const QString&msg);
    void logmsg(const QString&msg);

private:
    void loadSettings();
    void saveSettings();

    AddaRunConfiguration*addaconf;
    Ui::TargetMachinePage *ui;
};

#endif // TARGETMACHINEPAGE_H
