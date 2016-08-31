#-------------------------------------------------
#
# Project created by QtCreator 2010-07-14T17:54:25
#
#-------------------------------------------------
#if QT_VERSION >= 0x050000
QT       += core gui widgets
#else
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
#endif

TARGET = FlyControl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdialog.cpp \
        qextserial/qextserialport.cpp \
    Files/flycontrol.cpp \
    Files/showpid.cpp \
    Files/showsensordata.cpp \
    Files/showwaveform.cpp \
    qcustom/qcustomplot.cpp \
    Thread/threaddlg.cpp \
    Thread/workthread.cpp \
    Files/u30.cpp \
    Files/gyroscope.cpp \
    Files/mag.cpp \
    Files/baro.cpp \
    Files/serialset.cpp \
    Files/remotectrl.cpp \
    Files/filter.cpp \
    Files/ua18.cpp


HEADERS  += mainwindow.h \
    aboutdialog.h \
        qextserial/qextserialport_global.h \
        qextserial/qextserialport.h \
    Files/flycontrol.h \
    Files/showpid.h \
    Files/showsensordata.h \
    Files/showwaveform.h \
    qcustom/qcustomplot.h \
    Thread/threaddlg.h \
    Thread/workthread.h \
    Files/ConfigData.h \
    Files/u30.h \
    Files/gyroscope.h \
    Files/mag.h \
    Files/serialset.h \
    Files/baro.h \
    Files/serialset.h \
    Files/remotectrl.h \
    Files/filter.h \
    Files/ua18.h


win32 {
     SOURCES += qextserial/qextserialport_win.cpp
}

unix {
     SOURCES += qextserial/qextserialport_unix.cpp
}

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    Files/flycontrol.ui \
    Files/showpid.ui \
    Files/showsensordata.ui \
    Files/showwaveform.ui \
    Files/u30.ui \
    Files/gyroscope.ui \
    Files/mag.ui \
    Files/baro.ui \
    Files/serialset.ui \
    Files/remotectrl.ui \
    Files/filter.ui \
    Files/ua18.ui


RESOURCES += \
    images.qrc \
    logo.qrc
RC_FILE += myico.rc

OTHER_FILES += \
    myico.rc

DISTFILES += \
    ../Readme/Readme.txt
