#ifndef JOBSCRIPTPAGE_H
#define JOBSCRIPTPAGE_H

#include <QWidget>
#include "addarunconfiguration.h"

namespace Ui {
class JobScriptPage;
}

class JobScriptPage : public QWidget
{
    Q_OBJECT

public:
    explicit JobScriptPage(AddaRunConfiguration*addaconf,QWidget *parent = 0);
    ~JobScriptPage();

private slots:
    void on_comboBox_job_outputdir_currentIndexChanged(const QString &arg1);

private slots:
    void on_pushButton_job_generate_clicked();
    
    void on_pushButton_job_browse_clicked();

    void on_lineEdit_job_scriptname_editingFinished();

    void on_lineEdit_job_scriptname_textChanged(const QString &arg1);

    void on_pushButton_clicked();

signals:
    void errmsg(const QString&msg);
    void logmsg(const QString&msg);

private:
    void loadSettings();
    void saveSettings();

    AddaRunConfiguration*addaconf;
    Ui::JobScriptPage *ui;
};

#endif // JOBSCRIPTPAGE_H
