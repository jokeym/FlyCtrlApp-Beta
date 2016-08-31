#ifndef REMOTECTRL_H
#define REMOTECTRL_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class RemoteCtrl;
}

class RemoteCtrl : public QWidget
{
    Q_OBJECT

public:
    explicit RemoteCtrl(QWidget *parent = 0);
    ~RemoteCtrl();

    void Show_RC_Value();
    void Show_Motor();
    void Show_RC_Rate();

private slots:
    void ShowData_Slot();

private:
    Ui::RemoteCtrl *ui;

    QTimer *ShowDataTimer;
};

#endif // REMOTECTRL_H
