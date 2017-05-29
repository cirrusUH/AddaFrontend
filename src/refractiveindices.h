#ifndef REFRACTIVEINDICES_H
#define REFRACTIVEINDICES_H

#include <QObject>
#include <QStringList>
#include <complex>


class Warren2008 : public QObject
{

    std::vector<double> lambda;
    std::vector<double> n_re;
    std::vector<double> n_im;
public:
    Warren2008();
    ~Warren2008();

    std::complex<double> getRefractiveIndex(double lambda_nm,double Temperature_C=0);
signals:
    void errmsg(const QString&msg);
    void logmsg(const QString&msg);
};

class RefractiveIndices
{


    Warren2008 warren2008;

public:
    enum class Schemes {WATER_ICE_WARREN_2008};
    RefractiveIndices();
    ~RefractiveIndices();

    QStringList getAvailableSchemes();

    std::complex<double> getRefractiveIndex( Schemes scheme, double lambda, double Temperature);
signals:
    void errmsg(const QString&msg);
    void logmsg(const QString&msg);
};

#endif // REFRACTIVEINDICES_H
