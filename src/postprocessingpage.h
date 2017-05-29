#ifndef POSTPROCESSINGPAGE_H
#define POSTPROCESSINGPAGE_H

#include <QWidget>

#include "addarunconfiguration.h"

namespace Ui {
class PostProcessingPage;
}

class PostProcessingPage : public QWidget
{
    Q_OBJECT

public:
    explicit PostProcessingPage(AddaRunConfiguration*addaconf,QWidget *parent = 0);
    ~PostProcessingPage();

signals:
    void errmsg(const QString&msg);
    void logmsg(const QString&msg);

private:
    Ui::PostProcessingPage *ui;
    AddaRunConfiguration*addaconf;
};

#endif // POSTPROCESSINGPAGE_H
