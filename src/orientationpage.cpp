#include "orientationpage.h"
#include "ui_orientationpage.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QDir>


OrientationPage::OrientationPage(AddaRunConfiguration *addaconf, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrientationPage),
    addaconf(addaconf)
{
    ui->setupUi(this);

    QRegExp rx("^\\d+(?:,\\d+)+$");  // comma separated integers
    QValidator *validator = new QRegExpValidator(rx, this);

    ui->lineEdit_orientation_single->setValidator(validator);

    ui->comboBox_orientationpresets->addItem("UH Scheme I");
    // ui->comboBox_orientationpresets->addItem("UH Scheme II");

    ui->comboBox_orientation_level->addItem("0");
    ui->comboBox_orientation_level->addItem("1");
    ui->comboBox_orientation_level->addItem("0 and 1");

    /*
    ui->comboBox_orientation_level->addItem("2");
    ui->comboBox_orientation_level->addItem("3");
    ui->comboBox_orientation_level->addItem("4");
    */

    populatePresets();

    // this is the default of the page
    addaconf->setOrientation(0,0,0);
}

OrientationPage::~OrientationPage()
{
    delete ui;
}

void OrientationPage::populatePresets()
{

    // read the presets
    QDir dir(QCoreApplication::applicationDirPath()+"/Orientations/");
    dir.setFilter(QDir::Files | QDir::Hidden );
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    qDebug() << "     Bytes Filename" ;
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        qDebug() << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10)
                               .arg(fileInfo.fileName()));

        ui->comboBox_orientationpresets->addItem(fileInfo.fileName());
    }
}

void OrientationPage::on_comboBox_orientationpresets_currentIndexChanged(int index)
{

    if(index==0) // UH Scheme I
    {
        ui->comboBox_orientation_level->setEnabled(true);
        updateOrientation();
    }
    else
    {
        ui->comboBox_orientation_level->setEnabled(false);
    }

}


void OrientationPage::updateSingleOrientationFromSpinbox()
{
    int alpha = ui->spinBox_orientation_alpha->value();
    int beta  = ui->spinBox_orientation_beta->value();
    int gamma = ui->spinBox_orientation_gamma->value();

    addaconf->setOrientation( alpha, beta,gamma );

    qDebug() << "Orientations " << alpha <<" "<< beta <<" "<< gamma ;
}

void OrientationPage::on_spinBox_orientation_alpha_editingFinished()
{
    updateSingleOrientationFromSpinbox();
}

void OrientationPage::on_spinBox_orientation_beta_editingFinished()
{
    updateSingleOrientationFromSpinbox();
}

void OrientationPage::on_spinBox_orientation_gamma_editingFinished()
{
    updateSingleOrientationFromSpinbox();
}


void OrientationPage::on_radioButton_orientation_single_spinbox_clicked(bool checked)
{

}

void OrientationPage::on_radioButton_orientation_single_linedit_clicked(bool checked)
{
    ui->lineEdit_orientation_single->clear();
}

void OrientationPage::updateOrientation()
{
    qDebug() << "updated Orientation";
    if( !ui->radioButton_orientation_preset->isChecked() )
    {
        ui->comboBox_orientation_level->setEnabled(false);
        return;
    }

    // UH Scheme I
    if( ui->comboBox_orientationpresets->currentIndex() == 0 )
    {
        qDebug() << "here";
        ui->comboBox_orientation_level->setEnabled(true);

        int level = ui->comboBox_orientation_level->currentIndex();
        int numberofValues = addaconf->setOrientationSource( AddaRunConfiguration::OrientationSources::UH_SCHEME_I, level);
        ui->label_orientation_values->setText(QString("(values(%1))").arg(numberofValues));
    }
    else
    {
        ui->comboBox_orientation_level->setEnabled(false);
    }

}


void OrientationPage::on_radioButton_orientation_preset_clicked(bool checked)
{
    updateOrientation( );
    qDebug() << "void OrientationPage::on_radioButton_orientation_preset_clicked(bool checked" <<checked;
}

void OrientationPage::on_radioButton_orientation_from_file_clicked(bool checked)
{

}

void OrientationPage::on_lineEdit_orientation_single_editingFinished()
{
    if( ui->radioButton_orientation_single_linedit->isChecked())
    {

        QStringList l = ui->lineEdit_orientation_single->text().split(",");
        if( l.size() == 3)
        {
            int alpha = l.at(0).toInt();
            int beta = l.at(1).toInt();
            int gamma = l.at(2).toInt();
            addaconf->setOrientation( alpha, beta,gamma );
        }
    }
}

void OrientationPage::on_comboBox_orientation_level_currentIndexChanged(int index)
{
    updateOrientation();
}
