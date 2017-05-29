#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "helpabout.h"
#include "orientationpage.h"
#include "postprocessingpage.h"
#include "geometrypage.h"
#include "targetmachinepage.h"
#include "wavelengthpage.h"
#include "materialpage.h"
#include "jobscriptpage.h"
#include "addasettingspage.h"
#include "scatteredfieldpage.h"

#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settings( new QSettings("ADDA", "AddaConfigure") )
    , docWasModified(false)

{
    ui->setupUi(this);

    setWindowTitle(tr("Adda Run Configuration"));

    // delete ui->mainToolBar; // removes it
    removeToolBar(ui->mainToolBar);

    // ui->menuBar->hide(); //File, etc...


    addarunconfiguration = new AddaRunConfiguration();

    connect(addarunconfiguration, SIGNAL(errmsg(QString)), this, SLOT(displayErrorMsg(QString)) );
    connect(addarunconfiguration, SIGNAL(logmsg(QString)), this, SLOT(displayLogMsg(QString)) );

    restoreSettings();

    contentsWidget = ui->listWidget_contents;

    // WidgetContents->setViewMode(QListView::IconMode); later
    contentsWidget->setViewMode(QListView::ListMode);
    contentsWidget->setIconSize(QSize(96, 84));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(128);
    // WidgetContents->setSpacing(8);

    // addarunconf is already connected to the console

    pagesWidget = ui->stackedWidget_pages;
    // this order needs to match the order of entries in populateListView()
    GeometryPage*gp=new GeometryPage(addarunconfiguration, this);
    pagesWidget->addWidget(gp);
    connect(gp,SIGNAL(errmsg(QString)), this, SLOT(displayErrorMsg(QString)));
    connect(gp,SIGNAL(logmsg(QString)), this, SLOT(displayLogMsg(QString)));



    WaveLengthPage*wlp=new WaveLengthPage(addarunconfiguration, this);
    pagesWidget->addWidget(wlp);
    connect(wlp,SIGNAL(errmsg(QString)), this, SLOT(displayErrorMsg(QString)));
    connect(wlp,SIGNAL(logmsg(QString)), this, SLOT(displayLogMsg(QString)));

    MaterialPage*mp=new MaterialPage(addarunconfiguration, this);
    pagesWidget->addWidget(mp);
    connect(mp,SIGNAL(errmsg(QString)), this, SLOT(displayErrorMsg(QString)));
    connect(mp,SIGNAL(logmsg(QString)), this, SLOT(displayLogMsg(QString)));

    OrientationPage*op = new OrientationPage(addarunconfiguration, this);
    pagesWidget->addWidget(op);
    connect(op,SIGNAL(errmsg(QString)), this, SLOT(displayErrorMsg(QString)));
    connect(op,SIGNAL(logmsg(QString)), this, SLOT(displayLogMsg(QString)));

    ScatteredFieldPage*sfp = new ScatteredFieldPage(addarunconfiguration, this);
    pagesWidget->addWidget(sfp);
    connect(sfp,SIGNAL(errmsg(QString)), this, SLOT(displayErrorMsg(QString)));
    connect(sfp,SIGNAL(logmsg(QString)), this, SLOT(displayLogMsg(QString)));

    AddaSettingsPage*sp = new AddaSettingsPage(addarunconfiguration, this);
    pagesWidget->addWidget(sp);
    connect(sp,SIGNAL(errmsg(QString)), this, SLOT(displayErrorMsg(QString)));
    connect(sp,SIGNAL(logmsg(QString)), this, SLOT(displayLogMsg(QString)));

    PostProcessingPage*pp=new PostProcessingPage(addarunconfiguration, this);
    pagesWidget->addWidget(pp);
    connect(pp,SIGNAL(errmsg(QString)), this, SLOT(displayErrorMsg(QString)));
    connect(pp,SIGNAL(logmsg(QString)), this, SLOT(displayLogMsg(QString)));

    TargetMachinePage*tm=new TargetMachinePage(addarunconfiguration, this);
    pagesWidget->addWidget(tm);
    connect(tm,SIGNAL(errmsg(QString)), this, SLOT(displayErrorMsg(QString)));
    connect(tm,SIGNAL(logmsg(QString)), this, SLOT(displayLogMsg(QString)));

    JobScriptPage*jp=new JobScriptPage(addarunconfiguration, this);
    pagesWidget->addWidget(jp);
    connect(jp,SIGNAL(errmsg(QString)), this, SLOT(displayErrorMsg(QString)));
    connect(jp,SIGNAL(logmsg(QString)), this, SLOT(displayLogMsg(QString)));

    populateMainMenuListView();
    contentsWidget->setCurrentRow(0);

    displayLogMsg("Initialized...");

    // Recent Files
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),this, SLOT(openRecentFile()));
    }


    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        ui->menuRecent_Files->addAction(recentFileActs[i]);
    }
    updateRecentFileActions();

    repaint();

    // windows is different...
    QDir::setCurrent( QCoreApplication::applicationDirPath() );
    qDebug() << QCoreApplication::applicationDirPath();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::documentWasModified()
{
    docWasModified=true;
}

