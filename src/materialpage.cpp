#include "materialpage.h"
#include "ui_materialpage.h"

#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include "addageometry.h"


MaterialPage::MaterialPage(AddaRunConfiguration*addaconf, QWidget *parent) :
    QWidget(parent),
    addaconf(addaconf),
    ui(new Ui::MaterialPage)
{
    ui->setupUi(this);

    ui->lineEdit_material_im->setValidator( new QDoubleValidator(this) );
    ui->lineEdit_material_re->setValidator( new QDoubleValidator(this) );
    ui->lineEdit_material_scheme_Temp->setValidator( new QDoubleValidator(this) );

    populateMultiplePreset();
    populateSinglePresets();

    ui->comboBox_material_ref_scheme->addItems( refractiveindices.getAvailableSchemes() );
}

MaterialPage::~MaterialPage()
{
    delete ui;
}

void MaterialPage::on_radioButton_material_use_from_file_toggled(bool checked)
{
    if(checked)
    {
        if( addaconf->getGeometriesEmpty() == true  )
        {

            QMessageBox::information(0, "Geometry file error", "No geometry file selected." );
            ui->radioButton_material_single->setChecked(true);
        }
        else
        {
            if( addaconf->getFirstGeometryHasMaterials() == false)
            {
                QMessageBox::information(0, "Geometry file problem", "Geometry file has no materials." );
                ui->radioButton_material_single->setChecked(true);
            }
        }

        addaconf->useMaterialFromGeomFile();
    }
    else
    {

        // nothing to do really
    }
}


void MaterialPage::on_radioButton_material_single_preset_toggled(bool checked)
{
    Q_UNUSED(checked);
}

void MaterialPage::on_radioButton_material_single_toggled(bool checked)
{
    Q_UNUSED(checked);
    addaconf->setMaterialSource(AddaRunConfiguration::MaterialSources::USER_DEFINED_SINGLE);
}

void MaterialPage::on_lineEdit_material_re_editingFinished()
{
    QString m_re = ui->lineEdit_material_re->text();
    QString m_im = ui->lineEdit_material_im->text();
    addaconf->setMaterial(m_re.toDouble(),m_im.toDouble());
    addaconf->setMaterialSource(AddaRunConfiguration::MaterialSources::USER_DEFINED_SINGLE);
    addaconf->Debug();
}

void MaterialPage::on_lineEdit_material_im_editingFinished()
{
    QString m_re =  ui->lineEdit_material_re->text();
    QString m_im = ui->lineEdit_material_im->text();
    addaconf->setMaterial(m_re.toDouble(),m_im.toDouble());
    addaconf->setMaterialSource(AddaRunConfiguration::MaterialSources::USER_DEFINED_SINGLE);
    addaconf->Debug();
}


void MaterialPage::populateMultiplePreset()
{
    /*
  *    QDir dir("./Materials/");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();

    foreach(QFileInfo fileInfo, list)
    {
        if( fileInfo.fileName().contains("singlemateriallist.csv"))
        {
            continue;
        }

        ui->comboBox_multiplematerialslist->addItem( fileInfo.fileName() );
    }
    */
}

void MaterialPage::populateSinglePresets()
{
    QString fileName = QCoreApplication::applicationDirPath()  + "/Materials/singlemateriallist.csv";
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning( this, tr("Application"),
                              tr("Cannot read file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
    }

    QTextStream in(&file);

    QString buf = in.readAll();
    QStringList listbuf = buf.split('\n');

    foreach(QString s, listbuf)
    {
        // qDebug() << s;

        if( (s.length()==0) )
        {
            continue;
        }

        if( (s.at(0)=='#') )
        {
            continue;
        }

        // remove "
        s.replace("\"","");

        //qDebug()<< "> " << s;

        QStringList items = s.split(',');
        if(items.count()<3)
        {
            continue;
        }
        qDebug() << items.at(2) << "("<<items.at(0)<< "+i "<< items.at(1)<< ")";

        single_m_re.append( items.at(0).toDouble() );
        single_m_im.append( items.at(1).toDouble() );

        QString name = items.at(2);
        name.replace("\"","");
        single_m_name.append( name );

        ui->comboBox_singlematerials_preset->addItem( name +": (" + items.at(0)+ "+i "+ items.at(1) +")");
    }


}

void MaterialPage::on_radioButton_material_single_presets_toggled(bool checked)
{
    if(checked)
    {
        int index = ui->comboBox_singlematerials_preset->currentIndex();

        addaconf->setMaterial(single_m_re.at(index), single_m_im.at(index));
        addaconf->setMaterialSource(AddaRunConfiguration::MaterialSources::USER_DEFINED_SINGLE);
    }
    else
    {

    }
}

void MaterialPage::on_comboBox_singlematerials_preset_currentIndexChanged(int index)
{
    if( ui->radioButton_material_single_presets->isChecked())
    {
        addaconf->setMaterial(single_m_re.at(index), single_m_im.at(index));
        addaconf->setMaterialSource(AddaRunConfiguration::MaterialSources::USER_DEFINED_SINGLE);
        //addaconf->Debug();
    }
    addaconf->Debug();
}

void MaterialPage::on_radioButton_material_lambdascheme_toggled(bool checked)
{
    qDebug() << "MaterialPage::on_radioButton_material_lambdascheme_toggled(bool checked)";

    if( checked )
    {
        if(addaconf->lambdalist.size()<1)
        {
            emit errmsg("No wavelength(s) set");
            ui->radioButton_material_single->setChecked(true);
            return;
        }

        int index = ui->comboBox_material_ref_scheme->currentIndex();
        if(index==0)
        {
            addaconf->setMaterialScheme( RefractiveIndices::Schemes::WATER_ICE_WARREN_2008 );
        }
    }
}

void MaterialPage::on_comboBox_material_ref_scheme_currentIndexChanged(int index)
{
    if(ui->radioButton_material_lambdascheme->isChecked())
    {
        if(index==0)
        {
            addaconf->setMaterialScheme( RefractiveIndices::Schemes::WATER_ICE_WARREN_2008 );
            ui->label_material_temp->setEnabled(false);
            ui->lineEdit_material_scheme_Temp->setEnabled(false);
        }
        else
        {
            ui->label_material_temp->setEnabled(true);
            ui->lineEdit_material_scheme_Temp->setEnabled(true);
        }
    }
}


