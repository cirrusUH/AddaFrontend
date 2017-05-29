#ifndef OUTPUTPAGE_H
#define OUTPUTPAGE_H

#include <QWidget>

namespace Ui {
class OutputPage;
}

class OutputPage : public QWidget
{
    Q_OBJECT

public:
    explicit OutputPage(QWidget *parent = 0);
    ~OutputPage();

private:
    Ui::OutputPage *ui;
};

#endif // OUTPUTPAGE_H
