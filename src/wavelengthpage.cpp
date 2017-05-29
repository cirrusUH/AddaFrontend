#include "wavelengthpage.h"
#include "ui_wavelengthpage.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTimer>

WaveLengthPage::WaveLengthPage(AddaRunConfiguration*addaconf,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WaveLengthPage)
    , delayedvalidationtimer( new QTimer(this) )
    , addaconf(addaconf)
{
    ui->setupUi(this);

    QLineEdit*ls = ui->lineEdit_lambda_seq;
        ls->setValidator(new QRegExpValidator( QRegExp("^[\\-0-9,:. ]+$") , this));

    // this is only emitted in case the validator is passed
    connect(ls,SIGNAL(editingFinished()), this, SLOT(validateInput()));

    ls->setStyleSheet("QLineEdit { background: rgb(246,244,226); }");

    delayedvalidationtimer->setSingleShot(true);

    connect(ls,SIGNAL(textChanged(QString)), this, SLOT(restartvalidationtimer()));
    connect(delayedvalidationtimer,SIGNAL(timeout()), this, SLOT(validateInput()));

    populateSinglePresets();
    populateMultiplePreset();
}

WaveLengthPage::~WaveLengthPage()
{
    delete ui;
}

void WaveLengthPage::populateMultiplePreset()
{
    QDir dir( QApplication::applicationDirPath() + "/Wavelengths/" );
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();

    foreach(QFileInfo fileInfo, list)
    {
        if( fileInfo.fileName().contains("singlewavelengthslist.csv"))
        {
            continue;
        }

        ui->comboBox_multiplewavelenghtslist->addItem( fileInfo.fileName() );
    }
}

void WaveLengthPage::populateSinglePresets()
{
    QString fileName = "Wavelengths/singlewavelengthslist.csv";
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));

    }

    QTextStream in(&file);

    QString buf = in.readAll();
    QStringList listbuf = buf.split('\n');

    foreach(QString s, listbuf)
    {
        //qDebug() << s;

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
        if(items.count()<2)
        {
            continue;
        }
        //     qDebug() << items.at(1) << "("<<items.at(0)<<")";

        ui->comboBox_singlewavelenghtslist->addItem(items.at(0) + " nm ("+items.at(1)+")");
        lambdasfromfile.append(items.at(0).toDouble() );
    }
}

void WaveLengthPage::validateInput()
{
    QString expression = ui->lineEdit_lambda_seq->text();
    //  qDebug()<< "void WaveLengthPage::validateInput(const QString &s) const =" << expression;

    NumberSequenceToDoubleList sequencelist( expression.toStdString() );

    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Text,Qt::black);
    ui->lineEdit_lambda_seq->setPalette(*palette);

    if( sequencelist.error() )
    {
        palette->setColor(QPalette::Text,Qt::red);
        ui->lineEdit_lambda_seq->setPalette(*palette);
        ui->lineEdit_lambda_seq->setFocus();

        //       qDebug() << "Error in expression: " << QString::fromStdString( sequencelist.getErrorString() );
    }

    int N =  sequencelist.size();

    //    QVector<double> list;
    //    list =  QVector<double>::fromStdVector( sequencelist.getStdVector() );

    QVector<double> list =  QVector<double>::fromStdVector( sequencelist.getStdVector() );
    addaconf->setLambdas( list );

    /*
      not in qt4
     QVariant msg;
     QList<double> l  = list.toList();
     msg.setValue( l );

     QStringList sl = msg.toStringList();

     emit logmsg( sl.join(",") );
    */

    QString msg = "lambda =[ ";
    foreach(double l, list)
    {
        msg += QString::number(l) +", ";
    }
    msg += " ]";

    emit logmsg( msg );

    ui->label_wavelength_no_lambdas->setText( "(" + QString::number(N) + " value(s)" + ")" );
}

void WaveLengthPage::restartvalidationtimer() const
{
    delayedvalidationtimer->start(1000);
}



void WaveLengthPage::on_comboBox_singlewavelenghtslist_currentIndexChanged(int index)
{

    if( !lambdasfromfile.empty() )
    {
        addaconf->setLambda( lambdasfromfile.at(index) );
        //       qDebug() << "set lambda to " <<lambdasfromfile.at(index);
    }
}


void WaveLengthPage::on_radioButton_wavelength_single_clicked(bool checked)
{
    if(checked)
    {


        if( !lambdasfromfile.empty() )
        {
            int i = ui->comboBox_singlewavelenghtslist->currentIndex() ;
            addaconf->setLambda( lambdasfromfile.at(i)  );
            //         qDebug() << "set wavelength single" << lambdasfromfile.at(i) ;
        }
    }
}

void WaveLengthPage::on_radioButton_wavelength_lambdaeq_clicked(bool checked)
{
    ui->lineEdit_lambda_seq->clear();
}
