#ifndef SHOWPID_H
#define SHOWPID_H
#include "qextserial/qextserialport.h"
//#include "mainwindow.h"

#include <QWidget>

#define u8 unsigned char

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

//int coord_X,coord_Y;

typedef struct {
    unsigned char P;
    unsigned char I;
    unsigned char D;
}MYPID;

#if 1
typedef struct {
    MYPID ROLL;
    MYPID PITCH;
    MYPID YAW;
    MYPID ALT;
    MYPID PID1;
    MYPID PID2;
}ReadPID;
#endif

namespace Ui {
class ShowPID;
}

class ShowPID : public QWidget
{
    Q_OBJECT
    
public:
    explicit ShowPID(QWidget *parent = 0);
    ~ShowPID();

    void Analyse_Rev_PID_Data(unsigned char *data_buf);
    void ShowFrontPID(void);
    void ShowBackPID(void);

    void Analyse_Send_PID_Data(void);
    void WriteFrontPID(void);
    void WriteBackPID(void);
    void PackPID(void);


    
private slots:
    void on_ReadPID_clicked();

    void on_WritePID_clicked();

private:
    Ui::ShowPID *ui;
    QextSerialPort *myCom;
//    MainWindow

//    MainWindow mainwindow;
};

#endif // SHOWPID_H
