#include "outputpage.h"
#include "ui_outputpage.h"

OutputPage::OutputPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputPage)
{
    ui->setupUi(this);
}

OutputPage::~OutputPage()
{
    delete ui;
}
