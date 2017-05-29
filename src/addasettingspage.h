#ifndef ADDASETTINGSPAGE_H
#define ADDASETTINGSPAGE_H

#include <QWidget>
#include "addarunconfiguration.h"

#include "numbersequencetodoublelist.h"

namespace Ui {
class AddaSettingsPage;
}

class AddaSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit AddaSettingsPage(AddaRunConfiguration*addaconf, QWidget *parent = 0);
    ~AddaSettingsPage();

private slots:
    void on_comboBox_settings_solver_currentIndexChanged(const QString &arg1);

    void on_lineEdit_settings_additionalparam_textChanged(const QString &arg1);

private slots:
    void validateInput();
    void restartvalidationtimer() const;

    void on_lineEdit_settings_maxiter_editingFinished();


    void on_lineEdit_settings_maxiter_textChanged(const QString &arg1);

    void on_radioButton_dpl_derivefromfile_clicked(bool checked);

    void on_lineEdit_settings_lambda_user_textChanged(const QString &arg1);

    void on_lineEdit_settings_lambda_user_editingFinished();

signals:
    void errmsg(const QString&msg);
    void logmsg(const QString&msg);


private:
    AddaRunConfiguration*addaconf;
    QTimer*delayedvalidationtimer;
    Ui::AddaSettingsPage *ui;
};

#endif // ADDASETTINGSPAGE_H
