#include "geometrypage.h"
#include "ui_geometrypage.h"

#include <QFileDialog>
#include <QDebug>
#include <QSettings>

#include "addageometry.h"


GeometryPage::GeometryPage(AddaRunConfiguration*addaconf, QWidget *parent) :
    QWidget(parent),
    addaconf(addaconf),
    ui(new Ui::GeometryPage)
{
    ui->setupUi(this);
}

GeometryPage::~GeometryPage()
{
    delete ui;
}

void GeometryPage::updateFileInfo( QString filename )
{


}

void GeometryPage::on_pushButton_open_geom_file_clicked()
{
    QSettings settings("ADDA", "AddaConfigure");
    QString lastdir = settings.value("last_geometry_file_dir","").toString();

    QStringList files = QFileDialog::getOpenFileNames( this,
                                                       "Select one or more Adda geometry files to open",
                                                       lastdir,
                                                       "Images (*.nc)");

    if( files.size() > 1)
    {
        qDebug() << "only one scatterer file support at present ";
        ui->lineEdit_geom_file->setText(files.at(0));
    }

    if( !files.empty() )
    {
        ui->lineEdit_geom_file->setText(files.at(0));
    }
    else
    {
        return;
    }

    QFileInfo fi(files.at(0));
    settings.setValue("last_geometry_file_dir",fi.absoluteFilePath());

    qDebug() << "Is selected a directory " << fi.isDir();

    // look into file
    std::string filen = files.at(0).toUtf8().constData();
    AddaGeometry geomfile( filen );

    qDebug() << geomfile.getXdim();
    qDebug() << geomfile.getYdim();
    qDebug() << geomfile.getZdim();

    ui->textEdit_geom_file_info->clear();
    int x = geomfile.getXdim();
    int y = geomfile.getYdim();
    int z = geomfile.getZdim();
    ui->textEdit_geom_file_info->append("Dimensions [x,y,z] = [ "  + QString::number(x)
                                        + ", " + QString::number(y)
                                        + ", " + QString::number(z)  +"]" );

    ui->textEdit_geom_file_info->append("Occupied dipols: " + QString::number( geomfile.getOccNdpls())
                                        + " ("+  QString::number( int( 100* double(geomfile.getOccNdpls()) / double(x*y*z)  ) ) +"%)" );

    ui->textEdit_geom_file_info->append("Number of different materials (Nmat): " + QString::number(geomfile.getNmat()));


    if( geomfile.hasmaterials() )
    {
        QString materials;

        const std::vector<double>&m_re = geomfile.getMaterialList_re();
        const std::vector<double>&m_im = geomfile.getMaterialList_im();

        materials = "Materials (optional) given in file (refractive index): { ";
        for(size_t i=0; i<m_re.size(); i++)
        {
            materials += QString::number(m_re.at(i))+ " + i" + QString::number(m_im.at(i));
            if(m_re.size() >1 && i<(m_re.size()-1) )
            {
                materials += ", ";
            }
        }
        materials += " }";

        ui->textEdit_geom_file_info->append( materials );
    }
    else // no materials in file
    {
        ui->textEdit_geom_file_info->append("Materials (optional) given in file: no" );
    }

    QString gridsizestr = "Gridsize (optional): ";
    if( geomfile.hasgridsize() )
    {
        gridsizestr += QString::number(geomfile.getGridSize(),'g',4 ) + " nm" ;
    }
    else
    {
        gridsizestr += " not specified";
    }

    ui->textEdit_geom_file_info->append( gridsizestr );

    ui->textEdit_geom_file_info->append("Estim. total memory (assuming low mem iterative solver): "
                                        + QString::number( double(geomfile.computeMemoryTotal())/(1024*1024), 'f', 2)  +"MB");

    ui->textEdit_geom_file_info->append("Memory per processor (assuming np==nz and low mem iterative solver): "
                                        + QString::number( double(geomfile.computeMemoryPerProcessor())/(1024*1024), 'f', 2)  +"MB");


    addaconf->setGeometry( geomfile );
    addaconf->Debug();
}


void GeometryPage::on_lineEdit_geom_file_editingFinished()
{
    qDebug() << "Geom Page editing finished";
}
