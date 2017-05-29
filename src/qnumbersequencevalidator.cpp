#include "qnumbersequencevalidator.h"

QNumberSequenceValidator::QNumberSequenceValidator(QObject *parent)
{

}

QValidator::State QNumberSequenceValidator::validate(QString &input, int &pos) const
{

    NumberSequenceToDoubleList thelist( input.toStdString() );


    if( thelist.error != true)
    {
        return Acceptable ;
    }

    return Invalid;
}

