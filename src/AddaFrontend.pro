#-------------------------------------------------
#
# Project created by QtCreator 2017-01-18T10:51:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AddaFrontend
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    targetmachinepage.cpp \
    wavelengthpage.cpp \
    orientationpage.cpp \
    postprocessingpage.cpp \
    outputpage.cpp \
    helpabout.cpp \
    addageometry.cpp \
    addarunconfiguration.cpp \
    geometrypage.cpp \
    targetmachine_linuxbash.cpp \
    numbersequencetodoublelist.cpp \
    materialpage.cpp \
    refractiveindices.cpp \
    targetmachine_archer.cpp \
    jobscriptpage.cpp \
    addasettingspage.cpp \
    scatteredfieldpage.cpp \
    targetmachine_pbsgeneric.cpp

HEADERS  += mainwindow.h \
    targetmachinepage.h \
    wavelengthpage.h \
    orientationpage.h \
    postprocessingpage.h \
    outputpage.h \
    helpabout.h \
    addageometry.h \
    addarunconfiguration.h \
    geometrypage.h \
    targetmachine_linuxbash.h \
    numbersequencetodoublelist.h \
    materialpage.h \
    refractiveindices.h \
    targetmachine_archer.h \
    jobscriptpage.h \
    addasettingspage.h \
    scatteredfieldpage.h \
    targetmachine_pbsgeneric.h

FORMS    += mainwindow.ui \
    targetmachinepage.ui \
    wavelengthpage.ui \
    orientationpage.ui \
    postprocessingpage.ui \
    outputpage.ui \
    helpabout.ui \
    geometrypage.ui \
    targetmachine_linuxbash.ui \
    materialpage.ui \
    targetmachine_archer.ui \
    jobscriptpage.ui \
    addasettingspage.ui \
    scatteredfieldpage.ui \
    targetmachine_pbsgeneric.ui

RESOURCES += \
    Resources.qrc

QMAKE_CXXFLAGS += -std=c++1y

INCLUDEPATH += /opt/cray/netcdf/4.3.3.1/gnu/5.1/include "C:\netcdf 4.4.1.1\include"

# gcc <= 4.9.0
# LIBS += -L"c:\netcdf 4.4.1.1\lib" -L"/home/n02/n02/gr09aag/netcdf-4.4.1.1/lib" -lnetcdf

LIBS += -L/opt/cray/netcdf/4.3.3.1/gnu/5.1/lib -lnetcdf

# gcc > 4.9.0
#LIBS += -L"/home/n02/n02/gr09aag/netcdf-4.4.1.1/lib" -lnetcdf

# windows
# get boost.zip
# run bootstrap.bat from qt mingw commandline
# follow instructions:
# .\b2
#

#win32:LIBS += -L"c:\netcdf 4.4.1.1\lib" -lnetcdf

OTHER_FILES += \
    qthints.txt