void MainWindow::on_actionAbout_triggered()
{
    HelpAbout HA;
    HA.exec();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    saveSettings();
}

void MainWindow::on_actionNew_triggered()
{
    if (maybeSave())
    {
        setCurrentFile("");
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
        {
            loadFile(fileName);
        }
    }
}

bool MainWindow::on_actionSave_triggered()
{
    if (currentFile.isEmpty())
    {
        return on_actionSave_As_triggered();
    }
    else
    {
        return saveFile(currentFile);
    }
}

bool MainWindow::on_actionSave_As_triggered()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
    {
        return false;
    }
    return saveFile(dialog.selectedFiles().first());
}


void MainWindow::populateMainMenuListView()
{
    //this must have the same order as the addWidget list in the ctor

    QListWidgetItem *geometryButton = new QListWidgetItem(contentsWidget);
    //scattererButton->setIcon(QIcon(":/images/query.png"));
    geometryButton->setText(tr("Scatterer Geometry"));
    //scattererButton->setTextAlignment(Qt::AlignHCenter);
    geometryButton->setTextAlignment(Qt::AlignLeft);
    geometryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);



    QListWidgetItem *materialButton = new QListWidgetItem(contentsWidget);
    //wavelengthButton->setIcon(QIcon(":/images/update.png"));
    materialButton->setText(tr("Wavelength"));
    //wavelengthButton->setTextAlignment(Qt::AlignHCenter);
    materialButton->setTextAlignment(Qt::AlignLeft);
    materialButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *wavelengthButton = new QListWidgetItem(contentsWidget);
    //wavelengthButton->setIcon(QIcon(":/images/update.png"));
    wavelengthButton->setText(tr("Material "));
    //wavelengthButton->setTextAlignment(Qt::AlignHCenter);
    wavelengthButton->setTextAlignment(Qt::AlignLeft);
    wavelengthButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *orientationButton = new QListWidgetItem(contentsWidget);
    //wavelengthButton->setIcon(QIcon(":/images/update.png"));
    orientationButton->setText(tr("Orientation"));
    //wavelengthButton->setTextAlignment(Qt::AlignHCenter);
    orientationButton->setTextAlignment(Qt::AlignLeft);
    orientationButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *scatteredfieldButton = new QListWidgetItem(contentsWidget);
    //wavelengthButton->setIcon(QIcon(":/images/update.png"));
    scatteredfieldButton->setText(tr("Scattered Field"));
    //wavelengthButton->setTextAlignment(Qt::AlignHCenter);
    scatteredfieldButton->setTextAlignment(Qt::AlignLeft);
    scatteredfieldButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


    QListWidgetItem *addasettings = new QListWidgetItem(contentsWidget);
    //wavelengthButton->setIcon(QIcon(":/images/update.png"));
    addasettings->setText(tr("Settings"));
    //wavelengthButton->setTextAlignment(Qt::AlignHCenter);
    addasettings->setTextAlignment(Qt::AlignLeft);
    addasettings->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *postprocessingButton = new QListWidgetItem(contentsWidget);
    //updateButton->setIcon(QIcon(":/images/update.png"));
    postprocessingButton->setText(tr("Post processing"));
    //updateButton->setTextAlignment(Qt::AlignHCenter);
    postprocessingButton->setTextAlignment(Qt::AlignLeft);
    postprocessingButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


    QListWidgetItem *updateTargetmachineButton = new QListWidgetItem(contentsWidget);
    //updateButton->setIcon(QIcon(":/images/update.png"));
    updateTargetmachineButton->setText(tr("Target Machine"));
    //updateButton->setTextAlignment(Qt::AlignHCenter);
    updateTargetmachineButton->setTextAlignment(Qt::AlignLeft);
    updateTargetmachineButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *updateJobscriptButton = new QListWidgetItem(contentsWidget);
    //updateButton->setIcon(QIcon(":/images/update.png"));
    updateJobscriptButton->setText(tr("Jobscript"));
    //updateButton->setTextAlignment(Qt::AlignHCenter);
    updateJobscriptButton->setTextAlignment(Qt::AlignLeft);
    updateJobscriptButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void MainWindow::saveSettings()
{
    qDebug()<<"void MainWindow::saveSettings()";
    //  QSettings settings("ADDA", "AddaConfigure");
    settings->setValue("mainWindowGeometry", saveGeometry());
    settings->setValue("mainWindowState", saveState());

    settings->setValue("splitterSizes_horizontal", ui->splitter_horizontal->saveState());
}

