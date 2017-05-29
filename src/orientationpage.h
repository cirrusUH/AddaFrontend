#ifndef ORIENTATIONPAGE_H
#define ORIENTATIONPAGE_H

#include <QWidget>
#include "addarunconfiguration.h"

namespace Ui {
class OrientationPage;
}

class OrientationPage : public QWidget
{
    Q_OBJECT

public:
    explicit OrientationPage(AddaRunConfiguration*addaconf,QWidget *parent = 0);
    ~OrientationPage();

private slots:


    void populatePresets();

    // auto connected signals
    void on_spinBox_orientation_alpha_editingFinished();
    void on_spinBox_orientation_beta_editingFinished();
    void on_spinBox_orientation_gamma_editingFinished();

    void on_comboBox_orientationpresets_currentIndexChanged(int index);

    void on_radioButton_orientation_single_spinbox_clicked(bool checked);

    void on_radioButton_orientation_single_linedit_clicked(bool checked);

    void on_radioButton_orientation_preset_clicked(bool checked);

    void on_radioButton_orientation_from_file_clicked(bool checked);

    void on_lineEdit_orientation_single_editingFinished();

    void on_comboBox_orientation_level_currentIndexChanged(int index);

signals:
    void errmsg(const QString&msg);
    void logmsg(const QString&msg);

private:
    Ui::OrientationPage *ui;
    AddaRunConfiguration*addaconf;
    void updateSingleOrientationFromSpinbox();
    void updateOrientation();
};

#endif // ORIENTATIONPAGE_H
