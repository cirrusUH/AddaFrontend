#include "addasettingspage.h"
#include "ui_addasettingspage.h"

#include <QDebug>
#include <QTimer>

AddaSettingsPage::AddaSettingsPage(AddaRunConfiguration *addaconf, QWidget *parent) :
    QWidget(parent),
    addaconf(addaconf),
    delayedvalidationtimer( new QTimer(this) ),
    ui(new Ui::AddaSettingsPage)
{
    ui->setupUi(this);

    // \\ is translated into \ by the compiler!
    ui->lineEdit_settings_eps_seq->setValidator(new QRegExpValidator( QRegExp("^[\\-0-9,:. ]+$") , this));



    ui->lineEdit_settings_maxiter->setValidator(new QIntValidator(this));
    ui->lineEdit_settings_lambda_user->setValidator(new QDoubleValidator(this));

    // this is only emitted in case the validator is passed
    connect(ui->lineEdit_settings_eps_seq,SIGNAL(editingFinished()), this, SLOT(validateInput()));

    delayedvalidationtimer->setSingleShot(true);
    //connect(ui->lineEdit_lambda_seq,SIGNAL( cursorPositionChanged(int,int) ), this, SLOT(validateInputDelayed()));
    connect(ui->lineEdit_settings_eps_seq,SIGNAL(textChanged(QString)), this, SLOT(restartvalidationtimer()));
    connect(delayedvalidationtimer,SIGNAL(timeout()), this, SLOT(validateInput()));

    //ui->lineEdit_settings_eps_seq->setText("5.0");
    //ui->lineEdit_settings_maxiter->setText("50000");


    //  QPalette palette = ui->lineEdit_settings_eps_seq->palette();
     //     palette.setColor(ui->lineEdit_settings_eps_seq->backgroundRole(), QColor(246,244,226));
     //   palette.setColor(QPalette::Base, QColor(246,244,226));

        ui->lineEdit_settings_eps_seq->setStyleSheet("QLineEdit { background: rgb(246,244,226); }");

       // ui->lineEdit_settings_eps_seq->setPalette(palette);

}

AddaSettingsPage::~AddaSettingsPage()
{
    delete ui;
}

void AddaSettingsPage::on_comboBox_settings_solver_currentIndexChanged(const QString &arg1)
{
    addaconf->setIterativeSolver( arg1 );
}

void AddaSettingsPage::on_lineEdit_settings_additionalparam_textChanged(const QString &arg1)
{
    addaconf->setAdditionalParameters(arg1);
}

void AddaSettingsPage::validateInput()
{
    QLineEdit*le = ui->lineEdit_settings_eps_seq;
    QString expression = le->text();
    qDebug()<< "void AddaSettingsPage::validateInput(const QString &s) const =" << expression;

    NumberSequenceToDoubleList thelist( expression.toStdString() );

    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Text,Qt::black);
    le->setPalette(*palette);

    if( thelist.error() )
    {
        palette->setColor(QPalette::Text,Qt::red);
        le->setPalette(*palette);
        le->setFocus();

        qDebug() << "Error in expression: " << QString::fromStdString( thelist.getErrorString() );
        QString str = "Error in expression: " + QString::fromStdString( thelist.getErrorString() );
        // emit errmsg( "hug");
    }

    std::vector<double> seq = thelist.getStdVector();

    int N =  seq.size();

    QVector<double> list;
    list =  QVector<double>::fromStdVector(thelist.getStdVector());

    addaconf->setEpsList(list);

    QString str = "eps= [ ";
    for(auto &i: seq )
    {
        str+= QString::number( i) +" ";
    }
    str+= " ]";

    //  emit logmsg(str);

    ui->label_settings_no_eps->setText( "(" + QString::number(N) + " value(s)" + ")" ); // (0) value(s)
}

void AddaSettingsPage::restartvalidationtimer() const
{
    delayedvalidationtimer->start(1000);
}

void AddaSettingsPage::on_lineEdit_settings_maxiter_editingFinished()
{
    addaconf->setMaxIter( ui->lineEdit_settings_maxiter->text().toInt());
}


void AddaSettingsPage::on_lineEdit_settings_maxiter_textChanged(const QString &arg1)
{
    addaconf->setMaxIter( arg1.toInt() );
}

void AddaSettingsPage::on_radioButton_dpl_derivefromfile_clicked(bool checked)
{
    if(checked)
    {
        double gridsize = addaconf->getFirstGeometryGridSizeHint();

        if( gridsize>0 )
        {
            ui->label_settings_dpl_from_file->setText(QString::number( gridsize ) );
            if( addaconf->setDplComputeFromGeomFile() <0)
            {
                ui->radioButton_settings_dpl_override->setChecked(true);
            }
        }
        else
        {
            emit errmsg("Selected geometry file does not have a attribute gridsize_nm, add using:\n" \
                        "   ncatted -a gridsize_nm,geom,m,d,\"53.2\" file.nc");

            ui->radioButton_settings_dpl_override->setChecked(true);
        }

    }
}

void AddaSettingsPage::on_lineEdit_settings_lambda_user_textChanged(const QString &arg1)
{
    addaconf->setDplUser( arg1.toDouble() );
}

void AddaSettingsPage::on_lineEdit_settings_lambda_user_editingFinished()
{
    if( ui->radioButton_settings_dpl_override->isChecked())
    {
        addaconf->setDplUser( ui->lineEdit_settings_lambda_user->text().toDouble());
    }
}
