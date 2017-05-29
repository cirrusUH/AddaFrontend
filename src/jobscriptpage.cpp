#include "jobscriptpage.h"
#include "ui_jobscriptpage.h"

#include <QSettings>
#include <QDebug>
#include <QFileDialog>

JobScriptPage::JobScriptPage(AddaRunConfiguration*addaconf, QWidget *parent) :
    QWidget(parent),
    addaconf(addaconf),
    ui(new Ui::JobScriptPage)
{
    ui->setupUi(this);

    loadSettings();

    ui->lineEdit_job_scriptname->setText("script");
    addaconf->setScriptName("script");

    repaint();
}

JobScriptPage::~JobScriptPage()
{
    delete ui;
}

void JobScriptPage::on_pushButton_job_generate_clicked()
{
    addaconf->writeScriptfile();
}

void JobScriptPage::loadSettings()
{

    QSettings settings("ADDA", "AddaConfigure");
    QStringList budgetcodesList = settings.value("comboBox_job_outputdir").toStringList();

    if (!budgetcodesList.isEmpty())
    {
        ui->comboBox_job_outputdir->addItems(budgetcodesList );
    }
}

void JobScriptPage::saveSettings()
{
    QSettings settings("ADDA", "AddaConfigure");
    QStringList budgetcodesList;

    QComboBox*cb = ui->comboBox_job_outputdir;
    int maxItems = 5;
    int storeN = (cb->count()<maxItems)?cb->count():maxItems;

    for(int index = 0; index < storeN;index++)
    {
        budgetcodesList.append(ui->comboBox_job_outputdir->itemText(index));
    }
    settings.setValue("comboBox_job_outputdir", budgetcodesList);
}


void JobScriptPage::on_comboBox_job_outputdir_currentIndexChanged(const QString &arg1)
{
    qDebug()<< "New comboBox_job_outputdir " << arg1;
    saveSettings();

    addaconf->setOutputDirectory(arg1);
}


void JobScriptPage::on_pushButton_job_browse_clicked()
{
    QSettings settings("ADDA", "AddaConfigure");
    QString lastdir = settings.value("last_jobscript_path","").toString();

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    lastdir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    QStringList files;
    files.append(dir);

    if( !files.empty() )
    {
        // check if we already have it
        QComboBox*cb = ui->comboBox_job_outputdir;

        for(int i=0; i<cb->count(); i++)
        {
            if( files.at(0) == cb->itemText(i) )
            {
                cb->setCurrentIndex(i);
                qDebug() << "It already exists";
                return;
            }
        }

        cb->insertItem(0,files.at(0));
        cb->setCurrentIndex(0);
    }
    else
    {
        return;
    }

    QFileInfo fi(files.at(0));
    settings.setValue("last_jobscript_path",fi.absoluteFilePath());

    qDebug() << "Is selected a directory " << fi.isDir();


}

void JobScriptPage::on_lineEdit_job_scriptname_editingFinished()
{
    addaconf->setScriptName( ui->lineEdit_job_scriptname->text() );
}

void JobScriptPage::on_lineEdit_job_scriptname_textChanged(const QString &arg1)
{
    addaconf->setScriptName( arg1 );
}

void JobScriptPage::on_pushButton_clicked()
{
    addaconf->Debug();
}
