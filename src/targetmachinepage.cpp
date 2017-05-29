#include "targetmachinepage.h"
#include "ui_targetmachinepage.h"

#include "targetmachine_archer.h"
#include "targetmachine_linuxbash.h"
#include "targetmachine_pbsgeneric.h"


#include <QStandardItemModel>
#include <QDebug>
#include <QFileDialog>

TargetMachinePage::TargetMachinePage(AddaRunConfiguration*addaconf, QWidget *parent) :
    QWidget(parent),
    addaconf(addaconf),
    ui(new Ui::TargetMachinePage)
{
    ui->setupUi(this);

    ui->comboBox_targetMachine->insertItem(0,"Archer Cray XC30");
    ui->comboBox_targetMachine->insertItem(1,"Linux Bash script");
    ui->comboBox_targetMachine->insertItem(2,"PBS generic");
    
    /*
    ui->comboBox_targetMachine->insertItem(3,"Windows Powershell script");
    */

    ui->comboBox_targetMachine->setCurrentIndex(0);


    ui->stackedWidget_targetmachines->insertWidget(0, new TargetMachine_Archer(addaconf, this));
    ui->stackedWidget_targetmachines->insertWidget(1, new TargetMachine_LinuxBash(addaconf, this));
    ui->stackedWidget_targetmachines->insertWidget(2, new TargetMachine_PBSgeneric(addaconf, this));

    loadSettings();

    // QStandardItemModel
}

TargetMachinePage::~TargetMachinePage()
{
    delete ui;
}

void TargetMachinePage::on_comboBox_targetmachine_addabinarypath_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    QString addabinary = ui->comboBox_targetmachine_addabinarypath->currentText();
    qDebug()<<"New binary " << addabinary;
    saveSettings();

    addaconf->setAddaExecutablePath(addabinary);
}

void TargetMachinePage::loadSettings()
{
    QSettings settings("ADDA", "AddaConfigure");
    QStringList addabinarypathsList = settings.value("AddaBinaryPath").toStringList();

    if (!addabinarypathsList.isEmpty())
    {
        ui->comboBox_targetmachine_addabinarypath->addItems(addabinarypathsList );
        /*
                for(int i = 0; i < budgetcodesList.count();i++)
                {
                    ui->comboBox_targetmachine_archer_budgetcodes->addItem(budgetcodesList.at(i));
                }
         */
    }
}

void TargetMachinePage::saveSettings()
{
    QSettings settings("ADDA", "AddaConfigure");
    QStringList addabinarypathsList;
    for(int index = 0; index < ui->comboBox_targetmachine_addabinarypath->count();index++)
        addabinarypathsList.append(ui->comboBox_targetmachine_addabinarypath->itemText(index));

    settings.setValue("AddaBinaryPath", addabinarypathsList);
}

void TargetMachinePage::on_pushButton_clicked(bool checked)
{
    Q_UNUSED(checked);

    QSettings settings("ADDA", "AddaConfigure");
    QString lastdir = settings.value("last_addabinary_path","").toString();

    QString wildcard;

#ifdef Q_OS_WIN
    wildcard = "*.exe";
#else
    wildcard = "*";
#endif


    QStringList files = QFileDialog::getOpenFileNames( this,
                                                       "Select binary",
                                                       lastdir,
                                                       wildcard);

    if( files.size() > 1)
    {
        qDebug() << "only one scatterer file support at present ";
    }

    if( !files.empty() )
    {
        // check if we already have it

        for(int i=0; i<ui->comboBox_targetmachine_addabinarypath->count(); i++)
        {
            if( files.at(0) == ui->comboBox_targetmachine_addabinarypath->itemText(i) )
            {
                ui->comboBox_targetmachine_addabinarypath->setCurrentIndex(i);
                qDebug() << "We have it already";
                return;
            }
        }

        ui->comboBox_targetmachine_addabinarypath->addItem(files.at(0));
    }
    else
    {
        return;
    }

    QFileInfo fi(files.at(0));
    settings.setValue("last_addabinary_path",fi.absoluteFilePath());

    qDebug() << "Is selected a directory " << fi.isDir();
}

void TargetMachinePage::on_comboBox_targetMachine_currentIndexChanged(int index)
{
    if(index==0) // Archer
    {
        addaconf->setScriptType(AddaRunConfiguration::ScriptType::CRAY_XC30);
    }
    else if(index==1) // linux bash
    {
        addaconf->setScriptType(AddaRunConfiguration::ScriptType::LINUX_BASH);
    }
    else if(index==1) // linux bash
    {
        addaconf->setScriptType(AddaRunConfiguration::ScriptType::PBS_GENERIC);
    }
    else
    {
        qDebug() << "Unkwon Targetmachine";
    }
}
