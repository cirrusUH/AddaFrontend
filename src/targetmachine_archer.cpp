#include "targetmachine_archer.h"
#include "ui_targetmachine_archer.h"

#include <QDebug>

TargetMachine_Archer::TargetMachine_Archer(AddaRunConfiguration*addaconf, QWidget *parent) :
    QWidget(parent),
    addaconf(addaconf),
    ui(new Ui::TargetMachine_Archer)
{
    ui->setupUi(this);

    ui->lineEdit_targetmachine_percent->setValidator(new QIntValidator(this));

    loadSettings();

    repaint();
}

TargetMachine_Archer::~TargetMachine_Archer()
{

    delete ui;
}

void TargetMachine_Archer::loadSettings()
{
    QSettings settings("ADDA", "AddaConfigure");
    QStringList budgetcodesList = settings.value("targetmachine_archer_budgetcodes").toStringList();

    if (!budgetcodesList.isEmpty())
    {
        ui->comboBox_targetmachine_archer_budgetcodes->addItems(budgetcodesList );
        /*
                for(int i = 0; i < budgetcodesList.count();i++)
                {
                    ui->comboBox_targetmachine_archer_budgetcodes->addItem(budgetcodesList.at(i));
                }
                */
    }

}

void TargetMachine_Archer::saveSettings()
{
    QSettings settings("ADDA", "AddaConfigure");
    QStringList budgetcodesList;
    for(int index = 0; index < ui->comboBox_targetmachine_archer_budgetcodes->count();index++)
        budgetcodesList.append(ui->comboBox_targetmachine_archer_budgetcodes->itemText(index));

    settings.setValue("targetmachine_archer_budgetcodes", budgetcodesList);
}


void TargetMachine_Archer::on_comboBox_targetmachine_archer_budgetcodes_currentIndexChanged(const QString &arg1)
{
    qDebug()<< "New budgetcode " << arg1;
    addaconf->setArcherBudgetCode(arg1);
    saveSettings();
}

void TargetMachine_Archer::on_checkBox_targetmachine_archer_add_stuckjobmonitor_toggled(bool checked)
{
    addaconf->setStuckJobMonitorAdd(checked);
}



void TargetMachine_Archer::on_lineEdit_targetmachine_percent_editingFinished()
{
    QLineEdit*l=ui->lineEdit_targetmachine_percent;
    addaconf->setStuckJobMonitorPercent( l->text().toInt() );
}



void TargetMachine_Archer::on_comboBox_targetmachine_archer_queues_activated(const QString &arg1)
{
    if(arg1=="standard")
    {
        ui->lineEdit_targetmachine_archer_walltime->setText("24:00:00");
    }
    else if( arg1=="long")
    {
        ui->lineEdit_targetmachine_archer_walltime->setText("48:00:00");
    }
    else
    {
        qDebug() << "Unkown walltime for selected queue " << arg1;
    }

    addaconf->setQueueName(arg1);
}

void TargetMachine_Archer::on_comboBox_targetmachine_archer_budgetcodes_currentTextChanged(const QString &arg1)
{
    addaconf->setArcherBudgetCode(arg1);
}

void TargetMachine_Archer::on_comboBox_archer_jobtye_currentIndexChanged(const QString &arg1)
{
    addaconf->setJobtype(arg1);
}
