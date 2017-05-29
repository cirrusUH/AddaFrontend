#ifndef QNUMBERSEQUENCEVALIDATOR_H
#define QNUMBERSEQUENCEVALIDATOR_H

#include <QObject>
#include <QValidator>

#include "numbersequencetodoublelist.h"


class QNumberSequenceValidator : public QValidator
{
    Q_OBJECT

public:
    QNumberSequenceValidator(QObject *parent = 0);
    virtual QValidator::State validate ( QString & input, int & pos ) const;
signals:
    QString errorsignal();
};

#endif // QNUMBERSEQUENCEVALIDATOR_H
