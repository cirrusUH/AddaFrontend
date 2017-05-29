#include "targetmachine_pbsgeneric.h"
#include "ui_targetmachine_pbsgeneric.h"

TargetMachine_PBSgeneric::TargetMachine_PBSgeneric(AddaRunConfiguration*addaconf,QWidget *parent) :
    QWidget(parent),
    addaconf(addaconf),
    ui(new Ui::TargetMachine_PBSgeneric)
{
    ui->setupUi(this);
}

TargetMachine_PBSgeneric::~TargetMachine_PBSgeneric()
{
    delete ui;
}

void TargetMachine_PBSgeneric::on_radioButton_target_jobtype_arrayjob_toggled(bool checked)
{
    if(checked)
    {
        addaconf->setJobtype(AddaRunConfiguration::JobType::ARRAY_JOB);
    }
}

void TargetMachine_PBSgeneric::on_radioButton_target_jobtype_forloop_toggled(bool checked)
{
    if(checked)
    {
    addaconf->setJobtype(AddaRunConfiguration::JobType::FOR_LOOP);

    }
}
