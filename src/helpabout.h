#ifndef HELPABOUT_H
#define HELPABOUT_H

#include <QDialog>
#include "ui_helpabout.h"

class HelpAbout : public QDialog, private Ui::HelpAbout
{
    Q_OBJECT
public:
    explicit HelpAbout(QDialog *parent = 0);

private:

signals:

public slots:

};

#endif // HELPABOUT_H
