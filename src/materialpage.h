#ifndef MATERIALPAGE_H
#define MATERIALPAGE_H

#include <QWidget>
#include "addarunconfiguration.h"
#include "refractiveindices.h"

namespace Ui {
class MaterialPage;
}

class MaterialPage : public QWidget
{
    Q_OBJECT


    RefractiveIndices refractiveindices;

    QList<double> single_m_re;
    QList<double> single_m_im;
    QList<QString> single_m_name;

public:
    explicit MaterialPage(AddaRunConfiguration *addaconf, QWidget *parent = 0);
    ~MaterialPage();


private slots:
    void on_radioButton_material_use_from_file_toggled(bool checked);

    void on_radioButton_material_single_toggled(bool checked);
    void on_radioButton_material_single_preset_toggled(bool checked);
    void on_lineEdit_material_re_editingFinished();
    void on_lineEdit_material_im_editingFinished();

    void on_radioButton_material_single_presets_toggled(bool checked);

    void on_comboBox_singlematerials_preset_currentIndexChanged(int index);

    void on_radioButton_material_lambdascheme_toggled(bool checked);

    void on_comboBox_material_ref_scheme_currentIndexChanged(int index);

signals:
    void errmsg(const QString&msg);
    void logmsg(const QString&msg);

private:
    void populateSinglePresets();
    void populateMultiplePreset();

    AddaRunConfiguration*addaconf;
    Ui::MaterialPage *ui;
};

#endif // MATERIALPAGE_H
