#ifndef SERIALSET_H
#define SERIALSET_H

#include <QWidget>

#include <QMenuBar>
#include <QToolBar>
#include <QtCore>
#include <QMessageBox>
#include <QFile>
#include <QLCDNumber>
#include <QInputDialog>
#include <QFileDialog>
#include <QTextStream>
#include "qextserial/qextserialport.h"
#include "aboutdialog.h"
#include "Files/u30.h"
//#include "Files/ConfigData.h"

//延时，TIME_OUT是串口读写的延时
#define TIME_OUT 10

//连续发送定时器计时间隔,200ms
#define OBO_TIMER_INTERVAL 200

//载入文件时，如果文件大小超过TIP_FILE_SIZE字节则提示文件过大是否继续打开
#define TIP_FILE_SIZE 5000
//载入文件最大长度限制在MAX_FILE_SIZE字节内
#define MAX_FILE_SIZE 10000

namespace Ui {
class SerialSet;
}

class SerialSet : public QWidget
{
    Q_OBJECT

public:
    explicit SerialSet(QWidget *parent = 0);
    ~SerialSet();

     void startInit();

     void CmdTable();

     void SendData();



protected:
    void setComboBoxEnabled(bool status);

private:
    Ui::SerialSet *ui;

    QextSerialPort *myCom;
    QTimer *obotimer;
    unsigned int timerdly;
    unsigned int obotimerdly;
    QString write2fileName;    //写读取的串口数据到该文件
    U30 u30;

    QString Cmd_Str;

private slots:

    void on_delayspinBox_valueChanged(int );
    void on_obocheckBox_clicked();
    void on_clearUpBtn_clicked();
    void on_sendmsgBtn_clicked();
    void readMyCom();
    void sendMsg();
    //end by


    void on_OpenBtn_clicked();
    void on_CloseBtn_clicked();
    void on_AddBtn_clicked();
    void on_ReadFilesBtn_clicked();
    void on_SaveDataBtn_clicked();
    void on_CountClearBtn_clicked();
    void on_ClearDataBtn_clicked();
};

#endif // SERIALSET_H
