#include "refractiveindices.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTranslator>
#include <QDebug>

Warren2008::Warren2008()
{
    QString fileName = "Materials/Warren_IOP_2008_ASCIItable.dat";
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {

        QMessageBox::warning(0, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));

    }

    QTextStream in(&file);


    QString buf = in.readAll();
    QStringList listbuf = buf.split('\n');

    foreach(QString s, listbuf)
    {
        qDebug() << s;

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


        // qDebug() << items.at(0) << ", "<<items.at(1)<< ", "<< items.at(2)<< ")";

        lambda.push_back(items.at(0).toDouble());
        n_re.push_back(items.at(1).toDouble() );
        n_im.push_back(items.at(2).toDouble() );
    }

}

Warren2008::~Warren2008()
{}

std::complex<double> Warren2008::getRefractiveIndex(double lambda_nm, double Temperature_C)
{

    const double lambda_um = lambda_nm/1000.0;

    if( lambda_um<lambda.front() || lambda_um > lambda.back() )
    {
        qDebug() << "Warren 2008, lambda out of range.";
        return std::complex<double>(0,0);
    }

    /*
     * TODO
     * 'For intermediate wavelengths not given in the table one should
     * interpolate m_re linearly in log(wavelength); log(m_im) linearly in log(wavelength).
     */

    // list is sorted in lambda, skip to correct entry
    size_t i=0;
    for(i=0; i<(lambda.size()-1); i++)
    {
        //qDebug() << "i="<<i<<" lambda[i] = "<< lambda.at(i) << "lambda_um" << lambda_um;
        if( (lambda.at(i)<=lambda_um) && (lambda.at(i+1)>lambda_um) )
        {
            break;
        }

    }

    double l = lambda_um-lambda.at(i);

    double n_re_lininter;
    double n_im_lininter;

    if(l>0)
    {

        double x = log(lambda_um) - log(lambda.at(i));

        double dll   = log(lambda.at(i+1)) - log(lambda.at(i));
        double dn_re = n_re.at(i+1) - n_re.at(i);

        double k_re = dn_re/(dll);


        n_re_lininter = n_re.at(i) + k_re*x;


        double dn_im = log(n_im.at(i+1)) - log(n_im.at(i));

        double k_im = dn_im/(dll);

        n_im_lininter = exp( log(n_im.at(i)) + k_im*x );

    }
    else if(l<0)
    {
        qDebug() << "This should not happen, unless the file has wrong entries";
        /*   qDebug() << "index i = " << i << "lambda.at(i)=" << lambda.at(i)
                  << "lambda.at(i+1)=" << lambda.at(i+1)
                  <<"lambda_um=" << lambda_um
                  << "l="<<l
                  << "dn_re=" <<  n_re.at(i+1) - n_re.at(i);
                  */
    }
    else // is 0
    {
        n_re_lininter = n_re.at(i);
        n_im_lininter = n_im.at(i);
    }

    std::complex<double> n(n_re_lininter,n_im_lininter );

    return n;
}

RefractiveIndices::RefractiveIndices()
{}

RefractiveIndices::~RefractiveIndices()
{}

QStringList RefractiveIndices::getAvailableSchemes()
{
    QStringList a;
    a.append("Water_Ice_Warren2008");

    return a;
}

std::complex<double> RefractiveIndices::getRefractiveIndex(Schemes scheme, double lambda_nm, double Temperature_C)
{
    if( scheme == Schemes::WATER_ICE_WARREN_2008)
    {
        return warren2008.getRefractiveIndex(lambda_nm, Temperature_C);
    }
    else
    {
        qDebug() << "Unkown scheme in RefractiveIndices::getRefractiveIndex";
    }

    return std::complex<double>(0,0);
}


