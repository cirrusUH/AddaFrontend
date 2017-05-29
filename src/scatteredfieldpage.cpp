#include "scatteredfieldpage.h"
#include "ui_scatteredfieldpage.h"
#include <QDir>
#include <QDebug>

ScatteredFieldPage::ScatteredFieldPage(AddaRunConfiguration*addaconf, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScatteredFieldPage),
    addaconf(addaconf)
{
    ui->setupUi(this);

    populateMultiplePreset();
}

ScatteredFieldPage::~ScatteredFieldPage()
{
    delete ui;
}

void ScatteredFieldPage::populateMultiplePreset()
{
    QDir dir( QApplication::applicationDirPath() + "/Scattergrid/" );
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();

    qDebug() << "Scattered Fields";
    foreach(QFileInfo fileInfo, list)
    {
        qDebug() << fileInfo.fileName();
        ui->comboBox_scatgrid_fields->addItem( fileInfo.fileName(), fileInfo.absoluteFilePath() );
    }
}


void ScatteredFieldPage::on_comboBox_scatgrid_fields_currentIndexChanged(int index)
{

    QString absolutefile = ui->comboBox_scatgrid_fields->itemData(index).toString();
    if( ui->radioButton_scatgrid_presets->isChecked())
    {
        addaconf->setScatGridFilename(absolutefile);
    }
}