void MainWindow::restoreSettings()
{
    qDebug()<<"void MainWindow::restoreSettings()";

    // QSettings settings("ADDA", "AddaConfigure");
    restoreGeometry(settings->value("mainWindowGeometry").toByteArray());
    restoreState(settings->value("mainWindowState").toByteArray());

    ui->splitter_horizontal->restoreState(settings->value("splitterSizes_horizontal").toByteArray());

}


void MainWindow::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));

}

void MainWindow::displayLogMsg(const QString&msg)
{
    ui->textBrowser_console->append(msg);
    ui->textBrowser_console->repaint();
    qApp->processEvents();

}

void MainWindow::displayErrorMsg(const QString&msg)
{
    ui->textBrowser_console->append("<font color=\"red\">" + msg + "</font>" );
    ui->textBrowser_console->repaint();
    qApp->processEvents();
}

bool MainWindow::maybeSave()
{


    if (docWasModified)
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save)
        {
            return on_actionSave_triggered();
        }
        else if (ret == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    loadData(fileName);
    /*

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

*/
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}



bool MainWindow::saveFile(const QString &fileName)
{
    /*
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }
    */
    /*
    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

*/


    setCurrentFile(fileName);

    saveData();

    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}


void MainWindow::setCurrentFile(const QString &fileName)
{
    currentFile = fileName;
    setWindowFilePath(currentFile);

    //QSettings settings;
    QStringList files = settings->value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
    {
        files.removeLast();
    }
    settings->setValue("recentFileList", files);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::updateRecentFileActions()
{
    //   QSettings settings;
    QStringList files = settings->value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    //separatorAct->setVisible(numRecentFiles > 0);
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        loadFile(action->data().toString());
    }
}


