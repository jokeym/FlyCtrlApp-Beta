#include <QtGlobal>
#if QT_VERSION >= 0x050000
#include <QApplication>
#else
#include <QtGui/QApplication>
#endif
#include "mainwindow.h"
#include <QTextCodec>
#include "Files/ConfigData.h"
#include "Thread/threaddlg.h"

extern Coordinate Coord;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
#endif
    MainWindow w;
//    ThreadDlg mythread;
    w.show();
//    mythread.show();


    //设置窗体到屏幕中央
    QDesktopWidget* pDw = QApplication::desktop();//获得桌面窗体
//    QRect rect = pDw->screenGeometry();//获得分辨率
//    w.move(rect.width()/4,rect.height()/4);
//    qDebug()<<"main rect.width:"<<rect.width();
//    qDebug()<<"main rect.height:"<<rect.height();




    return a.exec();
}
