#include "postprocessingpage.h"
#include "ui_postprocessingpage.h"

PostProcessingPage::PostProcessingPage(AddaRunConfiguration*addaconf,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PostProcessingPage),
    addaconf(addaconf)
{
    ui->setupUi(this);
}

PostProcessingPage::~PostProcessingPage()
{
    delete ui;
}
