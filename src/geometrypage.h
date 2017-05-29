#ifndef SCATTERERPAGE_H
#define SCATTERERPAGE_H

#include <QWidget>

#include <netcdf.h>

#include "addarunconfiguration.h"

namespace Ui {
class GeometryPage;
}

class GeometryPage : public QWidget
{
    Q_OBJECT

    AddaRunConfiguration*addaconf;

public:
    explicit GeometryPage(AddaRunConfiguration *addaconf, QWidget *parent = 0);
    ~GeometryPage();

private slots:

    void on_pushButton_open_geom_file_clicked();

    void on_lineEdit_geom_file_editingFinished();

signals:
    void errmsg(const QString&msg);
    void logmsg(const QString&msg);

private:
    Ui::GeometryPage *ui;
    void openGeomFile(const QString&filename);
    void updateFileInfo(QString filename);
};

#endif // SCATTERERPAGE_H
