#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include <QSettings>
#include <QStackedWidget>

#include "addarunconfiguration.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:

private slots:



    //   void about();
    void documentWasModified();

    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

    // File:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    bool on_actionSave_triggered();
    bool on_actionSave_As_triggered();

    // Help
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();

    void openRecentFile();

private:
    Ui::MainWindow *ui;

    QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;
    void populateMainMenuListView();

    QSettings*settings;
    void saveSettings();
    void restoreSettings();


    // project files mechanism
    QString currentFile;
    bool docWasModified;
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    bool maybeSave();

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];


    AddaRunConfiguration*addarunconfiguration;

    // Console

    QString strippedName(const QString &fullFileName);
    void saveData();
    void loadData(const QString&fileName);

private slots:
    void displayErrorMsg(const QString&msg);
    void displayLogMsg(const QString&msg);

};

#endif // MAINWINDOW_H