void MainWindow::saveData()
{

    foreach( QObject*obj, children() )
    {
        qDebug() <<qobject_cast<QPushButton*>(obj);
    }

    //QList<QPushButton*> allPButtons = findChildren<QPushButton*>();

    QMap<QString,int> radiobuttonsettings;
    foreach(QRadioButton*i, findChildren<QRadioButton*>())
    {
        qDebug() << "RadioButtons " << i->accessibleName() << i->objectName() << ": "<< i->isChecked();
        radiobuttonsettings.insert(i->objectName(),  i->isChecked());
    }

    QMap<QString,int> checkboxsettings;
    foreach(QCheckBox*i, findChildren<QCheckBox*>())
    {
        qDebug() << i->accessibleName() << i->objectName() << ": "<< i->isChecked();
        checkboxsettings.insert(i->objectName(),  i->isChecked());
    }

    QMap<QString,QString> comboboxsettings;
    foreach(QComboBox*i, findChildren<QComboBox*>())
    {
        qDebug() << i->accessibleName() << i->objectName() << ": "<< i->currentText();
        comboboxsettings.insert(i->objectName(),  i->currentText());
    }
    qDebug() << "Saving Lineedits";
    QMap<QString,QString> lineeditsettings;
    foreach(QLineEdit*i, findChildren<QLineEdit*>())
    {
        qDebug() << i->accessibleName() << i->objectName() << ": "<< i->text();
        if(lineeditsettings.contains(i->objectName()) )
        {
            qDebug()<<"Widget with same name already saved. This is calling for trouble";
        }
        lineeditsettings.insert(i->objectName(),  i->text());
    }
    qDebug() << "Saving done";

    QMap<QString,QList<QString> > listwidgetsettings;
    foreach(QListWidget*i, findChildren<QListWidget*>())
    {

        QModelIndexList list = i->selectionModel()->selectedIndexes();

        QList<QString> slist;
        foreach(const QModelIndex &index, list){
            slist.append( index.data(Qt::DisplayRole ).toString());
        }
        qDebug() << slist;



        listwidgetsettings.insert(i->objectName(),  slist);
    }


    qDebug() << comboboxsettings << checkboxsettings << findChildren<QComboBox*>();



    /*
     * map.insert("red", Qt::red);
       map.insert("green", Qt::green);
    map.insert("blue", Qt::blue);
*/

    QFile file(currentFile);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug()<< "Cannot open file for writing: "
                << qPrintable(file.errorString());
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_3);

    // Write a header with a "magic number" and a version
    out << (quint32)0xA0B0C0D0;
    out << (qint32)123;

    out.setVersion(QDataStream::Qt_4_0);

    out << quint32(0x12345678);
    //<< image << map
    out  << checkboxsettings
         << comboboxsettings
         << lineeditsettings
         <<checkboxsettings
        << listwidgetsettings;

}

