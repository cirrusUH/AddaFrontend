#ifndef SCATTEREDFIELDPAGE_H
#define SCATTEREDFIELDPAGE_H

#include <QWidget>

namespace Ui {
class ScatteredFieldPage;
}

#include "addarunconfiguration.h"

class ScatteredFieldPage : public QWidget
{
    Q_OBJECT

public:
    explicit ScatteredFieldPage(AddaRunConfiguration*addaconf,QWidget *parent = 0);
    ~ScatteredFieldPage();

signals:
    void errmsg(const QString&msg);
    void logmsg(const QString&msg);

private slots:

    void on_comboBox_scatgrid_fields_currentIndexChanged(int index);

private:
    Ui::ScatteredFieldPage *ui;
    AddaRunConfiguration*addaconf;
    void populateMultiplePreset();
};

#endif // SCATTEREDFIELDPAGE_H
