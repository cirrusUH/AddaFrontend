#ifndef WAVELENGTHPAGE_H
#define WAVELENGTHPAGE_H

#include <QWidget>
#include <QDebug>

#include "addarunconfiguration.h"
#include "numbersequencetodoublelist.h"

namespace Ui {
class WaveLengthPage;
}

class WaveLengthPage : public QWidget
{
    Q_OBJECT

    QList<double> lambdasfromfile;

public:
    explicit WaveLengthPage(AddaRunConfiguration*addaconf,QWidget *parent = 0);
    ~WaveLengthPage();

private slots:
    void validateInput();
    void restartvalidationtimer() const;

    void on_comboBox_singlewavelenghtslist_currentIndexChanged(int index);
    void on_radioButton_wavelength_single_clicked(bool checked);

    void on_radioButton_wavelength_lambdaeq_clicked(bool checked);

signals:
    void errmsg(const QString&msg);
    void logmsg(const QString&msg);

private:
    void populateSinglePresets();
    void populateMultiplePreset();

    Ui::WaveLengthPage *ui;
    QTimer*delayedvalidationtimer;
    AddaRunConfiguration*addaconf;
};

#endif // WAVELENGTHPAGE_H