void MainWindow::loadData( const QString&fileName )
{

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file for reading: "
                 << qPrintable(file.errorString());
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_3);

    QMap<QString,int> radiobuttonsettings;
    QMap<QString,QString> comboboxsettings;
    QMap<QString,QString> lineeditsettings;
    QMap<QString,int> checkboxsettings;
    QMap<QString,QList<QString>> listwidgetsettings;
    // Read and check the header
    quint32 magic;
    in >> magic;
    if (magic != 0xA0B0C0D0)
    {
        //return XXX_BAD_FILE_FORMAT;
        qDebug() << "XXX_BAD_FILE_FORMAT";
    }
    // Read the version
    qint32 version,n;
    in >> version;
    if (version < 100)
    {
        // return XXX_BAD_FILE_TOO_OLD;
        qDebug() << "XXX_BAD_FILE_TOO_OLD";
    }
    if (version > 123)
    {
        //return XXX_BAD_FILE_TOO_NEW;
        qDebug() << "XXX_BAD_FILE_TOO_NEW";
    }
    /*
    if (version <= 110)
        in.setVersion(QDataStream::Qt_3_2);
    else
        in.setVersion(QDataStream::Qt_4_0);
    */

    in >> n >> radiobuttonsettings
       >> comboboxsettings >> lineeditsettings
       >>checkboxsettings >>listwidgetsettings;

    //qDebug() << image << map;
    qDebug() << n << radiobuttonsettings;


    for( auto objname: radiobuttonsettings.keys() )
    {
        int checked = radiobuttonsettings[objname] ;
        qDebug() <<"loading radiobutton " << objname << "=" <<  checked;
        QRadioButton*rb = ui->centralWidget->findChild<QRadioButton*>( objname  );
        if( rb )
        {
            rb->setChecked( radiobuttonsettings[objname] );
        }
    }

    for( auto objname: checkboxsettings.keys() )
    {
        int checked = checkboxsettings[objname] ;
        qDebug() << objname << "=" <<  checked;
        QCheckBox*rb = ui->centralWidget->findChild<QCheckBox*>( objname  );
        if( rb )
        {
            rb->setChecked( checkboxsettings[objname] );
        }
    }

    for( auto objname: comboboxsettings.keys() )
    {
        QString currentText = comboboxsettings[objname];
        qDebug() << objname << "=" <<  currentText;
        QComboBox*rb = ui->centralWidget->findChild<QComboBox*>( objname  );
        if( rb )
        {
            rb->setEditText( currentText );
            //rb->setCurrentText( currentText );
        }
        else
        {
            qDebug() << "QObject " << objname << " not found, cannot load this setting.";
        }
    }


    for( auto objname: lineeditsettings.keys() )
    {
        QString currentIndex = lineeditsettings[objname];
        qDebug() << objname << "=" <<  currentIndex;
        QLineEdit*rb = ui->centralWidget->findChild<QLineEdit*>( objname  );
        if( rb )
        {
            rb->setText( lineeditsettings[objname] );
            // hm this seems to be proteced: in qt4
            //emit rb->editingFinished();
        }
        else
        {
            qDebug() << "QObject " << objname << " not found, cannot load this setting.";
        }
    }



    qDebug() << "Loading";
    for( auto objname: listwidgetsettings.keys() )
    {
        QList<QString> loadedSelectedEntries = listwidgetsettings[objname];
        qDebug() << objname << "=" << loadedSelectedEntries;
        QListWidget*rb = ui->centralWidget->findChild<QListWidget*>( objname  );
        if( rb )
        {

            /*
                // check if the entries match

                // list of the items int the view
                QModelIndexList list = rb->selectionModel()->selectedIndexes();
                  QList<QString> slist;
                  foreach(const QModelIndex &index, list){
                      slist.append( index.data(Qt::DisplayRole ).toString());
                  }



                for(int i=0; i< rb->count(); i++)
                {
                        QString itemtext=rb->item(i)->text();
                        qDebug()<<"["<<i<<"]" << rb->item(i)->text();
                        slist.append( itemtext);
                }

                  qDebug() << "Items in the view: "<<slist;
                  qDebug() << "Items loaded:"<<currentSelectedEntries;
*/

            /* this need decoupling from the name of the listWidget

                // unselect the whole list
                QList<QListWidgetItem *>  allItemsList = rb->findItems("*",Qt::MatchWildcard);
                foreach( QListWidgetItem * item, allItemsList)
                {
                    item->setSelected(false);
                }

                // the loaded items
                for(int i=0; i<loadedSelectedEntries.count(); i++)
                {
                    QString s = loadedSelectedEntries.at(i);
                    qDebug() << s;

                    bool founditem=false;
                    for(int j=0; j<rb->count(); j++)
                    {
                        QString itemtext=rb->item(j)->text();
                        if( s == itemtext )
                        {
                            if( i == j)
                            {
                                qDebug() << "have item, and is corret position, do nothing";
                                rb->item(j)->setSelected(true);
                            }
                            else
                            {
                                qDebug() << "have item, but wrong position, so move";
                                QListWidgetItem*tmpItem = ui->listWidget->takeItem(j);
                                ui->listWidget->insertItem(i, tmpItem);
                                rb->item(i)->setSelected(true);
                            }
                        }
                    }


                    if(founditem==false)
                    {
                        qDebug() << "ListView did not have the item.";
                    }

                    */



        }
        else
        {
            qDebug() << "QObject " << objname << " not found, cannot load this setting.";
        }
    }


    setCurrentFile(fileName);

}
